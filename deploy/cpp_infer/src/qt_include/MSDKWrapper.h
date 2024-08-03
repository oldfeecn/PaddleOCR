#ifndef MSDKWRAPPER_H
#define MSDKWRAPPER_H

#include <QObject>
#include <windows.h>
#include <QLibrary>
#include <QMutex>
/**
 * @brief The MSDKWrapper class
 * 用于动态加载MSDK DLL并重写其方法
 */
class MSDKWrapper : public QObject
{
    Q_OBJECT

public:
    explicit MSDKWrapper(QObject *parent = nullptr);
    ~MSDKWrapper();
    void callReleaseFunction();
    HANDLE resourcePointer; 
    bool loadLibrary(const QString &path);
    QLibrary m_library;
    QMutex m_mutex;//线程安全
    HANDLE m_hdl1;
    // 设备操作函数
    HANDLE M_Open(int Nbr);
    HANDLE M_Open_VidPid(int Vid, int Pid);
    HANDLE M_ScanAndOpen();
    int M_Close(HANDLE m_hdl);
    
    int M_GetDevSn(HANDLE m_hdl, DWORD *dwp_LenResponse, unsigned char *ucp_Response);
    int M_SetUserData(HANDLE m_hdl, DWORD dw_LenUserData, unsigned char *ucp_UserData);
    int M_VerifyUserData(HANDLE m_hdl, DWORD dw_LenUserData, unsigned char *ucp_UserData);
    int M_ChkSupportMdy(HANDLE m_hdl);
    int M_GetAbsCfg(HANDLE m_hdl);
    int M_SetAbsCfg(HANDLE m_hdl, int e1d0);
    int M_SetNewVidPid(HANDLE m_hdl, int mVid, int mPid, int sVid, int sPid);
    int M_ResetVidPid(HANDLE m_hdl);
    int M_GetVidPid(HANDLE m_hdl, int IdIndex);
    int M_ChkSupportProdStrMdy(HANDLE m_hdl);
    int M_GetProdStr(HANDLE m_hdl, int m0s1, int *LenOfStr, unsigned char *ucp_Str);
    int M_SetProdStr(HANDLE m_hdl, int m0s1, int LenOfStr, unsigned char *ucp_Str);
    int M_ChkSupportEnc(HANDLE m_hdl);
    int M_ChkPwStat(HANDLE m_hdl);
    int M_SetPw(HANDLE m_hdl, int PwLen, int PwMaxCnt, unsigned char *ucp_pw);
    int M_VerifyPw(HANDLE m_hdl, int PwLen, unsigned char *ucp_pw);
    int M_WrEncData(HANDLE m_hdl, int DataLen, unsigned char *ucp_data);
    int M_RdEncData(HANDLE m_hdl, int DataLen, unsigned char *ucp_data);
    int M_InitParam(HANDLE m_hdl);
    int M_SetParam(HANDLE m_hdl, int ParamType, int Param1, int Param2);

    // 键盘操作函数
    int M_KeyPress(HANDLE m_hdl, int HidKeyCode, int Nbr);
    int M_KeyDown(HANDLE m_hdl, int HidKeyCode);
    int M_KeyUp(HANDLE m_hdl, int HidKeyCode);
    int M_KeyState(HANDLE m_hdl, int HidKeyCode);
    int M_KeyPress2(HANDLE m_hdl, int KeyCode, int Nbr);
    int M_KeyDown2(HANDLE m_hdl, int KeyCode);
    int M_KeyUp2(HANDLE m_hdl, int KeyCode);
    int M_KeyState2(HANDLE m_hdl, int KeyCode);
    int M_ReleaseAllKey(HANDLE m_hdl);
    int M_NumLockLedState(HANDLE m_hdl);
    int M_CapsLockLedState(HANDLE m_hdl);
    int M_ScrollLockLedState(HANDLE m_hdl);
    int M_KeyInputString(HANDLE m_hdl, char *InputStr, int InputLen);
    int M_KeyInputStringGBK(HANDLE m_hdl, char *InputStr, int InputLen);
    int M_KeyInputStringUnicode(HANDLE m_hdl, char *InputStr, int InputLen);

