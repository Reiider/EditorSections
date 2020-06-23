#include "replicate.h"
#include "ui_replicate.h"

Replicate::Replicate(QWidget *parent, DataBase *db) :
    QWidget(parent),
    ui(new Ui::Replicate)
{
    ui->setupUi(this);

    this->db = db;

    scrollLayout = new QVBoxLayout(ui->sa_tasks->widget());
    currentTest = new Test();
    sizeArea = 0;

    qsi = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

}

Replicate::~Replicate()
{
    delete ui;
}

void Replicate::showThisWindow()
{

}

void Replicate::clickToButton(QString id, QCheckBox *ch)
{
    int n = id.toInt();
    bool set = ch->isChecked();
    for(int i = 0; i < listRCCT.length(); i++){
        if(listRCCT[i]->getIdType() == n) listRCCT[i]->checking(set);
    }
}

void Replicate::preview(int id)
{

    QSqlQuery query(copyContent);
    query.exec("SELECT * FROM Content WHERE Id=" + QString::number(id) + ";");
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
    }
    else{
        QMessageBox::information(this, " ", "Ошибка, возможно задача с Id= "+QString::number(id)+" была только что удалена другим пользователем.");
    }
    ui->widget->update();
}

void Replicate::clickAtTask(int idType)
{
    bool allFalse = true;
    for(int i = 0; i < listRCCT.length(); i++){
        if(listRCCT[i]->getIdType() == idType && listRCCT[i]->isChecked()) {
            listBForT[idType]->chBox->setChecked(true);
            allFalse = false;
            break;
        }
    }
    if(allFalse){
        listBForT[idType]->chBox->setChecked(false);
    }
}

void Replicate::copying()
{
    ui->pb_copy->setEnabled(true);
    ui->pb_copy->setText("Копировать");
}

void Replicate::on_pushButton_4_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "" , tr("База данных Content (*.mdb)"));
    if(fileName == "") return;

    if(copyContent.connectionName() == "CopyContent"){
        copyContent.close();
        copyContent.removeDatabase("CopyContent");
    }
    copyContent = QSqlDatabase::addDatabase("QODBC", "CopyContent");
    copyContent.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MSAccess};DBQ="+fileName);
    copyContent.setPassword("Xq3FgVN8Hfdcz2Fe");
    if (!copyContent.open()) {
        QMessageBox::information(this, " ", "Не удалось открыть базу данных.");
        return;
    }
    QSqlQuery query(copyContent);
    if(!query.exec("SELECT Id, condition, PointName, PointX, PointY, PointZ, LineName, LineList, LinePFL1, LinePFL2, TaskType, FlatList, Answer, BlockActions FROM Content;")){
        QMessageBox::information(this, " ", "Не удалось открыть базу данных, ошибка структуры файла.");
        return;
    }

    ui->pushButton_4->setEnabled(false);

    query.exec("SELECT TestName FROM Test;");
    ui->cb_tests_listTest->clear();
    listTest.clear();
    while(query.next()){
       listTest.append(query.value(0).toString());
    }
    ui->cb_tests_listTest->addItems(listTest);

    loadTasks();
    ui->radioButton->setChecked(false);

    if(listTest.length() > 0) loadTest(listTest[0]);
    ui->radioButton_tests->setChecked(false);

    ui->pushButton_4->setEnabled(true);

}

void Replicate::on_radioButton_clicked()
{
    for(int i = 0; i < listRCCT.length(); i++){
        if(listRCCT[i]->isExist()) {
            listRCCT[i]->setVisible(!listRCCT[i]->isVisible());
            if(listRCCT[i]->isChecked()) listRCCT[i]->clickToCheckBox();
        }
    }
}


