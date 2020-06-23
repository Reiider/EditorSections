#include "constructing.h"
#include "ui_constructing.h"

Constructing::Constructing(QWidget *parent, OGL *widget, DataBase *db) :
    QWidget(parent),
    ui(new Ui::Constructing)
{
    ui->setupUi(this);

    this->db = db;

    this->widget = widget;

    updatePandL();

    ui->f_TitlPrism->setVisible(false);

    QMenu *menuSetting = new QMenu();
    QObject::connect(menuSetting->addAction("Изменить стандартный цвет точек"), SIGNAL(triggered()), this, SLOT(setStandartColorToPoint()));
    QObject::connect(menuSetting->addAction("Изменить цвет линий"), SIGNAL(triggered()), this, SLOT(setStandartColorToLine()));
    QObject::connect(menuSetting->addAction("Изменить цвет имён точек"), SIGNAL(triggered()), this, SLOT(setStandartColorToText()));
    ui->pushButton_setting->setMenu(menuSetting);
}

Constructing::~Constructing()
{
    delete ui;
}

void Constructing::clearAll()
{
    ui->horizontalSlider->setValue(27);
    ui->horizontalSlider->setSliderPosition(27);
    ui->f_TitlPrism->setVisible(false);
    ui->spinBox_prism_tilt_x->setValue(0);
    updatePandL();
    widget->setStandartColorToLine(0, 0, 0);
    widget->setStandartColorToText(Qt::red);
}

void Constructing::setElementVisible(QString elem, bool vis)
{
    if(elem == "TiltPrism"){
        ui->f_TitlPrism->setVisible(vis);
    }
}

void Constructing::setsForDecisionTask()
{
    //ui->f_SizeFigure->setVisible(false);
    ui->f_TitlPrism->setVisible(false);
    ui->f_SetColor->setVisible(false);
    ui->f_RenamePoint->setVisible(false);
    ui->f_MovePoint->setVisible(false);

    //ui->f_AddPoint->setVisible(false);
    //ui->f_ChangeLocationPoint->setVisible(false);
    //ui->f_DeletePoint->setVisible(false);
    ui->f_SetColorOfPoint->setVisible(false);
    //ui->f_SetRatioSegment->setVisible(false);

    //ui->f_AddLine->setVisible(false);
    //ui->f_AddParallelLine->setVisible(false);
    //ui->f_DeleteLine->setVisible(false);
    //ui->f_PerpendicularLine->setVisible(false);
    //ui->f_SegmentEqualGive->setVisible(false);

    ui->f_IntersecLineWithFlat->setVisible(false);
    //ui->f_IntersecTwoLine->setVisible(false);
}

