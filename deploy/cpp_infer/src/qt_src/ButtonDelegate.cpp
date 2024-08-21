// ButtonDelegate.cpp
#include "ButtonDelegate.h"
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QStyleOptionButton>

ButtonDelegate::ButtonDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    QStyleOptionButton button;
    QRect r = option.rect;
    int x = r.left() + r.width() - 30;
    int y = r.top() + (r.height() - 20) / 2;
    button.rect = QRect(x, y, 20, 20);
    button.text = "...";
    button.state = QStyle::State_Enabled;

    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
}

QSize ButtonDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(size.height() + 10); // 调整高度以适应按钮
    return size;
}

QWidget *ButtonDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPushButton *button = new QPushButton("...", parent);
    connect(button, &QPushButton::clicked, this, &ButtonDelegate::handleButtonClick);
    return button;
}

void ButtonDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // 不需要实现
}

void ButtonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    // 不需要实现
}

void ButtonDelegate::handleButtonClick()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        emit buttonClicked(button->property("index").toModelIndex());
    }
}
