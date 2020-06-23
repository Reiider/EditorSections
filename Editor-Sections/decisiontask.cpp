#include "decisiontask.h"
#include "ui_decisiontask.h"

DecisionTask::DecisionTask(QWidget *parent, DataBase *db) :
    QWidget(parent),
    ui(new Ui::DecisionTask)
{
    ui->setupUi(this);

    this->db = db;

    construct = new Constructing(0, ui->widget,db);
    ui->tab1->layout()->addWidget(construct);
    construct->setsForDecisionTask();

    setTaskType();
    construct->updatePandL();
    loadTests();

    testOpened = false;
    firstSave = true;

    QList<int> sizesSplitter;
    sizesSplitter.append(434);
    sizesSplitter.append(100000);
    ui->splitter->setSizes(sizesSplitter);

    answerDialog = new WindowDialogs();
    connect(answerDialog, SIGNAL(text_inputed(QString)), this, SLOT(answer_inputed(QString)));

    connect(&tToSaveTest, SIGNAL(timeout()), this, SLOT(tempSaveTestResult()));
    tToSaveTest.setInterval(10000);
}

DecisionTask::~DecisionTask()
{
    delete ui;
}

void DecisionTask::createPrism()
{
    bool ok;
    int v = QInputDialog::getInt(this, "Призма", "Укажите количество вершин в основании.", 3, 3, 1000, 1, &ok);
    if (ok){
        ui->widget->setColVershin(v);
        ui->widget->createPrism(1);

    }

    construct->updatePandL();
}

void DecisionTask::createPyramid()
{
    bool ok;
    int v = QInputDialog::getInt(this, "Пирамида", "Укажите количество вершин в основании.", 3, 3, 1000, 1, &ok);
    if (ok){
        ui->widget->setColVershin(v);
        ui->widget->createPyramid();

    }
    construct->updatePandL();
}

void DecisionTask::createCube()
{
    ui->widget->setColVershin(8);
    ui->widget->createCube();

    construct->updatePandL();
}

void DecisionTask::showThisWindow()
{
    Point::rr = 0;
    Point::gg = 0;
    Point::bb = 0;

    setTaskType();
    updateMiniTasks(currentIndexMiniTasks);
    ui->widget->repaintAll();

    construct->updatePandL();
    loadTests();
}

void DecisionTask::setUserName(QString name)
{
    UserName = name;
}

QList<QString> DecisionTask::getTasksType()
{
    return taskType;
}

QString DecisionTask::getTextTask()
{
    return ui->field_task->toPlainText();
}

void DecisionTask::closeApp()
{
    answerDialog->close();
}

void DecisionTask::openTest()
{
    makeTest();
}

void DecisionTask::setTaskType()
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

void DecisionTask::updateMiniTasks(QString s)
{
    QSqlQuery query(db->getDb("Content"));
    query.exec("SELECT * FROM Content;");
    QSqlRecord rec = query.record();
    Tasks.clear();
    miniTasks.clear();
    idMiniTask.clear();
    QString tem;
    int k = 1;
    char ch[10];
    while(query.next()){
        if(query.value(rec.indexOf("TaskType")).toString() == s){
            tem = query.value(rec.indexOf("condition")).toString();
            idMiniTask.append(query.value(rec.indexOf("id")).toInt());
            Tasks.append(tem);
            if(tem.length() > 20) tem.remove(70, tem.length() - 45);
            itoa(k, ch, 10);
            tem = ")   " + tem;
            miniTasks.append(ch + tem);
            k++;
        }
    }
    ui->comboBox_task->clear();
    for(int i = 0; i < miniTasks.length(); i++){
        ui->comboBox_task->addItem(miniTasks[i]);
    }
}

void DecisionTask::on_pushButton_answer_clicked()
{
    answerDialog->setWindow("answer");
    answerDialog->showNormal();
}


void DecisionTask::answer_inputed(QString s)
{
    QString a = ui->widget->isTrueAnswer(s);
    if(a == "Верно!") ui->widget->drawTrueAnswer();
    QMessageBox::information(this, " ", a, 0, 0);
    if(a == "Верно!"){
        ui->widget->drawTrueAnswer();
        if(testOpened){
            currentButtonTask->setEnabled(false);
            QColor *color = new QColor(0,240,0);
            QPalette *p = new QPalette(*color);
            currentButtonTask->setPalette(*p);

            ltl.updateTask(ui->widget->getDigest());
            QString path = FileManager::saveBinaryFile(ltl);
            saveResult(path);

            ui->tabWidget->setCurrentIndex(2);
        }
    }
}