void Replicate::on_pb_copy_clicked()
{
    ui->pb_copy->setEnabled(false);
    ui->pb_copy->setText("Копирование...");

    QTimer t;
    t.singleShot(2000, this, SLOT(copying()));

    QSqlQuery mainQuery(db->getDb("Content"));
    mainQuery.exec("SELECT Id FROM Content;");
    int maxId = 0;
    while(mainQuery.next()){
        if(mainQuery.value(0).toInt() > maxId) maxId = mainQuery.value(0).toInt();
    }
    QSqlQuery query(copyContent);
    for(int i = 0; i < listRCCT.length(); i++){
        if(listRCCT[i]->isChecked() && listRCCT[i]->isVisible()){
            listRCCT[i]->setExisting(true);
            if(ui->radioButton->isChecked()) listRCCT[i]->setVisible(!listRCCT[i]->isVisible()); // если стоит скрытие существующих, то скрываем их на ходу

            maxId++;
            query.exec("SELECT * FROM Content WHERE Id="+ QString::number(listRCCT[i]->getId()) +";");
            QSqlRecord rec = query.record();
            query.first();
            mainQuery.prepare("INSERT INTO Content VALUES (:id, :string1, :string2, :string3, :string4, :string5, :string6, :string7, :string8, :string9, :string10, :string11, :string12, :string13);");
            mainQuery.bindValue(":id", maxId);
            mainQuery.bindValue(":string1", "Копия " + query.value(rec.indexOf("condition")).toString());
            mainQuery.bindValue(":string2", query.value(rec.indexOf("PointName")).toString());
            mainQuery.bindValue(":string3", query.value(rec.indexOf("PointX")).toString());
            mainQuery.bindValue(":string4", query.value(rec.indexOf("PointY")).toString());
            mainQuery.bindValue(":string5", query.value(rec.indexOf("PointZ")).toString());
            mainQuery.bindValue(":string6", query.value(rec.indexOf("LineName")).toString());
            mainQuery.bindValue(":string7", query.value(rec.indexOf("LineList")).toString());
            mainQuery.bindValue(":string8", query.value(rec.indexOf("LinePFL1")).toString());
            mainQuery.bindValue(":string9", query.value(rec.indexOf("LinePFL2")).toString());
            mainQuery.bindValue(":string10", query.value(rec.indexOf("TaskType")).toString());
            mainQuery.bindValue(":string11", query.value(rec.indexOf("FlatList")).toString());
            mainQuery.bindValue(":string12", query.value(rec.indexOf("Answer")).toString());
            mainQuery.bindValue(":string13", query.value(rec.indexOf("blockActions")).toString());
            mainQuery.exec();
        }
    }
/*
    for(int i = 0; i < listRCCT.length(); i++){
        listRCCT[i]->deleteLater();
    }
    for(int i = 0; i < listBForT.length(); i++){
        listBForT[i]->chBox->deleteLater();
        listBForT[i]->deleteLater();
    }
    for(int i = 0; i < listWidgets.length(); i++){
        listWidgets[i]->deleteLater();
    }
    listWidgets.clear();
    listBForT.clear();
    listRCCT.clear();

    ui->pb_copy->setEnabled(true);

    */
}

void Replicate::loadTasks()
{
    QSqlQuery query(copyContent);
    listTaksType.clear();
    query.exec("SELECT DISTINCT TaskType FROM Content;");
    QSqlRecord rec = query.record();

    while(query.next()){
        listTaksType.append(query.value(0).toString());
    }

    for(int i = 0; i < listRCCT.length(); i++){
        listRCCT[i]->deleteLater();
    }
    for(int i = 0; i < listBForT.length(); i++){
        listBForT[i]->chBox->deleteLater();
        listBForT[i]->deleteLater();
    }
    for(int i = 0; i < listWidgets.length(); i++){
        listWidgets[i]->deleteLater();
    }
    listWidgets.clear();
    listBForT.clear();
    listRCCT.clear();

    sizeArea = ui->sa_tasks->widget()->size().width();
    QSqlQuery mainQuery(db->getDb("Content"));
    for(int i = 0; i < listTaksType.length(); i++){

        QHBoxLayout *hb = new QHBoxLayout();
        QWidget *wid = new QWidget();
        ButtonForTest *pb = new ButtonForTest();
        pb->chBox = new QCheckBox(listTaksType[i]);
        pb->chBox->setMaximumWidth(sizeArea - 50);
        pb->id = QString::number(i);
        QSpacerItem *s = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Maximum);
        connect(pb->chBox, SIGNAL(clicked()), pb, SLOT(openTask()));
        connect(pb, SIGNAL(currentChBox(QString,QCheckBox*)), this, SLOT(clickToButton(QString,QCheckBox*))); // id = index of listTaskType


        pb->chBox->setStyleSheet("border: none; font: 10pt 'MS Shell Dlg 2'; outline: none;");
        hb->setMargin(0);
        hb->addWidget(pb->chBox);
        hb->addSpacerItem(s);
        wid->setLayout(hb);
        scrollLayout->addWidget(wid);
        listBForT.append(pb);
        listWidgets.append(wid);


        query.exec("SELECT * FROM Content WHERE TaskType='"+ listTaksType[i] +"';");
        rec = query.record();
        while(query.next()){
            ReplicateTaskToCopy *rttc = new ReplicateTaskToCopy();
            connect(rttc, SIGNAL(preview(int)), this, SLOT(preview(int)));
            connect(rttc, SIGNAL(clicked(int)), this, SLOT(clickAtTask(int)));

            rttc->set(query.value(rec.indexOf("Id")).toInt(),
                      i,
                      query.value(rec.indexOf("condition")).toString(),
                      isExistTask(&mainQuery, &query, &rec)!="0"?true:false,
                      sizeArea);

            listRCCT.append(rttc);
            scrollLayout->addWidget(rttc);

        }
    }
    qsi->changeSize(0, 0, QSizePolicy::Maximum, QSizePolicy::Maximum);
    qsi = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollLayout->addSpacerItem(qsi);
}



