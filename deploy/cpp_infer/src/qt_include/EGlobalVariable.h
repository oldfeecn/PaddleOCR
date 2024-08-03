#ifndef EGV_H
#define EGV_H

#include <QGlobalStatic>
#include <QString>
#define STATICVar EGlobalVariable::instance()
#include<QScreen>
#include <QDebug>
#include <QUdpSocket>
#include <QtWin>
#include<QQueue>
#include <QByteArray>
#include <QtNetwork\qhostinfo.h>
#include <QtNetwork\qnetworkinterface.h>
class EGlobalVariable
{
public:

    EGlobalVariable() {
        getScreenWH();

    }
    virtual ~EGlobalVariable() {

    }
public:
    static EGlobalVariable* instance();
    static QMap<QString, QList<int>> keyMap;

public:
    QString get_localmachine_ip();
    void test();
    QString hookKeyToKeyStr(int val);
    int KeyToUsbKeyCode(QString keyStr);
    int KeyToWinKeyCode(QString keyStr);

    QString serverUserName;
    QString serverPWD;
    int screen_width;
    int screen_height;
    double sysDpiVal;
    void getScreenWH();
    enum KMMouseState {
        defaultState = 0,
        mouseLeftDown = 1,
        mouseLeftUp = 2,
        mouseRightDown = 3,
        mouseRightUp = 4
    }kmMouseState;
    static QUdpSocket* m_receiver;
    static DWORD  lastTime;
    //static QQueue<QByteArray>* kmMsgqueue;
    /// <summary>
    /// -2:发送消息 -3 接收消息 -4 停止消息
    /// </summary>
    static qint8 msgRadioState;

    //static KMMouseState = defaultState;
};

#endif // EGV_H

