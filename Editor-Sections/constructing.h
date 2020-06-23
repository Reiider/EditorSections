#ifndef CONSTRUCTING_H
#define CONSTRUCTING_H

#include <QWidget>
#include "ogl.h"
#include <point.h>
#include <linear.h>
#include <flat.h>
#include <QList>
#include "database.h"
#include "filemanager.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QString>
#include <QMenu>
#include "buttonblockaction.h"

namespace Ui {
class Constructing;
}

class Constructing : public QWidget
{
    Q_OBJECT

public:
    explicit Constructing(QWidget *parent = 0, OGL *widget = new OGL(), DataBase *db = new DataBase());
    ~Constructing();

    void setElementVisible(QString elem, bool vis);
    void setsForDecisionTask();
    //void setsForCreateTask(); //изначально всё для него
   // void setsForCreateDemo(); //то же самое что и в setsForDecisionTask, пока что


public slots:
    void createCube();
    void createPyramid();
    void createPrism();
    void createFigure();
    void setVisibleAction(int n, bool ok);
    void updatePandL();
    void clearAll();
    void nextStep();
    void backStep();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_add_point_clicked();

    void on_horizontalSlider_for_add_point_valueChanged(int value);

    void on_pushButton_del_point_clicked();

    void on_button_add_Line_clicked();

    void on_button_del_Line_clicked();

    void on_pushButton_clicked();

    void on_pushButton_intersec_line_v_pl_clicked();

    void on_pushButton_set_sootnoshenie_y_tochki_clicked();

    void on_pushButton_add_parallel_line_clicked();

    void on_pushOxp_clicked();

    void on_pushOxm_clicked();

    void on_pushOyp_clicked();

    void on_pushOym_clicked();

    void on_pushButton_perpendicular_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_color_black_clicked();

    void on_pushButton_color_blue_clicked();

    void on_pushButton_color_red_clicked();

    void on_pushButton_color_green_clicked();

    void on_pushButton_color_white_clicked();

    void on_comboBox_soo_otr_currentIndexChanged(int index);

    void on_toolButton_clicked();

    void setStandartColorToPoint();

    void setStandartColorToLine();

    void setStandartColorToText();

    void on_pushButton_prism_x_clicked();

    void on_horizontalSlider_2_valueChanged(int value);

    void on_pB_LS_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_pb_ResetAllConstructing_clicked();

private:
    Ui::Constructing *ui;

    QList<Point> listP;
    QList<Linear> listL;
    QList<Flat> listF;
    QList<QString> Tasks;
    QList<QString> miniTasks;
    QList<int> idMiniTask;

    QString currentIndexMiniTasks;
    void updateMiniTasks(QString arg1);

    void setTaskType();

    DataBase *db;

    QList<QString> taskType;

    OGL *widget;

};
#endif // CONSTRUCTING_H
