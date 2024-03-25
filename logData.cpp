#include "logData.h"
#include "ui_logData.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QDebug>

logData::logData(QWidget *parent) :
    QObject(parent),
    ui(new Ui::logData)
{
    //创建日志文件夹
    QString folderName = "logs";
        QDir dir;
        if (!dir.exists(folderName)) {
            if (dir.mkdir(folderName)) {
                qDebug() << "Folder created successfully: " << folderName;
            } else {
                qDebug() << "Failed to create folder: " << folderName;
            }
        } else {
            qDebug() << "Folder already exists: " << folderName;
        }
}

logData::~logData()
{

}

void logData::logSaveFile(const QString data)
{
    //获取日期当做日志文件的名字
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QDate currentDate = currentDateTime.date();
    QString filePath = "logs/" + currentDate.toString(Qt::ISODate) + ".txt";
    QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << data;
            file.close();
        } else {
            // 文件无法打开，处理错误

        }
}

QString logData::getTime()
{
    // 获取当前时间
    QDateTime currentDateTime = QDateTime::currentDateTime();
    // 获取当前时间的小时和分钟
    QTime currentTime = currentDateTime.time();
    // 返回当前时间的小时和分钟
    return currentTime.toString("hh:mm");
}

