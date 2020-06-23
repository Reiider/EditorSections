#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QStyle>
#include "authorization.h"
#include "decisiontask.h"
#include "createtask.h"
#include "createtest.h"
#include "adminpanel.h"
#include "statistic.h"
#include "viewtasks.h"
#include "createdemo.h"
#include "helpfunction.h"
#include "replicate.h"

#include <QAxObject>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void loadingDecisionTask();
    void loadingCreateTask();

    void errorDB(QString s);

    void decision_task_clicked();
    void create_task_clicked();


    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void reference();
    void about_the_programm();
    void exitLogin();



    void on_pb_decision_task_clicked();

    void on_pb_create_task_clicked();

    void on_pb_back_step_clicked();

    void on_pb_next_step_clicked();

    void on_pb_clear_all_clicked();

private:
    Ui::MainWindow *ui;

    QString selectWidget; //тукущий выбранный виджет
    Authorization *authorization;
    DecisionTask *decisionTask;
    CreateTask *createTask;
    CreateTest *createTest;
    AdminPanel *adminPanel;
    Statistic *statistic;
    ViewTasks *viewTasks;
    CreateDemo *createDemo;
    Replicate *replicate;

    void hideWidget();
    void setPBEnableWithoutSelectWidget();


    QString UserName;
    QList<QString> access;
    void setAcc(QString funk, bool ok);
    bool isRightIsAvailable(QString s);

    HelpFunction help;

    DataBase *db;

    bool ctrlIsPressed;
    bool runTimerAccess;
    QString lastAccess;
};

#endif // MAINWINDOW_H
