#ifndef LAYOUTTASKTYPE_H
#define LAYOUTTASKTYPE_H

#include "QHBoxLayout"
#include "QSpinBox"
#include "QLabel"
#include "QPushButton"
#include "QSpacerItem"

class LayoutTaskType: public QObject
{
    Q_OBJECT
private:
    QPushButton *button;
    QHBoxLayout *hBox;
    QLabel *label;
    QSpinBox *spin;
    QSpacerItem *spacer;
public:
    explicit LayoutTaskType(QObject *parent = 0);
    void set(QHBoxLayout *h, QLabel *l, QSpinBox *s, QSpacerItem *sp, QPushButton *b);
    QHBoxLayout* getLayout();
    QLabel* getLabel();
    QSpinBox* getSpin();
    QPushButton* getButton();

signals:
    void deleteType(QHBoxLayout *box);

public slots:
    void deteleType(); //удалить добавленный тип задачи (CreateTest)

};

#endif // LAYOUTTASKTYPE_H
