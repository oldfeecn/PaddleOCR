#pragma once

#include <QObject>
#ifdef Q_OS_WIN
#include <windows.h>
#endif
#include <QThread>
#include "EGlobalVariable.h"

class KeyAndMouseThread : public QObject
{
	Q_OBJECT

public:


	KeyAndMouseThread(QObject* parent);
	~KeyAndMouseThread();
	void KMmsgToSysEvent(QByteArray datagram);
	BOOL InstallKbHook();
	static LRESULT KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
	BOOL UninstallKbHook();
	static LRESULT LowLevelMouseProc(INT nCode, WPARAM wParam, LPARAM lParam);
	static void mouseInfo(MSLLHOOKSTRUCT* pkbhs, int keyCode);
public slots:
	void getStartKMThreadState(int state);
signals:
	void hook_KM_msg(QByteArray*);
	void sendMsg(QVariantMap& tableMap);//��������Ϣ
private:
	HHOOK mouse_hook = NULL;//��깳��
	HANDLE threadId;//�̱߳�ʶID
	HHOOK key_hook = NULL;//���̹���
	BOOL g_bCapsLock = FALSE;		//��Сд������	
	BOOL g_bShift = FALSE;
	int kmState = 0;
	MSG message;


};
