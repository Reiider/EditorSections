#include "createtask.h"
#include "ui_createtask.h"

CreateTask::CreateTask(QWidget *parent, DataBase *db) :
    QWidget(parent),
    ui(new Ui::CreateTask)
{
    ui->setupUi(this);

    this->db = db;

    construct = new Constructing(0, ui->widget,db);
    ui->tab1->layout()->addWidget(construct);

    setMaxId();

    redactTask = false;

    saveFigure = false;

    setTaskType();

    TypeAnswer.append("Авторский ответ");
    TypeAnswer.append("Пересечение прямой и плоскости");
    TypeAnswer.append("Пересечение двух плоскостей");
    TypeAnswer.append("Секущая плоскость по трем точкам");


    ui->sA_blockActions->setVisible(false);
    ui->sA_blockActions_red->setVisible(false);
    createListBlockActions();

    QList<int> sizesSplitter;
    sizesSplitter.append(434);
    sizesSplitter.append(100000);
    ui->splitter->setSizes(sizesSplitter);

    answerDialog = new WindowDialogs();
    connect(answerDialog, SIGNAL(s_authorAnswer(QString)), this, SLOT(s_authorAnswer(QString)));
    connect(answerDialog, SIGNAL(s_intersecLineWithFlat(QString,QString)), this, SLOT(s_intersecLineWithFlat(QString,QString)));
    connect(answerDialog, SIGNAL(s_intersecTwoFlats(QString,QString)), this, SLOT(s_intersecTwoFlats(QString,QString)));
    connect(answerDialog, SIGNAL(s_cuttingPlane(QString)), this, SLOT(s_cuttingPlane(QString)));
    connect(answerDialog, SIGNAL(s_updateCuttingPlane(QString)), this, SLOT(s_updateCuttingPlane(QString)));
    connect(answerDialog, SIGNAL(s_updateIntersecTwoFlats(QString,QString)), this, SLOT(s_updateIntersecTwoFlats(QString,QString)));
    connect(answerDialog, SIGNAL(s_saveTask()), this, SLOT(s_saveTask()));
}

CreateTask::~CreateTask()
{
    delete ui;
}

void CreateTask::closeApp()
{
    answerDialog->close();
}

void CreateTask::showThisWindow()
{
    setTaskType();
    updateMiniTasks(currentIndexMiniTasks);

    ui->widget->repaintAll();

    construct->updatePandL();
}

void CreateTask::clearAll()
{
    if(QMessageBox::question(this, "Очистить всё",
                                   "Вы, действительно, хотите очисить всё?",
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel) == QMessageBox::Ok)
    {
        ui->field_textTask->clear();
        ui->widget->clear();

        ui->scrollArea_create_task->setToolTip("");
        ui->scrollArea_create_task->setEnabled(true);
        ui->scrollArea_redact_task->setEnabled(false);

        construct->clearAll();

        Point::rr = 0;
        Point::gg = 0;
        Point::bb = 0;
    }
}

void CreateTask::nextStep()
{
    construct->nextStep();
}

void CreateTask::backStep()
{
    construct->backStep();
}

QString CreateTask::getTextTask()
{
    if(redactTask) return ui->field_textTask_redact->toPlainText();
    else return ui->field_textTask->toPlainText();
}

void CreateTask::createImgBMP(QString path)
{
    ui->widget->createImgBMP(path);
}

void CreateTask::setMaxId()
{
    QSqlQuery query(db->getDb("Content"));
    query.exec("SELECT Id FROM Content;");
    QSqlRecord rec = query.record();
    maxId = 0;
    while(query.next()){
        if(query.value(rec.indexOf("Id")).toInt() > maxId) maxId = query.value(rec.indexOf("Id")).toInt();
    }
}

