#pragma once

#include <QWidget>
#include "ui_KeyMouseWidget.h"
#include "msdk.h"
#include <windows.h>
#include <QDebug>
#include "EGlobalVariable.h"
#include "KeyAndMouseThread.h"
#include <QTextCodec>
class KeyMouseWidget : public QWidget
{
	Q_OBJECT

public:
	KeyMouseWidget(QWidget* parent = Q_NULLPTR);
	void InitConnect();
	void InitKeyMouseThread();
	~KeyMouseWidget();
	HANDLE box_handle = NULL;
	QList<QPushButton*> buttonList;//页面上所有按钮控件
	void initBox();
	QString uncharToQstring(unsigned char* id, int len);

	QThread* keyMouseThread = nullptr;
	KeyAndMouseThread* objKeyMouseThread;
	QString hookKeyStr;
	int logKeyMouse = 0;
private slots://信号函数

signals:
	void sendStartKMThreadState(int  state);//鼠标键盘消息
private:
	Ui::KeyMouseWidget ui;
};
