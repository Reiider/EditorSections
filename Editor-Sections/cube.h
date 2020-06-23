#ifndef CUBE_H
#define CUBE_H

#include <QtOpenGL>
#include <QList>
#include <point.h>
#include <linear.h>
#include <flat.h>
#include "helpfunction.h"

class Cube
{
    HelpFunction help;
    GLuint number;
    QList<Point> listPoints;
    QList<Linear> listLines;
    QList<Flat> listFlats;

    int numFornnp;//number for NameNextPoint
    QString getNameNextPoint(QString name, QString **nnp);

public:
    Cube();
    GLuint createCube(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp);
    GLuint rePaintCube();

    QList<Point> getListPoint();
    QList<Linear> getListLines();


};

#endif // CUBE_H
