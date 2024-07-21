#include "DxgiTextureStaging.h"
#include <QDebug>

DxgiTextureStaging::DxgiTextureStaging(ID3D11Device *device, ID3D11DeviceContext *context)
    : m_device(device), m_context(context), m_texture(nullptr)
{
    if (m_device) m_device->AddRef();
    if (m_context) m_context->AddRef();
}

DxgiTextureStaging::~DxgiTextureStaging()
{
    if (m_texture) m_texture->Release();
    if (m_device) m_device->Release();
    if (m_context) m_context->Release();
}

QPixmap DxgiTextureStaging::copyToImage(IDXGIResource *res)
{
    ID3D11Texture2D *textureRes = nullptr;
    HRESULT hr = res->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&textureRes));
    if (FAILED(hr)) {
        qDebug() << "Failed to ID3D11Texture2D result =" << QString::number(hr, 16);
        return QPixmap();
    }

    D3D11_TEXTURE2D_DESC desc;
    textureRes->GetDesc(&desc);

    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = desc.Width;
    texDesc.Height = desc.Height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_STAGING;
    texDesc.Format = desc.Format;
    texDesc.BindFlags = 0;
    texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

    hr = m_device->CreateTexture2D(&texDesc, nullptr, &m_texture);
    if (FAILED(hr)) {
        qDebug() << "Failed to CreateTexture2D result =" << QString::number(hr, 16);
        textureRes->Release();
        return QPixmap();
    }

    m_context->CopyResource(m_texture, textureRes);

    IDXGISurface1 *surface = nullptr;
    hr = m_texture->QueryInterface(__uuidof(IDXGISurface1), reinterpret_cast<void **>(&surface));
    if (FAILED(hr)) {
        qDebug() << "Failed to QueryInterface IDXGISurface1 ErrorCode =" << QString::number(hr, 16);
        textureRes->Release();
        m_texture->Release();
        return QPixmap();
    }

    DXGI_MAPPED_RECT map;
    hr = surface->Map(&map, DXGI_MAP_READ);
    if (FAILED(hr)) {
        qDebug() << "Failed to Map surface ErrorCode =" << QString::number(hr, 16);
        surface->Release();
        textureRes->Release();
        m_texture->Release();
        return QPixmap();
    }

    QPixmap pixmap = QPixmap::fromImage(QImage(static_cast<uchar *>(map.pBits),
                                               int(desc.Width), int(desc.Height), QImage::Format_ARGB32));
    surface->Unmap();
    surface->Release();
    textureRes->Release();
    m_texture->Release();
    m_texture = nullptr; // 重置 m_texture 指针
    return pixmap;
}
