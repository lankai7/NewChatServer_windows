#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "Server.h"

QT_BEGIN_NAMESPACE

namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    //服务器开关状态
    bool swi=0;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    Ui::Widget *ui;

private slots:
    //服务器开关按钮
    void on_ser1_on_clicked();
    void logGet(const QString logData);
private:
    //ip与端口号
    const char* s_ip = "127.0.0.1";
    const int s_PROT = 9999;
};


#endif // WIDGET_H
