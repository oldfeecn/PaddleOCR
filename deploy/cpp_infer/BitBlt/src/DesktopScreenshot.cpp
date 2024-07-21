 #include "DesktopScreenshot.h"
cv::Mat DesktopScreenshot::CaptureScreenshotGDI()
{
    // 获取屏幕的设备上下文
    HDC hScreenDC = GetDC(NULL);
    if (!hScreenDC) {
        std::cerr << "Failed to get device context." << std::endl;
        return cv::Mat();
    }

    // 获取屏幕的逻辑宽度和高度
    int logicalWidth = GetSystemMetrics(SM_CXSCREEN);
    int logicalHeight = GetSystemMetrics(SM_CYSCREEN);

    // 获取DPI缩放比例
    HDC hDesktopDC = GetDC(NULL);
    float dpiX = static_cast<float>(GetDeviceCaps(hDesktopDC, LOGPIXELSX)) / 96.0f;
    float dpiY = static_cast<float>(GetDeviceCaps(hDesktopDC, LOGPIXELSY)) / 96.0f;
    ReleaseDC(NULL, hDesktopDC);

    // 计算物理宽度和高度
    int physicalWidth = static_cast<int>(logicalWidth * dpiX);
    int physicalHeight = static_cast<int>(logicalHeight * dpiY);

    // 创建一个与屏幕兼容的位图
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    if (!hMemoryDC) {
        std::cerr << "Failed to create compatible memory device context." << std::endl;
        ReleaseDC(NULL, hScreenDC);
        return cv::Mat();
    }

    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, physicalWidth, physicalHeight);
    if (!hBitmap) {
        std::cerr << "Failed to create compatible bitmap." << std::endl;
        DeleteDC(hMemoryDC);
        ReleaseDC(NULL, hScreenDC);
        return cv::Mat();
    }

    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

    // 将屏幕内容复制到内存设备上下文中
    if (!BitBlt(hMemoryDC, 0, 0, physicalWidth, physicalHeight, hScreenDC, 0, 0, SRCCOPY)) {
        std::cerr << "Failed to capture the screen." << std::endl;
        SelectObject(hMemoryDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hMemoryDC);
        ReleaseDC(NULL, hScreenDC);
        return cv::Mat();
    }

    // 创建一个 OpenCV 矩阵来存放位图数据
    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = physicalWidth;
    bi.biHeight = -physicalHeight; // 负值表示位图是自上而下的
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    cv::Mat screenshot(physicalHeight, physicalWidth, CV_8UC4);
    if (!screenshot.isContinuous()) {
        std::cerr << "Failed to create continuous cv::Mat." << std::endl;
        SelectObject(hMemoryDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hMemoryDC);
        ReleaseDC(NULL, hScreenDC);
        return cv::Mat();
    }

    // 获取位图数据
    if (GetDIBits(hMemoryDC, hBitmap, 0, physicalHeight, screenshot.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS) == 0) {
        std::cerr << "Failed to get DIBits." << std::endl;
        SelectObject(hMemoryDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hMemoryDC);
        ReleaseDC(NULL, hScreenDC);
        return cv::Mat();
    }

    // 释放资源
    SelectObject(hMemoryDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);

    return screenshot;
}


