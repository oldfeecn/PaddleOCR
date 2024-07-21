#ifndef DXGITEXTURESTAGING_H
#define DXGITEXTURESTAGING_H
#ifdef Q_OS_WIN
# include <windows.h>
#  if defined (_WIN32_WINNT_WIN10)  //win10 dxgi1_6
#   include <dxgi1_6.h>
#   include <d3d11.h>
#  endif
#endif
#include <QPixmap>
#include <QImage>
#include <d3d11.h>
#include <dxgi1_2.h>

class DxgiTextureStaging
{
public:
    DxgiTextureStaging(ID3D11Device *device, ID3D11DeviceContext *context);
    ~DxgiTextureStaging();

    QPixmap copyToImage(IDXGIResource *res);

private:
    ID3D11Device *m_device;
    ID3D11DeviceContext *m_context;
    ID3D11Texture2D *m_texture;
};

#endif // DXGITEXTURESTAGING_H
