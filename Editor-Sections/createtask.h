#ifndef CREATETASK_H
#define CREATETASK_H

#include <QWidget>
#include "ogl.h"
#include <point.h>
#include <linear.h>
#include <flat.h>
#include <QList>
#include "database.h"
#include "filemanager.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QString>
#include <QMenu>
#include "buttonblockaction.h"
#include "constructing.h"
#include "windowdialogs.h"

namespace Ui {
class CreateTask;
}

class CreateTask : public QWidget
{
    Q_OBJECT

public:
    explicit CreateTask(QWidget *parent = 0, DataBase *db = new DataBase());
    ~CreateTask();

public slots:
    void closeApp();
    void showThisWindow();

    void clearAll();
    void nextStep();
    void backStep();

    QString getTextTask();
    void createImgBMP(QString path); //path = path + .bmp


private slots:
    void s_authorAnswer(QString answer);
    void s_intersecLineWithFlat(QString answer1, QString answer2);
    void s_intersecTwoFlats(QString answer1, QString answer2);
    void s_cuttingPlane(QString answer);
    void s_updateCuttingPlane(QString answer);
    void s_updateIntersecTwoFlats(QString answer1, QString answer2);
    void s_saveTask();

    void on_pushButton_saveFigure_clicked();

    void on_pushButton_redact_clicked();

    void on_pushButton_delete_clicked();

    void on_comboBox_TaskType_2_currentIndexChanged(const QString &arg1);

    void on_pushButton_set_answer_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_select_figure_clicked();

    void on_pushButton_save_task_clicked();

    void on_pushButton_select_figure_redact_clicked();

    void on_pushButton_saveFigure_redact_clicked();

    void on_pushButton_create_type_redact_clicked();

    void on_pushButton_set_answer_redact_clicked();

    void on_pushButton_save_task_redact_clicked();

    void on_pushButton_4_clicked();

    void on_pB_blockActions_clicked();

    void on_pB_blockActions_red_clicked();

private:
    Ui::CreateTask *ui;

    QList<QString> Tasks;
    QList<QString> TypeAnswer;
    QList<QString> miniTasks;
    QList<int> idMiniTask;
    void updatePandL();
    QString currentIndexMiniTasks;
    void updateMiniTasks(QString arg1);

    void setTaskType();

    bool saveFigure;
    bool redactTask;

    int maxId;
    void setMaxId();
    DataBase *db;

    ListTasksLog ltl;
    QList<QString> taskType;

    void createListBlockActions();
    void createPushBatton(QString name, int id);
    QList<ButtonBlockAction*> listBlockActions;
    QString blockActions; //при созднии задач тут хранятся заблокированые действия
    void updateBlockActionsString();
    void resetBlockActions();
    void setBlockActions(QString blockStr);
    void showActions(bool isRedact);

    Constructing *construct;
    WindowDialogs *answerDialog;
};

#endif // CREATETASK_H