    // 鼠标操作函数
    int M_LeftClick(HANDLE m_hdl, int Nbr);
    int M_LeftDoubleClick(HANDLE m_hdl, int Nbr);
    int M_LeftDown(HANDLE m_hdl);
    int M_LeftUp(HANDLE m_hdl);
    int M_RightClick(HANDLE m_hdl, int Nbr);
    int M_RightDown(HANDLE m_hdl);
    int M_RightUp(HANDLE m_hdl);
    int M_MiddleClick(HANDLE m_hdl, int Nbr);
    int M_MiddleDown(HANDLE m_hdl);
    int M_MiddleUp(HANDLE m_hdl);
    int M_ReleaseAllMouse(HANDLE m_hdl);
    int M_MouseKeyState(HANDLE m_hdl, int MouseKeyCode);
    int M_MouseWheel(HANDLE m_hdl, int Nbr);
    int M_ResetMousePos(HANDLE m_hdl);
    int M_MoveR(HANDLE m_hdl, int x, int y);
    int M_MoveTo(HANDLE m_hdl, int x, int y);
    int M_GetCurrMousePos(HANDLE m_hdl, int *x, int *y);
    int M_GetCurrMousePosX(HANDLE m_hdl);
    int M_GetCurrMousePosY(HANDLE m_hdl);

    int M_MoveR2(HANDLE m_hdl, int x, int y);
    int M_MoveTo2(HANDLE m_hdl, int x, int y);
    int M_GetCurrMousePos2(int *x, int *y);

    int M_ResolutionUsed(HANDLE m_hdl, int x, int y);
    int M_MoveTo3(HANDLE m_hdl, int x, int y);
    int M_MoveTo3_D(HANDLE m_hdl, int x, int y);

    int M_Delay(int time);
    int M_DelayRandom(int Min_time, int Max_time);
    int M_RandDomNbr(int Min_V, int Max_V);

private:
    typedef HANDLE(WINAPI *LPFN_M_Open)(int);
    typedef HANDLE(WINAPI *LPFN_M_Open_VidPid)(int, int);
    typedef HANDLE(WINAPI *LPFN_M_ScanAndOpen)();
    typedef int(WINAPI *LPFN_M_Close)(HANDLE);
    typedef int(WINAPI *LPFN_M_GetDevSn)(HANDLE, DWORD *, unsigned char *);
    typedef int(WINAPI *LPFN_M_SetUserData)(HANDLE, DWORD, unsigned char *);
    typedef int(WINAPI *LPFN_M_VerifyUserData)(HANDLE, DWORD, unsigned char *);
    typedef int(WINAPI *LPFN_M_ChkSupportMdy)(HANDLE);
    typedef int(WINAPI *LPFN_M_GetAbsCfg)(HANDLE);
    typedef int(WINAPI *LPFN_M_SetAbsCfg)(HANDLE, int);
    typedef int(WINAPI *LPFN_M_SetNewVidPid)(HANDLE, int, int, int, int);
    typedef int(WINAPI *LPFN_M_ResetVidPid)(HANDLE);
    typedef int(WINAPI *LPFN_M_GetVidPid)(HANDLE, int);
    typedef int(WINAPI *LPFN_M_ChkSupportProdStrMdy)(HANDLE);
    typedef int(WINAPI *LPFN_M_GetProdStr)(HANDLE, int, int *, unsigned char *);
    typedef int(WINAPI *LPFN_M_SetProdStr)(HANDLE, int, int, unsigned char *);
    typedef int(WINAPI *LPFN_M_ChkSupportEnc)(HANDLE);
    typedef int(WINAPI *LPFN_M_ChkPwStat)(HANDLE);
    typedef int(WINAPI *LPFN_M_SetPw)(HANDLE, int, int, unsigned char *);
    typedef int(WINAPI *LPFN_M_VerifyPw)(HANDLE, int, unsigned char *);
    typedef int(WINAPI *LPFN_M_WrEncData)(HANDLE, int, unsigned char *);
    typedef int(WINAPI *LPFN_M_RdEncData)(HANDLE, int, unsigned char *);
    typedef int(WINAPI *LPFN_M_InitParam)(HANDLE);
    typedef int(WINAPI *LPFN_M_SetParam)(HANDLE, int, int, int);
    
