#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlError>
#include <QString>

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    void open(QString name);
    void close(QString name);
    QSqlDatabase getDb(QString name);

signals:
    void errorDB(QString s);

public slots:

private:
    QSqlDatabase content;
    QSqlDatabase au;
    QSqlDatabase result;

};

#endif // DATABASE_H
