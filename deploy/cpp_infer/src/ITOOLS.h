#pragma once
#include "ui_ITOOLS.h"
#include <QMainWindow>

class ITOOLS : public QMainWindow {
    Q_OBJECT
    
public:
    ITOOLS(QWidget* parent = nullptr);
    ~ITOOLS();

private:
    Ui_ITOOLS* ui;
};