void Replicate::loadTest(QString testName)
{
    //очищаем
    currentTest->deleteTest();
    currentTest = new Test();
    for(int i = ui->tabWidget_list_variant->count(); i >= 0; i--){
        ui->tabWidget_list_variant->removeTab(i);
    }
    ui->widget->clear();
    ui->field_task->clear();
    //

    TempTest test1;
    test = test1;
    test.sName = testName;
    QSqlQuery mainQuery(db->getDb("Content"));
    QSqlQuery query(copyContent);
    query.exec("SELECT * FROM Test WHERE TestName='" + testName + "';");
    QSqlRecord rec = query.record();

    QStringList list;
    if(query.first()){
        list = query.value(rec.indexOf("ListTasks")).toString().split(";", QString::SkipEmptyParts);
    }
    else return;

    //разбиваем задачи по вариантам
    for(int i = 0; i < list.length(); i++){
        QStringList tasksInVariant = list[i].split(" ", QString::SkipEmptyParts);
        TempVariant tVariant;
        TempTypeTask tTypeTask;
        QString sType = ""; //для хранения типа задач, + если задачи нет, то она будет относиться к предыдущему типу
        for(int j = 0; j < tasksInVariant.length(); j++){
            TempTask tTask;

            query.exec("SELECT * FROM Content WHERE Id=" + tasksInVariant[j] + ";");
            rec = query.record();
            if(query.first()){
                tTask.id = tasksInVariant[j];
                tTask.text = query.value(rec.indexOf("condition")).toString();
                sType = query.value(rec.indexOf("TaskType")).toString();
                tTask.idInMain = isExistTask(&mainQuery, &query, &rec);
                tTask.isExist = tTask.idInMain!="0"?true:false;
            }
            else{
                tTask.id = "0";
                tTask.text = "Ошибка. Задача не найдена. Возможно она была удалена.";
                tTask.isExist = false;
            }
            if(tTypeTask.lTask.isEmpty() || tTypeTask.sName == sType){
                tTypeTask.sName = sType;
                tTypeTask.lTask.append(tTask);
            }
            else{
                tVariant.lTypeTask.append(tTypeTask);
                tTypeTask.lTask.clear();

                tTypeTask.sName = sType;
                tTypeTask.lTask.append(tTask);
            }
        }

        tVariant.lTypeTask.append(tTypeTask);
        tTypeTask.lTask.clear();

        test.lVariants.append(tVariant);
        tVariant.lTypeTask.clear();
    }
//------------------
    for(int i = 0; i < test.lVariants.length(); i++){  // по количеству вариантов
        TestVariant *var = new TestVariant;

        QWidget *widget = new QWidget;
        widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

        QVBoxLayout *layout = new QVBoxLayout;
        widget->setLayout(layout);
        var->v = layout;

        QScrollArea *scroll = new QScrollArea;
        scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        scroll->setWidgetResizable(true);
        scroll->setWidget(widget);

        QPalette palette;
        palette.setColor(QPalette::Background, Qt::white);
        scroll->setPalette(palette);
        scroll->setAttribute(Qt::WA_DeleteOnClose);


        ui->tabWidget_list_variant->addTab(scroll, "Вариант "+help.itoqs(i+1));

        QList<TempTypeTask> lTypeTask = test.lVariants[i].lTypeTask;
        for(int j = 0; j < lTypeTask.length(); j++){ // по количеству различных типов задач
            TestVariantType *type = new TestVariantType;

            QVBoxLayout *v = new QVBoxLayout;
            QLabel *lType = new QLabel(lTypeTask[j].sName); // добавляем название типа
            v->addWidget(lType);

            QList<TempTask> lTask = lTypeTask[j].lTask;
            for(int k = 0; k <lTask.length(); k++){ // по количеству задач определенного типа
                TestVariantTypeTask *task = new TestVariantTypeTask;

                QHBoxLayout *h = new QHBoxLayout;
                QSpacerItem *s = new QSpacerItem(50, 20, QSizePolicy::Maximum, QSizePolicy::Maximum);
                h->addSpacerItem(s); // отступ от края

                QComboBox *c = new QComboBox;
                c->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
                if(sizeArea > 170){
                    c->setMaximumWidth(sizeArea - 170);
                    c->setMinimumWidth(sizeArea - 170);
                }
                h->addWidget(c);
                QPushButton *b = new QPushButton("Предпросмотр");
                QObject::connect(b, SIGNAL(clicked()), task, SLOT(open()));
                QObject::connect(task, SIGNAL(openTask(int)), this, SLOT(preview(int)));
                b->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
                h->addWidget(b);

                task->set(h, s, c, b);
                task->addElem(lTask[k].text, lTask[k].id.toInt());

                v->addLayout(h);
                type->tasks.append(task);
            }
            type->v = v;
            type->l = lType;

            layout->addLayout(v);
            var->types.append(type);
        }
        QSpacerItem *sp = new QSpacerItem(20, 20, QSizePolicy::Maximum, QSizePolicy::Expanding);
        layout->addSpacerItem(sp);
        currentTest->variants.append(var);
    }
}

