#include "buttonfortest.h"

ButtonForTest::ButtonForTest(QObject *parent) :
    QObject(parent)
{
}

void ButtonForTest::openTask()
{
    emit openTask(id);
    emit currentButton(button);
    emit currentChBox(id, chBox);
}
