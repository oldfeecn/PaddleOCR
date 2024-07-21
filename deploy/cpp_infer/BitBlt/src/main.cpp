//  #include <opencv2/opencv.hpp>
//  #include <iostream>
//  #include "DesktopScreenshot.h"
//  #include <windows.h>
// #include <shellscalingapi.h> // 包含 GetDpiForMonitor 函数的定义

// #pragma comment(lib, "Shcore.lib") // 链接 Shcore.lib
// // #pragma comment(lib, "dwmapi.lib")


// //int main() {
// //	DesktopScreenshot ds;
// //	cv::Mat screenshot = ds.CaptureScreenshot();
// //	if (!screenshot.empty()) {
// //		// 保存截图为文件
// //		cv::imshow("Monitor Screenshot", screenshot);
// //		cv::waitKey(0);
// //		cv::imwrite("screenshot123456.png", screenshot);
// //		std::cout << "Screenshot saved as screenshot.png" << std::endl;
// //	}
// //	else {
// //		std::cerr << "Failed to capture screenshot." << std::endl;
// //	}
// //
// //	return 0;
// //}
// // 回调函数，用于监视器枚举


// int main() {
//     DISPLAY_DEVICE dd;
//     DEVMODE dm;
//     dd.cb = sizeof(dd);
//     dm.dmSize = sizeof(dm);
//     DesktopScreenshot ds;
//     for (DWORD i = 0; ; i++) {
//         if (!EnumDisplayDevicesA(NULL, i, &dd, 0)) {
//             break;
//         }
//             if (dd.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) {
//                 std::cout << "Display " << i << " Name: " << dd.DeviceName << std::endl;
//                 std::cout << "Display " << i << " Device ID: " << dd.DeviceID << std::endl;
//                 std::cout << "Display " << i << " Description: " << dd.DeviceString << std::endl;
//                 std::cout << "Display " << i << " State Flags: " << dd.StateFlags << std::endl;
//                 if (EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm)) {
//                      const WCHAR* desktopName = L"\\.\DISPLAY1"; // 替换为你想捕获的显示器名称
//                      cv::Mat screenshot = ds.CaptureScreenWithDXGI();
//                    // cv::Mat screenshot = ds.CaptureScreenWithDXGI(dd.DeviceName);
//                     cv::imshow("Monitor Screenshot", screenshot);
//                     cv::imwrite("screenshot123456.png", screenshot);
//                     cv::waitKey(0);
//                     std::cout << "Screenshot saved as screenshot.png" << std::endl;
//                 }
//                 else {
//                     std::cout << "Display " << i << " settings could not be retrieved." << std::endl;
//                 }
//         }
//     }

//     return 0;
// }

#include <d3d11.h>
#include <dxgi1_2.h>
#include <wincodec.h>
#include <iostream>
#include <gflags/gflags.h>
