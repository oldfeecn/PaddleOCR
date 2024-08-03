// DxgiManager.h
#ifndef DXGIMANAGER_H
#define DXGIMANAGER_H

#include <QPixmap>

#ifdef Q_OS_WIN
# include <windows.h>
#  if defined (_WIN32_WINNT_WIN10)  //win10 dxgi1_6
#   include <dxgi1_6.h>
#   include <d3d11.h>
#  endif
#endif
#include <d3d11.h>
#include <dxgi1_2.h>
#include "DxgiTextureStaging.h"

class DxgiManager
{
public:
    DxgiManager();
    ~DxgiManager();

    bool init(uint outputIndex=0);
    QString lastError() const;
    QPixmap grabScreen();

    void cleanup();

private:
    ID3D11Device *m_device;
    ID3D11DeviceContext *m_context;
    IDXGIOutputDuplication *m_duplication;
    DxgiTextureStaging *m_texture;
    QString m_lastError;
};

#endif // DXGIMANAGER_H