void DecisionTask::backStep()
{
    construct->backStep();
}

void DecisionTask::nextStep()
{
    construct->nextStep();
}

void DecisionTask::clearAll()
{
    if(QMessageBox::question(this, "Очистить всё",
                                   "Вы действительно хотите очисить всё?",
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel) == QMessageBox::Ok)
    {
        ui->field_task->clear();
        ui->widget->clear();
        construct->clearAll();
        Point::rr = 0;
        Point::gg = 0;
        Point::bb = 0;
    }
}

void DecisionTask::on_pushButton_select_task_clicked()
{

    if(ui->comboBox_task->currentText() != ""){
        QSqlQuery query(db->getDb("Content"));
        query.exec("SELECT * FROM Content WHERE condition='"+ Tasks[ui->comboBox_task->currentIndex()] +"';");
        QSqlRecord rec = query.record();
        if(query.first()){
            ui->field_task->setText(Tasks[ui->comboBox_task->currentIndex()]);
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
            setAvailableToActions(query.value(rec.indexOf("BlockActions")).toString());
        }
        currentTypeTask = ui->comboBox_task_type->currentText();
        currentTask = ui->field_task->toPlainText();

        ui->widget->update();
        construct->updatePandL();
    }

}

void DecisionTask::on_comboBox_task_type_currentIndexChanged(const QString &arg1)
{
    currentIndexMiniTasks = arg1;
    updateMiniTasks(currentIndexMiniTasks);
    construct->updatePandL();
}

void DecisionTask::selectTest(QString test, int n)
{
    ui->widget->clear();
    ltl.setLogin(UserName);
    ltl.setTest(test);

    QSqlQuery query(db->getDb("Content"));
    query.exec("SELECT * FROM Test WHERE TestName='"+ test +"';");
    listIdTask.clear();
    QList<QString> listTextTask;

    if(query.first()){
        QStringList listVar = query.value(1).toString().split(";", QString::SkipEmptyParts);  // 1 == ListTasks
        if(listVar.length() < n) {
            QMessageBox::information(this, "Ошибка", "Ошибка при открытии теста. 734692");
            return;
        }
        QStringList listNTask = listVar[n].split(" ", QString::SkipEmptyParts);
        for(int i = 0; i < listNTask.length(); i++){
            listIdTask.append(listNTask[i]);
            QSqlQuery content(db->getDb("Content"));
            content.exec("SELECT condition FROM Content WHERE Id="+listNTask[i]+";");
            if(content.first()) listTextTask.append(content.value(0).toString());
            else listTextTask.append("Задача была удалена.");
        }
    }
    else{
        QMessageBox::information(this, "Ошибка", "Ошибка при открытии теста. 817564");
        return;
    }


    listButtonTask.clear();
    QVBoxLayout *layout = new QVBoxLayout;
    //добавляем название теста
    QHBoxLayout *horL = new QHBoxLayout;
    horL->addSpacerItem(new QSpacerItem(20, 30, QSizePolicy::Expanding, QSizePolicy::Expanding));
    QLabel *label = new QLabel(test);
    label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    QFont *f = new QFont();
    f->setBold(true);
    label->setFont(*f);
    horL->addWidget(label);
    horL->addSpacerItem(new QSpacerItem(20, 30, QSizePolicy::Expanding, QSizePolicy::Expanding));
    layout->addLayout(horL);
    //
    for(int i = 0; i < listIdTask.length() ; i++){      // по всему списку заданий из теста, заполняем список заданиями
        QHBoxLayout *hl = new QHBoxLayout;
        QPlainTextEdit *t = new QPlainTextEdit(listTextTask[i]);
        ltl.appendCondition(listTextTask[i]); //добавляем условие задачи в класс для сохранения
        ButtonForTest *b = new ButtonForTest;
        b->button = new QPushButton("Решить задачу");
        b->id = listIdTask[i];

        if("Задача была удалена." == listTextTask[i]){
            b->button->setEnabled(false);
        }
        else {
            QObject::connect(b->button, SIGNAL(clicked()), b, SLOT(openTask()));
            QObject::connect(b, SIGNAL(openTask(QString)), this, SLOT(openTask(QString)));
            QObject::connect(b, SIGNAL(currentButton(QPushButton*)), this, SLOT(setCurrentButton(QPushButton*)));
        }

        t->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        t->setReadOnly(true);
        b->button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        listButtonTask.append(b->button);
        hl->addWidget(t);
        hl->addWidget(b->button);
        layout->addLayout(hl);
    }
    //* добавление кнопки
    QHBoxLayout *hl = new QHBoxLayout;
    hl->addSpacerItem(new QSpacerItem(20, 30, QSizePolicy::Expanding, QSizePolicy::Expanding));
    QPushButton *p = new QPushButton("Завершить тест");
    p->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    QObject::connect(p, SIGNAL(clicked()), this, SLOT(closeTest()));
    hl->addWidget(p);
    hl->addSpacerItem(new QSpacerItem(20, 30, QSizePolicy::Expanding, QSizePolicy::Expanding));
    layout->addLayout(hl);
    //
    layout->addSpacerItem(new QSpacerItem(20, 1000, QSizePolicy::Expanding, QSizePolicy::Expanding));

    QScrollArea *scroll = new QScrollArea;
    scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scroll->setWidgetResizable(true);

    QWidget *w = new QWidget;
    w->setLayout(layout);
    scroll->setWidget(w);

    QPalette palette;
    palette.setColor(QPalette::Background, Qt::white);
    scroll->setPalette(palette);
    scroll->setAttribute(Qt::WA_DeleteOnClose);

    ui->tabWidget->addTab(scroll, "Тест");
    ui->tabWidget->setCurrentIndex(2); // 2 указывает на вкладку с тестами
    ui->scrollArea_tasks->setEnabled(false); // делаем выбор задач и тестов неактивными
//    ui->pushButton_make_test->setEnabled(false);
    ui->scrollArea_tasks->setToolTip("Завершите тест перед тем как продолжить."); // добавляем подсказку
    testOpened = true;

    saveResult("");

    tToSaveTest.start();
}

