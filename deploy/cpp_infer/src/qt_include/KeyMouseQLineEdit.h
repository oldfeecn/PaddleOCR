#pragma once

#include <QLineEdit>
#include "EGlobalVariable.h"

class KeyMouseQLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	KeyMouseQLineEdit(QWidget* parent);
	~KeyMouseQLineEdit();
	void appendText(QString msg);
signals:
	void KeyMouseQLineEditMsg(int msg);
protected:
	void focusInEvent(QFocusEvent* event);
	void focusOutEvent(QFocusEvent* event);
};