QString Replicate::isExistTask(QSqlQuery *mainQuery, QSqlQuery *query, QSqlRecord *rec)
{
    mainQuery->exec("SELECT Id FROM Content WHERE condition='"+ query->value(rec->indexOf("condition")).toString() +
                                          "' AND PointName='"+ query->value(rec->indexOf("PointName")).toString() +
                                          "' AND PointX='"+ query->value(rec->indexOf("PointX")).toString() +
                                          "' AND PointY='"+ query->value(rec->indexOf("PointY")).toString() +
                                          "' AND PointZ='"+ query->value(rec->indexOf("PointZ")).toString() +
                                          "' AND LineName='"+ query->value(rec->indexOf("LineName")).toString() +
                                          "' AND LineList='"+ query->value(rec->indexOf("LineList")).toString() +
                                          "' AND LinePFL1='"+ query->value(rec->indexOf("LinePFL1")).toString() +
                                          "' AND LinePFL2='"+ query->value(rec->indexOf("LinePFL2")).toString() +
                                          "' AND FlatList='"+ query->value(rec->indexOf("FlatList")).toString() +
                                          "' AND Answer='"+ query->value(rec->indexOf("Answer")).toString() +
                                          "';");
    if(mainQuery->first()) return mainQuery->value(0).toString();
    else return "0";
}

void Replicate::on_cb_tests_listTest_currentIndexChanged(const QString &arg1)
{
    loadTest(arg1);
}

