#pragma once

#include <QMainWindow>
#include "ui_KeyMouseWidget.h"
#include "EGlobalVariable.h"
#include "KeyAndMouseThread.h"
#include "YSDKdll.h"
#include <windows.h>
#include <QDebug>
#include <QTextCodec>
#include <QStringLiteral>
class KeyMouseWidget : public QMainWindow
{
	Q_OBJECT

public:
	KeyMouseWidget(QWidget* parent = nullptr);
	void InitConnect();
	void InitKeyMouseThread();
	~KeyMouseWidget();
	HANDLE box_handle = NULL;
	QList<QPushButton*> buttonList;//ҳ�������а�ť�ؼ�
	void initBox();
	QString uncharToQstring(unsigned char* id, int len);
	QThread* keyMouseThread = nullptr;
	KeyAndMouseThread* objKeyMouseThread;
	QString hookKeyStr;
	int logKeyMouse = 0;
private slots://�źź���

signals:
	void sendStartKMThreadState(int  state);//��������Ϣ
private:
	Ui::KeyMouseWidgetClass ui;
};
