#include "MSDKWrapper.h"
#include <QLibrary>
#include <QDebug>

MSDKWrapper::MSDKWrapper(QObject *parent) : QObject(parent), m_library(nullptr)
{
    Q_M_Open = nullptr;
    Q_M_Open_VidPid = nullptr;
    Q_M_ScanAndOpen = nullptr;
    Q_M_Close = nullptr;
    Q_M_GetDevSn = nullptr;
    Q_M_SetUserData = nullptr;
    Q_M_VerifyUserData = nullptr;
    Q_M_ChkSupportMdy = nullptr;
    Q_M_GetAbsCfg = nullptr;
    Q_M_SetAbsCfg = nullptr;
    Q_M_SetNewVidPid = nullptr;
    Q_M_ResetVidPid = nullptr;
    Q_M_GetVidPid = nullptr;
    Q_M_ChkSupportProdStrMdy = nullptr;
    Q_M_GetProdStr = nullptr;
    Q_M_SetProdStr = nullptr;
    Q_M_ChkSupportEnc = nullptr;
    Q_M_ChkPwStat = nullptr;
    Q_M_SetPw = nullptr;
    Q_M_VerifyPw = nullptr;
    Q_M_WrEncData = nullptr;
    Q_M_RdEncData = nullptr;
    Q_M_InitParam = nullptr;
    Q_M_SetParam = nullptr;
    Q_M_KeyPress = nullptr;
    Q_M_KeyDown = nullptr;
    Q_M_KeyUp = nullptr;
    Q_M_KeyState = nullptr;
    Q_M_KeyPress2 = nullptr;
    Q_M_KeyDown2 = nullptr;
    Q_M_KeyUp2 = nullptr;
    Q_M_KeyState2 = nullptr;
    Q_M_ReleaseAllKey = nullptr;
    Q_M_NumLockLedState = nullptr;
    Q_M_CapsLockLedState = nullptr;
    Q_M_ScrollLockLedState = nullptr;
    Q_M_KeyInputString = nullptr;
    Q_M_KeyInputStringGBK = nullptr;
    Q_M_KeyInputStringUnicode = nullptr;
    Q_M_LeftClick = nullptr;
    Q_M_LeftDoubleClick = nullptr;
    Q_M_LeftDown = nullptr;
    Q_M_LeftUp = nullptr;
    Q_M_RightClick = nullptr;
    Q_M_RightDown = nullptr;
    Q_M_RightUp = nullptr;
    Q_M_MiddleClick = nullptr;
    Q_M_MiddleDown = nullptr;
    Q_M_MiddleUp = nullptr;
    Q_M_ReleaseAllMouse = nullptr;
    Q_M_MouseKeyState = nullptr;
    Q_M_MouseWheel = nullptr;
    Q_M_ResetMousePos = nullptr;
    Q_M_MoveR = nullptr;
    Q_M_MoveTo = nullptr;
    Q_M_GetCurrMousePos = nullptr;
    Q_M_GetCurrMousePosX = nullptr;
    Q_M_GetCurrMousePosY = nullptr;
    Q_M_MoveR2 = nullptr;
    Q_M_MoveTo2 = nullptr;
    Q_M_GetCurrMousePos2 = nullptr;
    Q_M_ResolutionUsed = nullptr;
    Q_M_MoveTo3 = nullptr;
    Q_M_MoveTo3_D = nullptr;
    Q_M_Delay = nullptr;
    Q_M_DelayRandom = nullptr;
    Q_M_RandDomNbr = nullptr;
}

