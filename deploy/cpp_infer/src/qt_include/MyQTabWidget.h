#pragma once

#include <QWidget>

class MyQTabWidget  : public QWidget
{
	Q_OBJECT

public:
	MyQTabWidget(QWidget *parent);
	~MyQTabWidget();
	void Init();
};
