#include "filemanager.h"

FileManager::FileManager()
{
}

QString FileManager::saveBinaryFile(ListTasksLog ltl)
{
    QString path = "./Отчеты/" + ltl.getLogin() +" "+ ltl.getTest() + " " + ltl.getVariant() + ".bin";
    QFile file( path );
    QDataStream stream( &file );

    if( file.open( QIODevice::WriteOnly )){
        stream << ltl;
        file.close();
        return path;
    }
    else {
        QMessageBox::information(0, "Ошибка", "Ошибка при попытке записи файла.");
        return "";
    }
}

ListTasksLog FileManager::openBinaryFile(QString path)
{
    QFile file( path );
    QDataStream stream( &file );

    if( file.open( QIODevice::ReadOnly )){
        ListTasksLog ltl;
        stream >> ltl;
        file.close();
        return ltl;
    }
    else{
        QMessageBox::information(0, "Файл не найден", "Файл не найден.");
        ListTasksLog ltl;
        ltl.setLogin("E"); //Error
        return ltl;
    }
}

ListTasksLog FileManager::openDemo(QString typeTask)
{
    typeTask = "./Демо/" + typeTask + ".bin";
    QFile file( typeTask );
    QDataStream stream( &file );

    if( file.open( QIODevice::ReadOnly )){
        ListTasksLog ltl;
        stream >> ltl;
        file.close();
        ltl.setLogin("");
        return ltl;
    }
    else{
        ListTasksLog ltl;
        ltl.setLogin("E"); //E - Error
        return ltl;
    }
}

void FileManager::saveDemo(ListTasksLog ltl)
{
    QString path = "./Демо/" + ltl.getTest() + ".bin";
    QFile file( path );
    QDataStream stream( &file );

    if( file.open( QIODevice::WriteOnly )){
        stream << ltl;
        file.close();
    }
    else {
        QMessageBox::information(0, "Ошибка", "Ошибка при сохранении файла.");
    }
}

void FileManager::deleteDemo(QString typeTask)
{
    QString path = "./Демо/" + typeTask + ".bin";
    QFile file( path );
    file.remove();
}
