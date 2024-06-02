#pragma once

#include <QPlainTextEdit>

class LogTextEdit : public QPlainTextEdit
{
	Q_OBJECT

public:
	LogTextEdit(QWidget* parent);
	~LogTextEdit();
	void appendText(const QString& text);
	QTextCursor cursor;
};