void Constructing::updatePandL()
{
    listP.clear();
    listL.clear();
    listF.clear();
    listP = widget->getListPoints();
    listL = widget->getListLines();
    listF = widget->getListFlats();
    QString temp;
    bool empty;

    ui->comboBox_firstP_to_add_line->clear();
    ui->comboBox_secP_to_add_line->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_firstP_to_add_line->addItem(listP[i].getName());
        ui->comboBox_secP_to_add_line->addItem(listP[i].getName());
    }

    ui->comboBox_list_p_for_add_p1->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_list_p_for_add_p1->addItem(listP[i].getName());
    }

    ui->comboBox_list_p_for_add_p2->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_list_p_for_add_p2->addItem(listP[i].getName());
    }

    ui->comboBox_select_point->clear();
    for(int i = widget->getColVershin(); i < listP.length(); i++){
        ui->comboBox_select_point->addItem(listP[i].getName());
    }

    ui->comboBox_del_line_P1->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_del_line_P1->addItem(listP[i].getName());
    }
    ui->comboBox_del_line_P2->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_del_line_P2->addItem(listP[i].getName());
    }

    ui->comboBox_intersec_2_line_1_P1->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_intersec_2_line_1_P1->addItem(listP[i].getName());
    }
    ui->comboBox_intersec_2_line_1_P2->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_intersec_2_line_1_P2->addItem(listP[i].getName());
    }

    ui->comboBox_intersec_2_line_2_P1->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_intersec_2_line_2_P1->addItem(listP[i].getName());
    }
    ui->comboBox_intersec_2_line_2_P2->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_intersec_2_line_2_P2->addItem(listP[i].getName());
    }

    ui->comboBox_intersec_line_v_pl_line_P1->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_intersec_line_v_pl_line_P1->addItem(listP[i].getName());
    }
    ui->comboBox_intersec_line_v_pl_line_P2->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_intersec_line_v_pl_line_P2->addItem(listP[i].getName());
    }

    ui->comboBox_intersec_line_v_pl_pl->clear();
    for(int i = 0; i < listF.length(); i++){
        ui->comboBox_intersec_line_v_pl_pl->addItem(listF[i].getName());
    }

    if(ui->comboBox_add_parallel_line_Point->count() == 0) empty = true;
    else empty = false;
    if(!empty) temp = ui->comboBox_add_parallel_line_Point->currentText();
    ui->comboBox_add_parallel_line_Point->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_add_parallel_line_Point->addItem(listP[i].getName());
    }
    if(!empty) ui->comboBox_add_parallel_line_Point->setCurrentText(temp);

    if(ui->comboBox_add_parallel_line_Line1->count() == 0) empty = true;
    else empty = false;
    if(!empty) temp = ui->comboBox_add_parallel_line_Line1->currentText();
    ui->comboBox_add_parallel_line_Line1->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_add_parallel_line_Line1->addItem(listP[i].getName());
    }
    if(!empty) ui->comboBox_add_parallel_line_Line1->setCurrentText(temp);

    if(ui->comboBox_add_parallel_line_Line2->count() == 0) empty = true;
    else empty = false;
    if(!empty) temp = ui->comboBox_add_parallel_line_Line2->currentText();
    ui->comboBox_add_parallel_line_Line2->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_add_parallel_line_Line2->addItem(listP[i].getName());
    }
    if(!empty) ui->comboBox_add_parallel_line_Line2->setCurrentText(temp);

    ui->comboBox_soo_line->clear();
    ui->comboBox_soo_otr->clear();
    ui->doubleSpinBox_sootnoshenie_1->setValue(0);
    ui->doubleSpinBox_sootnoshenie_2->setValue(0);

   /* ui->box_move_point->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->box_move_point->addItem(listP[i].getName());
    }*/

    ui->comboBox_perpendicular_point->clear();
    ui->comboBox_perpendicular_PtoLine1->clear();
    ui->comboBox_perpendicular_PtoLine2->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_perpendicular_point->addItem(listP[i].getName());
        ui->comboBox_perpendicular_PtoLine1->addItem(listP[i].getName());
        ui->comboBox_perpendicular_PtoLine2->addItem(listP[i].getName());
    }

    //переименовать точку
    QString current = ui->comboBox_RemanePointFrom->currentText();
    ui->comboBox_RemanePointFrom->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_RemanePointFrom->addItem(listP[i].getName());
    }
    if(ui->comboBox_RemanePointFrom->findText(current) != -1) ui->comboBox_RemanePointFrom->setCurrentText(current);

    //цвет точки
    ui->comboBox_PointColor->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_PointColor->addItem(listP[i].getName());
    }

    //отрезок равный данному
    if(ui->comboBox_LS_p->count() == 0) empty = true;
    else empty = false;
    if(!empty) temp = ui->comboBox_LS_p->currentText();
    ui->comboBox_LS_p->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_LS_p->addItem(listP[i].getName());
    }
    if(!empty) ui->comboBox_LS_p->setCurrentText(temp);

    if(ui->comboBox_LS_p_from->count() == 0) empty = true;
    else empty = false;
    if(!empty) temp = ui->comboBox_LS_p_from->currentText();
    ui->comboBox_LS_p_from->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_LS_p_from->addItem(listP[i].getName());
    }
    if(!empty) ui->comboBox_LS_p_from->setCurrentText(temp);

    if(ui->comboBox_LS_p_to->count() == 0) empty = true;
    else empty = false;
    if(!empty) temp = ui->comboBox_LS_p_to->currentText();
    ui->comboBox_LS_p_to->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_LS_p_to->addItem(listP[i].getName());
    }
    if(!empty) ui->comboBox_LS_p_to->setCurrentText(temp);

    if(ui->comboBox_LS_equal_p1->count() == 0) empty = true;
    else empty = false;
    if(!empty) temp = ui->comboBox_LS_equal_p1->currentText();
    ui->comboBox_LS_equal_p1->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_LS_equal_p1->addItem(listP[i].getName());
    }
    if(!empty) ui->comboBox_LS_equal_p1->setCurrentText(temp);

    if(ui->comboBox_LS_equal_p2->count() == 0) empty = true;
    else empty = false;
    if(!empty) temp = ui->comboBox_LS_equal_p2->currentText();
    ui->comboBox_LS_equal_p2->clear();
    for(int i = 0; i < listP.length(); i++){
        ui->comboBox_LS_equal_p2->addItem(listP[i].getName());
    }
    if(!empty) ui->comboBox_LS_equal_p2->setCurrentText(temp);
}

