#include "layouttasktype.h"

LayoutTaskType::LayoutTaskType(QObject *parent) :
    QObject(parent)
{
}

void LayoutTaskType::set(QHBoxLayout *h, QLabel *l, QSpinBox *s, QSpacerItem *sp, QPushButton *b)
{
    hBox = h;
    label = l;
    spin = s;
    spacer = sp;
    button = b;
}

QHBoxLayout *LayoutTaskType::getLayout()
{
    return hBox;
}

QLabel *LayoutTaskType::getLabel()
{
    return label;
}

QSpinBox *LayoutTaskType::getSpin()
{
    return spin;
}

QPushButton *LayoutTaskType::getButton()
{
    return button;
}

void LayoutTaskType::deteleType()
{
    label->~QLabel();
    spin->~QSpinBox();
    spacer->~QSpacerItem();
    button->~QPushButton();
    emit deleteType(hBox);
}