    typedef int(WINAPI *LPFN_M_KeyPress)(HANDLE, int, int);
    typedef int(WINAPI *LPFN_M_KeyDown)(HANDLE, int);
    typedef int(WINAPI *LPFN_M_KeyUp)(HANDLE, int);
    typedef int(WINAPI *LPFN_M_KeyState)(HANDLE, int);
    typedef int(WINAPI *LPFN_M_KeyPress2)(HANDLE, int, int);
    typedef int(WINAPI *LPFN_M_KeyDown2)(HANDLE, int);
    typedef int(WINAPI *LPFN_M_KeyUp2)(HANDLE, int);
    typedef int(WINAPI *LPFN_M_KeyState2)(HANDLE, int);
    typedef int(WINAPI *LPFN_M_ReleaseAllKey)(HANDLE);
    typedef int(WINAPI *LPFN_M_NumLockLedState)(HANDLE);
    typedef int(WINAPI *LPFN_M_CapsLockLedState)(HANDLE);
    typedef int(WINAPI *LPFN_M_ScrollLockLedState)(HANDLE);
    typedef int(WINAPI *LPFN_M_KeyInputString)(HANDLE, char *, int);
    typedef int(WINAPI *LPFN_M_KeyInputStringGBK)(HANDLE, char *, int);
    typedef int(WINAPI *LPFN_M_KeyInputStringUnicode)(HANDLE, char *, int);
    
    typedef int(WINAPI *LPFN_M_LeftClick)(HANDLE, int);
    typedef int(WINAPI *LPFN_M_LeftDoubleClick)(HANDLE, int);
    typedef int(WINAPI *LPFN_M_LeftDown)(HANDLE);
    typedef int(WINAPI *LPFN_M_LeftUp)(HANDLE);
    typedef int(WINAPI *LPFN_M_RightClick)(HANDLE, int);
    typedef int(WINAPI *LPFN_M_RightDown)(HANDLE);
    typedef int(WINAPI *LPFN_M_RightUp)(HANDLE);
    typedef int(WINAPI *LPFN_M_MiddleClick)(HANDLE, int);
    typedef int(WINAPI *LPFN_M_MiddleDown)(HANDLE);
    typedef int(WINAPI *LPFN_M_MiddleUp)(HANDLE);
    typedef int(WINAPI *LPFN_M_ReleaseAllMouse)(HANDLE);
    typedef int(WINAPI *LPFN_M_MouseKeyState)(HANDLE, int);
    typedef int(WINAPI *LPFN_M_MouseWheel)(HANDLE, int);
    typedef int(WINAPI *LPFN_M_ResetMousePos)(HANDLE);
    typedef int(WINAPI *LPFN_M_MoveR)(HANDLE, int, int);
    typedef int(WINAPI *LPFN_M_MoveTo)(HANDLE, int, int);
    typedef int(WINAPI *LPFN_M_GetCurrMousePos)(HANDLE, int *, int *);
    typedef int(WINAPI *LPFN_M_GetCurrMousePosX)(HANDLE);
    typedef int(WINAPI *LPFN_M_GetCurrMousePosY)(HANDLE);

    typedef int(WINAPI *LPFN_M_MoveR2)(HANDLE, int, int);
    typedef int(WINAPI *LPFN_M_MoveTo2)(HANDLE, int, int);
    typedef int(WINAPI *LPFN_M_GetCurrMousePos2)(int *, int *);

    typedef int(WINAPI *LPFN_M_ResolutionUsed)(HANDLE, int, int);
    typedef int(WINAPI *LPFN_M_MoveTo3)(HANDLE, int, int);
    typedef int(WINAPI *LPFN_M_MoveTo3_D)(HANDLE, int, int);

    typedef int(WINAPI *LPFN_M_Delay)(int);
    typedef int(WINAPI *LPFN_M_DelayRandom)(int, int);
    typedef int(WINAPI *LPFN_M_RandDomNbr)(int, int);

  

