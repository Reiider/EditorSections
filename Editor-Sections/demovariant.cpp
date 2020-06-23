#include "demovariant.h"
#include "ui_demovariant.h"

DemoVariant::DemoVariant(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DemoVariant)
{
    ui->setupUi(this);
}

DemoVariant::~DemoVariant()
{
    delete ui;
}

void DemoVariant::setText(QString s)
{
    ui->plainTextEdit_text->setPlainText(s);
}

void DemoVariant::setIndex(int i)
{
    index = i;
}

void DemoVariant::setPBRedact()
{
    ui->pushButton_readact->setText("Редактировать");
    ui->plainTextEdit_text->setReadOnly(true);
}

void DemoVariant::setPBSave()
{
    ui->pushButton_readact->setText("Сохранить");
    ui->plainTextEdit_text->setReadOnly(false);
}

bool DemoVariant::isSave()
{
    if(ui->pushButton_readact->text() == "Сохранить") return true;
    else return false;
}

void DemoVariant::on_pushButton_view_clicked()
{
    emit viewVariant(index);
}

void DemoVariant::on_pushButton_delete_clicked()
{
    emit deleteVariant(index);
}

void DemoVariant::on_pushButton_readact_clicked()
{
    if(ui->pushButton_readact->text() == "Редактировать"){
        emit redactVariant(index);
    }
    else {
        emit saveVariant(index, ui->plainTextEdit_text->toPlainText());
    }

}
