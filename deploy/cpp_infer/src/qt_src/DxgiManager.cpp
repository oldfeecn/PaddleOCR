#include "DxgiManager.h"
#include <QDebug>

DxgiManager::DxgiManager()
    : m_device(nullptr), m_context(nullptr), m_duplication(nullptr), m_texture(nullptr)
{
}

DxgiManager::~DxgiManager()
{
    if (m_duplication) {
        m_duplication->Release();
        m_duplication = nullptr;
    }
    delete m_texture;
}

bool DxgiManager::init(uint outputIndex)
{
    D3D_FEATURE_LEVEL feat = D3D_FEATURE_LEVEL_11_0;
    HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &m_device, &feat, &m_context);
    if (FAILED(hr)) {
        m_lastError = "Failed to D3D11CreateDevice ErrorCode = " + QString::number(hr, 16);
        return false;
    }

    IDXGIDevice *dxgiDevice = nullptr;
    hr = m_device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
    if (FAILED(hr)) {
        m_lastError = "Failed to QueryInterface IDXGIOutput6 ErrorCode = " + QString::number(hr, 16);
        m_device->Release();
        m_context->Release();
        return false;
    }

    IDXGIAdapter *dxgiAdapter = nullptr;
    hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));
    dxgiDevice->Release();
    if (FAILED(hr)) {
        m_lastError = "Failed to Get IDXGIAdapter ErrorCode = " + QString::number(hr, 16);
        m_device->Release();
        m_context->Release();
        return false;
    }

    QVector<IDXGIOutput *> outputs;
    IDXGIOutput *dxgiOutput = nullptr;
    for (uint i = 0; dxgiAdapter->EnumOutputs(i, &dxgiOutput) != DXGI_ERROR_NOT_FOUND; ++i) {
        outputs.push_back(dxgiOutput);
    }
    dxgiAdapter->Release();
    if (outputIndex >= outputs.size()) {
        m_lastError = "Invalid output index!";
        m_device->Release();
        m_context->Release();
        return false;
    }
    dxgiOutput = outputs.at(outputIndex);

    IDXGIOutput6 *dxgiOutput6 = nullptr;
    hr = dxgiOutput->QueryInterface(__uuidof(IDXGIOutput6), reinterpret_cast<void**>(&dxgiOutput6));
    dxgiOutput->Release();
    if (FAILED(hr)) {
        m_lastError = "Failed to QueryInterface IDXGIOutput6 ErrorCode = " + QString::number(hr, 16);
        m_device->Release();
        m_context->Release();
        return false;
    }

    hr = dxgiOutput6->DuplicateOutput(m_device, &m_duplication);
    dxgiOutput6->Release();
    if (FAILED(hr)) {
        m_lastError = "Failed to DuplicateOutput ErrorCode = " + QString::number(hr, 16);
        m_device->Release();
        m_context->Release();
        return false;
    }

    DXGI_OUTDUPL_DESC desc;
    m_duplication->GetDesc(&desc);
    m_texture = new DxgiTextureStaging(m_device, m_context);
    if (desc.DesktopImageInSystemMemory) {
        qDebug() << "Desc: CPU shared with GPU";
    } else {
        qDebug() << "Desc: CPU not shared with GPU";
    }

    return true;
}

QString DxgiManager::lastError() const
{
    return m_lastError;
}

QPixmap DxgiManager::grabScreen()
{
    IDXGIResource *desktopRes = nullptr;
    DXGI_OUTDUPL_FRAME_INFO frameInfo;
    HRESULT hr = m_duplication->AcquireNextFrame(100, &frameInfo, &desktopRes);
    if (FAILED(hr)) {
        m_lastError = "Failed to AcquireNextFrame ErrorCode = " + QString::number(hr, 16);
        return QPixmap();
    }

    QPixmap pixmap = m_texture->copyToImage(desktopRes);
    desktopRes->Release();
    m_duplication->ReleaseFrame();

    return pixmap;
}
