#ifndef ARRAYSTEP_H
#define ARRAYSTEP_H

#include <QList>
#include <point.h>
#include <linear.h>
#include <flat.h>
#include "string.h"
#include <string>
#include "possiblestep.h"

class ArrayStep
{
    QList<PossibleStep> array;
    int stepNumber;

public:
    ArrayStep();
    void append(PossibleStep step);
    bool previousStep(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp, GraphicAnswer *graphicAnswer);
    bool nextStep(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp, GraphicAnswer *graphicAnswer);
    void lastStep(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp, GraphicAnswer *graphicAnswer);
    void firstStep(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp, GraphicAnswer *graphicAnswer);
    void clear();
    void update(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp, GraphicAnswer *graphicAnswer);
    void getGraphicAnswer(GraphicAnswer *graphicAnswer);
    void setBeginPosition();
    void getCurrentStep(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp, GraphicAnswer *graphicAnswer);

    friend QDataStream& operator <<( QDataStream &stream, const ArrayStep &arr );
    friend QDataStream& operator >>( QDataStream &stream, ArrayStep &arr );
};

#endif // ARRAYSTEP_H
