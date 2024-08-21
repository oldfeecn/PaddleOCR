#include "LogTextEdit.h"

LogTextEdit::LogTextEdit(QWidget *parent)
	: QPlainTextEdit(parent)
{
}

LogTextEdit::~LogTextEdit()
{
}

void LogTextEdit::appendText(const QString &text)
{
	appendPlainText(text);
	moveCursor(QTextCursor::End);
	/*textCursor().movePosition(QTextCursor::End);
	setTextCursor(textCursor());
	cursor = textCursor();*/
}
