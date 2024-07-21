
#include "WindowShot.h"

//初始化变量
WindowShot::WindowShot()
{
    zoom = getZoom();//缩放率 比如1.25
}

BOOL CALLBACK WindowShot::EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    auto* instance = reinterpret_cast<WindowShot*>(lParam);

    // 获取窗口所在的监视器
    HMONITOR hWindowMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONULL);
    if (hWindowMonitor == instance->currentMonitor) {
        // 打印窗口句柄
        std::wcout << L"Window Handle: " << hwnd << std::endl;
        // 存储窗口句柄

        //instance->hwnd = hwnd;
    }
    return TRUE;  // 继续枚举其他窗口
}

void WindowShot::GetWindowsOnMonitor(HMONITOR hMonitor) {
    currentMonitor = hMonitor;
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(this));
}

//根据窗口标题是否包含该字符串，获得窗口截图
cv::Mat WindowShot::getWindowMat(const std::string& titleSection, WindowShot* windowShot)
{
    targetTitle = stringToWstring(titleSection);
    hwnd = getWindowHWND(titleSection);
    if (!hwnd) {
        // 枚举所有监视器
        if (!EnumDisplayMonitors(NULL, NULL, [](HMONITOR hMonitor, HDC, LPRECT, LPARAM lParam) -> BOOL {
            auto* windowShot = reinterpret_cast<WindowShot*>(lParam);
            MONITORINFOEX monitorInfo;
            monitorInfo.cbSize = sizeof(MONITORINFOEX);

            if (GetMonitorInfo(hMonitor, &monitorInfo)) {
                //std::wcout << L"Monitor Handle: " << hMonitor << std::endl;
                std::wcout << L"Monitor Name: " << monitorInfo.szDevice << std::endl;
               // windowShot->GetWindowsOnMonitor(hMonitor);

            }
            return TRUE;
            }, reinterpret_cast<LPARAM>(&windowShot))) {
            std::cerr << "Failed to enumerate monitors" << std::endl;
           
        }
        
    }
    std::cerr << "如果窗口小化 就将其展示" << std::endl;
    //如果窗口小化 就将其展示 
    if (IsIconic(hwnd)) {
        ShowWindow(hwnd, SW_RESTORE);
    }
    SetForegroundWindow(hwnd); // 将窗口置顶  
    rect = getWindowLoc(hwnd); // 窗口位置
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
    windowRect.x = rect.left;
    windowRect.y = rect.top;
    windowRect.width = width;
    windowRect.height = height;
    shotData = new char[width * height * 4];
    screenDC = GetDC(NULL);// 获取屏幕 DC
    compatibleDC = CreateCompatibleDC(screenDC);//兼容新DC
    // 创建位图
    hBitmap = CreateCompatibleBitmap(screenDC, width, height);
    SelectObject(compatibleDC, hBitmap);
    // 得到位图的数据
    BitBlt(compatibleDC, 0, 0, width, height, screenDC, rect.left, rect.top, SRCCOPY);
    GetBitmapBits(hBitmap, width * height * 4, shotData);
    // 创建图像
    cv::Mat windowMat(height, width, CV_8UC4, shotData);
    return windowMat;
}

//根据窗口标题是否包含该字符串，获得窗口截图
cv::Mat WindowShot::getWindowMat(const std::string& titleSection)
{
    hwnd = getWindowHWND(titleSection);
    if (!hwnd) {
  

    }

    //如果窗口小化 就将其展示 
    if (IsIconic(hwnd)) {
        ShowWindow(hwnd, SW_RESTORE);
    }
    SetForegroundWindow(hwnd); // 将窗口置顶  
    rect = getWindowLoc(hwnd); // 窗口位置
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
    windowRect.x = rect.left;
    windowRect.y = rect.top;
    windowRect.width = width;
    windowRect.height = height;
    shotData = new char[width * height * 4];
    screenDC = GetDC(NULL);// 获取屏幕 DC
    compatibleDC = CreateCompatibleDC(screenDC);//兼容新DC
    // 创建位图
    hBitmap = CreateCompatibleBitmap(screenDC, width, height);
    SelectObject(compatibleDC, hBitmap);
    // 得到位图的数据
    BitBlt(compatibleDC, 0, 0, width, height, screenDC, rect.left, rect.top, SRCCOPY);
    GetBitmapBits(hBitmap, width * height * 4, shotData);
    // 创建图像
    cv::Mat windowMat(height, width, CV_8UC4, shotData);
    return windowMat;
}
std::wstring WindowShot::toLower(const std::wstring& str) {
    std::wstring result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::towlower);
    return result;
}

std::wstring WindowShot::stringToWstring(const std::string& str) {
    if (str.empty()) {
        return std::wstring();
    }
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}
bool WindowShot::containsIgnoreCase(const std::wstring& str, const std::wstring& substr) {
    return toLower(str).find(toLower(substr)) != std::wstring::npos;
}
//根据窗口标题是否包含该字符串，获得窗口截图 将截图转为uchar* 供python使用
uchar* WindowShot::getWindowUchar(const std::string& titleSection)
{
    cv::Mat windowMat = this->getWindowMat(titleSection);
    int size = width * height * 4;
    free(windowUchar);
    windowUchar = (uchar*)malloc(sizeof(uchar) * size);
    memcpy(windowUchar, windowMat.data, size);
    return windowUchar;
}

cv::Mat WindowShot::getDesktopMat()
{
    width = GetSystemMetrics(SM_CXSCREEN) * zoom;
    height = GetSystemMetrics(SM_CYSCREEN) * zoom;
    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
    shotData = new char[width * height * 4];
    screenDC = GetDC(NULL);// 获取屏幕 DC
    compatibleDC = CreateCompatibleDC(screenDC);//兼容新DC
    // 创建位图
    hBitmap = CreateCompatibleBitmap(screenDC, width, height);
    SelectObject(compatibleDC, hBitmap);
    // 得到位图的数据
    BitBlt(compatibleDC, 0, 0, width, height, screenDC, rect.left, rect.top, SRCCOPY);
    GetBitmapBits(hBitmap, width * height * 4, shotData);
    // 创建图像
    cv::Mat desktopMat(height, width, CV_8UC4, shotData);
    return desktopMat;
}

/* 获取屏幕缩放值 */
double WindowShot::getZoom()
{
    // 获取窗口当前显示的监视器
    HWND hWnd = GetDesktopWindow();
    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    // 获取监视器逻辑宽度
    MONITORINFOEX monitorInfo;
    monitorInfo.cbSize = sizeof(monitorInfo);
    GetMonitorInfo(hMonitor, &monitorInfo);
    int cxLogical = (monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left);

    // 获取监视器物理宽度
    DEVMODE dm;
    dm.dmSize = sizeof(dm);
    dm.dmDriverExtra = 0;
    EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &dm);
    int cxPhysical = dm.dmPelsWidth;

    return cxPhysical * 1.0 / cxLogical;
}

WindowShot::~WindowShot()
{
    DeleteObject(hBitmap);
    DeleteDC(compatibleDC);
}

