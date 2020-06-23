#ifndef CREATEDEMO_H
#define CREATEDEMO_H

#include <QWidget>
#include "database.h"
#include "listtaskslog.h"
#include "filemanager.h"
#include "viewtasks.h"
#include "demotask.h"
#include "demovariant.h"
#include "constructing.h"
#include "windowdialogs.h"

namespace Ui {
class CreateDemo;
class ViewTasks;
}

class CreateDemo : public QWidget
{
    Q_OBJECT

public:
    explicit CreateDemo(QWidget *parent = 0, DataBase *db = new DataBase(), ViewTasks *vt = new ViewTasks());
    ~CreateDemo();

    void backStep();
    void nextStep();
    void clearAll();

public slots:
    void closeApp();
    void answer_inputed(QString s);
    void showThisWindow();

signals:
    void viewTaskInRedact();

private slots:
    void redactTask(int atLtl, int atTasks);
    void deleteTask(int i);
    void viewVariant(int i);
    void deleteVariant(int i);
    void redactVariant(int i);
    void saveVariant(int i, QString s);



    void on_pushButton_answer_clicked();

    void on_comboBox_task_type_currentIndexChanged(const QString &arg1);

    void on_pushButton_addDemoTask_clicked();

    void on_pushButton_closeTask_clicked();

    void on_pushButton_saveVariant_clicked();

    void on_pushButton_deleteDemoType_clicked();

private:
    Ui::CreateDemo *ui;

    Constructing *construct;

    QList<QString> Tasks;
    QList<QString> tasksForNewTask;
    QList<QString> miniTasks;
    QList<int> idMiniTask;
    void setTaskType();
    void updateMiniTasks(QString s);
    QString currentIndexMiniTasks;

    DataBase *db;
    ViewTasks *vt;
    //тесты

    ListTasksLog ltl;
    QList<QString> taskType;
    void taskSelected();
    void updateScrollForTasks();
    void updateScrollForVariants(int i);
    int currentTask;
    void openTask(int index);

    int iLastTaskType;// индекс последнего выбранного типа задач (что бы при 2ом переходе к простмотру "метода решения" индекс оставался тем же и ltl "не изменялся" (он меняется но остается тем же))

    WindowDialogs *answerDialog;

    QList<DemoVariant*> listDemoVariant;
};

#endif // CREATEDEMO_H
