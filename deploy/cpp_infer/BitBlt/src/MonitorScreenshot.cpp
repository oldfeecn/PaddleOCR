#include "MonitorScreenshot.h"
#include <iostream>


MonitorScreenshot::MonitorScreenshot() {}

MonitorScreenshot::~MonitorScreenshot() {}

std::vector<cv::Mat> MonitorScreenshot::CaptureAllMonitors() {
    MonitorInfo info;
    screenshots.clear(); // 清空截图列表
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&info);
    return screenshots;
}

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdc, LPRECT lprcMonitor, LPARAM dwData) {
    MonitorScreenshot* pThis = reinterpret_cast<MonitorScreenshot*>(dwData);
    MonitorInfo& info = *reinterpret_cast<MonitorInfo*>(dwData);
    info.hMonitor = hMonitor;
    info.rcMonitor = *lprcMonitor;
    // 获取屏幕 DC
    HDC hdcScreen = GetDC(NULL);
    // 兼容新DC
    HDC hdcCompatible = CreateCompatibleDC(hdcScreen);
    // 创建位图
    auto width = lprcMonitor->right - lprcMonitor->left;
    auto height = lprcMonitor->bottom - lprcMonitor->top;
    std::cout << "width" << width << "height" << height << std::endl;
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, lprcMonitor->right - lprcMonitor->left, lprcMonitor->bottom - lprcMonitor->top);
    SelectObject(hdcCompatible, hBitmap);
    // 得到位图的数据
    BitBlt(hdcCompatible, 0, 0, lprcMonitor->right - lprcMonitor->left, lprcMonitor->bottom - lprcMonitor->top, hdcScreen, lprcMonitor->left, lprcMonitor->top, SRCCOPY);
    // 创建图像
    std::vector<BYTE> buffer(4 * (lprcMonitor->right - lprcMonitor->left) * (lprcMonitor->bottom - lprcMonitor->top));
    cv::Mat screenshot(lprcMonitor->bottom - lprcMonitor->top, lprcMonitor->right - lprcMonitor->left, CV_8UC4, buffer.data());
    // 添加截图到成员变量
    std::cout << "width" << width << "height" << height << std::endl;
    cv::imshow("Monitor Screenshot", screenshot);
    cv::waitKey(0);
    if (screenshot.data && !screenshot.empty() && screenshot.total() > 0 && screenshot.dims > 0) {
        pThis->screenshots.push_back(screenshot);
    }
    cv::imshow("Monitor Screenshot", screenshot);
    cv::waitKey(0); 
    
   
    std::cout << "width"  << std::endl;
    DeleteObject(hBitmap);
    DeleteDC(hdcCompatible);
    ReleaseDC(NULL, hdcScreen);
    return TRUE;
}
