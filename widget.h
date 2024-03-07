#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    bool swi=0; //服务器开关状态

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    //服务器开关按钮
    void on_ser1_on_clicked();

private:
    Ui::Widget *ui;

};
#endif // WIDGET_H
