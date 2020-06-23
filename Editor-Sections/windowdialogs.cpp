#include "windowdialogs.h"
#include "ui_windowdialogs.h"

WindowDialogs::WindowDialogs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WindowDialogs)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

WindowDialogs::~WindowDialogs()
{
    delete ui;
}

void WindowDialogs::setWindow(QString nameWind, QString mess)
{
    hideAllWindows();
    if(nameWind == "answer"){
        this->setWindowTitle("Ответ");
        this->setMaximumSize(ui->fWriteAnswer->maximumSize());
        this->setMinimumSize(ui->fWriteAnswer->minimumSize());
        ui->fWriteAnswer->setVisible(true);
    }
    if(nameWind == "AuthorAnswer"){
        this->setWindowTitle("Авторский ответ");
        this->setMaximumSize(ui->fAuthorAnswer->maximumSize());
        this->setMinimumSize(ui->fAuthorAnswer->minimumSize());
        ui->fAuthorAnswer->setVisible(true);
        //this->move(ui->fAuthorAnswer->pos().x()+300, ui->fAuthorAnswer->pos().y());

    }
    if(nameWind == "IntersecLineWithFlat"){
        this->setWindowTitle("Пересечение прямой и плоскости");
        this->setMaximumSize(ui->fLineWithFlat->maximumSize());
        this->setMinimumSize(ui->fLineWithFlat->minimumSize());
        ui->fLineWithFlat->setVisible(true);
    }
    if(nameWind == "IntersecTwoFlats"){
        this->setWindowTitle("Пересечение двух плоскостей");
        this->setMaximumSize(ui->fTwoFlat->maximumSize());
        this->setMinimumSize(ui->fTwoFlat->minimumSize());
        ui->fTwoFlat->setVisible(true);
    }
    if(nameWind == "CuttingPlane"){
        this->setWindowTitle("Секущая плоскость");
        this->setMaximumSize(ui->fCuttingPlane->maximumSize());
        this->setMinimumSize(ui->fCuttingPlane->minimumSize());
        ui->fCuttingPlane->setVisible(true);
    }
    if(nameWind == "UpdateCuttingPlane"){
        this->setWindowTitle("");
        this->setMaximumSize(ui->fUpdateCuttingPlane->maximumSize());
        this->setMinimumSize(ui->fUpdateCuttingPlane->minimumSize());
        ui->fUpdateCuttingPlane->setVisible(true);
    }
    if(nameWind == "UpdateIntersecTwoFlats"){
        this->setWindowTitle("");
        this->setMaximumSize(ui->fUpdateIntersecTwoFlats->maximumSize());
        this->setMinimumSize(ui->fUpdateIntersecTwoFlats->minimumSize());
        ui->fUpdateIntersecTwoFlats->setVisible(true);
    }
    if(nameWind == "SaveTask"){
        this->setWindowTitle("Save");
        ui->SaveTask_label->setText(mess);
        this->setMaximumSize(ui->fSaveTask->maximumSize());
        QSize size;
        size.setHeight(ui->SaveTask_label->sizeHint().height() + 50);
        size.setWidth(ui->SaveTask_label->sizeHint().width());
        this->setMinimumSize(size);

        ui->fSaveTask->setVisible(true);
    }

    QDesktopWidget *qdw = QApplication::desktop();

    if(this->pos().x() == 0 && this->pos().y() == 0) this->move(qdw->screenGeometry().width()/2, qdw->screenGeometry().height()/2);
    else this->move(this->pos());
}

void WindowDialogs::hideAllWindows()
{
    ui->fWriteAnswer->setVisible(false);
    ui->fAuthorAnswer->setVisible(false);
    ui->fLineWithFlat->setVisible(false);
    ui->fTwoFlat->setVisible(false);
    ui->fCuttingPlane->setVisible(false);
    ui->fUpdateCuttingPlane->setVisible(false);
    ui->fUpdateIntersecTwoFlats->setVisible(false);
    ui->fSaveTask->setVisible(false);
}

void WindowDialogs::on_WriteAnswer_pb_OK_clicked()
{
    this->hide();
    emit text_inputed(ui->WriteAnswer_lineEdit->text());
}

void WindowDialogs::on_WriteAnswer_pb_Cancel_clicked()
{
    this->hide();
}

void WindowDialogs::on_pushButton_clicked()
{
    this->hide();
    emit s_authorAnswer(ui->AuthorAnswer_lineEdit->text());
}

void WindowDialogs::on_pushButton_2_clicked()
{
    this->hide();
}

void WindowDialogs::on_LineWithFlat_pb_OK_clicked()
{
    this->hide();
    emit s_intersecLineWithFlat(ui->LineWithFlat_le1->text(), ui->LineWithFlat_le2->text());
}

void WindowDialogs::on_LineWithFlat_pb_Cancel_clicked()
{
    this->hide();
}

void WindowDialogs::on_TwoFlat_pb_OK_clicked()
{
    this->hide();
    emit s_intersecTwoFlats(ui->TwoFlat_le1->text(), ui->TwoFlat_le2->text());
}

void WindowDialogs::on_TwoFlat_pb_Cancel_clicked()
{
    this->hide();
}

void WindowDialogs::on_CuttingPlane_pb_OK_clicked()
{
    this->hide();
    emit s_cuttingPlane(ui->CuttingPlane_le->text());
}

void WindowDialogs::on_CuttingPlane_pb_Cancel_clicked()
{
    this->hide();
}

void WindowDialogs::on_UpdateCuttingPlane_pb_OK_clicked()
{
    this->hide();
    emit s_updateCuttingPlane(ui->UpdateCuttingPlane_le->text());
}

void WindowDialogs::on_UpdateCuttingPlane_pb_Cancel_clicked()
{
    this->hide();
}

void WindowDialogs::on_UpdateIntersecTwoFlats_pb_OK_clicked()
{
    this->hide();
    emit s_updateIntersecTwoFlats(ui->UpdateIntersecTwoFlats_le_1->text(), ui->UpdateIntersecTwoFlats_le_2->text());
}

void WindowDialogs::on_UpdateIntersecTwoFlats_pb_Cancel_clicked()
{
    this->hide();
}

void WindowDialogs::on_SaveTask_pb_Save_clicked()
{
    this->hide();
    emit s_saveTask();
}

void WindowDialogs::on_SaveTask_pb_Cancel_clicked()
{
    this->hide();
}
