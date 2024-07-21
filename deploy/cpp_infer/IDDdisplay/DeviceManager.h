#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <windows.h>
#include <swdevice.h>

class DeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit DeviceManager(QObject *parent = nullptr);
    virtual ~DeviceManager();
    
    HRESULT createVirtualDevice();
    void closeVirtualDevice();

signals:
    void deviceCreated();
    void deviceClosed();

private:
    static VOID WINAPI creationCallback(
        _In_ HSWDEVICE hSwDevice,
        _In_ HRESULT hrCreateResult,
        _In_opt_ PVOID pContext,
        _In_opt_ PCWSTR pszDeviceInstanceId
    );

private:
    HSWDEVICE m_hSwDevice;
    HANDLE m_hEvent;
};

#endif // DEVICEMANAGER_H
