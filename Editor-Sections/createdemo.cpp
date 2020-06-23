#include "createdemo.h"
#include "ui_createdemo.h"

CreateDemo::CreateDemo(QWidget *parent, DataBase *db, ViewTasks *vt) :
    QWidget(parent),
    ui(new Ui::CreateDemo)
{
    ui->setupUi(this);

    construct = new Constructing(0, ui->widget, db);

    ui->tab1->layout()->addWidget(construct);

    construct->setsForDecisionTask();

    this->db = db;
    this->vt = vt;

    iLastTaskType = 0;
    setTaskType();

    currentTask = 0;

    construct->updatePandL();

    ui->frame_task->setVisible(false);

    answerDialog = new WindowDialogs();
    connect(answerDialog, SIGNAL(text_inputed(QString)), this, SLOT(answer_inputed(QString)));
}

CreateDemo::~CreateDemo()
{
    delete ui;
}

void CreateDemo::showThisWindow()
{
    setTaskType();
    if(iLastTaskType < taskType.length()) ui->comboBox_task_type->currentIndexChanged(taskType.at(iLastTaskType));
        else if(taskType.length() != 0) ui->comboBox_task_type->currentIndexChanged(taskType.at(0));
        else QMessageBox::information(this, "Ошибка", "Типы задач не найдены, возможно в базе дынных нет задач.");
    construct->updatePandL();
    ui->widget->repaintAll();
}

void CreateDemo::redactTask(int atLtl, int atTasks)
{
    iLastTaskType = ui->comboBox_task_type->currentIndex();
    currentTask = atLtl;
    openTask(atTasks);
    updateScrollForVariants(currentTask);
    taskSelected();
}

void CreateDemo::deleteTask(int i)
{
    ltl.removeTask(i);
    updateScrollForTasks();
    FileManager::saveDemo(ltl);
}

void CreateDemo::viewVariant(int i)
{
    ListTasksLog tempList;
    QString s = ltl.getListConditionTasks().at(currentTask);
    tempList.appendCondition(s);
    tempList.appendTask(ltl.getListVariants(currentTask).at(i), s);
    vt->set(tempList, false);
    vt->visibleComboBox(false);//скрываем за ненадобностью (комбобокс отвечает за просмотр разных типов задач)
    emit viewTaskInRedact();
}

void CreateDemo::deleteVariant(int i)
{
    ltl.removeVariant(currentTask, i);
    updateScrollForVariants(currentTask);
    FileManager::saveDemo(ltl);
}

void CreateDemo::redactVariant(int i)
{
    //сбросить все кнопки в состаяние редактировать, кроме той, что породила сигнал
    bool isRedact = false; //сейчс редактируется какая нибудь задача?
    for(int j = 0; j < listDemoVariant.length(); j++){
        if(listDemoVariant[j]->isSave()){
            isRedact = true;
            break;
        }
    }
    if(isRedact){
        if(QMessageBox::question(this, "Отмена",
                                 "Завершить текущее редактирование без дополнительного сохранения?",
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Cancel) == QMessageBox::Ok){
            for(int j = 0; j < listDemoVariant.length(); j++){
                listDemoVariant[j]->setPBRedact();
            }
            listDemoVariant[i]->setPBSave();

            ui->widget->setFromDigest(ltl.getListTasks().at(currentTask).at(i));
            construct->updatePandL();
        }
    }
    else{
        for(int j = 0; j < listDemoVariant.length(); j++){
            listDemoVariant[j]->setPBRedact();
        }
        listDemoVariant[i]->setPBSave();

        ui->widget->setFromDigest(ltl.getListTasks().at(currentTask).at(i));
        construct->updatePandL();
    }
}

void CreateDemo::saveVariant(int i, QString s)
{
    DigestOgl d = ui->widget->getDigest();
    d.report = s;
    ltl.updateDigest(currentTask, i, d);
    FileManager::saveDemo(ltl);
}


void CreateDemo::setTaskType()
{
    QSqlQuery query(db->getDb("Content"));
    query.exec("SELECT DISTINCT TaskType FROM Content;");
    QSqlRecord rec = query.record();

    taskType.clear();
    ui->comboBox_task_type->clear();

    while(query.next()){
        taskType.append(query.value(rec.indexOf("TaskType")).toString());
    }
    ui->comboBox_task_type->addItems(taskType);
}

