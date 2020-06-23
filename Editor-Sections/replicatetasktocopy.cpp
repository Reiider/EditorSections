#include "replicatetasktocopy.h"
#include "ui_replicatetasktocopy.h"

ReplicateTaskToCopy::ReplicateTaskToCopy(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReplicateTaskToCopy)
{
    ui->setupUi(this);
}

ReplicateTaskToCopy::~ReplicateTaskToCopy()
{
    delete ui;
}

void ReplicateTaskToCopy::set(int id, int idType, QString text, bool exist, int parentWidth)
{
    this->id = id;
    this->idType = idType;
    this->text = text;
    this->exist = exist;
    ui->cb_text->setText(text);
    ui->cb_text->setMaximumWidth(parentWidth - 160);
    ui->cb_text->setMinimumWidth(parentWidth - 160);
}

int ReplicateTaskToCopy::getId()
{
    return id;
}

bool ReplicateTaskToCopy::isExist()
{
    return exist;
}

void ReplicateTaskToCopy::setExisting(bool ex)
{
    exist = ex;
}

bool ReplicateTaskToCopy::isChecked()
{
    return ui->cb_text->isChecked();
}

int ReplicateTaskToCopy::getIdType()
{
    return idType;
}

void ReplicateTaskToCopy::checking(bool ch)
{
    ui->cb_text->setChecked(ch);
}

QString ReplicateTaskToCopy::getText()
{
    return text;
}

void ReplicateTaskToCopy::clickToCheckBox()
{
    ui->cb_text->setChecked(!ui->cb_text->isChecked());
    emit clicked(idType);
}

void ReplicateTaskToCopy::on_pb_preview_clicked()
{
    emit preview(id);
}

void ReplicateTaskToCopy::on_cb_text_clicked()
{
    emit clicked(idType);
}
