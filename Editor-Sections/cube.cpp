#include "cube.h"

Cube::Cube()
{
    numFornnp = 0;//nameNextPoint
}

QString Cube::getNameNextPoint(QString name, QString **nnp){  //if name = "0" get name next point, else relese name point
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



GLuint Cube::createCube(QList<Point> *listPoint, QList<Linear> *listLine, QList<Flat> *listFlat, QString **nnp)
{
    listPoints.clear();
    listLines.clear();
    listFlats.clear();

    Point s;
    double color[3] = {0,0,0};
    s.setPoint(getNameNextPoint("0", nnp), 0, 1, 1, 1);
    help.drawPoint(&s, color);
    listPoints.append(s);

    s.setPoint(getNameNextPoint("0", nnp), 0, 1, 1, -1);
    help.drawPoint(&s, color);
    listPoints.append(s);

    s.setPoint(getNameNextPoint("0", nnp), 0, -1, 1, -1);
    help.drawPoint(&s, color);
    listPoints.append(s);

    s.setPoint(getNameNextPoint("0", nnp), 0, -1, 1, 1);
    help.drawPoint(&s, color);
    listPoints.append(s);

    s.setPoint(getNameNextPoint("0", nnp), 0, 1, -1, 1);
    help.drawPoint(&s, color);
    listPoints.append(s);

    s.setPoint(getNameNextPoint("0", nnp), 0, 1, -1, -1);
    help.drawPoint(&s, color);
    listPoints.append(s);

    s.setPoint(getNameNextPoint("0", nnp), 0, -1, -1, -1);
    help.drawPoint(&s, color);
    listPoints.append(s);

    s.setPoint(getNameNextPoint("0", nnp), 0, -1, -1, 1);
    help.drawPoint(&s, color);
    listPoints.append(s);
//-----------------------------------------------------
    Point p1, p2, pp1, pp2, c1, c2;
    Linear l;
    pp1 = listPoints[0];
    pp2 = listPoints[4];
    c1 = pp1;
    c2 = pp2;
    for(int i = 1; i < 4; i++){
        p1 = listPoints[i];
        p2 = listPoints[i+4];

        l.setLinear(""+c1.getName()+p1.getName(), c1, p1, 0);
        help.drawLine(&l);
        listLines.append(l);

        l.setLinear(""+c2.getName()+p2.getName(), c2, p2, 0);
        help.drawLine(&l);
        listLines.append(l);

        l.setLinear(""+p1.getName()+p2.getName(), p1, p2, 0);
        help.drawLine(&l);
        listLines.append(l);
        c1 = p1;
        c2 = p2;
    }

    l.setLinear(""+p1.getName()+pp1.getName(), p1, pp1, 0);
    help.drawLine(&l);
    listLines.append(l);

    l.setLinear(""+p2.getName()+pp2.getName(), p2, pp2, 0);
    help.drawLine(&l);
    listLines.append(l);

    l.setLinear(""+pp1.getName()+pp2.getName(), pp1, pp2, 0);
    help.drawLine(&l);
    listLines.append(l);
//------------------------------
    Flat f;
    QList<Point> p;
       //создание плоскостей
    p.clear();
    p.append(listPoints[0]);
    p.append(listPoints[3]);
    p.append(listPoints[2]);
    p.append(listPoints[1]);
    f.setFlat(p);
    listFlats.append(f);

    p.clear();
    p.append(listPoints[4]);
    p.append(listPoints[5]);
    p.append(listPoints[6]);
    p.append(listPoints[7]);
    f.setFlat(p);
    listFlats.append(f);

    p.clear();
    p.append(listPoints[0]);
    p.append(listPoints[1]);
    p.append(listPoints[5]);
    p.append(listPoints[4]);
    f.setFlat(p);
    listFlats.append(f);

    p.clear();
    p.append(listPoints[1]);
    p.append(listPoints[2]);
    p.append(listPoints[6]);
    p.append(listPoints[5]);
    f.setFlat(p);
    listFlats.append(f);

    p.clear();
    p.append(listPoints[2]);
    p.append(listPoints[3]);
    p.append(listPoints[7]);
    p.append(listPoints[6]);
    f.setFlat(p);
    listFlats.append(f);

    p.clear();
    p.append(listPoints[3]);
    p.append(listPoints[0]);
    p.append(listPoints[4]);
    p.append(listPoints[7]);
    f.setFlat(p);
    listFlats.append(f);

    number = help.drawFigure(listFlats);

    *listPoint = listPoints;
    *listLine = listLines;
    *listFlat = listFlats;
    return number;
}

GLuint Cube::rePaintCube()
{
    return 0;
}

QList<Point> Cube::getListPoint()
{
    return listPoints;
}

QList<Linear> Cube::getListLines()
{
    return listLines;
}
