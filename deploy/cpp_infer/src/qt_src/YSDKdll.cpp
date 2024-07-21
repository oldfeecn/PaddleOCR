#include "YSDKdll.h"

Q_GLOBAL_STATIC(YSDKdll, rule)

YSDKdll* YSDKdll::instance()
{
	return rule();
}

int YSDKdll::GetFunAddr(LPCSTR dllfile, MyLogTextEdit* logEdit)
{

	if (-1 == ::GetFileAttributes(dllfile))
	{

		return -11; //�ļ�������
	}
	m_hModule = ::LoadLibrary(dllfile);


	if (m_hModule == NULL)
	{
		return -12; // ��������
	}
	open = (M_Open)GetProcAddress(m_hModule, "M_Open");
	open_VidPid = (M_Open_VidPid)GetProcAddress(m_hModule, "M_Open_VidPid");
	scanAndOpen = (M_ScanAndOpen)GetProcAddress(m_hModule, "M_ScanAndOpen");
	close = (M_Close)GetProcAddress(m_hModule, "M_Close");
	getDevSn = (M_GetDevSn)GetProcAddress(m_hModule, "M_GetDevSn");
	setUserData = (M_SetUserData)GetProcAddress(m_hModule, "M_SetUserData");
	verifyUserData = (M_VerifyUserData)GetProcAddress(m_hModule, "M_VerifyUserData");
	chkSupportMdy = (M_ChkSupportMdy)GetProcAddress(m_hModule, "M_ChkSupportMdy");
	getAbsCfg = (M_GetAbsCfg)GetProcAddress(m_hModule, "M_GetAbsCfg");
	setAbsCfg = (M_SetAbsCfg)GetProcAddress(m_hModule, "M_SetAbsCfg");
	setNewVidPid = (M_SetNewVidPid)GetProcAddress(m_hModule, "M_SetNewVidPid");
	resetVidPid = (M_ResetVidPid)GetProcAddress(m_hModule, "M_ResetVidPid");

	getVidPid = (M_GetVidPid)GetProcAddress(m_hModule, "M_GetVidPid");
	chkSupportProdStrMdy = (M_ChkSupportProdStrMdy)GetProcAddress(m_hModule, "M_ChkSupportProdStrMdy");
	getProdStr = (M_GetProdStr)GetProcAddress(m_hModule, "M_GetProdStr");
	setProdStr = (M_SetProdStr)GetProcAddress(m_hModule, "M_SetProdStr");
	chkSupportEnc = (M_ChkSupportEnc)GetProcAddress(m_hModule, "M_ChkSupportEnc");
	chkPwStat = (M_ChkPwStat)GetProcAddress(m_hModule, "M_ChkPwStat");
	setPw = (M_SetPw)GetProcAddress(m_hModule, "M_SetPw");
	verifyPw = (M_VerifyPw)GetProcAddress(m_hModule, "M_VerifyPw");
	wrEncData = (M_WrEncData)GetProcAddress(m_hModule, "M_WrEncData");
	rdEncData = (M_RdEncData)GetProcAddress(m_hModule, "M_RdEncData");
	initParam = (M_InitParam)GetProcAddress(m_hModule, "M_InitParam");
	setParam = (M_SetParam)GetProcAddress(m_hModule, "M_SetParam");
	keyPress = (M_KeyPress)GetProcAddress(m_hModule, "M_KeyPress");
	keyDown = (M_KeyDown)GetProcAddress(m_hModule, "M_KeyDown");
	keyUp = (M_KeyUp)GetProcAddress(m_hModule, "M_KeyUp");
	keyState = (M_KeyState)GetProcAddress(m_hModule, "M_KeyState");
	keyPress2 = (M_KeyPress2)GetProcAddress(m_hModule, "M_KeyPress2");
	keyDown2 = (M_KeyDown2)GetProcAddress(m_hModule, "M_KeyDown2");
	keyUp2 = (M_KeyUp2)GetProcAddress(m_hModule, "M_KeyUp2");
	keyState2 = (M_KeyState2)GetProcAddress(m_hModule, "M_KeyState2");
	releaseAllKey = (M_ReleaseAllKey)GetProcAddress(m_hModule, "M_ReleaseAllKey");
	numLockLedState = (M_NumLockLedState)GetProcAddress(m_hModule, "M_NumLockLedState");
	capsLockLedState = (M_CapsLockLedState)GetProcAddress(m_hModule, "M_CapsLockLedState");
	scrollLockLedState = (M_ScrollLockLedState)GetProcAddress(m_hModule, "M_ScrollLockLedState");
	keyInputString = (M_KeyInputString)GetProcAddress(m_hModule, "M_KeyInputString");
	keyInputStringGBK = (M_KeyInputStringGBK)GetProcAddress(m_hModule, "M_KeyInputStringGBK");
	keyInputStringUnicode = (M_KeyInputStringUnicode)GetProcAddress(m_hModule, "M_KeyInputStringUnicode");
	leftClick = (M_LeftClick)GetProcAddress(m_hModule, "M_LeftClick");
	leftDoubleClick = (M_LeftDoubleClick)GetProcAddress(m_hModule, "M_LeftDoubleClick");
	leftDown = (M_LeftDown)GetProcAddress(m_hModule, "M_LeftDown");
	leftUp = (M_LeftUp)GetProcAddress(m_hModule, "M_LeftUp");
	rightClick = (M_RightClick)GetProcAddress(m_hModule, "M_RightClick");
	rightDown = (M_RightDown)GetProcAddress(m_hModule, "M_RightDown");
	rightUp = (M_RightUp)GetProcAddress(m_hModule, "M_RightUp");
	middleClick = (M_MiddleClick)GetProcAddress(m_hModule, "M_MiddleClick");
	middleDown = (M_MiddleDown)GetProcAddress(m_hModule, "M_MiddleDown");
	middleUp = (M_MiddleUp)GetProcAddress(m_hModule, "M_MiddleUp");
	releaseAllMouse = (M_ReleaseAllMouse)GetProcAddress(m_hModule, "M_ReleaseAllMouse");
	mouseKeyState = (M_MouseKeyState)GetProcAddress(m_hModule, "M_MouseKeyState");
	mouseWheel = (M_MouseWheel)GetProcAddress(m_hModule, "M_MouseWheel");
	resetMousePos = (M_ResetMousePos)GetProcAddress(m_hModule, "M_ResetMousePos");
	moveR = (M_MoveR)GetProcAddress(m_hModule, "M_MoveR");
	moveTo = (M_MoveTo)GetProcAddress(m_hModule, "M_MoveTo");
	getCurrMousePos = (M_GetCurrMousePos)GetProcAddress(m_hModule, "M_GetCurrMousePos");
	getCurrMousePosX = (M_GetCurrMousePosX)GetProcAddress(m_hModule, "M_GetCurrMousePosX");
	getCurrMousePosY = (M_GetCurrMousePosY)GetProcAddress(m_hModule, "M_GetCurrMousePosY");
	moveR2 = (M_MoveR2)GetProcAddress(m_hModule, "M_MoveR2");
	moveTo2 = (M_MoveTo2)GetProcAddress(m_hModule, "M_MoveTo2");
	getCurrMousePos2 = (M_GetCurrMousePos2)GetProcAddress(m_hModule, "M_GetCurrMousePos2");
	resolutionUsed = (M_ResolutionUsed)GetProcAddress(m_hModule, "M_ResolutionUsed");
	moveTo3 = (M_MoveTo3)GetProcAddress(m_hModule, "M_MoveTo3");
	moveTo3_D = (M_MoveTo3_D)GetProcAddress(m_hModule, "M_MoveTo3_D");
	delay = (M_Delay)GetProcAddress(m_hModule, "M_Delay");
	delayRandom = (M_DelayRandom)GetProcAddress(m_hModule, "M_DelayRandom");
	randDomNbr = (M_RandDomNbr)GetProcAddress(m_hModule, "M_RandDomNbr");
	//�ж�һ�£��Ƿ���ȷ��ȡ�˺�����ַ
	if (!getProdStr) {
		logEdit->appendText(QStringLiteral("getProdStr ������ȡʧ��"));
	}
	if (!open) {
		logEdit->appendText(QStringLiteral("open ������ȡʧ��"));
	}
	if (!open_VidPid) {
		logEdit->appendText(QStringLiteral("open_VidPid ������ȡʧ��"));
	}
	if (!scanAndOpen) {
		logEdit->appendText(QStringLiteral("scanAndOpen ������ȡʧ��"));
	}
	if (!close) {
		logEdit->appendText(QStringLiteral("close ������ȡʧ��"));
	}
	if (!getDevSn) {
		logEdit->appendText(QStringLiteral("getDevSn ������ȡʧ��"));
	}
	if (!setUserData) {
		logEdit->appendText(QStringLiteral("setUserData ������ȡʧ��"));
	}
	if (!verifyUserData) {
		logEdit->appendText(QStringLiteral("verifyUserData ������ȡʧ��"));
	}
	if (!getAbsCfg) {
		logEdit->appendText(QStringLiteral("getAbsCfg ������ȡʧ��"));
	}
	if (!chkSupportMdy) {
		logEdit->appendText(QStringLiteral("chkSupportMdy ������ȡʧ��"));
	}
	if (!setAbsCfg) {
		logEdit->appendText(QStringLiteral("setAbsCfg ������ȡʧ��"));
	}
	if (!setNewVidPid) {
		logEdit->appendText(QStringLiteral("setNewVidPid ������ȡʧ��"));
	}
	if (!resetVidPid) {
		logEdit->appendText(QStringLiteral("resetVidPid ������ȡʧ��"));
	}
	if (!getVidPid) {
		logEdit->appendText(QStringLiteral("getVidPid ������ȡʧ��"));
	}
	if (!chkSupportProdStrMdy) {
		logEdit->appendText(QStringLiteral("chkSupportProdStrMdy ������ȡʧ��"));
	}
	if (!setProdStr) {
		logEdit->appendText(QStringLiteral("setProdStr ������ȡʧ��"));
	}
	if (!chkSupportEnc) {
		logEdit->appendText(QStringLiteral("chkSupportEnc ������ȡʧ��"));
	}
	if (!chkPwStat) {
		logEdit->appendText(QStringLiteral("chkPwStat ������ȡʧ��"));
	}
	if (!setPw) {
		logEdit->appendText(QStringLiteral("setPw ������ȡʧ��"));
	}
	if (!verifyPw) {
		logEdit->appendText(QStringLiteral("verifyPw ������ȡʧ��"));
	}
	if (!wrEncData) {
		logEdit->appendText(QStringLiteral("wrEncData ������ȡʧ��"));
	}
	if (!rdEncData) {
		logEdit->appendText(QStringLiteral("rdEncData ������ȡʧ��"));
	}
	if (!initParam) {
		logEdit->appendText(QStringLiteral("initParam ������ȡʧ��"));
	}
	if (!setParam) {
		logEdit->appendText(QStringLiteral("setParam ������ȡʧ��"));
	}
	if (!keyPress) {
		logEdit->appendText(QStringLiteral("keyPress ������ȡʧ��"));
	}
	if (!keyDown) {
		logEdit->appendText(QStringLiteral("keyDown ������ȡʧ��"));
	}
	if (!keyUp) {
		logEdit->appendText(QStringLiteral("keyUp ������ȡʧ��"));
	}
	if (!keyState) {
		logEdit->appendText(QStringLiteral("keyState ������ȡʧ��"));
	}
	return 1; // �ɹ�

}
