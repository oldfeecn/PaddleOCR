#include "ITOOLS.h"

ITOOLS::ITOOLS(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_ITOOLS)
{
    ui->setupUi(this);
     
}

ITOOLS::~ITOOLS()
{
    delete ui; 
}