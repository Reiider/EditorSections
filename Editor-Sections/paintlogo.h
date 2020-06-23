#ifndef PAINTLOGO_H
#define PAINTLOGO_H

#include <QtOpenGL>
#include <QList>
#include <point.h>
#include <linear.h>
#include <flat.h>

class PaintLogo
{
private:
    void drawLine(Linear *l);

    QList<Point> setPoints();
    void setLines();
    double r, g, b;
    double width;

public:
    PaintLogo();
    void setColor(double r, double g, double b);
    void setWidthLine(double width);
    void paintLogo(QList<Linear> *listLine);
};

#endif // PAINTLOGO_H