void CreateDemo::updateMiniTasks(QString s)
{
    QSqlQuery query(db->getDb("Content"));
    query.exec("SELECT id, condition FROM Content WHERE TaskType='" + s + "';");
    QSqlRecord rec = query.record();
    Tasks.clear();
    miniTasks.clear();
    idMiniTask.clear();
    QString tem;
    int k = 1;
    char ch[10];
    while(query.next()){
        tem = query.value(rec.indexOf("condition")).toString();
        idMiniTask.append(query.value(rec.indexOf("id")).toInt());
        Tasks.append(tem);
        if(tem.length() > 20) tem.remove(70, tem.length() - 45);
        itoa(k, ch, 10);
        tem = ")   " + tem;
        miniTasks.append(ch + tem);
        k++;
    }
    tasksForNewTask.clear();
    for(int i = 0; i < miniTasks.length(); i++){
        tasksForNewTask.append(miniTasks[i]);
    }
}

void CreateDemo::on_pushButton_answer_clicked()
{
    answerDialog->setWindow("answer");
    answerDialog->showNormal();
}


void CreateDemo::answer_inputed(QString s)
{
    QString a = ui->widget->isTrueAnswer(s);
    if(a == "Верно!") ui->widget->drawTrueAnswer();
    QMessageBox::information(this, " ", a, 0, 0);
}

void CreateDemo::backStep()
{
    construct->backStep();
}

void CreateDemo::nextStep()
{
    construct->nextStep();
}

void CreateDemo::clearAll()
{
    if(QMessageBox::question(this, "Очистить всё",
                                   "Вы, действительно, хотите очисить всё?",
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel) == QMessageBox::Ok)
    {

        ui->field_task->clear();
        ui->widget->clear();
        construct->clearAll();
        Point::rr = 0;
        Point::gg = 0;
        Point::bb = 0;

        ui->frame_task->setVisible(false);
        ui->frame_listTasks->setVisible(true);
    }
}

void CreateDemo::closeApp()
{
    answerDialog->close();
}

void CreateDemo::on_pushButton_addDemoTask_clicked()
{
    iLastTaskType = ui->comboBox_task_type->currentIndex();
    bool ok;
    QString task = QInputDialog::getItem(this, "", "Выберите задачу", tasksForNewTask, 0, false, &ok);
    if(!ok) return;

    int index = 0;
    for(index = 0; index < tasksForNewTask.length(); index++){
        if(task == tasksForNewTask[index]) break;
    }

    currentTask = 0;
    QList<QString> cond = ltl.getListConditionTasks();
    for(int i = 0; i < cond.length(); i++){
        if(Tasks[index] == cond[i]){
            QMessageBox::information(this, "Ошибка", "Задача уже находиться в списке.");
            return;
        }
        currentTask++;
    }

    openTask(index);

    ltl.appendCondition(ui->field_task->toPlainText());

    updateScrollForTasks();
    updateScrollForVariants(currentTask);
    taskSelected();
}

void CreateDemo::taskSelected()
{
    ui->frame_task->setVisible(true);
    ui->frame_listTasks->setVisible(false);
}

