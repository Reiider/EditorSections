#include "testvarianttype.h"

TestVariantType::TestVariantType(QObject *parent) :
    QObject(parent)
{
}

void TestVariantType::deleteType()
{
    for(int i = 0; i < tasks.length(); i++){
        tasks[i]->deleteTask();
    }
    tasks.clear();
    v->~QVBoxLayout();
    l->~QLabel();
}
