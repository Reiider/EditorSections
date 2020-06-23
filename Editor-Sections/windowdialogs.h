#ifndef WINDOWDIALOGS_H
#define WINDOWDIALOGS_H

#include <QWidget>
#include <QDesktopWidget>

namespace Ui {
class WindowDialogs;
}

class WindowDialogs : public QWidget
{
    Q_OBJECT

public:
    explicit WindowDialogs(QWidget *parent = 0);
    ~WindowDialogs();

    void setWindow(QString nameWind, QString mess = "");

signals:
    void text_inputed(QString s);
    void s_authorAnswer(QString s);
    void s_intersecLineWithFlat(QString s1, QString s2);
    void s_intersecTwoFlats(QString s1, QString s2);
    void s_cuttingPlane(QString s);
    void s_updateCuttingPlane(QString s);
    void s_updateIntersecTwoFlats(QString s1, QString s2);
    void s_saveTask();

private slots:
    void on_WriteAnswer_pb_OK_clicked();

    void on_WriteAnswer_pb_Cancel_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_LineWithFlat_pb_OK_clicked();

    void on_LineWithFlat_pb_Cancel_clicked();

    void on_TwoFlat_pb_OK_clicked();

    void on_TwoFlat_pb_Cancel_clicked();

    void on_CuttingPlane_pb_OK_clicked();

    void on_CuttingPlane_pb_Cancel_clicked();

    void on_UpdateCuttingPlane_pb_OK_clicked();

    void on_UpdateCuttingPlane_pb_Cancel_clicked();

    void on_UpdateIntersecTwoFlats_pb_OK_clicked();

    void on_UpdateIntersecTwoFlats_pb_Cancel_clicked();

    void on_SaveTask_pb_Save_clicked();

    void on_SaveTask_pb_Cancel_clicked();

private:
    Ui::WindowDialogs *ui;

    void hideAllWindows();
};

#endif // WINDOWDIALOGS_H
