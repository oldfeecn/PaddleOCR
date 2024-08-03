#include "vncWidget.h"
#include <QPainter>
#include <QDebug>
#include <chrono>


using namespace std::chrono;
HANDLE box_handle=nullptr;
vncWidget::vncWidget(QWidget *parent) :
    QWidget(parent),
    m_dxgiManager(new DxgiManager()), // 初始化指针
    m_grabDuration(0),
    m_timer(new QTimer(this)) // 初始化定时器
{
    setFocusPolicy(Qt::StrongFocus); // Ensure focus policy is set
    setMouseTracking(true); // Enable mouse tracking
    installEventFilter(this);
    connect(m_timer, &QTimer::timeout, this, &vncWidget::updateFrame);
    //startCapture();
    // 打开设备，假设我们要打开第一个设备
    
    
}
void vncWidget::openVIDPID(){
   
   
    if (box_handle == nullptr) {
            bool ok;
            box_handle = M_Open_VidPid(QString("1001").toInt(&ok, 16), QString("2001").toInt(&ok, 16));
         qDebug() <<"Failed to open device " ;
    } else{
        M_ResolutionUsed(box_handle,1920,1080);
        qDebug() <<" open device secc " ;
        qDebug() <<box_handle ;
    }
}
void vncWidget::CloseVIDPID()
{
       
    try {
        /* code */ 
        if (box_handle != nullptr) {
        qDebug() <<box_handle ;
        M_Close(box_handle);
        delete box_handle;
       }
       
    } catch (const std::exception &e) {
      
        qDebug() <<"Failed to open device" ;
    } catch (...) {
      
        qDebug() <<"Failed to open device" ;
    } 
    
}
vncWidget::~vncWidget()
{
    stopCapture();
     if (m_dxgiManager) {
        delete m_dxgiManager;
        m_dxgiManager = nullptr;
    }
    if (box_handle != nullptr) {
        qDebug() <<"Failed to open device" ;
        M_Close(box_handle);
       //delete box_handle;
    }
}

void vncWidget::startCapture(int outputIndex)
{
    
     if (m_dxgiManager) {
        delete m_dxgiManager;
        m_dxgiManager = new DxgiManager();
    }
    if (!m_dxgiManager->init(outputIndex)) {
        qDebug() << "DXGI Manager Initialization Failed:" << m_dxgiManager->lastError();
        return;
    }
    if (!m_timer->isActive()) {
        m_timer->start(1000 / 30);  // 每秒30帧
    }
}

void vncWidget::stopCapture()
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }
}

void vncWidget::updateFrame()
{
    //auto start = steady_clock::now();
     pixmap = m_dxgiManager->grabScreen();
    //auto end = steady_clock::now();
    //m_grabDuration = duration_cast<milliseconds>(end - start);

    if (!pixmap.isNull()) {
        m_pixmap = pixmap;
        update();
    } else {
        //qDebug() << "Failed to grab screen:" << m_dxgiManager->lastError();
    }
}

void vncWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    if (!m_pixmap.isNull()) {
        painter.drawPixmap(0, 0, m_pixmap);
    }

    // 在底部显示截图耗时
    //QString durationText = QString("Frame Grab Time: %1 ms").arg(m_grabDuration.count());
    //painter.setPen(Qt::white);
    //painter.drawText(10, height() - 10, durationText);
}
void vncWidget::keyPressEvent(QKeyEvent *event)
{   
    //qDebug() << "Key Pressed:" << event->key() << "Mouse Position:" << lastMousePosition;
    int key = event->nativeVirtualKey();
    int ret = M_KeyDown2(box_handle, key);
    qDebug() << "Key Pressed:" << event->key() << "Mouse Position:" << key;
    QWidget::keyPressEvent(event); // 调用基类的事件处理
}

void vncWidget::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->nativeVirtualKey();
    int ret = M_KeyUp2(box_handle, key);
    qDebug() << "Key Pressed:" << event->key() << "Mouse Position:" << key;
    QWidget::keyReleaseEvent(event); // 调用基类的事件处理
}

void vncWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos();
    if (event->button() == Qt::LeftButton) {
        int ret = M_LeftDown(box_handle);
    }
    else if (event->button() == Qt::RightButton) {
        int ret = M_RightDown(box_handle);
    }
    else if (event->button() == Qt::MiddleButton) {
        int ret = M_MiddleDown(box_handle);
    }

    QWidget::mousePressEvent(event); // 调用基类的事件处理
}

void vncWidget::mouseReleaseEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos();
    if (event->button() == Qt::LeftButton) {
        int ret = M_LeftUp(box_handle);
    }
    else if (event->button() == Qt::RightButton) {
        int ret = M_RightUp(box_handle);
    }
    else if (event->button() == Qt::MiddleButton) {
        int ret = M_MiddleUp(box_handle);
    }
    
    QWidget::mouseReleaseEvent(event); // 调用基类的事件处理
}

void vncWidget::mouseMoveEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos();
    qDebug() << "Mouse Moved to:" << lastMousePosition;
    if (box_handle == nullptr) {
        qDebug() <<"Failed to Not open device " ;
       
    }else{
        qDebug() <<"Failed to  open device "<< lastMousePosition.x()<< lastMousePosition.y();
         //ret=M_MoveTo3(deviceHandle,lastMousePosition.x(),lastMousePosition.y());
         //int ret = M_MoveTo3(box_handle, x, y);
         int ret= M_MoveTo3(box_handle,lastMousePosition.x(),lastMousePosition.y());
          
    }
    //int ret = YSDKDLL->moveTo(box_handle, x, y);
    QWidget::mouseMoveEvent(event); // 调用基类的事件处理
}

void vncWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    setFocus(); // 确保 widget 能获得焦点
}

bool vncWidget::eventFilter(QObject *obj, QEvent *event)
{
    return QWidget::eventFilter(obj, event);
}