    LPFN_M_Open Q_M_Open;
    LPFN_M_Open_VidPid Q_M_Open_VidPid;
    LPFN_M_ScanAndOpen Q_M_ScanAndOpen;
    LPFN_M_Close Q_M_Close;
    LPFN_M_GetDevSn Q_M_GetDevSn;
    LPFN_M_SetUserData Q_M_SetUserData;
    LPFN_M_VerifyUserData Q_M_VerifyUserData;
    LPFN_M_ChkSupportMdy Q_M_ChkSupportMdy;
    LPFN_M_GetAbsCfg Q_M_GetAbsCfg;
    LPFN_M_SetAbsCfg Q_M_SetAbsCfg;
    LPFN_M_SetNewVidPid Q_M_SetNewVidPid;
    LPFN_M_ResetVidPid Q_M_ResetVidPid;
    LPFN_M_GetVidPid Q_M_GetVidPid;
    LPFN_M_ChkSupportProdStrMdy Q_M_ChkSupportProdStrMdy;
    LPFN_M_GetProdStr Q_M_GetProdStr;
    LPFN_M_SetProdStr Q_M_SetProdStr;
    LPFN_M_ChkSupportEnc Q_M_ChkSupportEnc;
    LPFN_M_ChkPwStat Q_M_ChkPwStat;
    LPFN_M_SetPw Q_M_SetPw;
    LPFN_M_VerifyPw Q_M_VerifyPw;
    LPFN_M_WrEncData Q_M_WrEncData;
    LPFN_M_RdEncData Q_M_RdEncData;
    LPFN_M_InitParam Q_M_InitParam;
    LPFN_M_SetParam Q_M_SetParam;
    
    LPFN_M_KeyPress Q_M_KeyPress;
    LPFN_M_KeyDown Q_M_KeyDown;
    LPFN_M_KeyUp Q_M_KeyUp;
    LPFN_M_KeyState Q_M_KeyState;
    LPFN_M_KeyPress2 Q_M_KeyPress2;
    LPFN_M_KeyDown2 Q_M_KeyDown2;
    LPFN_M_KeyUp2 Q_M_KeyUp2;
    LPFN_M_KeyState2 Q_M_KeyState2;
    LPFN_M_ReleaseAllKey Q_M_ReleaseAllKey;
    LPFN_M_NumLockLedState Q_M_NumLockLedState;
    LPFN_M_CapsLockLedState Q_M_CapsLockLedState;
    LPFN_M_ScrollLockLedState Q_M_ScrollLockLedState;
    LPFN_M_KeyInputString Q_M_KeyInputString;
    LPFN_M_KeyInputStringGBK Q_M_KeyInputStringGBK;
    LPFN_M_KeyInputStringUnicode Q_M_KeyInputStringUnicode;
    
    LPFN_M_LeftClick Q_M_LeftClick;
    LPFN_M_LeftDoubleClick Q_M_LeftDoubleClick;
    LPFN_M_LeftDown Q_M_LeftDown;
    LPFN_M_LeftUp Q_M_LeftUp;
    LPFN_M_RightClick Q_M_RightClick;
    LPFN_M_RightDown Q_M_RightDown;
    LPFN_M_RightUp Q_M_RightUp;
    LPFN_M_MiddleClick Q_M_MiddleClick;
    LPFN_M_MiddleDown Q_M_MiddleDown;
    LPFN_M_MiddleUp Q_M_MiddleUp;
    LPFN_M_ReleaseAllMouse Q_M_ReleaseAllMouse;
    LPFN_M_MouseKeyState Q_M_MouseKeyState;
    LPFN_M_MouseWheel Q_M_MouseWheel;
    LPFN_M_ResetMousePos Q_M_ResetMousePos;
    LPFN_M_MoveR Q_M_MoveR;
    LPFN_M_MoveTo Q_M_MoveTo;
    LPFN_M_GetCurrMousePos Q_M_GetCurrMousePos;
    LPFN_M_GetCurrMousePosX Q_M_GetCurrMousePosX;
    LPFN_M_GetCurrMousePosY Q_M_GetCurrMousePosY;
    
    LPFN_M_MoveR2 Q_M_MoveR2;
    LPFN_M_MoveTo2 Q_M_MoveTo2;
    LPFN_M_GetCurrMousePos2 Q_M_GetCurrMousePos2;
    
    LPFN_M_ResolutionUsed Q_M_ResolutionUsed;
    LPFN_M_MoveTo3 Q_M_MoveTo3;
    LPFN_M_MoveTo3_D Q_M_MoveTo3_D;
    
    LPFN_M_Delay Q_M_Delay;
    LPFN_M_DelayRandom Q_M_DelayRandom;
    LPFN_M_RandDomNbr Q_M_RandDomNbr;
};

#endif // MSDKWRAPPER_H