void DecisionTask::saveResult(QString path)
{
    QSqlQuery qRes(db->getDb("Result"));

    qRes.exec("UPDATE Result SET FilePath='' WHERE FilePath='"+path+"';");

    if(firstSave){ // если это првое сохранение - добовляем
        qRes.prepare("INSERT INTO Result (Login, TestName, FilePath, AmountTasks, SolveTasks, TheDate, TheTime) VALUES (:Login, :TestName, :FilePath, :AmountTasks, :SolveTasks, :TheDate, :TheTime);");
        qRes.bindValue(":Login", UserName);
        qRes.bindValue(":TestName", ltl.getTest());
        qRes.bindValue(":FilePath", path);
        qRes.bindValue(":AmountTasks", listButtonTask.length());
        int k = 0;
        for(int i = 0; i < listButtonTask.length(); i++){
            if(!listButtonTask[i]->isEnabled()) k++;
        }
        qRes.bindValue(":SolveTasks", k);
        dataTest = QDateTime::currentDateTime().date().toString("yyyy.MM.dd");
        qRes.bindValue(":TheDate", dataTest);
        timeTest = QDateTime::currentDateTime().time().toString("hh:mm:ss");
        qRes.bindValue(":TheTime", timeTest);
        qRes.exec();
        firstSave = false;
    }
    else { //иначе - изменяем
        qRes.exec("SELECT Id FROM Result WHERE Login='" + UserName + "' AND TestName='" + ltl.getTest() + "' AND TheDate='" + dataTest + "' AND TheTime='" + timeTest + "';");
        QSqlRecord qResRec = qRes.record();
        qResRec = qRes.record();
        QString id = "";

        if(qRes.first()){
            id = qRes.value(qResRec.indexOf("Id")).toString();
        }

        if(path.length() > 11){
            qRes.exec("UPDATE Result SET FilePath='"+path+"' WHERE Id="+id+";");
        }
        int k = 0;
        for(int i = 0; i < listButtonTask.length(); i++){
            if(!listButtonTask[i]->isEnabled()) k++;
        }
        qRes.exec("UPDATE Result SET SolveTasks="+path.number(k)+" WHERE Id="+id+";");
    }
}

void DecisionTask::setAvailableToActions(QString blockActions)
{
    for(int i = 1; i <= 10; i++){
        construct->setVisibleAction(i, true);
    }
    QStringList list = blockActions.split(" ", QString::SkipEmptyParts);
    for(int i = 0; i < list.length(); i++){
        construct->setVisibleAction(list[i].toInt(), false);
    }
}