MSDKWrapper::~MSDKWrapper()
{
    QMutexLocker locker(&m_mutex);
    if (m_library.isLoaded())
    {
        try
        {
            callReleaseFunction();
        }
        catch (const std::exception &)
        {
        }

        m_library.unload();
    }
}
void MSDKWrapper::callReleaseFunction()
{
    try
    {
        if (Q_M_Close)
        {
            M_Close(m_hdl1);
        }
    }
    catch (const std::exception &)
    {
    }
}
bool MSDKWrapper::loadLibrary(const QString &path)
{
    QMutexLocker locker(&m_mutex);
    m_library.setFileName(path);
    if (!m_library.load())
    {
        qDebug() << "Failed to load library:" << m_library.errorString();
        return false;
    }

    Q_M_Open = (LPFN_M_Open)m_library.resolve("M_Open");
    Q_M_Open_VidPid = (LPFN_M_Open_VidPid)m_library.resolve("M_Open_VidPid");
    Q_M_ScanAndOpen = (LPFN_M_ScanAndOpen)m_library.resolve("M_ScanAndOpen");
    Q_M_Close = (LPFN_M_Close)m_library.resolve("M_Close");
    Q_M_GetDevSn = (LPFN_M_GetDevSn)m_library.resolve("M_GetDevSn");
    Q_M_SetUserData = (LPFN_M_SetUserData)m_library.resolve("M_SetUserData");
    Q_M_VerifyUserData = (LPFN_M_VerifyUserData)m_library.resolve("M_VerifyUserData");
    Q_M_ChkSupportMdy = (LPFN_M_ChkSupportMdy)m_library.resolve("M_ChkSupportMdy");
    Q_M_GetAbsCfg = (LPFN_M_GetAbsCfg)m_library.resolve("M_GetAbsCfg");
    Q_M_SetAbsCfg = (LPFN_M_SetAbsCfg)m_library.resolve("M_SetAbsCfg");
    Q_M_SetNewVidPid = (LPFN_M_SetNewVidPid)m_library.resolve("M_SetNewVidPid");
    Q_M_ResetVidPid = (LPFN_M_ResetVidPid)m_library.resolve("M_ResetVidPid");
    Q_M_GetVidPid = (LPFN_M_GetVidPid)m_library.resolve("M_GetVidPid");
    Q_M_ChkSupportProdStrMdy = (LPFN_M_ChkSupportProdStrMdy)m_library.resolve("M_ChkSupportProdStrMdy");
    Q_M_GetProdStr = (LPFN_M_GetProdStr)m_library.resolve("M_GetProdStr");
    Q_M_SetProdStr = (LPFN_M_SetProdStr)m_library.resolve("M_SetProdStr");
    Q_M_ChkSupportEnc = (LPFN_M_ChkSupportEnc)m_library.resolve("M_ChkSupportEnc");
    Q_M_ChkPwStat = (LPFN_M_ChkPwStat)m_library.resolve("M_ChkPwStat");
    Q_M_SetPw = (LPFN_M_SetPw)m_library.resolve("M_SetPw");
    Q_M_VerifyPw = (LPFN_M_VerifyPw)m_library.resolve("M_VerifyPw");
    Q_M_WrEncData = (LPFN_M_WrEncData)m_library.resolve("M_WrEncData");
    Q_M_RdEncData = (LPFN_M_RdEncData)m_library.resolve("M_RdEncData");
    Q_M_InitParam = (LPFN_M_InitParam)m_library.resolve("M_InitParam");
    Q_M_SetParam = (LPFN_M_SetParam)m_library.resolve("M_SetParam");
    Q_M_KeyPress = (LPFN_M_KeyPress)m_library.resolve("M_KeyPress");
    Q_M_KeyDown = (LPFN_M_KeyDown)m_library.resolve("M_KeyDown");
    Q_M_KeyUp = (LPFN_M_KeyUp)m_library.resolve("M_KeyUp");
    Q_M_KeyState = (LPFN_M_KeyState)m_library.resolve("M_KeyState");
    Q_M_KeyPress2 = (LPFN_M_KeyPress2)m_library.resolve("M_KeyPress2");
    Q_M_KeyDown2 = (LPFN_M_KeyDown2)m_library.resolve("M_KeyDown2");
    Q_M_KeyUp2 = (LPFN_M_KeyUp2)m_library.resolve("M_KeyUp2");
    Q_M_KeyState2 = (LPFN_M_KeyState2)m_library.resolve("M_KeyState2");
    Q_M_ReleaseAllKey = (LPFN_M_ReleaseAllKey)m_library.resolve("M_ReleaseAllKey");
    Q_M_NumLockLedState = (LPFN_M_NumLockLedState)m_library.resolve("M_NumLockLedState");
    Q_M_CapsLockLedState = (LPFN_M_CapsLockLedState)m_library.resolve("M_CapsLockLedState");
    Q_M_ScrollLockLedState = (LPFN_M_ScrollLockLedState)m_library.resolve("M_ScrollLockLedState");
    Q_M_KeyInputString = (LPFN_M_KeyInputString)m_library.resolve("M_KeyInputString");
    Q_M_KeyInputStringGBK = (LPFN_M_KeyInputStringGBK)m_library.resolve("M_KeyInputStringGBK");
    Q_M_KeyInputStringUnicode = (LPFN_M_KeyInputStringUnicode)m_library.resolve("M_KeyInputStringUnicode");
    Q_M_LeftClick = (LPFN_M_LeftClick)m_library.resolve("M_LeftClick");
    Q_M_LeftDoubleClick = (LPFN_M_LeftDoubleClick)m_library.resolve("M_LeftDoubleClick");
    Q_M_LeftDown = (LPFN_M_LeftDown)m_library.resolve("M_LeftDown");
    Q_M_LeftUp = (LPFN_M_LeftUp)m_library.resolve("M_LeftUp");
    Q_M_RightClick = (LPFN_M_RightClick)m_library.resolve("M_RightClick");
    Q_M_RightDown = (LPFN_M_RightDown)m_library.resolve("M_RightDown");
    Q_M_RightUp = (LPFN_M_RightUp)m_library.resolve("M_RightUp");
    Q_M_MiddleClick = (LPFN_M_MiddleClick)m_library.resolve("M_MiddleClick");
    Q_M_MiddleDown = (LPFN_M_MiddleDown)m_library.resolve("M_MiddleDown");
    Q_M_MiddleUp = (LPFN_M_MiddleUp)m_library.resolve("M_MiddleUp");
    Q_M_ReleaseAllMouse = (LPFN_M_ReleaseAllMouse)m_library.resolve("M_ReleaseAllMouse");
    Q_M_MouseKeyState = (LPFN_M_MouseKeyState)m_library.resolve("M_MouseKeyState");
    Q_M_MouseWheel = (LPFN_M_MouseWheel)m_library.resolve("M_MouseWheel");
    Q_M_ResetMousePos = (LPFN_M_ResetMousePos)m_library.resolve("M_ResetMousePos");
    Q_M_MoveR = (LPFN_M_MoveR)m_library.resolve("M_MoveR");
    Q_M_MoveTo = (LPFN_M_MoveTo)m_library.resolve("M_MoveTo");
    Q_M_GetCurrMousePos = (LPFN_M_GetCurrMousePos)m_library.resolve("M_GetCurrMousePos");
    Q_M_GetCurrMousePosX = (LPFN_M_GetCurrMousePosX)m_library.resolve("M_GetCurrMousePosX");
    Q_M_GetCurrMousePosY = (LPFN_M_GetCurrMousePosY)m_library.resolve("M_GetCurrMousePosY");
    Q_M_MoveR2 = (LPFN_M_MoveR2)m_library.resolve("M_MoveR2");
    Q_M_MoveTo2 = (LPFN_M_MoveTo2)m_library.resolve("M_MoveTo2");
    Q_M_GetCurrMousePos2 = (LPFN_M_GetCurrMousePos2)m_library.resolve("M_GetCurrMousePos2");
    Q_M_ResolutionUsed = (LPFN_M_ResolutionUsed)m_library.resolve("M_ResolutionUsed");
    Q_M_MoveTo3 = (LPFN_M_MoveTo3)m_library.resolve("M_MoveTo3");
    Q_M_MoveTo3_D = (LPFN_M_MoveTo3_D)m_library.resolve("M_MoveTo3_D");
    Q_M_Delay = (LPFN_M_Delay)m_library.resolve("M_Delay");
    Q_M_DelayRandom = (LPFN_M_DelayRandom)m_library.resolve("M_DelayRandom");
    Q_M_RandDomNbr = (LPFN_M_RandDomNbr)m_library.resolve("M_RandDomNbr");
    bool hasError = false;
    // Check all resolved functions
    if (!Q_M_Open)
    {
        qDebug() << "Failed to resolve M_Open";
        hasError = true;
    }
    if (!Q_M_Open_VidPid)
    {
        qDebug() << "Failed to resolve M_Open_VidPid";
        hasError = true;
    }
    if (!Q_M_ScanAndOpen)
    {
        qDebug() << "Failed to resolve M_ScanAndOpen";
        hasError = true;
    }
    if (!Q_M_Close)
    {
        qDebug() << "Failed to resolve M_Close";
        hasError = true;
    }
    if (!Q_M_GetDevSn)
    {
        qDebug() << "Failed to resolve M_GetDevSn";
        hasError = true;
    }
    if (!Q_M_SetUserData)
    {
        qDebug() << "Failed to resolve M_SetUserData";
        hasError = true;
    }
    if (!Q_M_VerifyUserData)
    {
        qDebug() << "Failed to resolve M_VerifyUserData";
        hasError = true;
    }
    if (!Q_M_ChkSupportMdy)
    {
        qDebug() << "Failed to resolve M_ChkSupportMdy";
        hasError = true;
    }
    if (!Q_M_GetAbsCfg)
    {
        qDebug() << "Failed to resolve M_GetAbsCfg";
        hasError = true;
    }
    if (!Q_M_SetAbsCfg)
    {
        qDebug() << "Failed to resolve M_SetAbsCfg";
        hasError = true;
    }
    if (!Q_M_SetNewVidPid)
    {
        qDebug() << "Failed to resolve M_SetNewVidPid";
        hasError = true;
    }
    if (!Q_M_ResetVidPid)
    {
        qDebug() << "Failed to resolve M_ResetVidPid";
        hasError = true;
    }
    if (!Q_M_GetVidPid)
    {
        qDebug() << "Failed to resolve M_GetVidPid";
        hasError = true;
    }
    if (!Q_M_ChkSupportProdStrMdy)
    {
        qDebug() << "Failed to resolve M_ChkSupportProdStrMdy";
        hasError = true;
    }
    if (!Q_M_GetProdStr)
    {
        qDebug() << "Failed to resolve M_GetProdStr";
    }
    if (!Q_M_SetProdStr)
    {
        qDebug() << "Failed to resolve M_SetProdStr";
        hasError = true;
    }
    if (!Q_M_ChkSupportEnc)
    {
        qDebug() << "Failed to resolve M_ChkSupportEnc";
        hasError = true;
    }
    if (!Q_M_ChkPwStat)
    {
        qDebug() << "Failed to resolve M_ChkPwStat";
        hasError = true;
    }
    if (!Q_M_SetPw)
    {
        qDebug() << "Failed to resolve M_SetPw";
        hasError = true;
    }
    if (!Q_M_VerifyPw)
    {
        qDebug() << "Failed to resolve M_VerifyPw";
        hasError = true;
    }
    if (!Q_M_WrEncData)
    {
        qDebug() << "Failed to resolve M_WrEncData";
        hasError = true;
    }
    if (!Q_M_RdEncData)
    {
        qDebug() << "Failed to resolve M_RdEncData";
        hasError = true;
    }
    if (!Q_M_InitParam)
    {
        qDebug() << "Failed to resolve M_InitParam";
        hasError = true;
    }
    if (!Q_M_SetParam)
    {
        qDebug() << "Failed to resolve M_SetParam";
        hasError = true;
    }
    if (!Q_M_KeyPress)
    {
        qDebug() << "Failed to resolve M_KeyPress";
        hasError = true;
    }
    if (!Q_M_KeyDown)
    {
        qDebug() << "Failed to resolve M_KeyDown";
        hasError = true;
    }
    if (!Q_M_KeyUp)
    {
        qDebug() << "Failed to resolve M_KeyUp";
        hasError = true;
    }
    if (!Q_M_KeyState)
    {
        qDebug() << "Failed to resolve M_KeyState";
        hasError = true;
    }
    if (!Q_M_KeyPress2)
    {
        qDebug() << "Failed to resolve M_KeyPress2";
        hasError = true;
    }
    if (!Q_M_KeyDown2)
    {
        qDebug() << "Failed to resolve M_KeyDown2";
        hasError = true;
    }
    if (!Q_M_KeyUp2)
    {
        qDebug() << "Failed to resolve M_KeyUp2";
        hasError = true;
    }
    if (!Q_M_KeyState2)
    {
        qDebug() << "Failed to resolve M_KeyState2";
        hasError = true;
    }
    if (!Q_M_ReleaseAllKey)
    {
        qDebug() << "Failed to resolve M_ReleaseAllKey";
        hasError = true;
    }
    if (!Q_M_NumLockLedState)
    {
        qDebug() << "Failed to resolve M_NumLockLedState";
        hasError = true;
    }
    if (!Q_M_CapsLockLedState)
    {
        qDebug() << "Failed to resolve M_CapsLockLedState";
        hasError = true;
    }
    if (!Q_M_ScrollLockLedState)
    {
        qDebug() << "Failed to resolve M_ScrollLockLedState";
        hasError = true;
    }
    if (!Q_M_KeyInputString)
    {
        qDebug() << "Failed to resolve M_KeyInputString";
        hasError = true;
    }
    if (!Q_M_KeyInputStringGBK)
    {
        qDebug() << "Failed to resolve M_KeyInputStringGBK";
        hasError = true;
    }
    if (!Q_M_KeyInputStringUnicode)
    {
        qDebug() << "Failed to resolve M_KeyInputStringUnicode";
        hasError = true;
    }
    if (!Q_M_LeftClick)
    {
        qDebug() << "Failed to resolve M_LeftClick";
        hasError = true;
    }
    if (!Q_M_LeftDoubleClick)
    {
        qDebug() << "Failed to resolve M_LeftDoubleClick";
        hasError = true;
    }
    if (!Q_M_LeftDown)
    {
        qDebug() << "Failed to resolve M_LeftDown";
        hasError = true;
    }
    if (!Q_M_LeftUp)
    {
        qDebug() << "Failed to resolve M_LeftUp";
        hasError = true;
    }
    if (!Q_M_RightClick)
    {
        qDebug() << "Failed to resolve M_RightClick";
        hasError = true;
    }
    if (!Q_M_RightDown)
    {
        qDebug() << "Failed to resolve M_RightDown";
        hasError = true;
    }
    if (!Q_M_RightUp)
    {
        qDebug() << "Failed to resolve M_RightUp";
        hasError = true;
    }
    if (!Q_M_MiddleClick)
    {
        qDebug() << "Failed to resolve M_MiddleClick";
        hasError = true;
    }
    if (!Q_M_MiddleDown)
    {
        qDebug() << "Failed to resolve M_MiddleDown";
        hasError = true;
    }
    if (!Q_M_MiddleUp)
    {
        qDebug() << "Failed to resolve M_MiddleUp";
        hasError = true;
    }
    if (!Q_M_ReleaseAllMouse)
    {
        qDebug() << "Failed to resolve M_ReleaseAllMouse";
        hasError = true;
    }
    if (!Q_M_MouseKeyState)
    {
        qDebug() << "Failed to resolve M_MouseKeyState";
        hasError = true;
    }
    if (!Q_M_MouseWheel)
    {
        qDebug() << "Failed to resolve M_MouseWheel";
        hasError = true;
    }
    if (!Q_M_ResetMousePos)
    {
        qDebug() << "Failed to resolve M_ResetMousePos";
        hasError = true;
    }
    if (!Q_M_MoveR)
    {
        qDebug() << "Failed to resolve M_MoveR";
        hasError = true;
    }
    if (!Q_M_MoveTo)
    {
        qDebug() << "Failed to resolve M_MoveTo";
        hasError = true;
    }
    if (!Q_M_GetCurrMousePos)
    {
        qDebug() << "Failed to resolve M_GetCurrMousePos";
        hasError = true;
    }
    if (!Q_M_GetCurrMousePosX)
    {
        qDebug() << "Failed to resolve M_GetCurrMousePosX";
        hasError = true;
    }
    if (!Q_M_GetCurrMousePosY)
    {
        qDebug() << "Failed to resolve M_GetCurrMousePosY";
        hasError = true;
    }
    if (!Q_M_MoveR2)
    {
        qDebug() << "Failed to resolve M_MoveR2";
        hasError = true;
    }
    if (!Q_M_MoveTo2)
    {
        qDebug() << "Failed to resolve M_MoveTo2";
        hasError = true;
    }
    if (!Q_M_GetCurrMousePos2)
    {
        qDebug() << "Failed to resolve M_GetCurrMousePos2";
        hasError = true;
    }
    if (!Q_M_ResolutionUsed)
    {
        qDebug() << "Failed to resolve M_ResolutionUsed";
        hasError = true;
    }
    if (!Q_M_MoveTo3)
    {
        qDebug() << "Failed to resolve M_MoveTo3";
        hasError = true;
    }
    if (!Q_M_MoveTo3_D)
    {
        qDebug() << "Failed to resolve M_MoveTo3_D";
        hasError = true;
    }
    if (!Q_M_Delay)
    {
        qDebug() << "Failed to resolve M_Delay";
        hasError = true;
    }
    if (!Q_M_DelayRandom)
    {
        qDebug() << "Failed to resolve M_DelayRandom";
        hasError = true;
    }
    if (!Q_M_RandDomNbr)
    {
        qDebug() << "Failed to resolve M_RandDomNbr";
        hasError = true;
    }

    if (hasError)
    {
        qDebug() << "Failed to resolve some functions";
        m_library.unload();
        return false;
    }
    /* if (!Q_M_Open || !Q_M_Open_VidPid || !Q_M_ScanAndOpen || !Q_M_Close || !Q_M_GetDevSn || !Q_M_SetUserData || !Q_M_VerifyUserData ||
         !Q_M_ChkSupportMdy || !Q_M_GetAbsCfg || !Q_M_SetAbsCfg || !Q_M_SetNewVidPid || !Q_M_ResetVidPid || !Q_M_GetVidPid ||
         !Q_M_ChkSupportProdStrMdy || !Q_M_GetProdStr || !Q_M_SetProdStr || !Q_M_ChkSupportEnc || !Q_M_ChkPwStat || !Q_M_SetPw ||
         !Q_M_VerifyPw || !Q_M_WrEncData || !Q_M_RdEncData || !Q_M_InitParam || !Q_M_SetParam || !Q_M_KeyPress || !Q_M_KeyDown ||
         !Q_M_KeyUp || !Q_M_KeyState || !Q_M_KeyPress2 || !Q_M_KeyDown2 || !Q_M_KeyUp2 || !Q_M_KeyState2 || !Q_M_ReleaseAllKey ||
         !Q_M_NumLockLedState || !Q_M_CapsLockLedState || !Q_M_ScrollLockLedState || !Q_M_KeyInputString || !Q_M_KeyInputStringGBK ||
         !Q_M_KeyInputStringUnicode || !Q_M_LeftClick || !Q_M_LeftDoubleClick || !Q_M_LeftDown || !Q_M_LeftUp || !Q_M_RightClick ||
         !Q_M_RightDown || !Q_M_RightUp || !Q_M_MiddleClick || !Q_M_MiddleDown || !Q_M_MiddleUp || !Q_M_ReleaseAllMouse ||
         !Q_M_MouseKeyState || !Q_M_MouseWheel || !Q_M_ResetMousePos || !Q_M_MoveR || !Q_M_MoveTo || !Q_M_GetCurrMousePos ||
         !Q_M_GetCurrMousePosX || !Q_M_GetCurrMousePosY || !Q_M_MoveR2 || !Q_M_MoveTo2 || !Q_M_GetCurrMousePos2 ||
         !Q_M_ResolutionUsed || !Q_M_MoveTo3 || !Q_M_MoveTo3_D || !Q_M_Delay || !Q_M_DelayRandom || !Q_M_RandDomNbr)
     {

     }*/

    return true;
}
// 设备操作函数
HANDLE MSDKWrapper::M_Open(int Nbr)
{
    if (Q_M_Open)
    {
        m_hdl1 = Q_M_Open(Nbr);
        return m_hdl1;
    }
    return INVALID_HANDLE_VALUE;
}

