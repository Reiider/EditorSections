#include "viewtasks.h"
#include "ui_viewtasks.h"

ViewTasks::ViewTasks(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewTasks)
{
    ui->setupUi(this);
}

ViewTasks::~ViewTasks()
{
    delete ui;
}

void ViewTasks::set(ListTasksLog ltl, bool isLog)
{
    ui->frame_combo_box->setVisible(false);
    ui->widget->clear();

    ui->textEdit_report->setText("");
    ui->label_current_variant->setText("");

    this->isLog = isLog;
    this->ltl = ltl;
    if(!isLog){
        ui->frame->setVisible(false);
        ui->frame_combo_box->setVisible(true);
        ui->label->setText("Описание метода решения");
        ui->label_isLogTest->setText("Вариант решения");
    }
    else{
        ui->frame->setVisible(true);
        ui->label_isLogTest->setText("Попытка");
        ui->label->setText("Отчет по текущей попытке");

        ui->label_login->setText(ltl.getLogin());
        ui->label_test->setText(ltl.getTest());
        ui->label_variant->setText(ltl.getVariant());
    }
    drawButton();
}

void ViewTasks::setTypeTasks(QList<QString> taskType)
{
    ui->cB_type_tasks->clear();
    ui->cB_type_tasks->addItems(taskType);
    ui->cB_type_tasks->setCurrentIndex(0);
}

void ViewTasks::visibleComboBox(bool vis)
{
    ui->frame_combo_box->setVisible(vis);
}

void ViewTasks::showThisWindow()
{
    ui->widget->repaintAll();
}

void ViewTasks::openTask(QString i)
{
    //QMessageBox::information(this, "", "1");
    dOgl = ltl.openTask(i.toInt());
    if(!isLog) dOgl.setBeginPosition();
    ui->textEdit_report->setText(dOgl.report);
    ui->label_current_variant->setText(ltl.getStrForLabel());
    ui->widget->setFromDigest(dOgl);
    if(!isLog){
        ui->pushButton->clicked();
    }
}

void ViewTasks::drawButton()
{
    ui->scrollArea->widget()->~QWidget();

    QVBoxLayout *layout = new QVBoxLayout;
    QList<QString> listText = ltl.getListConditionTasks();
    for(int i = 0; i < listText.length() ; i++){
        QList<DigestOgl> ld = ltl.getListTasks().at(i);
        QHBoxLayout *hl = new QHBoxLayout;
        QPlainTextEdit *t = new QPlainTextEdit(listText[i]);
        ButtonForTest *b = new ButtonForTest;
        b->button = new QPushButton("Открыть решение");
        if(ld.length() != 0){
            b->id = QString::number(i);
            QObject::connect(b->button, SIGNAL(clicked()), b, SLOT(openTask()));
            QObject::connect(b, SIGNAL(openTask(QString)), this, SLOT(openTask(QString)));
        }
        else{
            b->button->setEnabled(false);
        }
        t->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        t->setReadOnly(true);
        b->button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        hl->addWidget(t);
        hl->addWidget(b->button);
        layout->addLayout(hl);
    }

    layout->addSpacerItem(new QSpacerItem(20, 1000, QSizePolicy::Expanding, QSizePolicy::Expanding));

    QWidget *w = new QWidget;
    w->setLayout(layout);
    w->setAttribute(Qt::WA_DeleteOnClose);

    ui->scrollArea->setWidget(w);
}

void ViewTasks::on_pushButton_begin_clicked()
{
    ui->widget->firstStep();
}

void ViewTasks::on_pushButton_begin_2_clicked()
{
    ui->widget->lastStep();
}

void ViewTasks::on_pushButton_previous_clicked()
{
    ui->widget->previousStep();
}

void ViewTasks::on_pushButton_next_clicked()
{
    ui->widget->nextStep();
}

void ViewTasks::on_pushButton_clicked()
{
    dOgl = ltl.firstVariant();
    if(!isLog) dOgl.setBeginPosition();
    ui->textEdit_report->setText(dOgl.report);
    ui->widget->setFromDigest(dOgl);
    ui->label_current_variant->setText(ltl.getStrForLabel());
}

void ViewTasks::on_pushButton_2_clicked()
{
    dOgl = ltl.lastVariant();
    if(!isLog) dOgl.setBeginPosition();
    ui->textEdit_report->setText(dOgl.report);
    ui->widget->setFromDigest(dOgl);
    ui->label_current_variant->setText(ltl.getStrForLabel());
}

void ViewTasks::on_pushButton_4_clicked()
{
    dOgl = ltl.previousVariant();
    if(!isLog) dOgl.setBeginPosition();
    ui->textEdit_report->setText(dOgl.report);
    ui->widget->setFromDigest(dOgl);
    ui->label_current_variant->setText(ltl.getStrForLabel());
}

void ViewTasks::on_pushButton_5_clicked()
{
    dOgl = ltl.nextVariant();
    if(!isLog) dOgl.setBeginPosition();
    ui->textEdit_report->setText(dOgl.report);
    ui->widget->setFromDigest(dOgl);
    ui->label_current_variant->setText(ltl.getStrForLabel());
}

void ViewTasks::on_horizontalSlider_valueChanged(int value)
{
    ui->widget->changeSizeFigure(value);
}

void ViewTasks::on_cB_type_tasks_currentIndexChanged(const QString &arg1)
{
    ListTasksLog listT = FileManager::openDemo(arg1);
    set(listT, false);
}

