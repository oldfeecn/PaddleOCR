#pragma once
#include "ui_ITOOLS.h"
#include <QMainWindow>
#include <EGlobalVariable.h>
#include <qsystemtrayicon.h>
#include <QMenu>
class ITOOLS : public QMainWindow {
    Q_OBJECT
    
public:
    ITOOLS(QWidget* parent = nullptr);
    ~ITOOLS();
    void closeEvent(QCloseEvent* event);
    void InitUiLogo();
private:
    Ui_ITOOLS* ui;
};