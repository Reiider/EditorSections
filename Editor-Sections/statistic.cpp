#include "statistic.h"
#include "ui_statistic.h"

Statistic::Statistic(QWidget *parent, DataBase *db, ViewTasks *vt) :
    QWidget(parent),
    ui(new Ui::Statistic)
{
    ui->setupUi(this);

    colLastClick = -1;
    this->db = db;
    this->vt = vt;

    ui->tableWidget->setColumnCount(6);
    QTableWidgetItem *item;
    item = new QTableWidgetItem("Тест");
    ui->tableWidget->setHorizontalHeaderItem(0, item);
    item = new QTableWidgetItem("Логин");
    ui->tableWidget->setHorizontalHeaderItem(1, item);
    item = new QTableWidgetItem("Результат");
    ui->tableWidget->setHorizontalHeaderItem(2, item);
    item = new QTableWidgetItem("Дата");
    ui->tableWidget->setHorizontalHeaderItem(3, item);
    item = new QTableWidgetItem("Время");
    ui->tableWidget->setHorizontalHeaderItem(4, item);
    item = new QTableWidgetItem("Лог файл");
    ui->tableWidget->setHorizontalHeaderItem(5, item);

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    QObject::connect(ui->tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(doubleClicked(int,int)));

    previousRow = -1;
}

Statistic::~Statistic()
{
    delete ui;
}

void Statistic::deleteSelectedItems()
{
    QList<QTableWidgetItem*> list = ui->tableWidget->selectedItems();
    if(list.isEmpty()) return;

    QList<int> listRow;
    listRow.append(ui->tableWidget->row(list[0]));
    for(int i = 1; i < list.length(); i++){
        int t = ui->tableWidget->row(list[i]);
        if(listRow.indexOf(t) == -1) listRow.append(t);
    }
    //сотрировка по возростанию
    qSort(listRow.begin(), listRow.end());
    while(!listRow.isEmpty()){
        deleteId(listRow[listRow.length() - 1]);
        listRow.removeAt(listRow.length() - 1);
    }
}

void Statistic::update()
{
    QString temp = "";
    QSqlQuery query(db->getDb("Result"));
    query.exec("SELECT COUNT(Id) FROM Result;");
    QSqlRecord rec;
    int col = 0;
    if(query.first()){
        col = query.value(0).toInt();
    }

    ui->tableWidget->setRowCount(col);

    QTableWidgetItem *item;
    int i = 0;
    query.finish();
    query.exec("SELECT * FROM Result;");
    rec = query.record();
    listId.clear();
    while(query.next()){

        item = new QTableWidgetItem(query.value(rec.indexOf("TestName")).toString());
        ui->tableWidget->setItem(i, 0, item);

        item = new QTableWidgetItem(query.value(rec.indexOf("Login")).toString());
        ui->tableWidget->setItem(i, 1, item);

        double at = query.value(rec.indexOf("AmountTasks")).toDouble();
        double st = query.value(rec.indexOf("SolveTasks")).toDouble();
        if(at != 0) at = (st/at)*100;
        else at = 0;
        temp =  temp.number(at) + "%"+ " (" + query.value(rec.indexOf("SolveTasks")).toString() + " из "+ query.value(rec.indexOf("AmountTasks")).toString() + ")";
        item = new QTableWidgetItem(temp);
        ui->tableWidget->setItem(i, 2, item);

        item = new QTableWidgetItem(query.value(rec.indexOf("TheDate")).toString());
        ui->tableWidget->setItem(i, 3, item);

        item = new QTableWidgetItem(query.value(rec.indexOf("TheTime")).toString());
        ui->tableWidget->setItem(i, 4, item);

        item = new QTableWidgetItem(query.value(rec.indexOf("FilePath")).toString());
        ui->tableWidget->setItem(i, 5, item);

        listId.append(query.value(rec.indexOf("Id")).toInt());
        i++;
    }

    ui->tableWidget->sortItems(1);
    ui->tableWidget->sortItems(0);
    ui->tableWidget->sortItems(4, Qt::DescendingOrder);
    ui->tableWidget->sortItems(3, Qt::DescendingOrder);
}

void Statistic::deleteId(int id)
{
    QSqlQuery query(db->getDb("Result"));
    query.exec("DELETE FROM Result WHERE TestName='" + ui->tableWidget->item(id, 0)->text() +
               +"' AND Login='" + ui->tableWidget->item(id, 1)->text() +
               +"' AND TheDate='" + ui->tableWidget->item(id, 3)->text() +
               +"' AND TheTime='" + ui->tableWidget->item(id, 4)->text() +
               +"' AND FilePath='" + ui->tableWidget->item(id, 5)->text() +
               +"';");
    ui->tableWidget->removeRow(id);
}

void Statistic::showThisWindow()
{
    update();
}

void Statistic::doubleClicked(int row, int col)
{
    if(col == 0 || col == 1 || col == 3 || col == 4){
        if(colLastClick == col){
            ui->tableWidget->sortItems(col, Qt::DescendingOrder);
            colLastClick = -1;
        }
        else {
            ui->tableWidget->sortItems(col);
            colLastClick = col;
        }
    }
    if(col == 5){
        ListTasksLog ltl = FileManager::openBinaryFile(ui->tableWidget->item(row, col)->text());
        if(ltl.getLogin() != "E"){
            vt->set(ltl, true);
            vt->visibleComboBox(false);//скрываем за ненадобностью (комбобокс отвечает за просмотр разных типов задач)
            emit viewTaskInRedact();
        }
    }
}

