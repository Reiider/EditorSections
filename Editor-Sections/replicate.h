#ifndef REPLICATE_H
#define REPLICATE_H

#include <QWidget>
#include "database.h"
#include "replicatetasktocopy.h"
#include "writelog.h"
#include "QVBoxLayout"
#include "buttonfortest.h"
#include "helpfunction.h"
#include "test.h"


namespace Ui {
class Replicate;
}

struct TempTask{
    QString id;
    bool isExist;
    QString idInMain;
    QString text;
};

struct TempTypeTask{
    QList<TempTask> lTask;
    QString sName;
};

struct TempVariant{
    QList<TempTypeTask> lTypeTask;
};

struct TempTest{
    QList<TempVariant> lVariants;
    QString sName;
};

class Replicate : public QWidget
{
    Q_OBJECT

public:
    explicit Replicate(QWidget *parent = 0, DataBase *db = new DataBase());
    ~Replicate();

public slots:
    void showThisWindow();
    void clickToButton(QString id, QCheckBox *ch);
    void preview(int id);
    void clickAtTask(int idType);

private slots:

    void copying();

    void on_pushButton_4_clicked();

    void on_radioButton_clicked();


    void on_pb_copy_clicked();

    void on_cb_tests_listTest_currentIndexChanged(const QString &arg1);

    void on_pb_tests_copy_clicked();

    void on_radioButton_tests_clicked();

private:
    Ui::Replicate *ui;

    DataBase *db;
    QSqlDatabase copyContent;
    QList<QString> listTaksType;
    QList<ReplicateTaskToCopy*> listRCCT;
    QList<ButtonForTest*> listBForT;
    QList<QWidget*> listWidgets;
    QVBoxLayout *scrollLayout;

    QList<QString> listTest;
    HelpFunction help;
    Test *currentTest;
    TempTest test;

    void loadTasks();
    void loadTest(QString testName);

    QString isExistTask(QSqlQuery *mainQuery, QSqlQuery *query, QSqlRecord *rec);

    int sizeArea;

    QSpacerItem *qsi; // последний элемент в scrollLayout

};

#endif // REPLICATE_H
