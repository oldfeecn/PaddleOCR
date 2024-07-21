#include "KeyAndMouseThread.h"
#include "QDebug"

KeyAndMouseThread::KeyAndMouseThread(QObject* parent)
	: QObject(parent)
{


}

KeyAndMouseThread::~KeyAndMouseThread()
{

}

void KeyAndMouseThread::getStartKMThreadState(int state) {
	//qDebug() << "state" << state << endl;
	kmState = state;
	if (state == 1) {
		InstallKbHook();
#ifdef Q_OS_WIN
		//threadId=(HANDLE) GetCurrentThreadId();
#endif
		//threadId = QThread::currentThread();
		//qDebug() << "threadId" << threadId <<endl;
		while (GetMessage(&message, NULL, 0, 0) && state == 1) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		return;
	}
	if (state == 2) {
		UninstallKbHook();
		return;
	}
	return;
}

void KeyAndMouseThread::KMmsgToSysEvent(QByteArray datagram) {
	QString arrs = datagram;
	QStringList arr = arrs.split(':');
	if (arr[0] == "mouse") {
		POINT m;
		m.x = arr[3].toInt();
		m.y = arr[5].toInt();
		//km.MouseBtn(arr[1].toInt(), arr[3].toInt(), m);
		Sleep(arr[6].toInt());
	}
	else if (arr[0] == "key") {
		//km.KeyBtn(arr[1].toInt(), arr[3].toInt());
		Sleep(arr[4].toInt());
	}

}
BOOL KeyAndMouseThread::InstallKbHook()
{
	//�������EXE��ִ�У����������
	//HINSTANCE hinstDLL = LoadLibrary(TEXT("c:\\myapp\\syshook.dll"));
	//HOOKPROC hkprcMouse = (HOOKPROC)GetProcAddress(hinstDLL, "MouseHookProc");
	HINSTANCE hinstDLL = GetModuleHandle(NULL);
	//HOOKPROC hkprcMouse = LowLevelMouseProc;
	//�����뽫���ĸ���������ΪGetCurrentThreadId������ʹ�䱾�ػ���
	mouse_hook = SetWindowsHookEx(WH_MOUSE_LL, KeyAndMouseThread::LowLevelMouseProc, hinstDLL, NULL);
	key_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyAndMouseThread::KeyboardProc, hinstDLL, NULL);
	return(mouse_hook != NULL && key_hook != NULL);

}
LRESULT CALLBACK KeyAndMouseThread::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	char c[2];
	c[1] = 0;
	KBDLLHOOKSTRUCT* keyNum = (KBDLLHOOKSTRUCT*)lParam;
	//qDebug() << keyNum->vkCode << endl;
	//qDebug() << STATICVar->msgRadioState;
	if (2 == STATICVar->msgRadioState) {
		QByteArray datagram = (QString("key:%1:flags:%2:%3")
			.arg((int)keyNum->vkCode)
			.arg((int)keyNum->flags)).arg(::GetTickCount64() - STATICVar->lastTime).toLatin1();
		STATICVar->m_receiver->writeDatagram(datagram.data(), datagram.size(),
			QHostAddress::Broadcast, 45454);
		STATICVar->lastTime = ::GetTickCount64();
		//qDebug() << datagram;
	}
	else {
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	return NULL;
}
BOOL KeyAndMouseThread::UninstallKbHook()
{

	BOOL fOk = FALSE;
	if (mouse_hook) {
		fOk = UnhookWindowsHookEx(mouse_hook);
		mouse_hook = NULL;
	}
	if (key_hook) {
		fOk = UnhookWindowsHookEx(key_hook);
		key_hook = NULL;
	}
	/* if (threadId) {
		 CloseHandle(threadId);
	 }*/
	return(fOk);
}
LRESULT KeyAndMouseThread::LowLevelMouseProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
	MSLLHOOKSTRUCT* pkbhs = (MSLLHOOKSTRUCT*)lParam;
	//qDebug() << "pkbhs" << pkbhs->flags << pkbhs->dwExtraInfo << "wParam " << wParam << endl;

	if (nCode == HC_ACTION && 2 == STATICVar->msgRadioState) {

		switch (wParam)
		{
		case WM_MOUSEMOVE://����ƶ�
			mouseInfo(pkbhs, 200);
			break;
		case WM_LBUTTONDOWN://����������
			mouseInfo(pkbhs, 201);

			break;
		case WM_LBUTTONUP://������̧��
			mouseInfo(pkbhs, 202);
			break;

		case WM_RBUTTONDOWN://����Ҽ�����
			mouseInfo(pkbhs, 204);
			break;
		case WM_RBUTTONUP://����Ҽ�̧�� 
			mouseInfo(pkbhs, 205);
			break;

		case 519://�����¼�
			mouseInfo(pkbhs, 519);
			break;
		case 520://�����¼�
			mouseInfo(pkbhs, 520);
			break;
		case 522://�����¼�
			mouseInfo(pkbhs, 522);
			break;
		default:
			//qDebug() << "pkbhs" << pkbhs->flags << pkbhs->dwExtraInfo << "wParam " << wParam << endl;
			break;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void KeyAndMouseThread::mouseInfo(MSLLHOOKSTRUCT* pkbhs, int keyCode) {
	QByteArray datagram;
	datagram = (QString("mouse:%1:x:%2:y:%3:%4")
		.arg(keyCode)
		.arg((int)(pkbhs->pt.x))
		.arg((int)(pkbhs->pt.y))).arg(::GetTickCount64() - STATICVar->lastTime).toLatin1();
	STATICVar->m_receiver->writeDatagram(datagram.data(), datagram.size(),
		QHostAddress::Broadcast, 45454);
	STATICVar->lastTime = ::GetTickCount64();
	//qDebug() << datagram;
}