void CreateDemo::updateScrollForTasks()
{
    ui->scrollArea_listDemoTasks->widget()->close();
    QWidget *w = new QWidget;
    QVBoxLayout *layoutForTasks = new QVBoxLayout;

    QList<QString> cond = ltl.getListConditionTasks();
    for(int i = 0; i < cond.length(); i++){
        DemoTask *dt = new DemoTask;
        dt->setText(cond[i]);
        int index = 0;
        for(index = 0; index < Tasks.length(); index++){
            if(cond[i] == Tasks[index]) break;
        }
        dt->setIndex(i, index);
        connect(dt, SIGNAL(redactTask(int,int)), this, SLOT(redactTask(int,int)));
        connect(dt, SIGNAL(deleteTask(int)), this, SLOT(deleteTask(int)));
        layoutForTasks->addWidget(dt);
    }

    w->setAttribute(Qt::WA_DeleteOnClose);
    w->setLayout(layoutForTasks);
    ui->scrollArea_listDemoTasks->setWidget(w);

    layoutForTasks->addSpacerItem(new QSpacerItem(20, 30, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

void CreateDemo::on_comboBox_task_type_currentIndexChanged(const QString &arg1)
{
    ltl = FileManager::openDemo(arg1);
    ltl.setTest(arg1);
    currentIndexMiniTasks = arg1;
    updateMiniTasks(currentIndexMiniTasks);
    updateScrollForTasks();
    construct->updatePandL();
}

void CreateDemo::on_pushButton_closeTask_clicked()
{
    bool isRedact = false; //сейчс редактируется какая нибудь задача?
    for(int i = 0; i < listDemoVariant.length(); i++){
        if(listDemoVariant[i]->isSave()){
            isRedact = true;
            break;
        }
    }
    if(isRedact){
        if(QMessageBox::question(this, "Отмена",
                                 "Завершить текущее редактирование без дополнительного сохранения?",
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Cancel) == QMessageBox::Ok){
            on_comboBox_task_type_currentIndexChanged(ui->comboBox_task_type->currentText());
            ui->frame_task->setVisible(false);
            ui->frame_listTasks->setVisible(true);

            ui->field_task->clear();
            ui->widget->clear();
            construct->updatePandL();
        }
    }
    else {
        on_comboBox_task_type_currentIndexChanged(ui->comboBox_task_type->currentText());
        ui->frame_task->setVisible(false);
        ui->frame_listTasks->setVisible(true);

        ui->field_task->clear();
        ui->widget->clear();
        construct->updatePandL();
    }
}

void CreateDemo::on_pushButton_saveVariant_clicked()
{
    bool isRedact = false; //сейчс редактируется какая нибудь задача?
    for(int i = 0; i < listDemoVariant.length(); i++){
        if(listDemoVariant[i]->isSave()){
            isRedact = true;
            break;
        }
    }
    if(isRedact){
        if(QMessageBox::question(this, "Отмена",
                                 "Завершить текущее редактирование без дополнительного сохранения?",
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Cancel) == QMessageBox::Ok){
            DigestOgl d = ui->widget->getDigest();
            d.report = "Новый вариант решения";
            ltl.appendTask(d, ui->field_task->toPlainText());
            FileManager::saveDemo(ltl);
            //устанавливаем на всех кнопках редактировать, кроме последней
            for(int i = 0; i < listDemoVariant.length(); i++){
                listDemoVariant[i]->setPBRedact();
            }
            updateScrollForVariants(currentTask);
            listDemoVariant[listDemoVariant.length() - 1]->setPBSave();
        }
    }
    else {
        DigestOgl d = ui->widget->getDigest();
        d.report = "Новый вариант решения";
        ltl.appendTask(d, ui->field_task->toPlainText());
        FileManager::saveDemo(ltl);
        updateScrollForVariants(currentTask);
        listDemoVariant[listDemoVariant.length() - 1]->setPBSave();
    }
}

void CreateDemo::updateScrollForVariants(int i)
{
    if(ltl.getListTasks().length() <= i) return;
    QList<DigestOgl> list = ltl.getListVariants(i);

    ui->scrollArea_listDemoVariants->widget()->close();
    QWidget *w = new QWidget;
    QVBoxLayout *layoutForVariants = new QVBoxLayout;
    listDemoVariant.clear();
    for(int i = 0; i < list.length(); i++){
        DemoVariant *dv = new DemoVariant;
        dv->setText(list[i].report);
        dv->setIndex(i);
        connect(dv, SIGNAL(viewVariant(int)), this, SLOT(viewVariant(int)));
        connect(dv, SIGNAL(deleteVariant(int)), this, SLOT(deleteVariant(int)));
        connect(dv, SIGNAL(redactVariant(int)), this, SLOT(redactVariant(int)));
        connect(dv, SIGNAL(saveVariant(int,QString)), this, SLOT(saveVariant(int,QString)));
        layoutForVariants->addWidget(dv);
        listDemoVariant.append(dv);
    }

    w->setAttribute(Qt::WA_DeleteOnClose);
    w->setLayout(layoutForVariants);
    ui->scrollArea_listDemoVariants->setWidget(w);

    layoutForVariants->addSpacerItem(new QSpacerItem(20, 30, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

void CreateDemo::openTask(int index)
{
    QSqlQuery query(db->getDb("Content"));
    query.exec("SELECT * FROM Content WHERE TaskType='" + ui->comboBox_task_type->currentText() + "' AND condition='" + Tasks[index] + "';");
    QSqlRecord rec = query.record();
    if(query.first()){
       ui->field_task->setText(Tasks[index]);
       ui->widget->clear();
       ui->widget->openTask(query.value(rec.indexOf("PointName")).toString(),
                            query.value(rec.indexOf("PointX")).toString(),
                            query.value(rec.indexOf("PointY")).toString(),
                            query.value(rec.indexOf("PointZ")).toString(),
                            query.value(rec.indexOf("LineName")).toString(),
                            query.value(rec.indexOf("LineList")).toString(),
                            query.value(rec.indexOf("LinePFL1")).toString(),
                            query.value(rec.indexOf("LinePFL2")).toString(),
                            query.value(rec.indexOf("TaskType")).toString(),
                            query.value(rec.indexOf("FlatList")).toString(),
                            query.value(rec.indexOf("Answer")).toString());
    }

    ui->widget->update();
    construct->updatePandL();

}

void CreateDemo::on_pushButton_deleteDemoType_clicked()
{
    if(QMessageBox::question(this, "Удалить",
                                   "Вы, действительно, хотите удалить все демо данного типа?",
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel) == QMessageBox::Ok){
        FileManager::deleteDemo(ui->comboBox_task_type->currentText());
        ltl.removeAllTasks();
        updateScrollForTasks();
    }
}
