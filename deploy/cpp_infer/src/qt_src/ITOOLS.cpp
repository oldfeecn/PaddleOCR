#include "ITOOLS.h"


ITOOLS::ITOOLS(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_ITOOLS)
{
    ui->setupUi(this);
    InitUiLogo();
    setStyleSheet("background-color:#FFFFCC");
    //ui->myqtabWidget->setAutoFillBackground(false);
}

ITOOLS::~ITOOLS()
{
    delete ui; 
}

void ITOOLS::closeEvent(QCloseEvent *event)
{
     this->hide();
    event->ignore();
}

void ITOOLS::InitUiLogo()
{
    setFixedSize(QSize(EGlobalVariable::instance()->screen_width * 0.8, EGlobalVariable::instance()->screen_height - 120));
    setMinimumSize(0, 0);
    setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    move(EGlobalVariable::instance()->screen_width * 0.1, 15);
    setWindowTitle(QStringLiteral("ITOOLS"));
    setWindowIcon(QIcon(":/ITOOLS/imgs/itools.png"));//可执行程序图标
    QSystemTrayIcon* m_systemIcon = new QSystemTrayIcon(this);
    m_systemIcon->setToolTip("ITOOLS"); //设置系统托盘提示文字    
    m_systemIcon->setIcon(QIcon(":/ITOOLS/imgs/itools.png"));//添加系统托盘图标
    QMenu* meau = new QMenu();
    QAction* actionOpenMain = new QAction(QIcon(":/ITOOLS/imgs/itools.png"), QStringLiteral("打开主页"), this);
    meau->addAction(actionOpenMain);//打开主页
    QAction* actionMin = new QAction(QIcon(":/ITOOLS/imgs/min.png"), QStringLiteral("最小化"), this);
    meau->addAction(actionMin);//最小化
    QAction* actionExit = new QAction(QIcon(":/ITOOLS/imgs/Close.png"), QStringLiteral("退出程序"), this);
    meau->addSeparator();//分界线
    meau->addAction(actionExit);//退出
    m_systemIcon->setContextMenu(meau);
    //显示托盘图标

    m_systemIcon->show();
    //托盘图标单击和双击事件
    connect(m_systemIcon, &QSystemTrayIcon::activated,
        this, [this](QSystemTrayIcon::ActivationReason reason) {
            if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick)
            {
                this->showNormal();
                //一下两句缺一均不能有效将窗口置顶
                /*this->setWindowState(Qt::WindowActive);
                this->activateWindow();*/
            }
        });
    //打开主页事件
    connect(actionOpenMain, &QAction::triggered, this, [this](bool checked = false) {
        //激活窗口,提到前面
        this->activateWindow();
        this->showNormal();
        });
    //最下化托盘事件
    connect(actionMin, &QAction::triggered, this, [this](bool checked = false) {
        if (windowState() != Qt::WindowMinimized) {
            setWindowState(Qt::WindowMinimized);
        }
        });
    //退出程序事件
    connect(actionExit, &QAction::triggered, this, [this](bool checked = false) {
        QApplication::exit(0);
        });
    //ui.statusBar
}