/// @brief 
/// @param desktopName 
/// @return 
cv::Mat DesktopScreenshot::CaptureVirtualDesktopScreenshot(CHAR* desktopName) {
        // 获取显示器的设备上下文
        HDC hScreenDC = CreateDC(NULL, desktopName, NULL, NULL);
        if (!hScreenDC) {
            std::cerr << "Failed to create device context for display: " << desktopName << std::endl;
            return cv::Mat();
        }

        // 获取屏幕的宽度和高度
        int dpiX = GetDeviceCaps(hScreenDC, HORZRES);
        int dpiY = GetDeviceCaps(hScreenDC, VERTRES);
        // 获取屏幕的 DPI
        // 获取物理 DPI
        int  width = GetDeviceCaps(hScreenDC, DESKTOPHORZRES);
        int  height = GetDeviceCaps(hScreenDC, DESKTOPVERTRES);

        // 计算 DPI 缩放比例
        float dpiScalingFactorX = static_cast<float>(width) / dpiX;
        float dpiScalingFactorY = static_cast<float>(height) / dpiY;
        
        std::cout << "Display "  << " Width: " << width << std::endl;
        std::cout << "Display "  << " Height: " << height << std::endl;
        // 输出 DPI 信息
        std::cout << "Screen DPI (X, Y): (" << dpiX << ", " << dpiY << ")" << std::endl;
        std::cout << "Screen DPI (dpiScalingFactorX, dpiScalingFactorY): (" << dpiScalingFactorX << ", " << dpiScalingFactorY << ")" << std::endl;
        // 创建一个与屏幕兼容的位图
        HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
        if (!hBitmap) {
            std::cerr << "Failed to create compatible bitmap." << std::endl;
            DeleteDC(hScreenDC);
            return cv::Mat();
        }

        HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
        if (!hMemoryDC) {
            std::cerr << "Failed to create memory device context." << std::endl;
            DeleteObject(hBitmap);
            DeleteDC(hScreenDC);
            return cv::Mat();
        }

        HGDIOBJ oldObject = SelectObject(hMemoryDC, hBitmap);

        // 将屏幕内容复制到内存设备上下文中
        if (!BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY)) {
            std::cerr << "Failed to capture the screen." << std::endl;
            SelectObject(hMemoryDC, oldObject);
            DeleteObject(hBitmap);
            DeleteDC(hMemoryDC);
            DeleteDC(hScreenDC);
            return cv::Mat();
        }

        // 创建一个 OpenCV 矩阵来存放位图数据
        BITMAPINFOHEADER bi;
        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = width;
        bi.biHeight = -height; // 负值表示位图是自上而下的
        bi.biPlanes = 1;
        bi.biBitCount = 32;
        bi.biCompression = BI_RGB;
        bi.biSizeImage = 0;
        bi.biXPelsPerMeter = 0;
        bi.biYPelsPerMeter = 0;
        bi.biClrUsed = 0;
        bi.biClrImportant = 0;

        cv::Mat screenshot(height, width, CV_8UC4);
        if (!screenshot.isContinuous()) {
            std::cerr << "Failed to create continuous cv::Mat." << std::endl;
            SelectObject(hMemoryDC, oldObject);
            DeleteObject(hBitmap);
            DeleteDC(hMemoryDC);
            DeleteDC(hScreenDC);
            return cv::Mat();
        }

        GetDIBits(hMemoryDC, hBitmap, 0, height, screenshot.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

        // 释放资源
        SelectObject(hMemoryDC, oldObject);
        DeleteObject(hBitmap);
        DeleteDC(hMemoryDC);
        DeleteDC(hScreenDC);

        return screenshot;
    }

DesktopScreenshot::DesktopScreenshot() {
    // 初始化D3D11设备
    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
        D3D11_SDK_VERSION, &device, &featureLevel, &context);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create D3D11 device.");
    }
}

cv::Mat DesktopScreenshot::CaptureScreenWithDXGI(char* desktopName) {
    // 将 char* 转换为 wchar_t*
    std::vector<wchar_t> wDesktopName = ConvertToWideChar(desktopName);

    ComPtr<IDXGIOutputDuplication> deskDupl;
    ComPtr<IDXGIAdapter> adapter;
    ComPtr<IDXGIOutput> output;
    HRESULT hr = GetOutputFromDesktopName(wDesktopName.data(), &adapter, &output);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to get output from desktop name.");
    }

    ComPtr<IDXGIOutput1> output1;
    output.As(&output1);

    hr = output1->DuplicateOutput(device.Get(), &deskDupl);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to duplicate output.");
    }

    DXGI_OUTDUPL_FRAME_INFO frameInfo;
    ComPtr<IDXGIResource> desktopResource;
    hr = deskDupl->AcquireNextFrame(500, &frameInfo, &desktopResource);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to acquire next frame.");
    }

    ComPtr<ID3D11Texture2D> desktopImage;
    desktopResource.As(&desktopImage);

    D3D11_TEXTURE2D_DESC desc;
    desktopImage->GetDesc(&desc);

    D3D11_TEXTURE2D_DESC cpuDesc = desc;
    cpuDesc.Usage = D3D11_USAGE_STAGING;
    cpuDesc.BindFlags = 0;
    cpuDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    cpuDesc.MiscFlags = 0;
    ComPtr<ID3D11Texture2D> cpuTexture;
    hr = device->CreateTexture2D(&cpuDesc, nullptr, &cpuTexture);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create CPU accessible texture.");
    }

    context->CopyResource(cpuTexture.Get(), desktopImage.Get());

    D3D11_MAPPED_SUBRESOURCE mapped;
    hr = context->Map(cpuTexture.Get(), 0, D3D11_MAP_READ, 0, &mapped);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to map texture.");
    }

    // 打印前几个像素的 RGBA 值
    PrintPixelData(mapped.pData, desc.Width, desc.Height, mapped.RowPitch);
    cv::Mat image(desc.Height, desc.Width, CV_8UC4, mapped.pData, mapped.RowPitch);

    context->Unmap(cpuTexture.Get(), 0);
    deskDupl->ReleaseFrame();

    return image.clone();
}

void DesktopScreenshot::PrintPixelData(void* data, int width, int height, int rowPitch) {
    unsigned char* pixelData = static_cast<unsigned char*>(data);
    int pixelCount = std::min(width * height, 10); // 打印前100个像素的数据

    for (int i = 0; i < pixelCount; ++i) {
        unsigned char* pixel = pixelData + i * 4; // 假设每个像素4个字节RGBA
        std::cout << "Pixel " << i << ": R=" << static_cast<int>(pixel[0])
            << " G=" << static_cast<int>(pixel[1])
            << " B=" << static_cast<int>(pixel[2])
            << " A=" << static_cast<int>(pixel[3]) << std::endl;
    }
}

