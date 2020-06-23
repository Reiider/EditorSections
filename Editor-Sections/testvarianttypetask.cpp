#include "testvarianttypetask.h"

TestVariantTypeTask::TestVariantTypeTask(QObject *parent) :
    QObject(parent)
{
}

void TestVariantTypeTask::set(QHBoxLayout *hBox, QSpacerItem *spacer, QComboBox *comboBox, QPushButton *button)
{
    h = hBox;
    s = spacer;
    box = comboBox;
    b = button;
}

void TestVariantTypeTask::deleteTask()
{
    h->~QHBoxLayout();
    box->~QComboBox();
    b->~QPushButton();
    list.clear();
}

void TestVariantTypeTask::addElem(QString task, int id)
{
    box->addItem(task);
    list.append(id);
}

void TestVariantTypeTask::open()
{
    emit openTask(list[box->currentIndex()]);
}
