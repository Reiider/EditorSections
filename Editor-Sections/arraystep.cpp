#include "arraystep.h"

ArrayStep::ArrayStep()
{
    stepNumber = 0;
}

void ArrayStep::append(PossibleStep step)
{
    if(stepNumber != 0){
        for(int i = array.length() + stepNumber; stepNumber != 0; stepNumber++){
            array.removeAt(i);
        }
    }
    array.append(step);
}

bool ArrayStep::previousStep(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp, GraphicAnswer *graphicAnswer)
{
    if(array.length() - 1 + stepNumber - 1 >= 0 && (array.length() - 1 + stepNumber - 1 <= array.length() - 1)){
        stepNumber--;
        array[array.length() - 1 + stepNumber].restore(listPoint, listLine, listFlat, nnp, graphicAnswer);
        return true;
    }
    return false;
}

bool ArrayStep::nextStep(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp, GraphicAnswer *graphicAnswer)
{
    if(array.length() - 1 + stepNumber + 1 >= 0 && (array.length() - 1 + stepNumber + 1 <= array.length() - 1)){
        stepNumber++;
        array[array.length() - 1 + stepNumber].restore(listPoint, listLine, listFlat, nnp, graphicAnswer);
        return true;
    }
    return false;
}

void ArrayStep::lastStep(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp, GraphicAnswer *graphicAnswer)
{
    stepNumber = 0;
    array[array.length() - 1].restore(listPoint, listLine, listFlat, nnp, graphicAnswer);
}

void ArrayStep::firstStep(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp, GraphicAnswer *graphicAnswer)
{
    stepNumber = -array.length() + 1;
    array[0].restore(listPoint, listLine, listFlat, nnp, graphicAnswer);
}

void ArrayStep::clear()
{
    array.clear();
    stepNumber = 0;
}

void ArrayStep::update(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp, GraphicAnswer *graphicAnswer)
{
    if(array.length() - 1 + stepNumber >= 0 && (array.length() - 1 + stepNumber <= array.length() - 1)){
        array[array.length() - 1 + stepNumber].set(listPoint, listLine, listFlat, nnp, graphicAnswer);
    }
}

void ArrayStep::getGraphicAnswer(GraphicAnswer *graphicAnswer)
{
    array[array.length() - 1 + stepNumber].getGraphicAnswer(graphicAnswer);
}

void ArrayStep::setBeginPosition()
{
    stepNumber = -array.length() + 1;
}

void ArrayStep::getCurrentStep(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp, GraphicAnswer *graphicAnswer)
{
    array[array.length() - 1 + stepNumber].restore(listPoint, listLine, listFlat, nnp, graphicAnswer);
}
