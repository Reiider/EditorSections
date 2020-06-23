#include "database.h"

DataBase::DataBase(QObject *parent) :
    QObject(parent)
{
}

void DataBase::open(QString name)
{
    if(name == "Content"){
        content = QSqlDatabase::addDatabase("QODBC", name);
        content.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MSAccess};DBQ=Content.mdb");
        content.setPassword("Xq3FgVN8Hfdcz2Fe");
        if (!content.open()) {
            emit errorDB("Неудалось открыть базу данных Content.mdb, проверьте наличие файла.");
        }
    }
    else if(name == "Authorization"){
        au = QSqlDatabase::addDatabase("QODBC", name);
        au.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MSAccess};DBQ=Authorization.mdb");
        au.setPassword("Xq3FgVN8Hfdcz2Fe");
        if (!au.open()) {
            emit errorDB("Неудалось открыть базу данных Authorization.mdb, проверьте наличие файла.");
        }
    }
    else if(name == "Result"){
        result = QSqlDatabase::addDatabase("QODBC", name);
        result.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MSAccess};DBQ=Result.mdb");
        result.setPassword("Xq3FgVN8Hfdcz2Fe");
        if (!result.open()) {
            emit errorDB("Неудалось открыть базу данных Result.mdb, проверьте наличие файла.");
        }
    }
}

void DataBase::close(QString name)
{
    QSqlDatabase::removeDatabase(name);
}

QSqlDatabase DataBase::getDb(QString name)
{
    if(name == "Content") return content;
    else if(name == "Authorization") return au;
    else /*if(name == "Result")*/ return result;
}
