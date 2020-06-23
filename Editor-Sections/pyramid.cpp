#include "Pyramid.h"

Pyramid::Pyramid()
{
    numFornnp = 0;//nameNextPoint
}

QString Pyramid::getNameNextPoint(QString name, QString **nnp){  //if name = "0" get name next point, else relese name point
    QString s = name;
    if(name == "0"){
        for(int j = 0; j < 10; j++){
            for(int i = 0; i < 26; i++){
                if(nnp[i][j] == "free"){
                    nnp[i][j] = "busy";
                    if(j == 0) return nnp[i][10];
                    else{
                        char s[1];
                        itoa(j, s, 10);
                        return nnp[i][10]+s;
                    }
                }
            }
        }
    }
    else {
        int i = 0, j = 0;
        s.remove(0,1);
        if(s == '\0'){
            for(i = 0; i < 26; i++){
                if(name == nnp[i][10]) break;
            }
            nnp[i][0] = "free";
        }
        else{
            s = name;
            s.remove(1,1);
            j = s.toInt(0, 10);
            nnp[i][j] = "free";
        }
    }
    return "";
}



GLuint Pyramid::createPyramid(int colVershin, QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp)
{
    listPoints.clear();
    listLines.clear();
    listFlats.clear();

    double pi = 3.1415926535897932384626433832795;
    double alpha = 2*pi/colVershin;
    double color[3] = {0,0,0};

    Point s;
    //вершина пирамиды
    s.setPoint(getNameNextPoint("0", nnp), 0, 0, 0.7, 0);
    help.drawPoint(&s, color);
    listPoints.append(s);

    Point p1, p2, pp1;
    Linear l;
    for(int i = 1; i <= colVershin; i++){
        //получение точек правильной пирамиды с кол-вом вершин colVershin
        p1.setPoint(getNameNextPoint("0", nnp), 0, cos((i-1)*alpha), -0.3, sin((i-1)*alpha));
        help.drawPoint(&p1, color);
        listPoints.append(p1);

        if(i == 1) pp1 = p1; //запоминание первой токи, что бы соеденить линией с последней

        //линии с вершиной S
        l.setLinear(s.getName()+p1.getName(), s, p1, 0);
        help.drawLine(&l);
        listLines.append(l);

        //линии основания
        if(i != 1){
            l.setLinear(""+p2.getName()+p1.getName(), p2, p1, 0);
            help.drawLine(&l);
            listLines.append(l);
        }
        p2 = p1;
    }

    //линия основания между первой и последней точкой
    l.setLinear(""+pp1.getName()+p2.getName(), pp1, p2, 0);
    help.drawLine(&l);
    listLines.append(l);

       //создание прозрачных граней пирамиды
    Flat f;
    QList<Point> lp;

    s = listPoints[0];
    p1 = listPoints[1];
    for(int i = 2; i <= colVershin; i++){
        p2 = listPoints[i];
        lp.clear();

        lp.append(s);
        lp.append(p1);
        lp.append(p2);
        f.setFlat(lp);
        listFlats.append(f);

        p1 = p2;
    }
    lp.clear();

    lp.append(s);
    lp.append(listPoints[1]);
    lp.append(listPoints[colVershin]);
    f.setFlat(lp);
    listFlats.append(f);

    // n-угольник (основание)
    lp.clear();
    for(int i = 1; i <= colVershin; i++){
        p1 = listPoints[i];
        lp.append(p1);
    }
    f.setFlat(lp);
    listFlats.append(f);

    number = help.drawFigure(listFlats);

    *listPoint = listPoints;
    *listLine = listLines;
    *listFlat = listFlats;
    return number;
}

GLuint Pyramid::rePaintPyramid()
{
    return 0;
}

QList<Point> Pyramid::getListPoint()
{
    return listPoints;
}

QList<Linear> Pyramid::getListLines()
{
    return listLines;
}