void Constructing::setStandartColorToPoint()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет", 0);
    if(color.isValid()) widget->setStandartColorToPoint(color.red()/255., color.green()/255., color.blue()/255.);
}

void Constructing::setStandartColorToLine()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет", 0);
    if(color.isValid()) widget->setStandartColorToLine(color.red()/255., color.green()/255., color.blue()/255.);
}

void Constructing::setStandartColorToText()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет", 0);
    if(color.isValid()) widget->setStandartColorToText(color);
}

void Constructing::on_horizontalSlider_valueChanged(int value)
{
    widget->changeSizeFigure(value);
}

void Constructing::on_add_point_clicked()
{
    QString s, a1, a2;
    if(listP.isEmpty());
    else{
        a1 = ui->comboBox_list_p_for_add_p1->currentText();
        a2 = ui->comboBox_list_p_for_add_p2->currentText();
        s = widget->addPoint(a1, a2);
    }
    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
    updatePandL();
    ui->comboBox_soo_line->addItem(a1 + a2);
    ui->comboBox_soo_line->addItem(a2 + a1);
    ui->comboBox_soo_otr->addItem(a1 + widget->getNameLastPoint());
    ui->comboBox_soo_otr->addItem(a2 + widget->getNameLastPoint());
}


void Constructing::on_horizontalSlider_for_add_point_valueChanged(int value)
{
    if(!listP.isEmpty()){
        widget->moveLastPoint(value, ui->doubleSpinBox_sootnoshenie_1, ui->doubleSpinBox_sootnoshenie_2, ui->comboBox_soo_otr);
        listP.clear();
        listP = widget->getListPoints();
    }
}

void Constructing::on_pushButton_del_point_clicked()
{
    if(ui->comboBox_select_point->currentText() != "") widget->delPoint(ui->comboBox_select_point->currentText());
    updatePandL();
}

void Constructing::on_button_add_Line_clicked()
{
    QString s;
    if(ui->comboBox_firstP_to_add_line->currentText() != "" && ui->comboBox_secP_to_add_line->currentText() != ""){
        s = widget->addLine(ui->comboBox_firstP_to_add_line->currentText(), ui->comboBox_secP_to_add_line->currentText());
    }
    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
    updatePandL();
}

void Constructing::on_button_del_Line_clicked()
{
    QString s;
    if(ui->comboBox_del_line_P1->currentText() != "" && ui->comboBox_del_line_P2->currentText() != ""){
        int i,j;
        for(i = 0; i < listP.length(); i++){
            if(listP[i].getName() == ui->comboBox_del_line_P1->currentText()) break;
        }
        for(j = 0; j < listP.length(); j++){
            if(listP[j].getName() == ui->comboBox_del_line_P2->currentText()) break;
        }
        s = widget->delLine(listP[i], listP[j]);
        if(s!=""){
            QMessageBox::information(this, " ", s, 0, 0);
        }
    }
    updatePandL();
}

