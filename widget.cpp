#include "widget.h"
#include "ui_widget.h"
#include "server.h"
#include <QIcon>
#include "fileoperation.h"

Server* ser1;   //聊天服务器1号
//Server* ser2;    //图片服务器

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("服务器开关");
    this->setWindowIcon(QIcon("://icon/off.png"));

}

Widget::~Widget()
{
    delete ui;
}



void Widget::on_ser1_on_clicked()
{
    if(this->swi == 0){
        ser1 = new Server("127.0.0.1",9999);
        this->setWindowIcon(QIcon("://icon/on.png"));
        this->swi=1;
    }
    else if(this->swi==1){
        delete ser1;
        //delete ser2;        this->setWindowIcon(QIcon("://icon/off.png"));
        this->swi=0;
    }
}


