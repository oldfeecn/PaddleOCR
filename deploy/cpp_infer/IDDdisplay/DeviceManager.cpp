#include "DeviceManager.h"
#include <QDebug>

DeviceManager::DeviceManager(QObject *parent)
    : QObject(parent), m_hSwDevice(nullptr), m_hEvent(nullptr)
{
    // 创建事件对象，用于在设备创建完成时进行信号通知
    m_hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

DeviceManager::~DeviceManager()
{
    closeVirtualDevice();
    if (m_hEvent)
    {
        CloseHandle(m_hEvent);
        m_hEvent = nullptr;
    }
}

VOID WINAPI DeviceManager::creationCallback(
    _In_ HSWDEVICE hSwDevice,
    _In_ HRESULT hrCreateResult,
    _In_opt_ PVOID pContext,
    _In_opt_ PCWSTR pszDeviceInstanceId
)
{
    Q_UNUSED(hSwDevice);
    Q_UNUSED(hrCreateResult);
    Q_UNUSED(pszDeviceInstanceId);

    auto *pThis = static_cast<DeviceManager*>(pContext);
    if (pThis)
        emit pThis->deviceCreated();
}

HRESULT DeviceManager::createVirtualDevice()
{
    SW_DEVICE_CREATE_INFO createInfo = { 0 };
    PCWSTR description = L"Idd Display Driver";

    PCWSTR instanceId = L"IddDisPlayDriver";
    PCWSTR hardwareIds = L"IddDisPlayDriver\0\0";
    PCWSTR compatibleIds = L"IddDisPlayDriver\0\0";

    createInfo.cbSize = sizeof(createInfo);
    createInfo.pszzCompatibleIds = compatibleIds;
    createInfo.pszInstanceId = instanceId;
    createInfo.pszzHardwareIds = hardwareIds;
    createInfo.pszDeviceDescription = description;

    createInfo.CapabilityFlags = SWDeviceCapabilitiesRemovable |
        SWDeviceCapabilitiesSilentInstall |
        SWDeviceCapabilitiesDriverRequired;

    // 创建设备
    HRESULT hr = SwDeviceCreate(L"IddDisPlayDriver",
        L"HTREE\\ROOT\\0",
        &createInfo,
        0,
        nullptr,
        DeviceManager::creationCallback,
        this, // 将当前对象作为上下文传递给回调函数
        &m_hSwDevice);
    if (FAILED(hr))
    {
        qDebug() << "SwDeviceCreate failed with 0x" << QString::number(hr, 16);
        return hr;
    }

    return S_OK;
}

void DeviceManager::closeVirtualDevice()
{
    if (m_hSwDevice)
    {
        SwDeviceClose(m_hSwDevice);
        m_hSwDevice = nullptr;
        emit deviceClosed();
    }
}
