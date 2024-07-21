#include "vncWidget.h"
#include <QPainter>
#include <QDebug>
#include <chrono>

using namespace std::chrono;

vncWidget::vncWidget(QWidget *parent) :
    QWidget(parent),
    m_dxgiManager(new DxgiManager()), // 初始化指针
    m_grabDuration(0),
    m_timer(new QTimer(this)) // 初始化定时器
{
    if (!m_dxgiManager->init(1)) {
        qDebug() << "DXGI Manager Initialization Failed:" << m_dxgiManager->lastError();
        return;
    }
    
    connect(m_timer, &QTimer::timeout, this, &vncWidget::updateFrame);
    //startCapture();
}

vncWidget::~vncWidget()
{
    stopCapture();
     if (m_dxgiManager) {
        delete m_dxgiManager;
        m_dxgiManager = nullptr;
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
    QPixmap pixmap = m_dxgiManager->grabScreen();
    //auto end = steady_clock::now();
    //m_grabDuration = duration_cast<milliseconds>(end - start);

    if (!pixmap.isNull()) {
        m_pixmap = pixmap;
        update();
    } else {
        qDebug() << "Failed to grab screen:" << m_dxgiManager->lastError();
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
