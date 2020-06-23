#ifndef TEST_H
#define TEST_H

#include <QObject>
#include "testvariant.h"
class Test : public QObject
{
    Q_OBJECT
public:
    QList<TestVariant*> variants;
    bool setVariant; // если варианты уже создавались
    explicit Test(QObject *parent = 0);
    void deleteTest();
signals:

public slots:

};

#endif // TEST_H
