#pragma once

#include <QTabWidget>
#include <QWidget>
class MyQTabWidget  : public QTabWidget
{
	Q_OBJECT

public:
	MyQTabWidget(QWidget *parent);
	~MyQTabWidget();
	void Init();
};
