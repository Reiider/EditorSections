#include "adminpanel.h"
#include "ui_adminpanel.h"

AdminPanel::AdminPanel(QWidget *parent, DataBase *db) :
    QWidget(parent),
    ui(new Ui::AdminPanel)
{
    ui->setupUi(this);

    forLoadUser = new QTimer(this);
    connect(forLoadUser, SIGNAL(timeout()), this, SLOT(loadUsers()));

    this->db = db;
}

AdminPanel::~AdminPanel()
{
    delete ui;
}

void AdminPanel::showThisWindow()
{
    loadUsers();
}

void AdminPanel::startListningNewUsers()
{
    forLoadUser->start(1000);
}

void AdminPanel::stopListningNewUsers()
{
    forLoadUser->stop();
}

void AdminPanel::on_select_clicked()
{
    QSqlQuery query(db->getDb("Authorization"));
    query.exec("SELECT * FROM Authorization WHERE Login='" + ui->users->currentText() + "';");
    QSqlRecord rec = query.record();

    if(query.first()){
        if(ui->rights->isEnabled()){
            if(ui->UserName->text() == "Все"){
                if(QMessageBox::Yes != QMessageBox::question(this, " ", "Отменить общие изменения?")){
                    return;
                }
            }
            else {
                if(QMessageBox::Yes != QMessageBox::question(this, " ", "Отменить изменения текущего пользователя?")){
                    return;
                }
            }
        }
        ui->rights->setEnabled(true);
        ui->rightAdmin->setEnabled(true);
        ui->UserName->setText(query.value(rec.indexOf("Login")).toString());
        QString acc = query.value(rec.indexOf("Access")).toString();
        if(acc == "Admin"){
            ui->rights->setEnabled(false);
            setAcc("Admin", true);
        }
        else{
            QString temp;
            for(int i = 0; i < acc.length(); i++){
                if(help.one_of_line(acc, i) == " " && temp != ""){
                    setAcc(temp, true);
                    temp.clear();
                }
                else temp += help.one_of_line(acc, i);

                if(i+1 == acc.length() && temp != ""){
                    setAcc(temp, true);
                    temp.clear();
                }
            }
        }
        return;
    }

    if(ui->users->currentText() == "выбрать всех"){
        if(ui->rights->isEnabled()){
            if(ui->UserName->text() == "Все"){
                if(QMessageBox::Yes != QMessageBox::question(this, " ", "Отменить общие изменения?")){
                    return;
                }
            }
            else {
                if(QMessageBox::Yes != QMessageBox::question(this, " ", "Отменить изменения текущего пользователя?")){
                    return;
                }
            }
        }
        ui->UserName->setText("Все");
        ui->cTask->setChecked(false);
        ui->mTest->setChecked(false);
        ui->cTest->setChecked(false);
        ui->vDemo->setChecked(false);
        ui->cDemo->setChecked(false);

        ui->adm->setChecked(false);

        ui->rights->setEnabled(true);
        ui->rightAdmin->setEnabled(false);
    }
}

void AdminPanel::loadUsers()
{
    QSqlQuery query(db->getDb("Authorization"));
    query.exec("SELECT Login FROM Authorization;");
    QSqlRecord rec = query.record();
    query.next(); // пропускаем первого (админа)

    QString temp = ui->users->currentText();
    users.clear();
    while(query.next()){
        users.append(query.value(rec.indexOf("Login")).toString());
    }
    users.append("выбрать всех");
    ui->users->blockSignals(true);
    ui->users->clear();
    ui->users->addItems(users);

    if(users.indexOf(temp) != -1) ui->users->setCurrentText(temp);
    else on_users_currentTextChanged(ui->users->currentText());
    ui->users->blockSignals(false);
}

void AdminPanel::on_cancel_clicked()
{
    ui->UserName->setText("");
    ui->cTask->setChecked(false);
    ui->mTest->setChecked(false);
    ui->cTest->setChecked(false);
    ui->vDemo->setChecked(false);
    ui->cDemo->setChecked(false);

    ui->adm->setChecked(false);

    ui->rights->setEnabled(false);
    ui->rightAdmin->setEnabled(false);

    on_users_currentTextChanged(ui->users->currentText());
}

