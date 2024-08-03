#ifndef VNCWIDGET_H
#define VNCWIDGET_H
#include "EGlobalVariable.h"
#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include "DxgiManager.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QThread>
//#include "msdk.h"
//#include "HandleWrapper.h"
#include "MSDKWrapper.h"
#include "eventinfo.h"
#include <QElapsedTimer>
#include <QMutex>
#include <QDateTime>
#include <QImageWriter>
#include "SqlToolPool.h"
#include "Databasesql.h"
class vncWidget : public QWidget
{
    Q_OBJECT

public:
    explicit vncWidget(QWidget *parent = nullptr);
    void changeImgPath(QString m_imgPath);
    ~vncWidget();
    QString imgPath;
   
public slots:
    void startCapture(int outputIndex);
    void stopCapture();
    void openVIDPID();
    void CloseVIDPID();
    void logEvent(KeyMessage type, EventStatus status);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
private slots:
    void updateFrame();
signals:    
    void captureAndSaveImage( QPixmap pixmap,  QString imgPath,QString eventInfo);
public:
    DxgiManager* m_dxgiManager;
    QPixmap m_pixmap;
    QPixmap pixmap;
    QTimer* m_timer;
    std::chrono::milliseconds m_grabDuration;
    QPoint lastMousePosition;
    bool eventFilter(QObject *obj, QEvent *event) override;
    MSDKWrapper wrapper;
    QQueue<QString> eventQueue;
    int eventKey=0 ;
    EventInfo eventInfo;
    QMutex queueMutex;          // 互斥锁用于保护 eventQueue
    QElapsedTimer eventTimer; // 用于跟踪事件间隔时间
    bool eventTimerStarted;   // 标记计时器是否已经启动
    // 示例函数：遍历 QQueue 并拼接为一个 QString
    QString concatenateQueue(QQueue<QString>& queue);
   
};

#endif // VNCWIDGET_H
