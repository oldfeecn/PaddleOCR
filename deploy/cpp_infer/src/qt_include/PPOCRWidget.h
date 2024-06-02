#pragma once

#include <QWidget>
#include "ui_PPOCRWidget.h"
#include <QDebug>
#include <QFileDialog>
class PPOCRWidget : public QWidget
{
	Q_OBJECT

public:
	PPOCRWidget(QWidget *parent = nullptr);
	~PPOCRWidget();

	void Init();
	void ConncetSightInit();
	bool haveChange=false;
	void haveChangeFun(QString value);
	void haveChangeFun(int value);
	void haveChangeFun(double value);
private:
	Ui::PPOCRWidgetClass ui;
};
