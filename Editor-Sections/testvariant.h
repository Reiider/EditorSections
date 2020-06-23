#ifndef TESTVARIANT_H
#define TESTVARIANT_H

#include <QObject>
#include "testvarianttype.h"

class TestVariant : public QObject
{
    Q_OBJECT
public:
    QList<TestVariantType*> types;
    QVBoxLayout *v;

    explicit TestVariant(QObject *parent = 0);
    void deleteVariant();
signals:

public slots:

};

#endif // TESTVARIANT_H