HANDLE MSDKWrapper::M_Open_VidPid(int Vid, int Pid)
{
    if (Q_M_Open_VidPid)
    {
        m_hdl1 = Q_M_Open_VidPid(Vid, Pid);
        if (M_GetVidPid(m_hdl1, 1) == Vid)
        {
            qDebug() << "open MSDK DLL INFO" << m_hdl1;
            return m_hdl1;
        }
    }
    return NULL;
}

HANDLE MSDKWrapper::M_ScanAndOpen()
{
    if (Q_M_ScanAndOpen)
    {
        m_hdl1 = Q_M_ScanAndOpen();
        if (INVALID_HANDLE_VALUE == m_hdl1)
        {
            return m_hdl1;
        }
        else
        {
            qDebug() << "open MSDK DLL error";
        }
    }
    return INVALID_HANDLE_VALUE;
}

int MSDKWrapper::M_Close(HANDLE m_hdl)
{
    if (Q_M_Close)
    {
        if (NULL != m_hdl1)
        {
            qDebug() << "Q_M_Close MSDK DLL INFO" << m_hdl1;
            int a = Q_M_Close(m_hdl1);
            qDebug() << "Q_M_Close MSDK DLL INFO" << a;
            m_hdl1 = NULL;
            return a;
        }
        else
        {
            qDebug() << " MSDK DLL not open";
        }
    }
    return -1;
}