void Constructing::on_pushButton_clicked()
{
    QString s;
    if(listL.isEmpty());
    else
        s = widget->addPointIntersection_LineWithLine(ui->comboBox_intersec_2_line_1_P1->currentText(),
                                                          ui->comboBox_intersec_2_line_1_P2->currentText(),
                                                          ui->comboBox_intersec_2_line_2_P1->currentText(),
                                                          ui->comboBox_intersec_2_line_2_P2->currentText());
    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
    updatePandL();
}

void Constructing::on_pushButton_intersec_line_v_pl_clicked()
{
    QString s;
    if(listL.isEmpty() || listF.isEmpty());
    else
       s = widget->addPointIntersection_LineWithFlat(ui->comboBox_intersec_line_v_pl_line_P1->currentText(), ui->comboBox_intersec_line_v_pl_line_P2->currentText(), ui->comboBox_intersec_line_v_pl_pl->currentText());
    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
    updatePandL();
}

void Constructing::on_pushButton_set_sootnoshenie_y_tochki_clicked()
{
    if(ui->doubleSpinBox_sootnoshenie_2->value() != 0 && ui->comboBox_soo_line->currentText() != ""){
        QString a = widget->setSootnoshenie(ui->comboBox_soo_otr->currentText(), ui->doubleSpinBox_sootnoshenie_1->value(), ui->doubleSpinBox_sootnoshenie_2->value());
        if(a != "") QMessageBox::information(this, " ", a, 0, 0);
    }
    else if(ui->doubleSpinBox_sootnoshenie_2->value() == 0 && ui->comboBox_soo_line->currentText() != "") QMessageBox::information(this, " ", "Деление на нуль.", 0, 0);
}

void Constructing::on_pushButton_add_parallel_line_clicked()
{
    QString s;
    if(ui->comboBox_add_parallel_line_Point->currentText() != "" && ui->comboBox_add_parallel_line_Line1->currentText() != "" && ui->comboBox_add_parallel_line_Line2->currentText() != ""){
        s = widget->addParallelLine(ui->comboBox_add_parallel_line_Point->currentText(), ui->comboBox_add_parallel_line_Line1->currentText(), ui->comboBox_add_parallel_line_Line2->currentText());
    }
    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
    updatePandL();
}

void Constructing::on_pushOxp_clicked()
{

    /*
    QString s;
    if(ui->box_move_point->currentText() != ""){
        s = widget->movePoint(ui->box_move_point->currentText(), ui->horizontalSliderOXYZ->value()/50.,0);
    }
    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
    */
    QString s;
    if(ui->le_move_point->text() != ""){
        s = widget->movePoint(ui->le_move_point->text(), ui->horizontalSliderOXYZ->value()/50., 0, ui->cb_move_point->isChecked());
    }
    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
}

void Constructing::on_pushOxm_clicked()
{
    /*
    QString s;
    if(ui->box_move_point->currentText() != ""){
        s = widget->movePoint(ui->box_move_point->currentText(), - ui->horizontalSliderOXYZ->value()/50.,0);
    }
    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
    */
    QString s;
    if(ui->le_move_point->text() != ""){
        s = widget->movePoint(ui->le_move_point->text(), - ui->horizontalSliderOXYZ->value()/50., 0, ui->cb_move_point->isChecked());
    }
    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
}

void Constructing::on_pushOyp_clicked()
{
    /*
    QString s;
    if(ui->box_move_point->currentText() != ""){
       s = widget->movePoint(ui->box_move_point->currentText(), 0, ui->horizontalSliderOXYZ->value()/50.);
    }
    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
    */
    QString s;
    if(ui->le_move_point->text() != ""){
       s = widget->movePoint(ui->le_move_point->text(), 0, ui->horizontalSliderOXYZ->value()/50., ui->cb_move_point->isChecked());
    }
    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
}

