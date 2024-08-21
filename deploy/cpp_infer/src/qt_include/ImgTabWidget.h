#ifndef IMGTABWIDGET_H
#define IMGTABWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QMouseEvent>
#include <QObject>
#include <QEvent>
#include "SqlToolPool.h"
#include "ButtonDelegate.h"
#include <QStringListModel>
#include <QDebug>
namespace Ui {
class ImgTabWidget;
}

class ImgTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImgTabWidget(QWidget *parent = nullptr);
    void initDate();
    ~ImgTabWidget();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
private:
    QString srcImgPath;
    QString recImgPath;
    Ui::ImgTabWidget *ui;
    QStringListModel* model;
};

#endif // IMGTABWIDGET_H
