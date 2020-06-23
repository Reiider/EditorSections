#ifndef WRITELOG_H
#define WRITELOG_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QString>


class WriteLog
{
public:
    WriteLog();

    static const bool isWrite = false;
    static void write(QString str);
    static void tempWrite(QString str);
};

#endif // WRITELOG_H
