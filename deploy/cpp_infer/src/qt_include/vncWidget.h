#ifndef VNCWIDGET_H
#define VNCWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include "DxgiManager.h"

class vncWidget : public QWidget
{
    Q_OBJECT

public:
    explicit vncWidget(QWidget *parent = nullptr);
    ~vncWidget();

public slots:
    void startCapture(int outputIndex);
    void stopCapture();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateFrame();

private:
    DxgiManager* m_dxgiManager;
    QPixmap m_pixmap;
    QTimer* m_timer;
    std::chrono::milliseconds m_grabDuration;
};

#endif // VNCWIDGET_H
