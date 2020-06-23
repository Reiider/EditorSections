#include "possiblestep.h"

PossibleStep::PossibleStep()
{
    this->nnp = new QString*[26];
    for(int i = 0; i < 26; i++){
        this->nnp[i] = new QString[11];
    }
}

void PossibleStep::set(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp, GraphicAnswer *graphicAnswer)
{
    points.clear();
    for(int i = 0; i < listPoint->length(); i++){
        points.append(listPoint->at(i));
    }
    lines.clear();
    for(int i = 0; i < listLine->length(); i++){
        lines.append(listLine->at(i));
    }
    flats.clear();
    for(int i = 0; i < listFlat->length(); i++){
        flats.append(listFlat->at(i));
    }
    this->graphicAnswer.clear();
    this->graphicAnswer.setFlats(graphicAnswer->getFlats(), graphicAnswer->getColorFlats());

    this->nnp = new QString*[26];
    for(int i = 0; i < 26; i++){
        this->nnp[i] = new QString[11];
    }

    for(int j = 0; j < 10; j++){
        for(int i = 0; i < 26; i++){
            this->nnp[i][j] = nnp[i][j];
        }
    }
}

void PossibleStep::restore(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp, GraphicAnswer *graphicAnswer)
{
    while(listPoint->length() > 0){
        listPoint->takeFirst().deleteNum();
    }
    while(listLine->length() > 0){
        listLine->takeFirst().deleteNum();
    }
    listFlat->clear();

    double color[3] = {0,0,0};
    QList<Point> pl;
    for(int i = 0; i < points.length(); i++){
        pl.append(points[i]);
    }
    QList<Linear> ll;
    for(int i = 0; i < lines.length(); i++){
        ll.append(lines[i]);
    }

    for(int i = 0; i < pl.length(); i++){
       help.drawPoint(&pl[i], color);
    }
    for(int i = 0; i < ll.length(); i++){
       help.drawLine(&ll[i]);
    }


    //скопировать всё
    listPoint->clear();
    *listPoint = pl;
    listLine->clear();
    *listLine = ll;
    listFlat->clear();
    *listFlat = flats;
    graphicAnswer->clear();
    graphicAnswer->setFlats(this->graphicAnswer.getFlats(), this->graphicAnswer.getColorFlats());
    for(int j = 0; j < 10; j++){
        for(int i = 0; i < 26; i++){
            nnp[i][j] = this->nnp[i][j];
        }
    }
}

void PossibleStep::getGraphicAnswer(GraphicAnswer *graphicAnswer)
{
    graphicAnswer->clear();
    graphicAnswer->setFlats(this->graphicAnswer.getFlats(), this->graphicAnswer.getColorFlats());
}
