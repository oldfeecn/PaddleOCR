#include "ImgTabWidget.h"
#include "ui_ImgTabWidget.h"

ImgTabWidget::ImgTabWidget(QWidget *parent) : QWidget(parent),
                                              ui(new Ui::ImgTabWidget),
                                              model(new QStringListModel(this)) // Initialize the model
{
    ui->setupUi(this);
    ui->lineEdit_2->installEventFilter(this);
    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_3->installEventFilter(this);
    initDate();
}
void ImgTabWidget::initDate(){
    QList<QMap<QString, QVariant>> valuesList;
	bool success = SqlToolPool::selectData(QString("SELECT id, pathName FROM dnf_jietu WHERE recstatus = 1 ORDER BY id LIMIT 10;"), valuesList);
	if (success && !valuesList.isEmpty())
	{
             
             QStringList sqlStringList;
            for (const QMap<QString, QVariant> &row : valuesList)
            {
               QString pathName = row.value("pathName").toString();
               sqlStringList.append(pathName);
               qDebug()<< pathName;
            }
          model->setStringList(sqlStringList);
          ui->listViewDate->setModel(model);
          ui->listViewDate->setItemDelegate(nullptr);
                ButtonDelegate *delegate = new ButtonDelegate(ui->listViewDate);
                ui->listViewDate->setItemDelegate(delegate);
                QObject::connect(delegate, &ButtonDelegate::buttonClicked, [&](const QModelIndex &index) {
                    qDebug() << "Button clicked on item:" << index.data().toString();
                });   
         ui->listViewDate->show();
    }
}
bool ImgTabWidget::eventFilter(QObject *obj, QEvent *event)
{

    if (obj == ui->lineEdit_2)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                                "",
                                                                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
                if (!dir.isEmpty())
                {
                    ui->lineEdit_2->setText(dir);
                    recImgPath = dir;
                }
            }
        }
    };

    if (obj == ui->lineEdit)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                                "",
                                                                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
                if (!dir.isEmpty())
                {
                    ui->lineEdit->setText(dir);
                    srcImgPath = dir;
                }
            }
        }
    };
    if (obj == ui->lineEdit_3)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                QString fileName = QFileDialog::getOpenFileName(this, "Open File", srcImgPath, "All Files (*)");
                if (!fileName.isEmpty())
                {
                    ui->lineEdit_3->setText(fileName);
                }
            }
        }
    };
    return QWidget::eventFilter(obj, event);
}

ImgTabWidget::~ImgTabWidget()
{
    delete ui;
}