void Replicate::on_pb_tests_copy_clicked()
{
    if(ui->cb_tests_listTest->currentText() == "") return;
    QString name = ui->cb_tests_listTest->currentText();
    QString finishName = ui->cb_tests_listTest->currentText();
    QSqlQuery mainQuery(db->getDb("Content"));
    mainQuery.exec("SELECT * FROM Test WHERE TestName='"+ name +"';");
    if(mainQuery.first()){
        if(QMessageBox::question(this, "Добавление копии",
                                       "Тест с таким названием уже существует, добавить копию?",
                                       QMessageBox::Ok | QMessageBox::Cancel,
                                       QMessageBox::Cancel) == QMessageBox::Ok)
        {
            for(int i = 1; i < 100; i++){
                mainQuery.exec("SELECT * FROM Test WHERE TestName='"+ name + " (" + QString::number(i) + ")" +"';");
                if(!mainQuery.first()){
                    finishName = name + " (" + QString::number(i) + ")";
                    break;
                }
            }
        }
        else{
            return;
        }
    }

    QSqlQuery query(copyContent);
    query.exec("SELECT * FROM Test WHERE TestName='"+ name +"';");
    if(!query.first()){
        QMessageBox::information(this, "Ошибка", "ошибка при копировани теста");
        return;
    }
    QString list;
    QString count = query.value(2).toString();

    mainQuery.exec("SELECT Id FROM Content;");
    int maxId = 0;
    while(mainQuery.next()){
        if(mainQuery.value(0).toInt() > maxId) maxId = mainQuery.value(0).toInt();
    }

    for(int i = 0; i < test.lVariants.length(); i++){
        QList<TempTypeTask> lTypeTask = test.lVariants[i].lTypeTask;
        for(int j = 0; j < lTypeTask.length(); j++){ // по количеству различных типов задач
            QList<TempTask> lTask = lTypeTask[j].lTask;
            for(int k = 0; k <lTask.length(); k++){ // по количеству задач определенного типа
                if(lTask[k].isExist){
                    list += lTask[k].idInMain + " ";
                }
                else{
                    maxId++;
                    query.exec("SELECT * FROM Content WHERE Id="+ lTask[k].id +";");
                    QSqlRecord rec = query.record();
                    query.first();
                    mainQuery.prepare("INSERT INTO Content VALUES (:id, :string1, :string2, :string3, :string4, :string5, :string6, :string7, :string8, :string9, :string10, :string11, :string12, :string13);");
                    mainQuery.bindValue(":id", maxId);
                    mainQuery.bindValue(":string1", query.value(rec.indexOf("condition")).toString());
                    mainQuery.bindValue(":string2", query.value(rec.indexOf("PointName")).toString());
                    mainQuery.bindValue(":string3", query.value(rec.indexOf("PointX")).toString());
                    mainQuery.bindValue(":string4", query.value(rec.indexOf("PointY")).toString());
                    mainQuery.bindValue(":string5", query.value(rec.indexOf("PointZ")).toString());
                    mainQuery.bindValue(":string6", query.value(rec.indexOf("LineName")).toString());
                    mainQuery.bindValue(":string7", query.value(rec.indexOf("LineList")).toString());
                    mainQuery.bindValue(":string8", query.value(rec.indexOf("LinePFL1")).toString());
                    mainQuery.bindValue(":string9", query.value(rec.indexOf("LinePFL2")).toString());
                    mainQuery.bindValue(":string10", query.value(rec.indexOf("TaskType")).toString());
                    mainQuery.bindValue(":string11", query.value(rec.indexOf("FlatList")).toString());
                    mainQuery.bindValue(":string12", query.value(rec.indexOf("Answer")).toString());
                    mainQuery.bindValue(":string13", query.value(rec.indexOf("blockActions")).toString());
                    mainQuery.exec();

                    list += QString::number(maxId) + " ";
                }
            }
        }
        list += ";";
    }

    mainQuery.prepare("INSERT INTO Test VALUES (:name, :list, :count);");
    mainQuery.bindValue(":name", finishName);
    mainQuery.bindValue(":list", list);
    mainQuery.bindValue(":count", count);
    mainQuery.exec();

    on_radioButton_tests_clicked();
}

void Replicate::on_radioButton_tests_clicked()
{
    if(ui->radioButton_tests->isChecked()){
        QSqlQuery mainQuery(db->getDb("Content"));
        mainQuery.exec("SELECT * FROM Test;");
        while(mainQuery.next()){
            QSqlQuery query(copyContent);
            query.exec("SELECT * FROM Test WHERE TestName='"+ mainQuery.value(0).toString() +
                       "' AND ListTasks='" + mainQuery.value(1).toString() +
                       "' AND CountVariants='" + mainQuery.value(2).toString() + "';");
            if(query.first()){
                ui->cb_tests_listTest->removeItem(ui->cb_tests_listTest->findText(query.value(0).toString()));
            }
        }
    }
    else{
        ui->cb_tests_listTest->clear();
        ui->cb_tests_listTest->addItems(listTest);
    }

}
