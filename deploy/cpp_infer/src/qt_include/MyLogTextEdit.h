#pragma once

#include <QTextEdit>

class MyLogTextEdit : public QTextEdit
{
	Q_OBJECT

public:
	MyLogTextEdit(QWidget* parent);
	~MyLogTextEdit();
	void appendText(const QString& text);
	QTextCursor cursor;
};
