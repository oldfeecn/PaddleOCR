#pragma once
#include "FindWindow.h"
#include <xlocbuf>
#include <windows.h>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
struct WindowRect
{
    int x;
    int y;
    int width;
    int height;
};

class WindowShot
{
public:
    HWND hwnd;
    HMONITOR currentMonitor;
    WindowShot();
    double static getZoom();
    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
    void GetWindowsOnMonitor(HMONITOR hMonitor);
    cv::Mat getWindowMat(const std::string& titleSection);
    cv::Mat getWindowMat(const std::string& titleSection, WindowShot* windowShot);
    uchar* getWindowUchar(const std::string& titleSection);
    WindowRect windowRect;
    cv::Mat getDesktopMat();
    ~WindowShot();
    
private:
    int width;
    int height;
    double zoom;
    uchar* windowUchar;
    RECT rect;
    HDC screenDC;
    HDC compatibleDC;
    HBITMAP hBitmap;
    LPVOID shotData;

    std::wstring targetTitle;
    std::wstring toLower(const std::wstring& str);
    bool containsIgnoreCase(const std::wstring& str, const std::wstring& substr);
    std::wstring stringToWstring(const std::string& str);
};
