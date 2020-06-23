#ifndef DECISIONTASK_H
#define DECISIONTASK_H

#include <QWidget>
#include "ogl.h"
#include <point.h>
#include <linear.h>
#include <flat.h>
#include <QList>
#include <QMessageBox>
#include <QInputDialog>
#include <QString>
#include "helpfunction.h"
#include "database.h"
#include "buttonfortest.h"
#include "filemanager.h"
#include "windowdialogs.h"
#include <QDateTime>
#include "constructing.h"

namespace Ui {
class DecisionTask;
}

class DecisionTask : public QWidget
{
    Q_OBJECT

public:
    explicit DecisionTask(QWidget *parent = 0, DataBase *db = new DataBase());
    ~DecisionTask();

    void setUserName(QString name);
    QList<QString> getTasksType();
    QString getTextTask();

signals:
    void clickOnButton(QString n);
    void setLableVariant(QString s);

public slots:
    void closeApp();
    void showThisWindow();

    void openTest();
    void backStep();
    void makeTest();
    void nextStep();
    void clearAll();
    void refresh();

    void createImgBMP(QString path); //path = path + .bmp

private slots:
    void tempSaveTestResult();

    void closeTest();
    void loadTests();
    void openTask(const QString &n);
    void setCurrentButton(QPushButton *button);

    void createPrism();
    void createPyramid();
    void createCube();

    void answer_inputed(QString s);

    void on_pushButton_answer_clicked();

    void on_pushButton_select_task_clicked();

    void on_comboBox_task_type_currentIndexChanged(const QString &arg1);

    void on_pushButton_2_clicked();

private:
    Ui::DecisionTask *ui;

    QString UserName;

    Constructing *construct;

    QList<QString> Tasks;
    QList<QString> miniTasks;
    QList<int> idMiniTask;
    void setTaskType();

    void updateMiniTasks(QString s);
    QString currentIndexMiniTasks;

    HelpFunction help;

    DataBase *db;
    //тесты
    bool testOpened;
    QList<QString> listTest;
    QList<QString> listIdTask;
    QPushButton *currentButtonTask; //кнопка, которую делаем неактивной, если решено верно
    QList<QPushButton*> listButtonTask; //список кнопок для определения решенных задач по enabled кнопки
    void selectTest(QString test, int n); //открытие теста с именем test

    void setAcc(QString s, bool ok); // доступ к функциям приложения

    void saveResult(QString path);
    bool firstSave; // for saveResult
    QString dataTest;
    QString timeTest;

    ListTasksLog ltl;

    QList<QString> taskType;

    QString currentTypeTask;
    QString currentTask;


    void setAvailableToActions(QString blockActions);

    WindowDialogs *answerDialog;

    QTimer tToSaveTest;


};

#endif // DECISIONTASK_H
