#include "widget.h"
#include "ui_widget.h"
#include <QIcon>
#include "fileoperation.h"

Server* ser1;   //聊天服务器1号
extern logData* log;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("服务器开关");
    this->setWindowIcon(QIcon("://icon/off.png"));
    connect(log,&logData::logPut,this,&Widget::logGet);

}

Widget::~Widget()
{
    QString data = ui->textBrowser->toPlainText();
    log->logSaveFile(data);
    delete ui;
}


void Widget::on_ser1_on_clicked()
{
    if(this->swi == 0){
        QString time = log->getTime();
        emit log->logPut("[" + time + "]" + "服务器开启中...\n");
        ser1 = new Server(this->s_ip, this->s_PROT);
        this->setWindowIcon(QIcon("://icon/on.png"));
        this->swi=1;
    }
    else if(this->swi==1){
        delete ser1;
        this->setWindowIcon(QIcon("://icon/off.png"));
        this->swi=0;
        QString time = log->getTime();
        emit log->logPut("[" + time + "]" + "服务器关闭成功\n");
    }
}

void Widget::logGet(const QString logData)
{
    ui->textBrowser->insertPlainText(logData);
}


