#include "authorization.h"
#include "ui_authorization.h"

Authorization::Authorization(QWidget *parent, DataBase *db) :
    QWidget(parent),
    ui(new Ui::Authorization)
{
    ui->setupUi(this);

    this->db = db;

    QSqlQuery query(db->getDb("Authorization"));
    query.exec("SELECT Login FROM Authorization;");

    int id = 0;
    while(query.next()){
        id++;
    }
    if(id == 0) firstReg = true;
    else firstReg = false;

    if(firstReg) ui->lable_first_reg->setText("Текущая регистрация ясвляется первой, поэтому\nпользователь с данным логином получит полный доступ\nк приложению и возможность изменять права доступа\nдругих пользователей. Все последующие регистрации\nизначально будут иметь доступ только к 'Решение задач'.");

    ui->entry_login->setFocus();

    QTimer time;
    time.singleShot(100, this, SLOT(createCube()));

    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(rotate()));
    timer->setInterval(33);

    ui->entry->setEnabled(false);
    ui->reg->setEnabled(false);
    QTimer t;
    t.singleShot(4000, this, SLOT(disableEntryForLoad()));
}

Authorization::~Authorization()
{
    delete ui;
}

void Authorization::changeMainLable()
{
    ui->lable_first_reg->setText("");
}

void Authorization::createCube()
{
    ui->widget->createCube();
    ui->widget->hidePointName();
    ui->widget->paintLogo();

    timer->start();
}

void Authorization::exitLogin()
{
    timer->start();
    ui->entry_login->setText("");
    ui->entry_password->setText("");
    ui->reg_login->setText("");
    ui->reg_password->setText("");
    ui->reg_repeat_password->setText("");

    ui->checkBox->setChecked(false);
    on_checkBox_clicked(false);
    ui->checkBox_reg_password->setChecked(false);
    on_checkBox_reg_password_clicked(false);
    ui->checkBox_reg_repeat_password->setChecked(false);
    on_checkBox_reg_repeat_password_clicked(false);

    ui->entry_login->setFocus();

    QSqlQuery query(db->getDb("Authorization"));
    query.exec("SELECT Login FROM Authorization;");
    int id = 0;
    while(query.next()){
        id++;
    }
    if(id == 0) firstReg = true;
    else firstReg = false;
}

void Authorization::disableEntryForLoad()
{
    ui->entry->setEnabled(true);
    ui->reg->setEnabled(true);
}

void Authorization::rotate()
{
    ui->widget->rotateSplash();
}

void Authorization::on_checkBox_clicked(bool checked)
{
    if(checked) ui->entry_password->setEchoMode(QLineEdit::Normal);
    else ui->entry_password->setEchoMode(QLineEdit::Password);
}

void Authorization::on_checkBox_reg_password_clicked(bool checked)
{
    if(checked) ui->reg_password->setEchoMode(QLineEdit::Normal);
    else ui->reg_password->setEchoMode(QLineEdit::Password);
}

void Authorization::on_checkBox_reg_repeat_password_clicked(bool checked)
{
    if(checked) ui->reg_repeat_password->setEchoMode(QLineEdit::Normal);
    else ui->reg_repeat_password->setEchoMode(QLineEdit::Password);
}

void Authorization::on_entry_clicked()
{
    ui->entry_login->setText(ui->entry_login->text().trimmed());
    QSqlQuery query(db->getDb("Authorization"));
    query.exec("SELECT * FROM Authorization WHERE Login='" + ui->entry_login->text() + "' AND Password='" + ui->entry_password->text() + "';");
    QSqlRecord rec = query.record();
    if(query.first() && query.value(rec.indexOf("Login")).toString() == ui->entry_login->text()){
        QString login = query.value(rec.indexOf("Login")).toString();
        QString access = query.value(rec.indexOf("Access")).toString();

        timer->stop();
        emit setAccess(login, access);
        return;
    }
    else{
        query.exec("SELECT Login FROM Authorization WHERE Login='" + ui->entry_login->text() + "';");
        if(query.first() && ui->entry_login->text() == query.value(0).toString()){
            ui->entry_password->setStyleSheet("background-color: red;");
        }
        else{
            ui->entry_login->setStyleSheet("background-color: red;");
            ui->entry_password->setStyleSheet("background-color: red;");
        }
        return;
    }
}

void Authorization::on_entry_login_textChanged()
{
    ui->entry_login->setStyleSheet("background-color: white;");
}

void Authorization::on_entry_password_textChanged()
{
    ui->entry_password->setStyleSheet("background-color: white;");
}

void Authorization::on_reg_clicked()
{
    ui->reg_login->setText(ui->reg_login->text().trimmed());
    QString login = ui->reg_login->text();
    QString password = ui->reg_password->text();
    QString repeatPassword = ui->reg_repeat_password->text();
    if(login.length() < 3){
        ui->reg_login->setText("");
        ui->reg_login->setPlaceholderText("Слишком короткий логин");
        ui->reg_login->setStyleSheet("background-color: red;");
        return;
    }
    QRegExp rx("[^\\|/:*?\"<>]+");
    if(!rx.exactMatch(login)){
        ui->reg_login->setText("");
        ui->reg_login->setPlaceholderText("Недопустимый символ");
        ui->reg_login->setStyleSheet("background-color: red;");
        return;
    }
    if(password.length() < 5){
        ui->reg_password->setText("");
        ui->reg_password->setPlaceholderText("Слишком короткий пароль");
        ui->reg_password->setStyleSheet("background-color: red;");
        return;
    }
    if(password != repeatPassword){
        ui->reg_repeat_password->setStyleSheet("background-color: red;");
        return;
    }

    QSqlQuery query(db->getDb("Authorization"));
    query.exec("SELECT Login FROM Authorization WHERE Login = '" + login + "';");
    if(query.first()){
        ui->reg_login->setStyleSheet("background-color: red;");
        QMessageBox::information(this, " ", "Данный логин занят");
        return;
    }
    QString access;
    if(firstReg) access = "Admin";//"createTask makeTest createTest viewDemo createDemo";
    else access = "";

    query.prepare("INSERT INTO Authorization (Login, Password, Access) VALUES (:Login, :Password, :Access);");
    query.bindValue(":Login", login);
    query.bindValue(":Password", password);
    query.bindValue(":Access", access);
    if(query.exec()){
        timer->stop();
        emit setAccess(login, access);
    }
    else{
        QMessageBox::information(this, "Ошибка", "Невозможно зарегистрироваться, отсутствует база данных.");
    }

}

void Authorization::on_reg_login_textChanged()
{
    ui->reg_login->setStyleSheet("background-color: white;");
}

void Authorization::on_reg_password_textChanged()
{
    ui->reg_password->setStyleSheet("background-color: white;");
}

void Authorization::on_reg_repeat_password_textChanged()
{
    ui->reg_repeat_password->setStyleSheet("background-color: white;");
}
