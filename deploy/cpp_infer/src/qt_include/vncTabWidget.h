#pragma once

#include <QWidget>
#include "ui_vncTabWidget.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "SqlToolPool.h"
#include <QDebug>
#include <QProcess>
#include <QDesktopWidget>
#include <QWindow>
#include <windows.h>
void findWindowByTitle(const QString& title);
std::vector<RECT> getMonitors();
class vncTabWidget : public QWidget
{
	Q_OBJECT

public:
	vncTabWidget(QWidget *parent = nullptr);
    void moveVMAndMoveToScreen(int screenIndex);
    void startVM(const QString &vmPath);
	void findWindowByTitle(QString title, int monitorIndex);	
    ~vncTabWidget();
	QString vmPath;
	
private:
	Ui::vncTabWidgetClass ui;
	
};
