#pragma once

#include <QWidget>
#include "ui_PPOCRWidget.h"

class PPOCRWidget : public QWidget
{
	Q_OBJECT

public:
	PPOCRWidget(QWidget *parent = nullptr);
	~PPOCRWidget();

private:
	Ui::PPOCRWidgetClass ui;
};
