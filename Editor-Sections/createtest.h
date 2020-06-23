#ifndef CREATETEST_H
#define CREATETEST_H

#include <QWidget>
#include "database.h"
#include "QString"
#include "layouttasktype.h"
#include "QHBoxLayout"
#include "helpfunction.h"
#include "test.h"
#include <QRegExp>
#include "listtaskslog.h"

namespace Ui {
class CreateTest;
}

class CreateTest : public QWidget
{
    Q_OBJECT

public:
    explicit CreateTest(QWidget *parent = 0, DataBase *db = new DataBase());
    ~CreateTest();

public slots:
    void showThisWindow();

private slots:
    void deleteType(QHBoxLayout *box); // удаление типа задач из списка (указатель на его layout)
    void deleteTypeRed(QHBoxLayout *box);
    void openTask(int n); // открытие задачи с id = n
    void setRandomTask(Test *test);


    void on_pushButton_add_theme_clicked();

    void on_pushButton_create_variants_clicked();

    void on_pushButton_save_test_clicked();

    void on_pushButton_delete_test_clicked();

    void on_pushButton_redact_test_clicked();

    void on_pushButton_add_theme_red_clicked();

    void on_pushButton_create_variants_red_clicked();

    void on_pushButton_save_test_red_clicked();

    void on_pushButton_cancel_red_clicked();

private:
    Ui::CreateTest *ui;
    DataBase *db;
    HelpFunction help;
    Test *test;
    Test *testRed;

    void setTaskType();

    QList<LayoutTaskType*> listTaskType;
    QList<LayoutTaskType*> listTaskTypeRed;
    void fillBoxTask(TestVariantTypeTask *task, QString s);

    void loadTests();
    QList<QString> listTest;
    QString oldTestName;

    ListTasksLog ltl;
    QList<QString> taskType;
};

#endif // CREATETEST_H
