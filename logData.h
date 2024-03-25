#ifndef LOGDATA_H
#define LOGDATA_H

#include <QWidget>

/*日志输出与保存*/

namespace Ui {
class logData;
}

class logData : public QObject
{
    Q_OBJECT

public:
    explicit logData(QWidget *parent = nullptr);
    ~logData();
    //保存日志文件
    void logSaveFile(const QString data);
    //获取时间
    QString getTime();
private:
    Ui::logData *ui;
signals:
    //日志获取信号
    void logPut(const QString logData);

};

#endif // LOGDATA_H
