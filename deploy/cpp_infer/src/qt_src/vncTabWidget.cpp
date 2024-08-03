#include "vncTabWidget.h"
#include <setupapi.h> // 确保包含此头文件

vncTabWidget::vncTabWidget(QWidget *parent)
	: QWidget(parent)
{
   
	ui.setupUi(this);
   	ui.vncDisConnectbtn->setDisabled(true);
    
    ui.vnc_widget->changeImgPath(ui.SaveImg_lineEdit->text());
	//SqlToolPool::test();
	//连接截图
    connect(ui.vncConnectbtn, &QPushButton::clicked,
		 this, [this](bool checked) {
         ui.vncDisConnectbtn->setDisabled(false);
			ui.vnc_widget->startCapture(ui.spinBox_outputIndex->value());
		 });
    connect(ui.vncDisConnectbtn, &QPushButton::clicked,
		 this, [this](bool checked) {
          ui.vncDisConnectbtn->setDisabled(true);
			 ui.vncConnectbtn->setDisabled(false);
			 ui.vnc_widget->stopCapture();
		 });  
	//启动虚拟机
    connect(ui.startVMBtn, &QPushButton::clicked,
        this, [this](bool checked) {
            // 虚拟机路径
            vmPath = "D:\\神盾虚拟机\\810648297\\810648297.vmx";
            startVM(vmPath);
		});
    //移动虚拟机
    connect(ui.moveVMBtn, &QPushButton::clicked,
        this, [this](bool checked) {
            moveVMAndMoveToScreen(2);
		});  
    //复位虚拟机
    connect(ui.moveVMBtn2, &QPushButton::clicked,
        this, [this](bool checked) {
            moveVMAndMoveToScreen(0);
        });
         //连接键盘
    connect(ui.keyMouseBtnOpen, &QPushButton::clicked,
        this, [this](bool checked) {
            ui.vnc_widget->openVIDPID();
           	});   
         //断开键盘
    connect(ui.keyMouseBtnClose, &QPushButton::clicked,
        this, [this](bool checked) {
            ui.vnc_widget->CloseVIDPID();
		});   
          //图片保存路径
    connect(ui.SaveImg_lineEdit, &QLineEdit::textChanged,
        this, [this](QString path) {
            ui.vnc_widget->imgPath=path;
		});   
    
}
void vncTabWidget::moveVMAndMoveToScreen(int screenIndex=2) {
    findWindowByTitle(QString("810648297 - VMware Workstation"),screenIndex);
}
void vncTabWidget::startVM(const QString& vmPath) {
        // 启动虚拟机
        QProcess::startDetached("\"C:\\Program Files (x86)\\VMware\\VMware Workstation\\vmrun.exe\" start " + vmPath);
}

// 回调函数，用于枚举窗口
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    // lParam 传递的是指向 QString 的指针
    auto* params = reinterpret_cast<std::pair<QString*, int>*>(lParam);
    QString* targetTitle =  params->first;
    int monitorIndex = params->second;

    wchar_t windowTitle[256];
    GetWindowTextW(hwnd, windowTitle, sizeof(windowTitle) / sizeof(wchar_t));
    QString currentTitle = QString::fromWCharArray(windowTitle);

    // 判断窗口标题是否包含目标子串
    if (currentTitle.contains(*targetTitle, Qt::CaseInsensitive)) {
        qDebug() << "找到包含目标子串的窗口句柄:" << hwnd << "标题:" << currentTitle;
        // 获取第三显示屏的矩形区域
        RECT monitorRect = getMonitors()[monitorIndex]; // 第三显示屏的索引是2（从0开始计数）

        // 确保获取到有效的显示器矩形区域
        if (monitorRect.right > monitorRect.left && monitorRect.bottom > monitorRect.top) {
            // 移动窗口到第三显示屏并全屏显示
            SetWindowPos(hwnd, HWND_TOPMOST, monitorRect.left, monitorRect.top,
                monitorRect.right - monitorRect.left, monitorRect.bottom - monitorRect.top,
                SWP_NOACTIVATE | SWP_NOZORDER);
            // 取消窗口的最上层状态（可选）
            SetWindowPos(hwnd, HWND_NOTOPMOST, monitorRect.left, monitorRect.top,
                monitorRect.right - monitorRect.left, monitorRect.bottom - monitorRect.top,
                SWP_NOACTIVATE | SWP_NOZORDER);
        }
        else {
            qDebug() << "无法移动窗口，第三显示屏的矩形区域无效";
        }
        

        
    }

    return TRUE; // 返回 TRUE 以继续枚举窗口
}
std::vector<RECT> getMonitors() {
    std::vector<RECT> monitors;

    // 回调函数，用于枚举显示器
    auto enumMonitorsProc = [](HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) -> BOOL {
        auto* monitorRects = reinterpret_cast<std::vector<RECT>*>(dwData);
        MONITORINFO monitorInfo;
        monitorInfo.cbSize = sizeof(MONITORINFO); // 正确初始化 cbSize
        if (GetMonitorInfo(hMonitor, &monitorInfo)) {
            monitorRects->push_back(monitorInfo.rcMonitor);
        }
        return TRUE; // 继续枚举
    };

    // 枚举显示器
    EnumDisplayMonitors(NULL, NULL, enumMonitorsProc, reinterpret_cast<LPARAM>(&monitors));

    return monitors;
}
// 定义一个结构体来存储参数
struct EnumParams {
    QString* targetTitle;
    int monitorIndex;
};
// 查找窗口并处理
void vncTabWidget::findWindowByTitle(QString title, int monitorIndex) {
    EnumParams params = { &title, monitorIndex };
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&params));
}

vncTabWidget::~vncTabWidget()
{
	
}

