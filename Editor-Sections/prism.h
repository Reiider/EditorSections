#ifndef PRISM_H
#define PRISM_H


#include<QtOpenGL>
#include <QList>
#include <point.h>
#include <linear.h>
#include <flat.h>
#include "helpfunction.h"

class Prism
{
    HelpFunction help;
    GLuint number;
    QList<Point> listPoints;
    QList<Linear> listLines;
    QList<Flat> listFlats;

    int numFornnp;//number for NameNextPoint
    QString getNameNextPoint(QString name, QString **nnp);

public:
    Prism();
    GLuint createPrism(int colVershin, QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, double lengthBokovogoRebra, QString **nnp);
    GLuint rePaintPrism();

    QList<Point> getListPoint();
    QList<Linear> getListLines();
};

#endif // PRISM_H
