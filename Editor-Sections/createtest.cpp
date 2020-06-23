#include "createtest.h"
#include "ui_createtest.h"

CreateTest::CreateTest(QWidget *parent, DataBase *db) :
    QWidget(parent),
    ui(new Ui::CreateTest)
{
    ui->setupUi(this);

    this->db = db;
    setTaskType();
    test = new Test();
    testRed = new Test();
    loadTests();
}

CreateTest::~CreateTest()
{
    delete ui;
}

void CreateTest::showThisWindow()
{
    setTaskType();
    loadTests();
    ui->widget->repaintAll();
}

void CreateTest::openTask(int n)
{
    QSqlQuery query(db->getDb("Content"));
    query.exec("SELECT * FROM Content WHERE Id=" + QString::number(n) + ";");
    QSqlRecord rec = query.record();
    if(query.first()){
       ui->field_task->clear();
       ui->field_task->appendPlainText(query.value(rec.indexOf("condition")).toString());
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
}

struct TasksInType{
    QList<int> listNumber; //индекс задачи в списке
    QString nameType;
};

void CreateTest::setRandomTask(Test *test)
{
    //составляем списки доступных задач по каждому из создаваемых типов
    QList<TasksInType> listType;
    for(int i = 0; i < test->variants[0]->types.length(); i++){
        //есть ли такой тип уже в списке? (т.к. они могут повторятся)
        QString nameType = test->variants[0]->types[i]->l->text();
        bool isExist = false;
        for(int j = 0; j < listType.length(); j++){
            if(listType[j].nameType == nameType){
                isExist = true;
                break;
            }
        }
        if(!isExist){
            TasksInType typeTask;
            for(int j = 0; j < test->variants[0]->types[i]->tasks[0]->list.length(); j++){
                typeTask.listNumber.append(j);
            }
            typeTask.nameType = test->variants[0]->types[i]->l->text();
            listType.append(typeTask);
        }
    }
    //
    //запоняем рандомно задачи в каждом типе
    QList<TasksInType> randomListType;
    for(int i = 0; i < listType.length(); i++){
        TasksInType typeTask;
        while(listType[i].listNumber.length() > 1){
            int k = qrand() % listType[i].listNumber.length();
            typeTask.listNumber.append(listType[i].listNumber[k]);
            listType[i].listNumber.removeAt(k);
        }
        typeTask.listNumber.append(listType[i].listNumber[0]);//забрасываем в список последний элемент
        typeTask.nameType = listType[i].nameType;
        randomListType.append(typeTask);
    }
    QList<TasksInType> copyRandomListType = randomListType; //копия, на случай если не хватит зада в списке
    //
    //устанавливаем номера задач в боксы
    bool ok = true; //хватает ли задач, что бы в тесте они не посторялись
    QString s = ""; //информационное сообщение
    for(int i = 0; i < test->variants.length(); i++){ //по каждому варианту
        TestVariant *var = test->variants[i];
        for(int j = 0; j < var->types.length(); j++){ //по каждому типу задач в варианте
            //ищем соответствующий тип
            int numT = 0;//индекс типа в списке рандома
            for(int k = 0; k < randomListType.length(); k++){
                if(var->types[j]->l->text() == randomListType[k].nameType){
                    numT = k;
                    break;
                }
            }
            //
            ok = true;
            TestVariantType *type = var->types[j];
            for(int k = 0; k < type->tasks.length(); k++){
                if(randomListType[numT].listNumber.length() == 0) {
                    ok = false;
                    s = "Количество задач типа '"+type->l->text()+"' превышает количество задач данного типа в Базе Данных. Задачи будут повторяться.";
                    randomListType = copyRandomListType;
                }
                type->tasks[k]->box->setCurrentIndex(randomListType[numT].listNumber.first());
                randomListType[numT].listNumber.removeFirst();
            }
            if(!ok) QMessageBox::information(this, " ", s);
        }
    }

    /*
    bool ok = true; //хватает ли задач, что бы в тесте они не посторялись
    QString s = ""; //информационное сообщение
    for(int i = 0; i < test->variants.length(); i++){ //по каждому варианту

        TestVariant *var = test->variants[i];
        for(int j = 0; j < var->types.length(); j++){ //по каждому типу задач в варианте
            ok = true;
            TestVariantType *type = var->types[j];
            if(type->tasks.length() > type->tasks[0]->box->count()){
                for(int k = 0; k < type->tasks.length(); k++){
                    type->tasks[k]->box->setCurrentIndex( );
                }
                ok = false;
                s = "Количество задач типа '"+type->l->text()+"' превышает количество задач данного типа в Базе Данных. Задачи будут повторяться.";
            }
            else {
                QList<int> list;
                int k = 0, max = 10000;
                while(k != type->tasks.length()){ // заполняем список случайными, неповторяющимися значениями
                    int ran = qrand()%(type->tasks[0]->box->count());
                    bool no = true;
                    for(int l = 0; l < list.length(); l++){
                        if(ran == list[l]) no = false;
                    }
                    if(max != 0) max--;
                    else no = true;
                    if(no) {
                        list.append(ran);
                        k++;
                    }
                }
                for(int l = 0; l < type->tasks.length(); l++){
                    type->tasks[l]->box->setCurrentIndex(list[l]);
                }
            }
            if(!ok && i == 0) QMessageBox::information(this, " ", s);
        }
    }*/
}

void CreateTest::setTaskType()
{
    QSqlQuery query(db->getDb("Content"));
    query.exec("SELECT DISTINCT TaskType FROM Content;");

    ui->comboBox_themes->clear();
    ui->comboBox_themes_red->clear();
    taskType.clear();

    while(query.next()){
        taskType.append(query.value(0).toString());
    }
    ui->comboBox_themes->addItems(taskType);
    ui->comboBox_themes_red->addItems(taskType);
}

void CreateTest::fillBoxTask(TestVariantTypeTask *task, QString s)
{
    QSqlQuery query(db->getDb("Content"));
    query.exec("SELECT * FROM Content WHERE TaskType='" + s + "';");
    QSqlRecord rec = query.record();
    task->box->clear();
    task->list.clear();
    QString tem;
    while(query.next()){
        tem = query.value(rec.indexOf("condition")).toString();
        if(tem.length() > 47) tem.remove(47, tem.length() - 47);
        task->addElem(tem, query.value(rec.indexOf("id")).toInt());
    }
}

void CreateTest::loadTests()
{
    QSqlQuery query(db->getDb("Content"));
    query.exec("SELECT TestName FROM Test;");
    listTest.clear();
    while(query.next()){
       listTest.append(query.value(0).toString());
    }
    ui->comboBox_name_tests->clear();
    ui->comboBox_name_tests->addItems(listTest);
    ui->comboBox_name_tests_red->clear();
    ui->comboBox_name_tests_red->addItems(listTest);
}

void CreateTest::deleteType(QHBoxLayout *box)
{
    for(int i = 0; i < listTaskType.length(); i++){
        if(listTaskType[i]->getLayout() == box) {
            listTaskType.removeAt(i);
            break;
        }
    }
    ui->verticalLayout_selected_theme->removeItem(box);
    box->~QHBoxLayout();
}

void CreateTest::deleteTypeRed(QHBoxLayout *box)
{
    for(int i = 0; i < listTaskTypeRed.length(); i++){
        if(listTaskTypeRed[i]->getLayout() == box) {
            listTaskTypeRed.removeAt(i);
            break;
        }
    }
    ui->verticalLayout_selected_theme_red->removeItem(box);
    box->~QHBoxLayout();
}

void CreateTest::on_pushButton_add_theme_clicked()
{
    LayoutTaskType *type = new LayoutTaskType;

    QHBoxLayout *layout = new QHBoxLayout;
    QLabel *label = new QLabel(ui->comboBox_themes->currentText());
    QSpinBox *spin = new QSpinBox();
    QSpacerItem *spacer = new QSpacerItem(20, 30, QSizePolicy::Expanding, QSizePolicy::Maximum);
    QPushButton *button = new QPushButton("X");

    type->set(layout, label, spin, spacer, button);

    label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    layout->addWidget(label);

    spin->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    spin->setMinimumWidth(50);
    spin->setMinimum(1);
    spin->setToolTip("Укажите количество задач данного типа");
    layout->addWidget(spin);

    layout->addSpacerItem(spacer);

    QObject::connect(button, SIGNAL(clicked()), type, SLOT(deteleType()));
    QObject::connect(type, SIGNAL(deleteType(QHBoxLayout*)), this, SLOT(deleteType(QHBoxLayout*)));
    layout->addWidget(button);

    listTaskType.append(type);
    ui->verticalLayout_selected_theme->addLayout(layout);
}

void CreateTest::on_pushButton_create_variants_clicked()
{
    bool ok = true;
    int colVariant = ui->spinBox_variants->value();

    if(test->setVariant && colVariant != 0){
        if(QMessageBox::Yes == QMessageBox::question(this, " ", "Обновить варианты?")){
            test->deleteTest();
            test = new Test();
            for(int i = ui->tabWidget_variants->count(); i >= 0; i--){
                ui->tabWidget_variants->removeTab(i);
            }
            ui->widget->clear();
            ui->field_task->clear();
        }
        else ok = false;
    }

    if(colVariant == 0) QMessageBox::information(this, " ", "Укажите количество вариантов");
    else if(listTaskType.length() == 0){
        QMessageBox::information(this, " ", "Выберите один или несколько типов задач");
    }
    else if(ok){
        for(int i = 0; i < colVariant; i++){  // по количеству вариантов
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

            ui->tabWidget_variants->addTab(scroll, "Вариант "+help.itoqs(i+1));

            for(int j = 0; j < listTaskType.length(); j++){ // по количеству различных типов задач
                TestVariantType *type = new TestVariantType;

                QVBoxLayout *v = new QVBoxLayout;
                QLabel *lType = new QLabel(listTaskType[j]->getLabel()->text()); // добавляем название типа
                v->addWidget(lType);

                for(int k = 0; k < listTaskType[j]->getSpin()->value(); k++){ // по количеству задач определенного типа
                    TestVariantTypeTask *task = new TestVariantTypeTask;

                    QHBoxLayout *h = new QHBoxLayout;
                    QSpacerItem *s = new QSpacerItem(50, 20, QSizePolicy::Maximum, QSizePolicy::Maximum);
                    h->addSpacerItem(s); // отступ от края

                    QComboBox *c = new QComboBox;
                    c->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
                    h->addWidget(c);
                    QPushButton *b = new QPushButton("Предпросмотр");
                    QObject::connect(b, SIGNAL(clicked()), task, SLOT(open()));
                    QObject::connect(task, SIGNAL(openTask(int)), this, SLOT(openTask(int)));
                    b->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
                    h->addWidget(b);

                    task->set(h, s, c, b);
                    fillBoxTask(task, listTaskType[j]->getLabel()->text());

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
            test->variants.append(var);
        }
        if(colVariant != 0) test->setVariant = true;

        setRandomTask(test);
    }
}

void CreateTest::on_pushButton_save_test_clicked()
{
    QString name = ui->lineEdit->text();
    QRegExp rx("[^\\|/:*?\"<>]+");
    if(!rx.exactMatch(name)){
        QMessageBox::information(this, " ", "Название теста не может содержать следующих знаков \\ / | : * ? \" < >");
        return;
    }
    if(name != ""){
        QSqlQuery query(db->getDb("Content"));
        query.exec("SELECT * FROM Test WHERE TestName='" + name + "';");
        bool isExist = false; //теста с таким именем нет
        if(query.first()){
            isExist = true;
            QMessageBox::information(this, " ", "Тест с таким названием уже существует.");
            ui->lineEdit->setFocus();
        }
        if(!isExist){
            bool stop = false;
            if(test->variants.length() > 0){
                if(test->variants.length() != ui->spinBox_variants->value()){
                    if(QMessageBox::Yes != QMessageBox::question(this, "Внимание", "Количество вариантов в тесте не совподает с указанным. Возможно, вы забыли пересоздать список вариантов. Продолжить сохранение?")){
                        return;
                    }
                }
                //составляем список вариантов
                QString list = "";
                QList<QString> repeatTask;
                for(int i = 0; i < test->variants.length(); i++){
                    TestVariant *var = test->variants[i];
                    repeatTask.clear();
                    for(int j = 0; j < var->types.length(); j++){
                        TestVariantType *type = var->types[j];
                        for(int k = 0; k < type->tasks.length(); k++){
                            TestVariantTypeTask *task = type->tasks[k];
                            QString s = help.itoqs(task->list[task->box->currentIndex()]);
                            //проверяем повторяется ли задача в варианте
                            for(int l = 0; l < repeatTask.length(); l++){
                                if(repeatTask[l] == s){
                                    if(QMessageBox::Yes != QMessageBox::question(this, " ", "В варианте "+help.itoqs(i+1)+" есть повторяющиеся задачи. Продолжить?")){
                                        stop = true;
                                        break;
                                    }
                                }
                            }
                            if(stop) break;
                            repeatTask.append(s);
                            list += s;
                            list += " ";
                        }
                        if(stop) break;
                    }
                    if(stop) break;

                    list.remove(list.length()-1, 1);
                    list += ";";
                }

                if(!stop){
                    //записываем в базу данных
                    query.prepare("INSERT INTO Test VALUES (:name, :list, :count);");
                    query.bindValue(":name", name);
                    query.bindValue(":list", list);
                    query.bindValue(":count", test->variants.length());

                    query.exec();

                    QMessageBox::information(this, " ", "Тест сохранен.");

                    test->deleteTest();
                    test = new Test();
                    for(int i = ui->tabWidget_variants->count(); i >= 0; i--){
                        ui->tabWidget_variants->removeTab(i);
                    }
                    ui->lineEdit->setText("");
                    ui->spinBox_variants->setValue(0);

                    for(int i = listTaskType.length()-1; i >= 0; i--){
                        listTaskType[i]->getButton()->clicked(true);
                    }
                    ui->comboBox_themes->setCurrentIndex(0);
                    loadTests();

                    ui->widget->clear();
                    ui->field_task->clear();
                }
            }
            else {
                QMessageBox::information(this, " ", "Создайте список вариантов.");
            }
        }
    }
    else{
        QMessageBox::information(this, " ", "Введите название теста.");
    }
}

void CreateTest::on_pushButton_delete_test_clicked()
{
    if(ui->comboBox_name_tests->currentText() != ""){
        if(QMessageBox::Yes == QMessageBox::question(this, "Подтверждение", "Вы действительно хотите удалить тест '"+ui->comboBox_name_tests->currentText()+"'?")){
            QSqlQuery query(db->getDb("Content"));
            query.exec("DELETE FROM Test WHERE TestName='" + ui->comboBox_name_tests->currentText() + "';");
            loadTests();
        }
    }
}

void CreateTest::on_pushButton_redact_test_clicked()
{
    if(oldTestName != ""){
        on_pushButton_cancel_red_clicked();
        if(oldTestName != "") return;
    }

    ui->scrollArea_red->setEnabled(true);
    ui->scrollArea_red->setToolTip("");

    oldTestName = ui->comboBox_name_tests_red->currentText();
    ui->lineEdit_red->setText(oldTestName);

    QString list;
    QSqlQuery query(db->getDb("Content"));
    query.exec("SELECT * FROM Test WHERE TestName='" + ui->lineEdit_red->text() + "';");
    QSqlRecord rec = query.record();
    if(query.first()){
        ui->spinBox_variants_red->setValue(query.value(rec.indexOf("CountVariants")).toInt());
        list = query.value(rec.indexOf("ListTasks")).toString();
    }

    //разбиваем задачи по вариантам
    QList<QString> listVariants;
    QList<int> listIter;
    QString temp = "";
    for(int i = 0; i < list.length(); i++){
        temp += help.one_of_line(list,i);
        if(help.one_of_line(list,i) == ";"){
            listVariants.append(temp);
            listIter.append(0); //номер текущего символа  //тут ошибка что ли? почему 0?...
            temp = "";
        }
    }

    QList<QString> typeTasks; //типы задач
    QList<int> typeCount; //количество задач типа
    //параллельно проверяя по одной задачи из вариантов, устанавливаем их типы, и узнаем типы удаленных задач, если такие имеются
    while(help.one_of_line(listVariants[0], listIter[0]) != ";"){ //цикл по количеству задач в любом из вариантов
        QList<QString> typeTasksTemp;
        for(int i = 0; i < listVariants.length(); i++){ //цикл по количеству вариантов
            temp = "";
            for(int j = listIter[i]; j < listVariants[i].length(); j++){
                if(help.one_of_line(listVariants[i], j) == " " || help.one_of_line(listVariants[i], j) == ";"){ //ищем задачу и записываем ее тип и id
                    bool find = false;
                    QSqlQuery content(db->getDb("Content"));
                    content.exec("SELECT * FROM Content WHERE Id=" + temp + ";");
                    QSqlRecord rContent = content.record();
                    if(content.first()){
                        typeTasksTemp.append(content.value(rContent.indexOf("TaskType")).toString());
                        find = true;
                    }
                    if(help.one_of_line(listVariants[i], j) == ";"){
                        listIter[i] = j;
                    }
                    else listIter[i] = j+1;
                    if(!find) {
                        typeTasksTemp.append("-");
                    }
                    break;
                }
                else temp += help.one_of_line(listVariants[i], j);
            }
        }

        bool typeIsfind = true;
        for(int j = 0; j < typeTasksTemp.length(); j++){
            if(typeTasksTemp[j] != "-"){
                typeIsfind = false;
                if(typeTasks.length() == 0){
                    typeTasks.append(typeTasksTemp[j]);
                    typeCount.append(1);

                }
                else if(typeTasks[typeTasks.length()-1] == typeTasksTemp[j]){
                    typeCount[typeCount.length()-1] = typeCount[typeCount.length()-1] + 1;

                }
                else {
                    typeTasks.append(typeTasksTemp[j]);
                    typeCount.append(1);

                }
                break;
            }
        }
        if(typeIsfind){
            typeCount[typeCount.length()-1] = typeCount[typeCount.length()-1] + 1;
            QMessageBox::information(this, " ", "Так как задачи одного из типов были удалены, их тип заменен на '"+typeTasks[typeTasks.length()-1]+"'. А сами задачи необходимо задать вручную.");
        }
    }

    //получили типы задач и сколько задач какого типа должно быть
    //добавляем типы в layout и указываем количество задач
    for(int i = 0; i < typeTasks.length(); i++){
        ui->comboBox_themes_red->setCurrentText(typeTasks[i]);
        on_pushButton_add_theme_red_clicked();
        listTaskTypeRed[listTaskTypeRed.length()-1]->getSpin()->setValue(typeCount[i]);
    }

    //создаем варианты задач
    testRed->setVariant = false;
    on_pushButton_create_variants_red_clicked();

    //устанавливаем соответствующие задачи
    temp = "";
    int m = 0;
    for(int i = 0; i < testRed->variants.length(); i++){
        TestVariant *var = testRed->variants[i];
        for(int j = 0; j < var->types.length(); j++){
            TestVariantType *type = var->types[j];
            for(int k = 0; k < type->tasks.length(); k++){
                temp = "";
                TestVariantTypeTask *task = type->tasks[k];
                for(; m < list.length(); m++){
                    if(help.one_of_line(list, m) == " " || help.one_of_line(list,m) == ";"){
                        if(task->list.indexOf(temp.toInt()) == -1){
                           task->box->addItem("Задача была удалена.");
                           task->box->setCurrentText("Задача была удалена.");
                           QMessageBox::information(this, " ", "Одна из задач Варианта "+help.itoqs(i+1)+" была удалена. \nВыберите из списка другую задачу.");

                        }
                        else task->box->setCurrentIndex(task->list.indexOf(temp.toInt()));
                        m++;
                        break;
                    }
                    else temp += help.one_of_line(list,m);
                }
            }
        }
    }
}

void CreateTest::on_pushButton_add_theme_red_clicked()
{
    LayoutTaskType *type = new LayoutTaskType;

    QHBoxLayout *layout = new QHBoxLayout;
    QLabel *label = new QLabel(ui->comboBox_themes_red->currentText());
    QSpinBox *spin = new QSpinBox();
    QSpacerItem *spacer = new QSpacerItem(20, 30, QSizePolicy::Expanding, QSizePolicy::Maximum);
    QPushButton *button = new QPushButton("X");

    type->set(layout, label, spin, spacer, button);

    label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    layout->addWidget(label);

    spin->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    spin->setMinimumWidth(50);
    spin->setMinimum(1);
    spin->setToolTip("Укажите количество задач данного типа");
    layout->addWidget(spin);

    layout->addSpacerItem(spacer);

    QObject::connect(button, SIGNAL(clicked()), type, SLOT(deteleType()));
    QObject::connect(type, SIGNAL(deleteType(QHBoxLayout*)), this, SLOT(deleteTypeRed(QHBoxLayout*)));
    layout->addWidget(button);

    listTaskTypeRed.append(type);
    ui->verticalLayout_selected_theme_red->addLayout(layout);
}

void CreateTest::on_pushButton_create_variants_red_clicked()
{
    bool ok = true;
    int colVariant = ui->spinBox_variants_red->value();
    if(colVariant == 0) QMessageBox::information(this, " ", "Укажите количество вариантов");

    if(testRed->setVariant && colVariant != 0){
        if(QMessageBox::Yes == QMessageBox::question(this, " ", "Обновить варианты?")){
            testRed->deleteTest();
            testRed = new Test();
            for(int i = ui->tabWidget_variants_red->count(); i >= 0; i--){
                ui->tabWidget_variants_red->removeTab(i);
            }
            ui->widget->clear();
            ui->field_task->clear();
        }
        else ok = false;
    }
    if(ok){
        for(int i = 0; i < colVariant; i++){  // по количеству вариантов
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

            ui->tabWidget_variants_red->addTab(scroll, "Вариант "+help.itoqs(i+1));

            for(int j = 0; j < listTaskTypeRed.length(); j++){ // по количеству различных типов задач
                TestVariantType *type = new TestVariantType;

                QVBoxLayout *v = new QVBoxLayout;
                QLabel *lType = new QLabel(listTaskTypeRed[j]->getLabel()->text()); // добавляем название типа
                v->addWidget(lType);

                for(int k = 0; k < listTaskTypeRed[j]->getSpin()->value(); k++){ // по количеству задач определенного типа
                    TestVariantTypeTask *task = new TestVariantTypeTask;

                    QHBoxLayout *h = new QHBoxLayout;
                    QSpacerItem *s = new QSpacerItem(50, 20, QSizePolicy::Maximum, QSizePolicy::Maximum);
                    h->addSpacerItem(s); // отступ от края

                    QComboBox *c = new QComboBox;
                    c->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
                    h->addWidget(c);
                    QPushButton *b = new QPushButton("Предпросмотр");
                    QObject::connect(b, SIGNAL(clicked()), task, SLOT(open()));
                    QObject::connect(task, SIGNAL(openTask(int)), this, SLOT(openTask(int)));
                    b->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
                    h->addWidget(b);

                    task->set(h, s, c, b);
                    fillBoxTask(task, listTaskTypeRed[j]->getLabel()->text());

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
            testRed->variants.append(var);
        }
        if(colVariant != 0) testRed->setVariant = true;

        setRandomTask(testRed);
    }
}

void CreateTest::on_pushButton_save_test_red_clicked()
{
    QString name = ui->lineEdit_red->text();
    if(name != ""){
        QSqlQuery query(db->getDb("Content"));
        query.exec("SELECT * FROM Test WHERE TestName='" + name + "';");
        bool isExist = false; //теста с таким именем нет
        if(oldTestName != name){
            if(query.first()){
                isExist = true;
                QMessageBox::information(this, " ", "Тест с таким названием уже существует.");
                ui->lineEdit_red->setFocus();
            }
        }
        if(!isExist){
            bool stop = false;
            if(testRed->variants.length() > 0){
                if(testRed->variants.length() != ui->spinBox_variants_red->value()){
                    if(QMessageBox::Yes != QMessageBox::question(this, "Внимание", "Количество вариантов в тесте не совподает с указанным. Возможно, вы забыли пересоздать список вариантов. Продолжить сохранение?")){
                        return;
                    }
                }
                //составляем список вариантов
                QString list = "";
                QList<QString> repeatTask;
                for(int i = 0; i < testRed->variants.length(); i++){
                    TestVariant *var = testRed->variants[i];
                    repeatTask.clear();
                    for(int j = 0; j < var->types.length(); j++){
                        TestVariantType *type = var->types[j];
                        for(int k = 0; k < type->tasks.length(); k++){
                            TestVariantTypeTask *task = type->tasks[k];
                            QString s = help.itoqs(task->list[task->box->currentIndex()]);
                            //проверяем повторяется ли задача в варианте
                            for(int l = 0; l < repeatTask.length(); l++){
                                if(repeatTask[l] == s){
                                    if(QMessageBox::Yes != QMessageBox::question(this, " ", "В варианте "+help.itoqs(i+1)+" есть посторяющиеся задачи. Продолжить?")){
                                        stop = true;
                                        break;
                                    }
                                }
                            }
                            if(stop) break;
                            repeatTask.append(s);
                            list += s;
                            list += " ";
                        }
                        if(stop) break;
                    }
                    if(stop) break;

                    list.remove(list.length()-1, 1);
                    list += ";";
                }

                if(!stop){
                    //изменяем в базе данных
                    QString name = ui->lineEdit_red->text();
                    query.exec("UPDATE Test SET TestName='"+name+"' WHERE TestName='"+oldTestName+"';");
                    query.exec("UPDATE Test SET ListTasks='"+list+"' WHERE TestName='"+name+"';");
                    query.exec("UPDATE Test SET CountVariants='" + help.itoqs(testRed->variants.length()) + "' WHERE TestName='"+name+"';");

                    QMessageBox::information(this, " ", "Тест сохранен.");

                    testRed->deleteTest();
                    testRed = new Test();
                    for(int i = ui->tabWidget_variants_red->count(); i >= 0; i--){
                        ui->tabWidget_variants_red->removeTab(i);
                    }
                    ui->lineEdit_red->setText("");
                    ui->spinBox_variants_red->setValue(0);

                    for(int i = listTaskTypeRed.length()-1; i >= 0; i--){
                        listTaskTypeRed[i]->getButton()->clicked(true);
                    }
                    ui->comboBox_themes_red->setCurrentIndex(0);
                    loadTests();

                    ui->scrollArea_red->setEnabled(false);
                    ui->scrollArea_red->setToolTip("Выберите задачу для редактирования перед тем как продолжить");

                    oldTestName = "";
                    ui->widget->clear();
                    ui->field_task->clear();
                }
            }
            else {
                QMessageBox::information(this, " ", "Создайте список вариантов.");
            }
        }
    }
    else{
        QMessageBox::information(this, " ", "Введите название теста.");
    }
}

void CreateTest::on_pushButton_cancel_red_clicked()
{
    if(QMessageBox::Yes == QMessageBox::question(this, " ", "Отменить текущее редактирование?")){
        testRed->deleteTest();
        testRed = new Test();
        for(int i = ui->tabWidget_variants_red->count(); i >= 0; i--){
            ui->tabWidget_variants_red->removeTab(i);
        }
        ui->lineEdit_red->setText("");
        ui->spinBox_variants_red->setValue(0);

        for(int i = listTaskTypeRed.length()-1; i >= 0; i--){
            listTaskTypeRed[i]->getButton()->clicked(true);
        }
        ui->comboBox_themes_red->setCurrentIndex(0);

        ui->scrollArea_red->setEnabled(false);
        ui->scrollArea_red->setToolTip("Выберите задачу для редактирования перед тем как продолжить");
        oldTestName = "";

        ui->widget->clear();
        ui->field_task->clear();
    }
}
