#include "writelog.h"

WriteLog::WriteLog()
{
}

void WriteLog::write(QString str)
{
    if(isWrite){
        QString path = "./logFile.txt";
        QFile file( path );
        QTextStream stream( &file );

        if( file.open( QIODevice::Append )){
            stream << str + "\r\n";
            file.close();
        }
    }
}

void WriteLog::tempWrite(QString str)
{
    QString path = "./logFile.txt";
    QFile file( path );
    QTextStream stream( &file );

    if( file.open( QIODevice::Append )){
        stream << str + "\r\n";
        file.close();
    }
}
