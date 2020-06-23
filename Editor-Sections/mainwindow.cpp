#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //кнопка "помощь"



    db = new DataBase();
    connect(db, SIGNAL(errorDB(QString)), this, SLOT(errorDB(QString)));
    db->open("Authorization");
    db->open("Content");
    db->open("Result");

    ctrlIsPressed = false;
    runTimerAccess = true;

    QTimer t;
    t.singleShot(200, this, SLOT(loadingDecisionTask()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::loadingDecisionTask()
{
    decisionTask = new DecisionTask(0, db);
        decisionTask->setVisible(true);
        ui->frame_widget->layout()->addWidget(decisionTask);
    QTimer t1;
    t1.singleShot(200, this, SLOT(loadingCreateTask()));
}

void MainWindow::loadingCreateTask()
{
    createTask = new CreateTask(0, db);
        createTask->setVisible(false);
        ui->frame_widget->layout()->addWidget(createTask);

}


void MainWindow::errorDB(QString s)
{
    QMessageBox::information(this, "Ошибка", s);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!authorization->isVisible()){
        if(QMessageBox::question(this, "Закрыть приложение",
                                       "Вы, действительно, хотите закрыть приложение?\r\nВсе несохраненные данные будут потеряны.",
                                       QMessageBox::Ok | QMessageBox::Cancel,
                                       QMessageBox::Cancel) == QMessageBox::Ok)
        {
            decisionTask->closeApp();
            createTask->closeApp();
            createDemo->closeApp();
            event->accept();
        }
        else {
            event->setAccepted(false);
            return;
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control){
        ctrlIsPressed = true;
    }
    if(event->key() == Qt::Key_Z && ctrlIsPressed){
        on_pb_back_step_clicked();
    }
    if(event->key() == Qt::Key_Y && ctrlIsPressed){
        on_pb_next_step_clicked();
    }
    if(event->key() == Qt::Key_Delete && statistic->isVisible()){
        statistic->deleteSelectedItems();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control){
        ctrlIsPressed = false;
    }
}


void MainWindow::reference()
{
    QString put = QCoreApplication::applicationDirPath();
    put.append("/help.chm");
    QDesktopServices::openUrl(QUrl::fromLocalFile(put));
}

void MainWindow::about_the_programm()
{
    QMessageBox::information(this, "О программе", "Данное приложение предназначено для: решения задач на прекционных чертежах; для создания изображений любых типов; для проведения построений многранников (позиционные и метрические здачи) ", 0, 0);
}

void MainWindow::exitLogin()
{
    if(QMessageBox::question(this, "Вход",
                                   "Вы, действительно, хотите выйти?\r\nВсе несохраненные данные будут потеряны.",
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel) == QMessageBox::Cancel)
    {
        return;
    }
    runTimerAccess = false;
    authorization->changeMainLable();
    ui->frame_menu->setVisible(false);
    hideWidget();
    authorization->setVisible(true);
    authorization->exitLogin();

    decisionTask->deleteLater();
    decisionTask = new DecisionTask(0, db);
        decisionTask->setVisible(false);
        ui->frame_widget->layout()->addWidget(decisionTask);

    createTask->deleteLater();
    createTask = new CreateTask(0, db);
        createTask->setVisible(false);
        ui->frame_widget->layout()->addWidget(createTask);

}


void MainWindow::hideWidget()
{
    if(decisionTask->isVisible()){
        decisionTask->setVisible(false);
    }
    if(createTask->isVisible()){
        createTask->setVisible(false);
    }

}

void MainWindow::setPBEnableWithoutSelectWidget()
{
    if(selectWidget != "DecisionTask"){
        ui->pb_decision_task->setEnabled(true);
    }
    if(selectWidget != "CreateTask"){
        ui->pb_create_task->setEnabled(true);
    }

}

bool MainWindow::isRightIsAvailable(QString s)
{
    if(access.length() != 0){
        if(access[0] == "Admin") return true;
    }
    if(access.indexOf(s) == -1){
        return false;
    }
    else return true;
}

void MainWindow::on_pb_decision_task_clicked()
{
    hideWidget();
    QTimer t;
    t.singleShot(0, this, SLOT(decision_task_clicked()));
}

void MainWindow::decision_task_clicked()
{
    decisionTask->setVisible(true);
    ui->pb_decision_task->setEnabled(false);
    selectWidget = "DecisionTask";
    setPBEnableWithoutSelectWidget();

    QTimer t;
    t.singleShot(0, decisionTask, SLOT(showThisWindow()));
}

void MainWindow::on_pb_create_task_clicked()
{
    hideWidget();
    QTimer t;
    t.singleShot(0, this, SLOT(create_task_clicked()));
}


void MainWindow::create_task_clicked()
{
    createTask->setVisible(true);
    ui->pb_create_task->setEnabled(false);
    selectWidget = "CreateTask";
    setPBEnableWithoutSelectWidget();
    QTimer t;
    t.singleShot(0, createTask, SLOT(showThisWindow()));
}

void MainWindow::on_pb_back_step_clicked()
{
    if(decisionTask->isVisible()){
        decisionTask->backStep();
    }
    else if(createTask->isVisible()){
        createTask->backStep();
    }
    else if(createDemo->isVisible()){
        createDemo->backStep();
    }
}

void MainWindow::on_pb_next_step_clicked()
{
    if(decisionTask->isVisible()){
        decisionTask->nextStep();
    }
    else if(createTask->isVisible()){
        createTask->nextStep();
    }
    else if(createDemo->isVisible()){
        createDemo->nextStep();
    }
}

void MainWindow::on_pb_clear_all_clicked()
{
    if(decisionTask->isVisible()){
        decisionTask->clearAll();
    }
    else if(createTask->isVisible()){
        createTask->clearAll();
    }
    else if(createDemo->isVisible()){
        createDemo->clearAll();
    }
}

