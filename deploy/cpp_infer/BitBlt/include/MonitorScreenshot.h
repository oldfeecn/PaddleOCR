#pragma once
#include <windows.h>
#include <vector>
#include <opencv2/opencv.hpp>
struct MonitorInfo {
        HMONITOR hMonitor;
        RECT rcMonitor;
    };
class MonitorScreenshot {
public:
    MonitorScreenshot();
    ~MonitorScreenshot();
    std::vector<cv::Mat> CaptureAllMonitors();
    std::vector<cv::Mat> screenshots; // 定义为类的成员变量
private:
    
};

// 声明全局回调函数
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdc, LPRECT lprcMonitor, LPARAM dwData);