std::vector<wchar_t> DesktopScreenshot::ConvertToWideChar(const char* input) {
    int len = MultiByteToWideChar(CP_ACP, 0, input, -1, nullptr, 0);
    if (len == 0) {
        throw std::runtime_error("Failed to convert to wide char.");
    }

    std::vector<wchar_t> output(len);
    MultiByteToWideChar(CP_ACP, 0, input, -1, output.data(), len);
    return output;
}

std::string DesktopScreenshot::ConvertToMultiByte(const wchar_t* input) {
    int len = WideCharToMultiByte(CP_ACP, 0, input, -1, nullptr, 0, nullptr, nullptr);
    if (len == 0) {
        throw std::runtime_error("Failed to convert to multi-byte char.");
    }

    std::vector<char> output(len);
    WideCharToMultiByte(CP_ACP, 0, input, -1, output.data(), len, nullptr, nullptr);
    return std::string(output.data());
}

HRESULT DesktopScreenshot::GetOutputFromDesktopName(const wchar_t* desktopName, ComPtr<IDXGIAdapter>* adapterOut, ComPtr<IDXGIOutput>* outputOut) {
    ComPtr<IDXGIFactory1> factory;
    HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(factory.GetAddressOf()));
    if (FAILED(hr)) {
        return hr;
    }

    for (UINT adapterIndex = 0; ; ++adapterIndex) {
        ComPtr<IDXGIAdapter> adapter;
        if (factory->EnumAdapters(adapterIndex, &adapter) == DXGI_ERROR_NOT_FOUND) {
            break;
        }

        for (UINT outputIndex = 0; ; ++outputIndex) {
            ComPtr<IDXGIOutput> output;
            if (adapter->EnumOutputs(outputIndex, &output) == DXGI_ERROR_NOT_FOUND) {
                break;
            }

            DXGI_OUTPUT_DESC desc;
            output->GetDesc(&desc);
            // 输出设备名称
            std::string deviceName = ConvertToMultiByte(desc.DeviceName);
            std::cout << "================ " << deviceName << std::endl;

            if (wcscmp(desc.DeviceName, desktopName) == 0) {
                *adapterOut = adapter;
                *outputOut = output;
                return S_OK;
            }
        }
    }

    return E_FAIL;
}

cv::Mat DesktopScreenshot::CaptureScreenWithDXGI() {
    // 默认捕获第一个输出
    ComPtr<IDXGIFactory1> factory;
    HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(factory.GetAddressOf()));
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create DXGI factory.");
    }

    ComPtr<IDXGIAdapter> adapter;
    hr = factory->EnumAdapters(0, &adapter);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to enumerate adapters.");
    }

    ComPtr<IDXGIOutput> output;
    hr = adapter->EnumOutputs(0, &output);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to enumerate outputs.");
    }

    ComPtr<IDXGIOutput1> output1;
    output.As(&output1);

    ComPtr<IDXGIOutputDuplication> deskDupl;
    hr = output1->DuplicateOutput(device.Get(), &deskDupl);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to duplicate output.");
    }

    DXGI_OUTDUPL_FRAME_INFO frameInfo;
    ComPtr<IDXGIResource> desktopResource;
    hr = deskDupl->AcquireNextFrame(500, &frameInfo, &desktopResource);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to acquire next frame.");
    }

    ComPtr<ID3D11Texture2D> desktopImage;
    desktopResource.As(&desktopImage);

    D3D11_TEXTURE2D_DESC desc;
    desktopImage->GetDesc(&desc);

    D3D11_TEXTURE2D_DESC cpuDesc = desc;
    cpuDesc.Usage = D3D11_USAGE_STAGING;
    cpuDesc.BindFlags = 0;
    cpuDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    cpuDesc.MiscFlags = 0;
    ComPtr<ID3D11Texture2D> cpuTexture;
    hr = device->CreateTexture2D(&cpuDesc, nullptr, &cpuTexture);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create CPU accessible texture.");
    }

    context->CopyResource(cpuTexture.Get(), desktopImage.Get());

    D3D11_MAPPED_SUBRESOURCE mapped;
    hr = context->Map(cpuTexture.Get(), 0, D3D11_MAP_READ, 0, &mapped);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to map texture.");
    }

    // 打印前几个像素的 RGBA 值
    PrintPixelData(mapped.pData, desc.Width, desc.Height, mapped.RowPitch);
    cv::Mat image(desc.Height, desc.Width, CV_8UC4, mapped.pData, mapped.RowPitch);

    context->Unmap(cpuTexture.Get(), 0);
    deskDupl->ReleaseFrame();

    return image.clone();
}