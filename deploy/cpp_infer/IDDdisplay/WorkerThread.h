#include <QCoreApplication>
#include <QThread>
#include <QLibrary>
#include <QDebug>

// 定义线程类
class WorkerThread : public QThread
{
public:
    void run() override
    {
        // 加载 DeviceManager.dll
        QLibrary deviceManagerLib("DeviceManager.dll");
        if (!deviceManagerLib.load()) {
            qDebug() << "Failed to load DeviceManager.dll:" << deviceManagerLib.errorString();
            return;
        }

        // 获取函数指针
        typedef void (*RunInfiniteLoopFunc)();
        RunInfiniteLoopFunc runInfiniteLoop = reinterpret_cast<RunInfiniteLoopFunc>(deviceManagerLib.resolve("startDeviceManager"));
        if (!runInfiniteLoop) {
            qDebug() << "Failed to resolve runInfiniteLoop function";
            return;
        }

        // 调用 DLL 中的函数，这个函数可能是一个无限循环
        runInfiniteLoop();

        // 卸载 DLL
        deviceManagerLib.unload();
    }
};
