#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QDebug>
#include <QTextCodec>
#include <QString>
#include <QInputDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include "listtaskslog.h"
//#include <QDateTime>

class FileManager
{
public:
    FileManager();
    static QString saveBinaryFile(ListTasksLog ltl);
    static ListTasksLog openBinaryFile(QString path);
    static ListTasksLog openDemo(QString typeTask);
    static void saveDemo(ListTasksLog ltl);
    static void deleteDemo(QString typeTask);

};

#endif // FILEMANAGER_H
