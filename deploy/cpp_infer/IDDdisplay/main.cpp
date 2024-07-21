#include <QCoreApplication>
#include <iostream>
#include <conio.h>
#include "DeviceManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DeviceManager deviceManager;

    // 连接信号和槽
    QObject::connect(&deviceManager, &DeviceManager::deviceCreated, [&]() {
        std::cout << "Device created successfully" << std::endl;
    });

    QObject::connect(&deviceManager, &DeviceManager::deviceClosed, [&]() {
        std::cout << "Device closed" << std::endl;
    });

    // 创建设备
    HRESULT hr = deviceManager.createVirtualDevice();
    if (FAILED(hr))
    {
        std::cout << "Failed to create device" << std::endl;
        return 1;
    }

    // 等待用户输入 'x' 键退出
    std::cout << "Press 'x' to exit and destroy the software device" << std::endl;
    bool bExit = false;
    do
    {
        int key = _getch();
        if (key == 'x' || key == 'X')
        {
            bExit = true;
        }
    } while (!bExit);

    // 关闭设备
    deviceManager.closeVirtualDevice();

    return a.exec();
}
