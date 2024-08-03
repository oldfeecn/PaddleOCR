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
#include "msdk.h"
#include "HandleWrapper.h"
class vncWidget : public QWidget
{
    Q_OBJECT

public:
    explicit vncWidget(QWidget *parent = nullptr);
    ~vncWidget();
public slots:
    void startCapture(int outputIndex);
    void stopCapture();
    void openVIDPID();
    void CloseVIDPID();
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void updateFrame();

private:
    DxgiManager* m_dxgiManager;
    QPixmap m_pixmap;
    QPixmap pixmap;
    QTimer* m_timer;
    std::chrono::milliseconds m_grabDuration;
    QPoint lastMousePosition;
    bool eventFilter(QObject *obj, QEvent *event) override;
    
};

#endif // VNCWIDGET_H
