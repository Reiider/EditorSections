#ifndef PYRAMID_H
#define PYRAMID_H

#include<QtOpenGL>
#include <QList>
#include <point.h>
#include <linear.h>
#include <flat.h>
#include "helpfunction.h"

class Pyramid
{
private:
    GLuint number;
    QList<Point> listPoints;
    QList<Linear> listLines;
    QList<Flat> listFlats;
    HelpFunction help;
    int numFornnp;//number for NameNextPoint
    QString getNameNextPoint(QString name, QString **nnp);

public:
    Pyramid();
    GLuint createPyramid(int colVershin, QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp);
    GLuint rePaintPyramid();

    QList<Point> getListPoint();
    QList<Linear> getListLines();

};

#endif // PYRAMID_H
