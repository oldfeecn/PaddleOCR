#include "KeyMouseQLineEdit.h"

KeyMouseQLineEdit::KeyMouseQLineEdit(QWidget *parent)
	: QLineEdit(parent)
{
}

KeyMouseQLineEdit::~KeyMouseQLineEdit()
{
}

void KeyMouseQLineEdit::appendText(QString msg)
{
	this->setText(this->text().append(",").append(msg));
}

void KeyMouseQLineEdit::focusInEvent(QFocusEvent *event)
{
	emit KeyMouseQLineEditMsg(4);
}
void KeyMouseQLineEdit::focusOutEvent(QFocusEvent *event)
{
	emit KeyMouseQLineEditMsg(5);
}