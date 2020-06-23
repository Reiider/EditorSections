#ifndef TESTVARIANTTYPE_H
#define TESTVARIANTTYPE_H

#include <QObject>
#include "testvarianttypetask.h"
#include "QList"
#include "QLabel"
#include "QVBoxLayout"
class TestVariantType : public QObject
{
    Q_OBJECT
public:
    QVBoxLayout *v;
    QLabel *l;
    QList<TestVariantTypeTask*> tasks;

    explicit TestVariantType(QObject *parent = 0);
    void deleteType();
signals:

public slots:

};

#endif // TESTVARIANTTYPE_H
