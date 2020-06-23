#ifndef REPLICATETASKTOCOPY_H
#define REPLICATETASKTOCOPY_H

#include <QWidget>

namespace Ui {
class ReplicateTaskToCopy;
}

class ReplicateTaskToCopy : public QWidget
{
    Q_OBJECT

public:
    explicit ReplicateTaskToCopy(QWidget *parent = 0);
    ~ReplicateTaskToCopy();

    void set(int id, int idType, QString text, bool exist, int parentWidth);
    int getId();
    bool isExist();
    void setExisting(bool ex);
    bool isChecked();
    int getIdType();
    void checking(bool ch);
    QString getText();
    void clickToCheckBox();

signals:
    void preview(int id);
    void clicked(int idType);

private slots:

    void on_pb_preview_clicked();

    void on_cb_text_clicked();

private:
    Ui::ReplicateTaskToCopy *ui;

    bool exist; //задача уже имеется в основной базе данных
    int id; // id задачи
    QString text;
    int idType;
};

#endif // REPLICATETASKTOCOPY_H