void CreateTask::createListBlockActions()
{
    QVBoxLayout *vbox1 = new QVBoxLayout();
    QVBoxLayout *vbox2 = new QVBoxLayout();
    ui->sA_blockActions->widget()->setLayout(vbox1);
    ui->sA_blockActions_red->widget()->setLayout(vbox2);

    createPushBatton("Добавить точку", 1);
    createPushBatton("Изменить расположение точки", 2);
    createPushBatton("Задать соотношение отрезков", 3);
    createPushBatton("Удалить точку", 4);
    createPushBatton("Добавить прямую", 5);
    createPushBatton("Построить отрезок равный заданному", 6);
    createPushBatton("Добавить параллельную прямую", 7);
    createPushBatton("Провести перпендикуляр", 8);
    createPushBatton("Удалить прямую", 9);
    createPushBatton("Пересечение двух прямых", 10);
    vbox1->addSpacerItem(new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Expanding));
    vbox2->addSpacerItem(new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

void CreateTask::createPushBatton(QString name, int id)
{
    ButtonBlockAction *pb = new ButtonBlockAction(name, id);
    listBlockActions.append(pb);
}

void CreateTask::updateBlockActionsString()
{
    blockActions.clear();
    for(int i = 0; i < listBlockActions.length(); i++){
        if(listBlockActions[i]->isBlocked()){
            blockActions += QString::number(listBlockActions[i]->getId())+ " ";
        }
    }
}

void CreateTask::resetBlockActions()
{
    ui->sA_blockActions->setVisible(false);
    ui->sA_blockActions_red->setVisible(false);
    for(int i = 0; i < listBlockActions.length(); i++){
        listBlockActions[i]->reset();
    }
}

void CreateTask::setBlockActions(QString blockStr)
{
    QStringList list = blockStr.split(" ", QString::SkipEmptyParts);
    for(int i = 0; i < list.length(); i++){
        listBlockActions[list[i].toInt()-1]->clicked();
    }
}

void CreateTask::showActions(bool isRedact)
{
    for(int i = 0; i < listBlockActions.length(); i++){
        if(isRedact) ui->sA_blockActions_red->widget()->layout()->addWidget(listBlockActions[i]);
        else ui->sA_blockActions->widget()->layout()->addWidget(listBlockActions[i]);
    }
}

void CreateTask::on_pushButton_saveFigure_clicked()
{
    if(ui->widget->saveFigure()) saveFigure = true;
}

void CreateTask::on_pushButton_redact_clicked()
{
    bool flag = true;
    if(!ui->widget->getListPoints().isEmpty()){
        if(QMessageBox::Yes != QMessageBox::question(this, " ", "При переходе к редактированию задачи текущие данные НЕ будут сохранены. \nПродолжить?")){
           flag = false;
        }
    }
    if(flag && ui->comboBox_selestTask->currentText() != ""){
        resetBlockActions();

        ui->comboBox_TaskType_redact->setCurrentIndex(ui->comboBox_TaskType_2->currentIndex());

        ui->scrollArea_create_task->setToolTip("Завершите редактирование задачи прежде чем создавать новую");
        ui->scrollArea_create_task->setEnabled(false);
        ui->scrollArea_redact_task->setEnabled(true);

        redactTask = true;
        ui->widget->setRedactTask();

        QSqlQuery query(db->getDb("Content"));
        query.exec("SELECT * FROM Content WHERE condition='" + Tasks[ui->comboBox_selestTask->currentIndex()] + "' AND TaskType='" + ui->comboBox_TaskType_2->currentText() + "';");
        QSqlRecord rec = query.record();

        if(query.first()){
            ui->field_textTask_redact->setText(query.value(rec.indexOf("condition")).toString());
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
            setBlockActions(query.value(rec.indexOf("BlockActions")).toString());
        }

        ui->widget->update();
        construct->updatePandL();
    }

}

void CreateTask::on_pushButton_delete_clicked()
{
    if(QMessageBox::Yes == QMessageBox::question(this, "Подтверждение", "Вы действительно хотите удалить эту задачу?")){
        QSqlQuery query(db->getDb("Content"));
        QString DelFCWId = "DELETE FROM Content WHERE Id=";
        QString temp = "";
        temp = temp.number(idMiniTask[ui->comboBox_selestTask->currentIndex()]);
        query.exec(DelFCWId + temp + ";");
        updateMiniTasks(currentIndexMiniTasks);
        construct->updatePandL();
    }

}

void CreateTask::on_comboBox_TaskType_2_currentIndexChanged(const QString &arg1)
{
    currentIndexMiniTasks = arg1;
    updateMiniTasks(currentIndexMiniTasks);
    construct->updatePandL();
}

void CreateTask::on_pushButton_set_answer_clicked()
{
    if(!saveFigure){
           QMessageBox::information(this, " ", "Сохраните построенную фигуру.", 0, 0);
    }
    else{

        bool ok;
        QString typeAnswer = QInputDialog::getItem(this, "Ответ", "Выберите тип ответа", TypeAnswer, 0, false, &ok);
        if (ok){
            if("Авторский ответ" == typeAnswer){
                answerDialog->setWindow("AuthorAnswer");
                answerDialog->showNormal();
            }
            if("Пересечение прямой и плоскости" == typeAnswer){
                answerDialog->setWindow("IntersecLineWithFlat");
                answerDialog->showNormal();
            }
            if("Пересечение двух плоскостей" == typeAnswer){
                answerDialog->setWindow("IntersecTwoFlats");
                answerDialog->showNormal();
            }
            if("Секущая плоскость по трем точкам" == typeAnswer){
                answerDialog->setWindow("CuttingPlane");
                answerDialog->showNormal();
            }
        }
    }
}

void CreateTask::s_authorAnswer(QString answer)
{
    QString s = "";
    if(answer.split(" ", QString::SkipEmptyParts).length() > 2){
        if(QMessageBox::question(this, "?",
                                 "Данный ответ будет являтся секущей плоскостью?")
                                  == QMessageBox::No){
            s = ui->widget->setAnswer("Авторский ответ по точкам", answer, "no");
        }
        else s = ui->widget->setAnswer("Авторский ответ по точкам", answer, "");
    }
    else{
        s = ui->widget->setAnswer("Авторский ответ по точкам", answer, "");
    }

    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
    construct->updatePandL();
}

void CreateTask::s_intersecLineWithFlat(QString answer1, QString answer2)
{
    QString s = ui->widget->setAnswer("Пересечение прямой и плоскости", answer1, answer2);
    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
    else{
        answerDialog->setWindow("UpdateCuttingPlane");
        answerDialog->showNormal();
    }
    construct->updatePandL();
}

void CreateTask::s_intersecTwoFlats(QString answer1, QString answer2)
{
    QString s = ui->widget->setAnswer("Пересечение двух плоскостей", answer1, answer2);
    if(s!=""){
        if(s == "1"){
            answerDialog->setWindow("UpdateIntersecTwoFlats");
            answerDialog->showNormal();
        }
        else QMessageBox::information(this, " ", s, 0, 0);
    }
    construct->updatePandL();
}

void CreateTask::s_cuttingPlane(QString answer)
{
    QString s = ui->widget->setAnswer("Секущая плоскость", answer, "");
    if(s!=""){
        if(s == "1"){
            answerDialog->setWindow("UpdateCuttingPlane");
            answerDialog->showNormal();
        }
        else QMessageBox::information(this, " ", s, 0, 0);
    }
    construct->updatePandL();
}

void CreateTask::s_updateCuttingPlane(QString answer)
{
    QString s = ui->widget->updateAnswerCuttingPlane(answer);
    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
}

void CreateTask::s_updateIntersecTwoFlats(QString answer1, QString answer2)
{
    QString s = ui->widget->updateIntersecTwoFlats(answer1, answer2);
    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
}

void CreateTask::on_pushButton_2_clicked()
{
    QString s = ui->lineNewTaskType->text();
    ui->comboBox_TaskType->addItem(s);
    ui->comboBox_TaskType_2->addItem(s);
    ui->comboBox_TaskType->setCurrentText(s);
    ui->lineNewTaskType->setText("");
}

void CreateTask::on_pushButton_select_figure_clicked()
{
    construct->createFigure();
}

void CreateTask::on_pushButton_save_task_clicked()
{
    QString string;
    if(ui->field_textTask->toPlainText() == "" && (!redactTask || ui->field_textTask_redact->toPlainText() == ""))
        QMessageBox::information(this, " ", "Введите текст задачи.", 0, 0);
    else if(!saveFigure)
       QMessageBox::information(this, " ", "Сохраните построенную фигуру.", 0, 0);
    else if(ui->widget->getAnswer() == "")
       QMessageBox::information(this, " ", "Введите ответ на задачу.", 0, 0);
    else {
        string = "Проверьте правильность введенных данных.\n\n";
        if(redactTask) string += "Текст задачи: " + ui->field_textTask_redact->toPlainText() + "\n";
            else string += "Текст задачи: " + ui->field_textTask->toPlainText() + "\n";
        string += "Ответ: " + ui->widget->getAnswer() + "\n";
        if(redactTask) string += "Тип задачи: " + ui->comboBox_TaskType_redact->currentText();
        else string += "Тип задачи: " + ui->comboBox_TaskType->currentText();
        answerDialog->setWindow("SaveTask", string);
        answerDialog->showNormal();
    }
}

void CreateTask::s_saveTask()
{
    QString PointName;
    QString PointX;
    QString PointY;
    QString PointZ;
    QString LineName;
    QString LineList;
    QString LinePFL1;
    QString LinePFL2;
    QString TaskType;
    QString FlatList;
    QString Answer;
    QString info = ui->widget->getSaveTask(  &PointName,
                                             &PointX,
                                             &PointY,
                                             &PointZ,
                                             &LineName,
                                             &LineList,
                                             &LinePFL1,
                                             &LinePFL2,
                                             &TaskType,
                                             &FlatList,
                                             &Answer);
    if(info != ""){
        if(info != "processed"){
            QMessageBox::information(this, " ", info, 0, 0);
        }
    }
    else {
        updateBlockActionsString();
        QSqlQuery query(db->getDb("Content"));

        if(redactTask){
            TaskType = ui->comboBox_TaskType_redact->currentText();
            query.exec("SELECT * FROM Content WHERE condition='" + Tasks[ui->comboBox_selestTask->currentIndex()] + "';");
            QSqlRecord rec = query.record();
            if(query.first()){
                QString id = query.value(rec.indexOf("Id")).toString();

                query.exec("UPDATE Content SET condition='"+ui->field_textTask_redact->toPlainText()+"' WHERE Id="+id+";");
                query.exec("UPDATE Content SET PointName='"+PointName+"' WHERE Id="+id+";");
                query.exec("UPDATE Content SET PointX='"+PointX+"' WHERE Id="+id+";");
                query.exec("UPDATE Content SET PointY='"+PointY+"' WHERE Id="+id+";");
                query.exec("UPDATE Content SET PointZ='"+PointZ+"' WHERE Id="+id+";");
                query.exec("UPDATE Content SET LineName='"+LineName+"' WHERE Id="+id+";");
                query.exec("UPDATE Content SET LineList='"+LineList+"' WHERE Id="+id+";");
                query.exec("UPDATE Content SET LinePFL1='"+LinePFL1+"' WHERE Id="+id+";");
                query.exec("UPDATE Content SET LinePFL2='"+LinePFL2+"' WHERE Id="+id+";");
                query.exec("UPDATE Content SET TaskType='"+TaskType+"' WHERE Id="+id+";");
                query.exec("UPDATE Content SET FlatList='"+FlatList+"' WHERE Id="+id+";");
                query.exec("UPDATE Content SET Answer='"+Answer+"' WHERE Id="+id+";");
                query.exec("UPDATE Content SET BlockActions='"+blockActions+"' WHERE Id="+id+";");

                ui->scrollArea_create_task->setToolTip("");
                ui->scrollArea_create_task->setEnabled(true);
                ui->scrollArea_redact_task->setEnabled(false);

                ui->widget->unsetRedactTask();
            }

        }
        else{
            TaskType = ui->comboBox_TaskType->currentText();
            maxId++;

            query.prepare("INSERT INTO Content VALUES (:id, :string1, :string2, :string3, :string4, :string5, :string6, :string7, :string8, :string9, :string10, :string11, :string12, :string13);");
            query.bindValue(":id", maxId);
            query.bindValue(":string1", ui->field_textTask->toPlainText());
            query.bindValue(":string2", PointName);
            query.bindValue(":string3", PointX);
            query.bindValue(":string4", PointY);
            query.bindValue(":string5", PointZ);
            query.bindValue(":string6", LineName);
            query.bindValue(":string7", LineList);
            query.bindValue(":string8", LinePFL1);
            query.bindValue(":string9", LinePFL2);
            query.bindValue(":string10", TaskType);
            query.bindValue(":string11", FlatList);
            query.bindValue(":string12", Answer);
            query.bindValue(":string13", blockActions);
            query.exec();
        }

        ui->widget->clear();
        ui->widget->update();
        construct->updatePandL();
        saveFigure = false;
        redactTask = false;

        construct->setElementVisible("TiltPrism", false);
        ui->field_textTask->setText("");
        ui->field_textTask_redact->setText("");
        updateMiniTasks(currentIndexMiniTasks);
        ui->lineNewTaskType->setText("");
        ui->lineNewTaskType_redact->setText("");

        Point::rr = 0;
        Point::gg = 0;
        Point::bb = 0;
        ui->widget->setStandartColorToLine(0, 0, 0);
        ui->widget->setStandartColorToText(Qt::red);

        resetBlockActions();
    }
    updateMiniTasks(currentIndexMiniTasks);
}

void CreateTask::on_pushButton_select_figure_redact_clicked()
{
    on_pushButton_select_figure_clicked();
}

void CreateTask::on_pushButton_saveFigure_redact_clicked()
{
    on_pushButton_saveFigure_clicked();
}

void CreateTask::on_pushButton_create_type_redact_clicked()
{
    QString s = ui->lineNewTaskType_redact->text();
    ui->comboBox_TaskType_redact->addItem(s);
    ui->comboBox_TaskType_2->addItem(s);
    ui->comboBox_TaskType_redact->setCurrentText(s);
    ui->lineNewTaskType_redact->setText("");
}

void CreateTask::on_pushButton_set_answer_redact_clicked()
{
    on_pushButton_set_answer_clicked();
}

void CreateTask::on_pushButton_save_task_redact_clicked()
{
    on_pushButton_save_task_clicked();
}

void CreateTask::on_pushButton_4_clicked()
{
    if(QMessageBox::Yes != QMessageBox::question(this, " ", "Отменить редактирование задачи?")){
       return;
    }
    ui->comboBox_TaskType_redact->setCurrentIndex(ui->comboBox_TaskType_2->currentIndex());

    ui->scrollArea_create_task->setToolTip("");
    ui->scrollArea_create_task->setEnabled(true);
    ui->scrollArea_redact_task->setEnabled(false);

    ui->widget->clear();
    ui->field_textTask_redact->setText("");

    Point::rr = 0;
    Point::gg = 0;
    Point::bb = 0;
    ui->widget->setStandartColorToLine(0, 0, 0);
    ui->widget->setStandartColorToText(Qt::red);

    redactTask = false;
    ui->widget->unsetRedactTask();

}

void CreateTask::on_pB_blockActions_clicked()
{
    if(ui->pB_blockActions->isChecked()){
        showActions(false);
        ui->sA_blockActions->setVisible(true);
    }
    else{
        ui->sA_blockActions->setVisible(false);
    }
}

void CreateTask::on_pB_blockActions_red_clicked()
{
    if(ui->pB_blockActions_red->isChecked()){
        showActions(true);
        ui->sA_blockActions_red->setVisible(true);
    }
    else{
        ui->sA_blockActions_red->setVisible(false);
    }
}

void CreateTask::updateMiniTasks(QString s)
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
    ui->comboBox_selestTask->clear();
    ui->comboBox_selestTask->addItems(miniTasks);
}

void CreateTask::setTaskType()
{
    QSqlQuery query(db->getDb("Content"));
    query.exec("SELECT DISTINCT TaskType FROM Content;");
    QSqlRecord rec = query.record();

    ui->comboBox_TaskType->clear();
    ui->comboBox_TaskType_2->clear();
    ui->comboBox_TaskType_redact->clear();
    taskType.clear();

    while(query.next()){
        taskType.append(query.value(rec.indexOf("TaskType")).toString());
    }
    ui->comboBox_TaskType->addItems(taskType);
    ui->comboBox_TaskType_2->addItems(taskType);
    ui->comboBox_TaskType_redact->addItems(taskType);
}