void AdminPanel::on_save_clicked()
{
    QString acc = "";
    if(ui->cTask->isChecked()) acc += "createTask ";
    if(ui->mTest->isChecked()) acc += "makeTest ";
    if(ui->cTest->isChecked()) acc += "createTest ";
    if(ui->vDemo->isChecked()) acc += "viewDemo ";
    if(ui->cDemo->isChecked()) acc += "createDemo ";

    if(ui->adm->isChecked()) acc = "Admin";

    QSqlQuery query(db->getDb("Authorization"));
    if(ui->UserName->text() == "Все"){
        for(int i = 0; i < users.length(); i++){
            query.exec("UPDATE Authorization SET Access='"+acc+"' WHERE Login='"+users[i]+"' AND Access<>'Admin';");
        }
    }
    else query.exec("UPDATE Authorization SET Access='"+acc+"' WHERE Login='"+ui->UserName->text()+"';");

    ui->rights->setEnabled(false);
    ui->rightAdmin->setEnabled(false);

    on_users_currentTextChanged(ui->users->currentText());
}

void AdminPanel::on_pushButton_del_clicked()
{
    if(ui->users->currentText() != "") {
        if(QMessageBox::Yes == QMessageBox::question(this, " ", "Вы, действительно хотите удалить пользователя " + ui->users->currentText() + "?")){
            QSqlQuery query(db->getDb("Authorization"));
            query.exec("DELETE FROM Authorization WHERE Login='" + ui->users->currentText() + "';");
            loadUsers();
        }
    }
}

void AdminPanel::on_users_currentTextChanged(const QString &arg1)
{
    /*if( (arg1 == ui->UserName->text()) || (arg1 == "выбрать всех" && ui->UserName->text() == "Все")){
        return;
    }*/
    if(ui->rights->isEnabled() || ui->rightAdmin->isEnabled()){
        if(ui->UserName->text() == "Все"){
            if(QMessageBox::Yes != QMessageBox::question(this, " ", "Отменить общие изменения?")){
                ui->users->setCurrentIndex(ui->users->findText("выбрать всех"));
                return;
            }
        }
        else {
            if(QMessageBox::Yes != QMessageBox::question(this, " ", "Отменить изменения текущего пользователя?")){
                ui->users->setCurrentIndex(ui->users->findText(ui->UserName->text()));
                return;
            }
        }
    }

    QSqlQuery query(db-> getDb("Authorization"));
    query.exec("SELECT * FROM Authorization WHERE Login='" + arg1 + "';");
    QSqlRecord rec = query.record();

    ui->cTask->setChecked(false);
    ui->mTest->setChecked(false);
    ui->cTest->setChecked(false);
    ui->vDemo->setChecked(false);
    ui->cDemo->setChecked(false);

    ui->adm->setChecked(false);

    ui->rights->setEnabled(false);
    ui->rightAdmin->setEnabled(false);

    if(arg1 == "выбрать всех"){
        ui->UserName->setText("Все");
        return;
    }
    if(query.first()){
        ui->UserName->setText(arg1);
        QString acc = query.value(rec.indexOf("Access")).toString();

        if(acc == "Admin"){
            setAcc("Admin", true);
        }
        else{
            QString temp;
            for(int i = 0; i < acc.length(); i++){
                if(help.one_of_line(acc, i) == " " && temp != ""){
                    setAcc(temp, true);
                    temp.clear();
                }
                else temp += help.one_of_line(acc, i);

                if(i+1 == acc.length() && temp != ""){
                    setAcc(temp, true);
                    temp.clear();
                }
            }
        }
    }
}

void AdminPanel::setAcc(QString s, bool ok)
{
    if(s == "createTask"){
        ui->cTask->setChecked(ok);
    }
    else if(s == "makeTest"){
        ui->mTest->setChecked(ok);
    }
    else if(s == "createTest"){
        ui->cTest->setChecked(ok);
    }
    else if(s == "viewDemo"){
        ui->vDemo->setChecked(ok);
    }
    else if(s == "createDemo"){
        ui->cDemo->setChecked(ok);
    }
    else if(s == "Admin"){
        ui->adm->setChecked(ok);
    }
    else QMessageBox::information(this, " ", "ошибка в правах доступа");
}

void AdminPanel::on_adm_clicked()
{
    if(!ui->adm->isChecked()){
        ui->rights->setEnabled(true);
    }
    else{
        ui->rights->setEnabled(false);

        ui->cTask->setChecked(false);
        ui->mTest->setChecked(false);
        ui->cTest->setChecked(false);
        ui->vDemo->setChecked(false);
        ui->cDemo->setChecked(false);
    }
}
