#include "MyLogTextEdit.h"

MyLogTextEdit::MyLogTextEdit(QWidget* parent)
	: QTextEdit(parent)
{
}

MyLogTextEdit::~MyLogTextEdit()
{
}

void MyLogTextEdit::appendText(const QString& text)
{
	append(text);
	moveCursor(QTextCursor::End);
	/*textCursor().movePosition(QTextCursor::End);
	setTextCursor(textCursor());
	cursor = textCursor();*/

}
