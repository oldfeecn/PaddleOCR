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
    m_timer(new QTimer(this)), // 初始化定时器
    eventTimerStarted(false) 
{
    setFocusPolicy(Qt::StrongFocus); // Ensure focus policy is set
    setMouseTracking(true); // Enable mouse tracking
    installEventFilter(this);
    connect(m_timer, &QTimer::timeout, this, &vncWidget::updateFrame);
    //startCapture();
    // 打开设备，假设我们要打开第一个设备
    QString dllPath = "msdk.dll"; // 替换为 DLL 文件的实际路径
    if (!wrapper.loadLibrary(dllPath)) {
        qCritical() << "Failed to load DLL";
        return;
    }
}
void vncWidget::changeImgPath(QString m_imgPath){
    imgPath=m_imgPath;
}
void vncWidget::openVIDPID(){
   
   
    if (box_handle == nullptr) {
            bool ok;
            box_handle = wrapper.M_Open_VidPid(QString("1001").toInt(&ok, 16), QString("2001").toInt(&ok, 16));
         //qDebug() <<"Failed to open device " ;
    } else{
        wrapper.M_ResolutionUsed(box_handle,1920,1080);
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
        wrapper.M_Close(box_handle);
        box_handle = nullptr;
        //delete box_handle;
        //wrapper.deleteLater();
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
        wrapper.M_Close(box_handle);
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
        eventTimerStarted = false; // 重新启动计时器
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
    {
     QMutexLocker locker(&queueMutex);
        auto start = steady_clock::now();
        pixmap = m_dxgiManager->grabScreen();
        //auto end = steady_clock::now();
            if (!pixmap.isNull()) {
                m_pixmap = pixmap;
                update();
                QString eventInfo = concatenateQueue(eventQueue);
                //emit captureAndSaveImage(m_pixmap, imgPath,eventInfo);
                 // 获取当前时间的日期和时间
                QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
                QString filename = QString("%1%2.bmp").arg(imgPath).arg(timestamp);
                 if(SqlToolPool::insertData(Sql_dnf_jietu_inserts.arg(QString("%1.bmp").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz"))).arg(eventInfo))) {
                     if(!pixmap.save(filename,"bmp")){
                             qDebug()<<"save img error!!!";
                     };
                }
               
            } 
       
        auto end = steady_clock::now();
        m_grabDuration = duration_cast<milliseconds>(end - start);
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
    QString durationText = QString("Frame Grab Time: %1 ms").arg(m_grabDuration.count());
    painter.setPen(Qt::white);
    painter.drawText(10, height() - 10, durationText);
}
void vncWidget::keyPressEvent(QKeyEvent *event)
{   
   
    //qDebug() << "Key Pressed:" << event->key() << "Mouse Position:" << lastMousePosition;
    eventKey= event->nativeVirtualKey();
    int ret = wrapper.M_KeyDown2(box_handle, eventKey);
    //qDebug() << "Key Pressed:" << event->key() << "Mouse Position:" << key;
    logEvent(KeyMessage::KeyboardMessage, EventStatus::Pressed);
    QWidget::keyPressEvent(event); // 调用基类的事件处理
}

void vncWidget::keyReleaseEvent(QKeyEvent *event)
{
   
    eventKey = event->nativeVirtualKey();
    int ret = wrapper.M_KeyUp2(box_handle, eventKey);
    //qDebug() << "Key Pressed:" << event->key() << "Mouse Position:" << key;
    logEvent(KeyMessage::KeyboardMessage, EventStatus::Released);
    QWidget::keyReleaseEvent(event); // 调用基类的事件处理
}

void vncWidget::mousePressEvent(QMouseEvent *event)
{
     int ret ;
    lastMousePosition = event->pos();
    switch (event->button())
    {
    case Qt::LeftButton:
         ret = wrapper.M_LeftDown(box_handle);
        logEvent(KeyMessage::LeftButton, EventStatus::Pressed);
        break;
    case Qt::RightButton:
         ret = wrapper.M_RightDown(box_handle);
        logEvent(KeyMessage::RightButton, EventStatus::Pressed);
        break;
    case Qt::MiddleButton:
         ret = wrapper.M_MiddleDown(box_handle);
        logEvent(KeyMessage::MiddleButton, EventStatus::Pressed);
        break;    
    default:
        break;
    }
    
    QWidget::mousePressEvent(event); // 调用基类的事件处理
}

void vncWidget::mouseReleaseEvent(QMouseEvent *event)
{
   
    lastMousePosition = event->pos();
    int ret ;
     switch (event->button())
    {
    case Qt::LeftButton:
         ret= wrapper.M_LeftUp(box_handle);
        logEvent(KeyMessage::LeftButton, EventStatus::Released);
        break;
    case Qt::RightButton:
         ret = wrapper.M_RightUp(box_handle);
        logEvent(KeyMessage::RightButton, EventStatus::Released);
        break;
    case Qt::MiddleButton:
         ret = wrapper.M_MiddleUp(box_handle);
        logEvent(KeyMessage::MiddleButton, EventStatus::Released);
        break;    
    default:
        break;
    }
    
    QWidget::mouseReleaseEvent(event); // 调用基类的事件处理
}
void vncWidget::wheelEvent(QWheelEvent *event) {
        // 获取滚轮的角度，单位是 1/8 度
        int delta = event->angleDelta().y();
        
        // 正值表示向上滚动，负值表示向下滚动
        if (delta > 0) {
            qDebug() << "Mouse wheel scrolled up";
            logEvent(KeyMessage::MouseWhell, EventStatus::Pressed);
            wrapper.M_MouseWheel(box_handle,1);
        } else if (delta < 0) {
            qDebug() << "Mouse wheel scrolled down";
            logEvent(KeyMessage::MouseWhell, EventStatus::MouseWhellSta);
            wrapper.M_MouseWheel(box_handle,-1);
        }
        
        // 处理完事件后，调用基类的处理函数（如果需要）
        QWidget::wheelEvent(event);

}
void vncWidget::mouseMoveEvent(QMouseEvent *event)
{
   
    lastMousePosition = event->pos();
    //qDebug() << "Mouse Moved to:" << lastMousePosition;
    if (box_handle == nullptr) {
       // qDebug() <<"Failed to Not open device " ;
       
    }else{
        //qDebug() <<"Failed to  open device "<< lastMousePosition.x()<< lastMousePosition.y();
         //ret=wrapper.M_MoveTo3(deviceHandle,lastMousePosition.x(),lastMousePosition.y());
         //int ret = wrapper.M_MoveTo3(box_handle, x, y);
         int ret= wrapper.M_MoveTo3(box_handle,lastMousePosition.x(),lastMousePosition.y());
          
    }
    //int ret = YSDKDLL->moveTo(box_handle, x, y);
    logEvent(KeyMessage::MoveMessage, EventStatus::Pressed);
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
void vncWidget::logEvent(KeyMessage type, EventStatus status) {
   
    int delay = 0;
    if (eventTimerStarted) {
        delay = eventTimer.elapsed();
    } else {
        eventTimer.start();
        eventTimerStarted = true;
    }
    eventInfo = {type, status, lastMousePosition.x(),lastMousePosition.y(),eventKey,delay};
    QString eventLog = eventInfo.toString();
    // 使用 QMutexLocker 来自动管理锁
    {
        QMutexLocker locker(&queueMutex);
         eventQueue.enqueue(eventLog);
    }
   eventTimer.restart(); // 重启计时器以记录下一次事件的时间间隔
   //qDebug() << eventLog;
}

// 示例函数：遍历 QQueue 并拼接为一个 QString
QString vncWidget::concatenateQueue(QQueue<QString>& queue) {
   QString result;
   while (!queue.isEmpty()) {
        if (!result.isEmpty()) {
            result.append("");  // 添加逗号作为分隔符
        }
        result.append(queue.dequeue());
    }
    return result;
}