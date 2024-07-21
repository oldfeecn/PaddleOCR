#ifndef DESKTOPSCREENSHOT_H
#define DESKTOPSCREENSHOT_H

#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl/client.h>
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>  // std::min

using namespace Microsoft::WRL;

class DesktopScreenshot {
public:
    DesktopScreenshot();

    // 截取整个桌面的屏幕截图并返回一个 cv::Mat 对象
    
    cv::Mat CaptureScreenshotGDI();
    cv::Mat CaptureVirtualDesktopScreenshot(CHAR* desktopName); 
    cv::Mat CaptureScreenWithDXGI(char* desktopName);
    cv::Mat CaptureScreenWithDXGI(); // 默认捕获方法
private:
    void PrintPixelData(void* data, int width, int height, int rowPitch);
    std::vector<wchar_t> ConvertToWideChar(const char* input);
    std::string ConvertToMultiByte(const wchar_t* input);
    HRESULT GetOutputFromDesktopName(const wchar_t* desktopName, ComPtr<IDXGIAdapter>* adapterOut, ComPtr<IDXGIOutput>* outputOut);

    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
};

#endif // DESKTOPSCREENSHOT_H