int MSDKWrapper::M_GetDevSn(HANDLE m_hdl, DWORD *dwp_LenResponse, unsigned char *ucp_Response)
{
    if (Q_M_GetDevSn)
    {
        return Q_M_GetDevSn(m_hdl, dwp_LenResponse, ucp_Response);
    }
    return -1;
}

int MSDKWrapper::M_SetUserData(HANDLE m_hdl, DWORD dw_LenUserData, unsigned char *ucp_UserData)
{
    if (Q_M_SetUserData)
    {
        return Q_M_SetUserData(m_hdl, dw_LenUserData, ucp_UserData);
    }
    return -1;
}

int MSDKWrapper::M_VerifyUserData(HANDLE m_hdl, DWORD dw_LenUserData, unsigned char *ucp_UserData)
{
    if (Q_M_VerifyUserData)
    {
        return Q_M_VerifyUserData(m_hdl, dw_LenUserData, ucp_UserData);
    }
    return -1;
}

int MSDKWrapper::M_ChkSupportMdy(HANDLE m_hdl)
{
    if (Q_M_ChkSupportMdy)
    {
        return Q_M_ChkSupportMdy(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_GetAbsCfg(HANDLE m_hdl)
{
    if (Q_M_GetAbsCfg)
    {
        return Q_M_GetAbsCfg(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_SetAbsCfg(HANDLE m_hdl, int e1d0)
{
    if (Q_M_SetAbsCfg)
    {
        return Q_M_SetAbsCfg(m_hdl, e1d0);
    }
    return -1;
}

int MSDKWrapper::M_SetNewVidPid(HANDLE m_hdl, int mVid, int mPid, int sVid, int sPid)
{
    if (Q_M_SetNewVidPid)
    {
        return Q_M_SetNewVidPid(m_hdl, mVid, mPid, sVid, sPid);
    }
    return -1;
}

int MSDKWrapper::M_ResetVidPid(HANDLE m_hdl)
{
    if (Q_M_ResetVidPid)
    {
        return Q_M_ResetVidPid(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_GetVidPid(HANDLE m_hdl, int IdIndex)
{
    if (Q_M_GetVidPid)
    {
        int a = Q_M_GetVidPid(m_hdl, IdIndex);
        qDebug() << QString::number(a, 16).toUpper();
        return a;
    }
    return -1;
}

int MSDKWrapper::M_ChkSupportProdStrMdy(HANDLE m_hdl)
{
    if (Q_M_ChkSupportProdStrMdy)
    {
        return Q_M_ChkSupportProdStrMdy(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_GetProdStr(HANDLE m_hdl, int m0s1, int *LenOfStr, unsigned char *ucp_Str)
{
    if (Q_M_GetProdStr)
    {
        return Q_M_GetProdStr(m_hdl, m0s1, LenOfStr, ucp_Str);
    }
    return -1;
}

int MSDKWrapper::M_SetProdStr(HANDLE m_hdl, int m0s1, int LenOfStr, unsigned char *ucp_Str)
{
    if (Q_M_SetProdStr)
    {
        return Q_M_SetProdStr(m_hdl, m0s1, LenOfStr, ucp_Str);
    }
    return -1;
}

int MSDKWrapper::M_ChkSupportEnc(HANDLE m_hdl)
{
    if (Q_M_ChkSupportEnc)
    {
        return Q_M_ChkSupportEnc(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_ChkPwStat(HANDLE m_hdl)
{
    if (Q_M_ChkPwStat)
    {
        return Q_M_ChkPwStat(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_SetPw(HANDLE m_hdl, int PwLen, int PwMaxCnt, unsigned char *ucp_pw)
{
    if (Q_M_SetPw)
    {
        return Q_M_SetPw(m_hdl, PwLen, PwMaxCnt, ucp_pw);
    }
    return -1;
}

int MSDKWrapper::M_VerifyPw(HANDLE m_hdl, int PwLen, unsigned char *ucp_pw)
{
    if (Q_M_VerifyPw)
    {
        return Q_M_VerifyPw(m_hdl, PwLen, ucp_pw);
    }
    return -1;
}

int MSDKWrapper::M_WrEncData(HANDLE m_hdl, int DataLen, unsigned char *ucp_data)
{
    if (Q_M_WrEncData)
    {
        return Q_M_WrEncData(m_hdl, DataLen, ucp_data);
    }
    return -1;
}

int MSDKWrapper::M_RdEncData(HANDLE m_hdl, int DataLen, unsigned char *ucp_data)
{
    if (Q_M_RdEncData)
    {
        return Q_M_RdEncData(m_hdl, DataLen, ucp_data);
    }
    return -1;
}

int MSDKWrapper::M_InitParam(HANDLE m_hdl)
{
    if (Q_M_InitParam)
    {
        return Q_M_InitParam(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_SetParam(HANDLE m_hdl, int ParamType, int Param1, int Param2)
{
    if (Q_M_SetParam)
    {
        return Q_M_SetParam(m_hdl, ParamType, Param1, Param2);
    }
    return -1;
}
// 键盘操作函数
int MSDKWrapper::M_KeyPress(HANDLE m_hdl, int HidKeyCode, int Nbr)
{
    if (Q_M_KeyPress)
    {
        return Q_M_KeyPress(m_hdl, HidKeyCode, Nbr);
    }
    return -1;
}

int MSDKWrapper::M_KeyDown(HANDLE m_hdl, int HidKeyCode)
{
    if (Q_M_KeyDown)
    {
        return Q_M_KeyDown(m_hdl, HidKeyCode);
    }
    return -1;
}

int MSDKWrapper::M_KeyUp(HANDLE m_hdl, int HidKeyCode)
{
    if (Q_M_KeyUp)
    {
        return Q_M_KeyUp(m_hdl, HidKeyCode);
    }
    return -1;
}

int MSDKWrapper::M_KeyState(HANDLE m_hdl, int HidKeyCode)
{
    if (Q_M_KeyState)
    {
        return Q_M_KeyState(m_hdl, HidKeyCode);
    }
    return -1;
}

int MSDKWrapper::M_KeyPress2(HANDLE m_hdl, int KeyCode, int Nbr)
{
    if (Q_M_KeyPress2)
    {
        return Q_M_KeyPress2(m_hdl, KeyCode, Nbr);
    }
    return -1;
}

int MSDKWrapper::M_KeyDown2(HANDLE m_hdl, int KeyCode)
{
    if (Q_M_KeyDown2)
    {
        return Q_M_KeyDown2(m_hdl, KeyCode);
    }
    return -1;
}

int MSDKWrapper::M_KeyUp2(HANDLE m_hdl, int KeyCode)
{
    if (Q_M_KeyUp2)
    {
        return Q_M_KeyUp2(m_hdl, KeyCode);
    }
    return -1;
}

int MSDKWrapper::M_KeyState2(HANDLE m_hdl, int KeyCode)
{
    if (Q_M_KeyState2)
    {
        return Q_M_KeyState2(m_hdl, KeyCode);
    }
    return -1;
}

int MSDKWrapper::M_ReleaseAllKey(HANDLE m_hdl)
{
    if (Q_M_ReleaseAllKey)
    {
        return Q_M_ReleaseAllKey(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_NumLockLedState(HANDLE m_hdl)
{
    if (Q_M_NumLockLedState)
    {
        return Q_M_NumLockLedState(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_CapsLockLedState(HANDLE m_hdl)
{
    if (Q_M_CapsLockLedState)
    {
        return Q_M_CapsLockLedState(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_ScrollLockLedState(HANDLE m_hdl)
{
    if (Q_M_ScrollLockLedState)
    {
        return Q_M_ScrollLockLedState(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_KeyInputString(HANDLE m_hdl, char *InputStr, int InputLen)
{
    if (Q_M_KeyInputString)
    {
        return Q_M_KeyInputString(m_hdl, InputStr, InputLen);
    }
    return -1;
}

int MSDKWrapper::M_KeyInputStringGBK(HANDLE m_hdl, char *InputStr, int InputLen)
{
    if (Q_M_KeyInputStringGBK)
    {
        return Q_M_KeyInputStringGBK(m_hdl, InputStr, InputLen);
    }
    return -1;
}

int MSDKWrapper::M_KeyInputStringUnicode(HANDLE m_hdl, char *InputStr, int InputLen)
{
    if (Q_M_KeyInputStringUnicode)
    {
        return Q_M_KeyInputStringUnicode(m_hdl, InputStr, InputLen);
    }
    return -1;
}
// 鼠标操作函数
int MSDKWrapper::M_LeftClick(HANDLE m_hdl, int Nbr)
{
    if (Q_M_LeftClick)
    {
        return Q_M_LeftClick(m_hdl, Nbr);
    }
    return -1;
}

int MSDKWrapper::M_LeftDoubleClick(HANDLE m_hdl, int Nbr)
{
    if (Q_M_LeftDoubleClick)
    {
        return Q_M_LeftDoubleClick(m_hdl, Nbr);
    }
    return -1;
}

int MSDKWrapper::M_LeftDown(HANDLE m_hdl)
{
    if (Q_M_LeftDown)
    {
        return Q_M_LeftDown(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_LeftUp(HANDLE m_hdl)
{
    if (Q_M_LeftUp)
    {
        return Q_M_LeftUp(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_RightClick(HANDLE m_hdl, int Nbr)
{
    if (Q_M_RightClick)
    {
        return Q_M_RightClick(m_hdl, Nbr);
    }
    return -1;
}

int MSDKWrapper::M_RightDown(HANDLE m_hdl)
{
    if (Q_M_RightDown)
    {
        return Q_M_RightDown(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_RightUp(HANDLE m_hdl)
{
    if (Q_M_RightUp)
    {
        return Q_M_RightUp(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_MiddleClick(HANDLE m_hdl, int Nbr)
{
    if (Q_M_MiddleClick)
    {
        return Q_M_MiddleClick(m_hdl, Nbr);
    }
    return -1;
}

int MSDKWrapper::M_MiddleDown(HANDLE m_hdl)
{
    if (Q_M_MiddleDown)
    {
        return Q_M_MiddleDown(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_MiddleUp(HANDLE m_hdl)
{
    if (Q_M_MiddleUp)
    {
        return Q_M_MiddleUp(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_ReleaseAllMouse(HANDLE m_hdl)
{
    if (Q_M_ReleaseAllMouse)
    {
        return Q_M_ReleaseAllMouse(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_MouseKeyState(HANDLE m_hdl, int MouseKeyCode)
{
    if (Q_M_MouseKeyState)
    {
        return Q_M_MouseKeyState(m_hdl, MouseKeyCode);
    }
    return -1;
}

int MSDKWrapper::M_MouseWheel(HANDLE m_hdl, int Nbr)
{
    if (Q_M_MouseWheel)
    {
        return Q_M_MouseWheel(m_hdl, Nbr);
    }
    return -1;
}

int MSDKWrapper::M_ResetMousePos(HANDLE m_hdl)
{
    if (Q_M_ResetMousePos)
    {
        return Q_M_ResetMousePos(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_MoveR(HANDLE m_hdl, int x, int y)
{
    if (Q_M_MoveR)
    {
        return Q_M_MoveR(m_hdl, x, y);
    }
    return -1;
}

int MSDKWrapper::M_MoveTo(HANDLE m_hdl, int x, int y)
{
    if (Q_M_MoveTo)
    {
        return Q_M_MoveTo(m_hdl, x, y);
    }
    return -1;
}

int MSDKWrapper::M_GetCurrMousePos(HANDLE m_hdl, int *x, int *y)
{
    if (Q_M_GetCurrMousePos)
    {
        return Q_M_GetCurrMousePos(m_hdl, x, y);
    }
    return -1;
}

int MSDKWrapper::M_GetCurrMousePosX(HANDLE m_hdl)
{
    if (Q_M_GetCurrMousePosX)
    {
        return Q_M_GetCurrMousePosX(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_GetCurrMousePosY(HANDLE m_hdl)
{
    if (Q_M_GetCurrMousePosY)
    {
        return Q_M_GetCurrMousePosY(m_hdl);
    }
    return -1;
}

int MSDKWrapper::M_MoveR2(HANDLE m_hdl, int x, int y)
{
    if (Q_M_MoveR2)
    {
        return Q_M_MoveR2(m_hdl, x, y);
    }
    return -1;
}

int MSDKWrapper::M_MoveTo2(HANDLE m_hdl, int x, int y)
{
    if (Q_M_MoveTo2)
    {
        return Q_M_MoveTo2(m_hdl, x, y);
    }
    return -1;
}

int MSDKWrapper::M_GetCurrMousePos2(int *x, int *y)
{
    if (Q_M_GetCurrMousePos2)
    {
        return Q_M_GetCurrMousePos2(x, y);
    }
    return -1;
}

int MSDKWrapper::M_ResolutionUsed(HANDLE m_hdl, int x, int y)
{
    if (Q_M_ResolutionUsed)
    {
        return Q_M_ResolutionUsed(m_hdl, x, y);
    }
    return -1;
}

int MSDKWrapper::M_MoveTo3(HANDLE m_hdl, int x, int y)
{
    if (Q_M_MoveTo3)
    {
        return Q_M_MoveTo3(m_hdl, x, y);
    }
    return -1;
}

int MSDKWrapper::M_MoveTo3_D(HANDLE m_hdl, int x, int y)
{
    if (Q_M_MoveTo3_D)
    {
        return Q_M_MoveTo3_D(m_hdl, x, y);
    }
    return -1;
}

int MSDKWrapper::M_Delay(int time)
{
    if (Q_M_Delay)
    {
        return Q_M_Delay(time);
    }
    return -1;
}

int MSDKWrapper::M_DelayRandom(int Min_time, int Max_time)
{
    if (Q_M_DelayRandom)
    {
        return Q_M_DelayRandom(Min_time, Max_time);
    }
    return -1;
}

int MSDKWrapper::M_RandDomNbr(int Min_V, int Max_V)
{
    if (Q_M_RandDomNbr)
    {
        return Q_M_RandDomNbr(Min_V, Max_V);
    }
    return -1;
}
