#include "prism.h"

Prism::Prism()
{
    numFornnp = 0;//nameNextPoint
}

GLuint Prism::createPrism(int colVershin, QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, double lBR, QString **nnp)
{
    listPoints.clear();
    listLines.clear();
    listFlats.clear();

    double pi = 3.1415926535897932384626433832795;
    double alpha = 2*pi/colVershin;
    double color[3] = {0,0,0};

    Point p1, p2, pp1, pp2, pnp1, pnp2;
    Linear l;

    if(colVershin == 4){
        p1.setPoint(getNameNextPoint("0", nnp), 0, 1/sqrt(2), lBR, 1/sqrt(2));
        help.drawPoint(&p1, color);
        listPoints.append(p1);

        p2.setPoint(getNameNextPoint("0", nnp), 0, 1/sqrt(2), -lBR, 1/sqrt(2));
        help.drawPoint(&p2, color);
        listPoints.append(p2);

        pp2 = p2;
        pp1 = p1; //запоминание первых точек, что бы соеденить линии с последними

        //вертикальные линии
        l.setLinear(p1.getName()+p2.getName(), p1, p2, 0);
        help.drawLine(&l);
        listLines.append(l);

        pnp1 = p1;
        pnp2 = p2;
//
        p1.setPoint(getNameNextPoint("0", nnp), 0, 1/sqrt(2), lBR, -1/sqrt(2));
        help.drawPoint(&p1, color);
        listPoints.append(p1);

        p2.setPoint(getNameNextPoint("0", nnp), 0, 1/sqrt(2), -lBR, -1/sqrt(2));
        help.drawPoint(&p2, color);
        listPoints.append(p2);

        //вертикальные линии
        l.setLinear(p1.getName()+p2.getName(), p1, p2, 0);
        help.drawLine(&l);
        listLines.append(l);

        //линии основания

            l.setLinear(p1.getName()+pnp1.getName(), p1, pnp1, 0);
            help.drawLine(&l);
            listLines.append(l);

            l.setLinear(p2.getName()+pnp2.getName(), p2, pnp2, 0);
            help.drawLine(&l);
            listLines.append(l);

        pnp1 = p1;
        pnp2 = p2;
//
        p1.setPoint(getNameNextPoint("0", nnp), 0, -1/sqrt(2), lBR, -1/sqrt(2));
        help.drawPoint(&p1, color);
        listPoints.append(p1);

        p2.setPoint(getNameNextPoint("0", nnp), 0, -1/sqrt(2), -lBR, -1/sqrt(2));
        help.drawPoint(&p2, color);
        listPoints.append(p2);

        //вертикальные линии
        l.setLinear(p1.getName()+p2.getName(), p1, p2, 0);
        help.drawLine(&l);
        listLines.append(l);

        //линии основания

            l.setLinear(p1.getName()+pnp1.getName(), p1, pnp1, 0);
            help.drawLine(&l);
            listLines.append(l);

            l.setLinear(p2.getName()+pnp2.getName(), p2, pnp2, 0);
            help.drawLine(&l);
            listLines.append(l);

        pnp1 = p1;
        pnp2 = p2;
//
        p1.setPoint(getNameNextPoint("0", nnp), 0, -1/sqrt(2), lBR, 1/sqrt(2));
        help.drawPoint(&p1, color);
        listPoints.append(p1);

        p2.setPoint(getNameNextPoint("0", nnp), 0, -1/sqrt(2), -lBR, 1/sqrt(2));
        help.drawPoint(&p2, color);
        listPoints.append(p2);

        //вертикальные линии
        l.setLinear(p1.getName()+p2.getName(), p1, p2, 0);
        help.drawLine(&l);
        listLines.append(l);

        //линии основания

            l.setLinear(p1.getName()+pnp1.getName(), p1, pnp1, 0);
            help.drawLine(&l);
            listLines.append(l);

            l.setLinear(p2.getName()+pnp2.getName(), p2, pnp2, 0);
            help.drawLine(&l);
            listLines.append(l);

        pnp1 = p1;
        pnp2 = p2;
    }
    else{
        for(int i = 1; i <= colVershin; i++){
            //получение точек правильной призмы с кол-вом вершин colVershin

            p1.setPoint(getNameNextPoint("0", nnp), 0, cos((i-1)*alpha), lBR, sin((i-1)*alpha));
            help.drawPoint(&p1, color);
            listPoints.append(p1);

            p2.setPoint(getNameNextPoint("0", nnp), 0, cos((i-1)*alpha), -lBR, sin((i-1)*alpha));
            help.drawPoint(&p2, color);
            listPoints.append(p2);

            if(i == 1) {
                pp2 = p2;
                pp1 = p1; //запоминание первых точек, что бы соеденить линии с последними
            }
            //вертикальные линии
            l.setLinear(p1.getName()+p2.getName(), p1, p2, 0);
            help.drawLine(&l);
            listLines.append(l);

            //линии основания
            if(i != 1){
                l.setLinear(p1.getName()+pnp1.getName(), p1, pnp1, 0);
                help.drawLine(&l);
                listLines.append(l);

                l.setLinear(p2.getName()+pnp2.getName(), p2, pnp2, 0);
                help.drawLine(&l);
                listLines.append(l);
            }
            pnp1 = p1;
            pnp2 = p2;
        }
    }

    //линии основания между первой и последней парой точкой
    l.setLinear(p1.getName()+pp1.getName(), p1, pp1, 0);
    help.drawLine(&l);
    listLines.append(l);

    l.setLinear(p2.getName()+pp2.getName(), p2, pp2, 0);
    help.drawLine(&l);
    listLines.append(l);

       //создание прозрачных граней призмы
    Flat f;
    QList<Point> lp;

    p1 = listPoints[0];
    p2 = listPoints[1];
    for(int i = 2; i <= colVershin*2-1; i = i + 2){
        pp2 = listPoints[i];
        pp1 = listPoints[i+1];

        lp.clear();
        lp.append(p1);
        lp.append(p2);
        lp.append(pp1);
        lp.append(pp2);
        f.setFlat(lp);
        listFlats.append(f);

        p1 = pp2;
        p2 = pp1;
    }

    lp.clear();
    lp.append(p1);
    lp.append(p2);
    lp.append(listPoints[1]);
    lp.append(listPoints[0]);
    f.setFlat(lp);
    listFlats.append(f);

    // n-угольники (основания)
    lp.clear();
    for(int i = 0; i <= colVershin*2-1; i = i + 2){
        p1 = listPoints[i];
        lp.append(p1);
    }
    f.setFlat(lp);
    listFlats.append(f);
    //
    lp.clear();
    for(int i = 1; i <= colVershin*2-1; i = i + 2){
        p1 = listPoints[i];
        lp.append(p1);
    }
    f.setFlat(lp);
    listFlats.append(f);
    //
    number = help.drawFigure(listFlats);

    *listPoint = listPoints;
    *listLine = listLines;
    *listFlat = listFlats;
    return number;
}


QString Prism::getNameNextPoint(QString name, QString **nnp){  //if name = "0" get name next point, else relese name point
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


GLuint Prism::rePaintPrism()
{
    return 0;
}

QList<Point> Prism::getListPoint()
{
    return listPoints;
}

QList<Linear> Prism::getListLines()
{
    return listLines;
}
