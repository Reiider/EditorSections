#ifndef VIEWTASKS_H
#define VIEWTASKS_H

#include <QWidget>
#include <QList>
#include "filemanager.h"
#include "listtaskslog.h"
#include "buttonfortest.h"
#include <QTimer>
#include <QMessageBox>
#include "digestogl.h"

namespace Ui {
class ViewTasks;
}

class ViewTasks : public QWidget
{
    Q_OBJECT

public:
    explicit ViewTasks(QWidget *parent = 0);
    ~ViewTasks();

    void set(ListTasksLog ltl, bool isLog);
    void setTypeTasks(QList<QString> taskType);
    void visibleComboBox(bool vis);

public slots:
    void showThisWindow();


private slots:
    void openTask(QString i);

    void on_pushButton_begin_clicked();

    void on_pushButton_begin_2_clicked();

    void on_pushButton_previous_clicked();

    void on_pushButton_next_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_cB_type_tasks_currentIndexChanged(const QString &arg1);

private:
    Ui::ViewTasks *ui;
    bool isLog; //это лог? если нет то это "демо решение" (презентация)
    ListTasksLog ltl;
    DigestOgl dOgl;

    void drawButton();
};

#endif // VIEWTASKS_H