void Constructing::on_pushOym_clicked()
{
    /*
    QString s;
    if(ui->box_move_point->currentText() != ""){
       s = widget->movePoint(ui->box_move_point->currentText(), 0, - ui->horizontalSliderOXYZ->value()/50.);
    }
    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
    */
    QString s;
    if(ui->le_move_point->text() != ""){
       s = widget->movePoint(ui->le_move_point->text(), 0, - ui->horizontalSliderOXYZ->value()/50., ui->cb_move_point->isChecked());
    }
    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
}

void Constructing::on_pushButton_perpendicular_clicked()
{
    QString s;
    if(ui->comboBox_perpendicular_point->currentText() != ""){
        s = widget->createPerpendicularLine(ui->comboBox_perpendicular_point->currentText(), ui->comboBox_perpendicular_PtoLine1->currentText(), ui->comboBox_perpendicular_PtoLine2->currentText());
    }
    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
    updatePandL();
}

void Constructing::on_pushButton_3_clicked()
{
    QString s;
    if(ui->comboBox_RemanePointFrom->currentText() != "" && ui->lineRemanePointTo->text() != ""){
        s = widget->renamePoint(ui->comboBox_RemanePointFrom->currentText(), ui->lineRemanePointTo->text());
    }
    if(s!=""){
        ui->lineRemanePointTo->setText("");
        QMessageBox::information(this, " ", s, 0, 0);
    }
    else {
         ui->comboBox_RemanePointFrom->clear();
         for(int i = 0; i < listP.length(); i++){
             ui->comboBox_RemanePointFrom->addItem(listP[i].getName());
         }
         ui->lineRemanePointTo->setText("");
    }
    updatePandL();
}

void Constructing::nextStep()
{
    QString s;
    s = widget->nextStep();

    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
    updatePandL();
}

void Constructing::backStep()
{
    QString s;
    s = widget->previousStep();

    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
    updatePandL();
}

void Constructing::on_pushButton_color_black_clicked()
{
    widget->setColorOfPoint(ui->comboBox_PointColor->currentText(), 0, 0, 0);
}

void Constructing::on_pushButton_color_blue_clicked()
{
    widget->setColorOfPoint(ui->comboBox_PointColor->currentText(), 0, 0, 1);
}

void Constructing::on_pushButton_color_red_clicked()
{
    widget->setColorOfPoint(ui->comboBox_PointColor->currentText(), 1, 0, 0);
}

void Constructing::on_pushButton_color_green_clicked()
{
    widget->setColorOfPoint(ui->comboBox_PointColor->currentText(), 0, 1, 0);
}

void Constructing::on_pushButton_color_white_clicked()
{
    widget->setColorOfPoint(ui->comboBox_PointColor->currentText(), 1, 1, 1);
}

void Constructing::on_comboBox_soo_otr_currentIndexChanged(int index)
{
    if(index == 0){
        ui->comboBox_soo_line->setCurrentIndex(0);
    }
    else ui->comboBox_soo_line->setCurrentIndex(1);
}

void Constructing::on_toolButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет", 0);
    if(color.isValid()) widget->setColorOfPoint(ui->comboBox_PointColor->currentText(), color.red()/255., color.green()/255., color.blue()/255.);
}

void Constructing::on_pushButton_prism_x_clicked()
{
    widget->tiltPrism(ui->spinBox_prism_tilt_x->value(), "x", -1);
    ui->spinBox_prism_tilt_x->setValue(0);
}

void Constructing::on_horizontalSlider_2_valueChanged(int value)
{
    widget->tiltPrism(0, "y", value/100.);
}

void Constructing::on_pB_LS_clicked()
{
    if(ui->comboBox_LS_p->count() == 0) return;
    QString s;
    s = widget->addLS(ui->comboBox_LS_p->currentText(), ui->comboBox_LS_p_from->currentText(), ui->comboBox_LS_p_to->currentText(),
                          ui->comboBox_LS_equal_p1->currentText(), ui->comboBox_LS_equal_p2->currentText());

    if(s!=""){
        QMessageBox::information(this, " ", s, 0, 0);
    }
    updatePandL();
}

