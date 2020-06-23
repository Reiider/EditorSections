#include "test.h"

Test::Test(QObject *parent) :
    QObject(parent)
{
    setVariant = false;
}

void Test::deleteTest()
{
    for(int i = 0; i < variants.length(); i++){
        variants[i]->deleteVariant();
    }
    variants.clear();
}
