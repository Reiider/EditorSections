#include "demotask.h"
#include "ui_demotask.h"

DemoTask::DemoTask(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DemoTask)
{
    ui->setupUi(this);
}

DemoTask::~DemoTask()
{
    delete ui;
}

void DemoTask::setText(QString s)
{
    ui->plainTextEdit_text->setPlainText(s);
}

void DemoTask::setIndex(int atLtl, int atTasks)
{
    indexTask = atTasks;
    indexLtl = atLtl;
}

void DemoTask::on_pushButton_redact_clicked()
{
    emit redactTask(indexLtl, indexTask);
}

void DemoTask::on_pushButton_delete_clicked()
{
    emit deleteTask(indexLtl);
}