void Constructing::setVisibleAction(int n, bool ok)
{
    switch (n) {
    case 1:
        ui->f_AddPoint->setVisible(ok);
        break;
    case 2:
        ui->f_ChangeLocationPoint->setVisible(ok);
        break;
    case 3:
        ui->f_SetRatioSegment->setVisible(ok);
        break;
    case 4:
        ui->f_DeletePoint->setVisible(ok);
        break;
    case 5:
        ui->f_AddLine->setVisible(ok);
        break;
    case 6:
        ui->f_SegmentEqualGive->setVisible(ok);
        break;
    case 7:
        ui->f_AddParallelLine->setVisible(ok);
        break;
    case 8:
        ui->f_PerpendicularLine->setVisible(ok);
        break;
    case 9:
        ui->f_DeleteLine->setVisible(ok);
        break;
    case 10:
        ui->f_IntersecTwoLine->setVisible(ok);
        break;
    default:
        break;
    }
}

void Constructing::createCube()
{
    widget->unsetRedactTask();
    widget->setColVershin(8);
    widget->createCube();
    setElementVisible("TiltPrism", false);

    updatePandL();
}

void Constructing::createPyramid()
{
    widget->unsetRedactTask();
    bool ok;
    int v = QInputDialog::getInt(this, "Пирамида", "Укажите количество вершин в основании.", 3, 3, 1000, 1, &ok);
    if (ok){
        if(!listP.isEmpty()){
            if(QMessageBox::question(this, "Новая фигура",
                                           "Данные текущей фигуры будут потеряны, продолжить?",
                                           QMessageBox::Ok | QMessageBox::Cancel,
                                           QMessageBox::Cancel) == QMessageBox::Ok)
            {
                widget->setColVershin(v);
                widget->createPyramid();
                setElementVisible("TiltPrism", false);
            }
            else return;
        }
        else {
            widget->setColVershin(v);
            widget->createPyramid();
            setElementVisible("TiltPrism", false);
        }
    }
    updatePandL();
}

void Constructing::createPrism()
{
    bool ok;
    int v = QInputDialog::getInt(this, "Призма", "Укажите количество вершин в основании.", 3, 3, 1000, 1, &ok);
    if (ok){
        if(!listP.isEmpty()){
            if(QMessageBox::question(this, "Новая фигура",
                                           "Данные текущей фигуры будут потеряны, продолжить?",
                                           QMessageBox::Ok | QMessageBox::Cancel,
                                           QMessageBox::Cancel) == QMessageBox::Ok)
            {
                widget->setColVershin(v);
                widget->createPrism(1);
                setElementVisible("TiltPrism", true);
            }
            else return;
        }
        else {
            widget->setColVershin(v);
            widget->createPrism(1);
            setElementVisible("TiltPrism", true);
        }

    }
    updatePandL();
}

void Constructing::createFigure()
{
    QList<QString> type;
    type.append("Куб");
    type.append("Пирамида");
    type.append("Призма");

    bool ok;
    QString v = QInputDialog::getItem(this, "Фигуры", "Выберите тип фигуры", type, 0, false, &ok);
    if (ok){
        if(v == "Куб"){
            if(!listP.isEmpty()){
                if(QMessageBox::question(this, "Новая фигура",
                                               "Данные текущей фигуры будут потеряны, продолжить?",
                                               QMessageBox::Ok | QMessageBox::Cancel,
                                               QMessageBox::Cancel) == QMessageBox::Ok)
                {
                    createCube();
                }
                else return;
            }
            else createCube();
        }
        else if(v == "Пирамида"){
            createPyramid();
        }
        else if(v == "Призма"){
            createPrism();
        }
    }
    updatePandL();
}

void Constructing::on_spinBox_valueChanged(int arg1)
{
    widget->setSizeFont(arg1);
}

void Constructing::on_pb_ResetAllConstructing_clicked()
{
    widget->firstStep();
}
