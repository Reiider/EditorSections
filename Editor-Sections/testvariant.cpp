#include "testvariant.h"

TestVariant::TestVariant(QObject *parent) :
    QObject(parent)
{
}

void TestVariant::deleteVariant()
{
    for(int i = 0; i < types.length(); i++){
        types[i]->deleteType();
    }
    types.clear();
    v->~QVBoxLayout();
}