void DecisionTask::closeTest()
{
    QPushButton *p;
    bool end = true;
    for(int i = 0; i < listButtonTask.length(); i++){
     p = listButtonTask[i];
     if(p->isEnabled()) {
         end = false;
         break;
     }
    }
    if(!end){
        if(QMessageBox::Yes != QMessageBox::question(this, " ", "Некоторые задачи не решены, сохранить текущий результат и выйти?")){
            return;
        }
    }
    //сохранить результат
    ltl.updateTask(ui->widget->getDigest());
    QString path = FileManager::saveBinaryFile(ltl);
    saveResult(path);
    ListTasksLog l;
    ltl = l;
    firstSave = true;
    //
    int k = 0;
    for(int i = 0; i < listButtonTask.length(); i++){
        if(!listButtonTask[i]->isEnabled()) k++;
    }
    QMessageBox::information(this, "Результат", "Решено " + path.number(k) + " из " + path.number(listButtonTask.length()));

    testOpened = false;
    tToSaveTest.stop();

    ui->scrollArea_tasks->setEnabled(true);
//    ui->pushButton_make_test->setEnabled(true);
    ui->scrollArea_tasks->setToolTip("");
    ui->tabWidget->removeTab(2);
    ui->tabWidget->setCurrentIndex(0);
    emit setLableVariant("");
//    ui->label_variant->setText("");

    ui->field_task->clear();
    ui->widget->clear();
    construct->clearAll();

}

void DecisionTask::loadTests()
{
    QSqlQuery query(db->getDb("Content"));
    query.exec("SELECT TestName FROM Test;");
    listTest.clear();
    while(query.next()){
       listTest.append(query.value(0).toString());
    }
}

void DecisionTask::openTask(const QString &n)
{
    QString path = "";
    if(ltl.getI() != -1) {
        ltl.updateTask(ui->widget->getDigest());
        path = FileManager::saveBinaryFile(ltl);
    }
    // сохранение промежуточных результатов
    saveResult(path);

    QSqlQuery query(db->getDb("Content"));
    query.exec("SELECT * FROM Content WHERE Id=" + n + ";");
    QSqlRecord rec = query.record();
    if(query.first()){
       ui->field_task->setText(query.value(rec.indexOf("condition")).toString());
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
       setAvailableToActions(query.value(rec.indexOf("BlockActions")).toString());
    }
    else{
        QMessageBox::information(this, " ", "Ошибка, возможно задача с Id= "+n+" была только что удалена другим пользователем.");
    }
    ui->widget->update();
    construct->updatePandL();

    ltl.appendTask(ui->widget->getDigest(), ui->field_task->toPlainText());

    ui->tabWidget->setCurrentIndex(1);
}

void DecisionTask::setCurrentButton(QPushButton *button)
{
    currentButtonTask = button;
}

void DecisionTask::makeTest()
{
    if(testOpened){
        ui->tabWidget->setCurrentIndex(2);
        return;
    }
    loadTests();
    bool ok;
    int countVariants = 1;
    QString testName = QInputDialog::getItem(this, "Тесты", "Выберите тест", listTest, 0, false, &ok);
    if(ok) {
        QSqlQuery query(db->getDb("Content"));
        query.exec("SELECT * FROM Test;");
        QSqlRecord rec = query.record();
        while(query.next()){
           if(testName == query.value(rec.indexOf("TestName")).toString()){
               countVariants = query.value(rec.indexOf("CountVariants")).toInt();
               break;
           }
        }
        QList<QString> listVariants;
        for(int i = 0; i < countVariants; i++){
            listVariants.append("Вариант "+help.itoqs(i+1));
        }
        QString n = QInputDialog::getItem(this, testName, "Выберите вариант", listVariants, 0, false, &ok);
        if(ok) {
            emit setLableVariant(n);
            ltl.setVariant(n);
            selectTest(testName, listVariants.indexOf(n));
        }
    }
}

void DecisionTask::on_pushButton_2_clicked()
{
    bool ok;
    QInputDialog::getMultiLineText(this, "Отчет", "Отчет по текущей задаче", ui->widget->getReport(), &ok);
}

void DecisionTask::refresh()
{
    ui->widget->repaintAll();
}

void DecisionTask::createImgBMP(QString path)
{
    ui->widget->createImgBMP(path);
}

void DecisionTask::tempSaveTestResult()
{
    ltl.updateTask(ui->widget->getDigest());
    QString path = FileManager::saveBinaryFile(ltl);
    saveResult(path);
}
