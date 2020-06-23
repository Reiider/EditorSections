#ifndef BUTTONFORTEST_H
#define BUTTONFORTEST_H

#include <QObject>
#include <QMainWindow>
#include <QList>
#include <QSqlRecord>
#include <QtSql/QSqlDatabase>
#include <QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QInputDialog>
#include <QString>
#include "QHBoxLayout"

class ButtonForTest : public QObject
{
    Q_OBJECT
private:

public:
    explicit ButtonForTest(QObject *parent = 0);
    QPushButton *button;
    QCheckBox *chBox;
    QString id;

signals:
    void openTask(QString n);
    void currentButton(QPushButton *pb);
    void currentChBox(QString n, QCheckBox *ch);

public slots:
    void openTask(); //открыть задачу с Id = id и задать текущую кнопку как выбранную (MainWindow)

};

#endif // BUTTONFORTEST_H
