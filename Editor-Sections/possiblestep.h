#ifndef POSSIBLESTEP_H
#define POSSIBLESTEP_H

#include "helpfunction.h"
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glaux.h>
#include <QList>
#include <point.h>
#include <linear.h>
#include <flat.h>
#include "string.h"
#include <string>
#include "graphicanswer.h"

class PossibleStep
{
private:
    HelpFunction help;

    QList<Point> points;
    QList<Linear> lines;
    QList<Flat> flats;
    GraphicAnswer graphicAnswer;
    QString **nnp;

public:
    PossibleStep();

    void set(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp, GraphicAnswer *graphicAnswer);
    void restore(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp, GraphicAnswer *graphicAnswer); //Выполняет отрисовку шага
    void getGraphicAnswer(GraphicAnswer *graphicAnswer);

    friend QDataStream& operator <<( QDataStream &stream, const PossibleStep &ps );
    friend QDataStream& operator >>( QDataStream &stream, PossibleStep &ps );
};

#endif // POSSIBLESTEP_H
