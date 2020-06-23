#ifndef TESTVARIANTTYPETASK_H
#define TESTVARIANTTYPETASK_H

#include <QObject>
#include "QHBoxLayout"
#include "QPushButton"
#include "QSpacerItem"
#include "QComboBox"
#include "QList"

class TestVariantTypeTask : public QObject
{
    Q_OBJECT

public:
    QHBoxLayout *h;
    QSpacerItem *s;
    QComboBox *box;  //
    QPushButton *b;
    QList<int> list; //

    explicit TestVariantTypeTask(QObject *parent = 0);
    void set(QHBoxLayout *hBox, QSpacerItem *spacer, QComboBox *comboBox, QPushButton *button);
    void deleteTask();
    void addElem(QString task, int id);
signals:
    void openTask(int id);
public slots:
    void open();
};

#endif // TESTVARIANTTYPETASK_H
