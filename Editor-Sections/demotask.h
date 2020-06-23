#ifndef DEMOTASK_H
#define DEMOTASK_H

#include <QWidget>

namespace Ui {
class DemoTask;
}

class DemoTask : public QWidget
{
    Q_OBJECT

public:
    explicit DemoTask(QWidget *parent = 0);
    ~DemoTask();

    void setText(QString s);
    void setIndex(int atLtl, int atTasks);

signals:
    void redactTask(int atLtl, int atTasks);
    void deleteTask(int i);

private slots:
    void on_pushButton_redact_clicked();

    void on_pushButton_delete_clicked();

private:
    Ui::DemoTask *ui;
    int indexTask;
    int indexLtl;
};

#endif // DEMOTASK_H
