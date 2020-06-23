#include "helpfunction.h"

double *HelpFunction::colorForLine = new double[3];

HelpFunction::HelpFunction()
{
}

bool HelpFunction::isLeftVectorThree(Point p1, Point p2, Point p3, Point v3)
{
    Point v1, v2;
    v1.setPoint("", 0, p2.getX()-p1.getX(), p2.getY() - p1.getY(), p2.getZ() - p1.getZ());
    v2.setPoint("", 0, p3.getX()-p1.getX(), p3.getY() - p1.getY(), p3.getZ() - p1.getZ());
    double vecMultyply = v1.getX()*(v2.getY()*v3.getZ() - v2.getZ()*v3.getY())
                       - v1.getY()*(v2.getX()*v3.getZ() - v2.getZ()*v3.getX())
                       + v1.getZ()*(v2.getX()*v3.getY() - v2.getY()*v3.getZ());
    if(vecMultyply >= 0) return false;
    else return true;
}

bool HelpFunction::isPointsInTriangle(QList<Point> list, int p1, int p2, int p3)
{
    double areaTr = rounding(areaOfTriangle(list[p1], list[p2], list[p3]));
    for(int i = 0; i < list.length(); i++){
        if(i!=p1 && i!=p2 && i!=p3){
            if(areaTr == rounding(areaOfTriangle(list[p1], list[p2], list[i])
                                           +areaOfTriangle(list[p2], list[p3], list[i])
                                           +areaOfTriangle(list[p3], list[p1], list[i]))){
                return true;
            }
        }
    }
    return false;
}

double HelpFunction::rounding(double d)
{
    int n = (int) (d*100000.);
    return (double) (n/100000.);
}

double HelpFunction::areaOfTriangle(Point p1, Point p2, Point p3)
{
    //находим вектора
    Point v1, v2, v3;
    v1.setPoint("v1", 0, p2.getX() - p1.getX(), p2.getY() - p1.getY(), p2.getZ() - p1.getZ());
    v2.setPoint("v2", 0, p3.getX() - p1.getX(), p3.getY() - p1.getY(), p3.getZ() - p1.getZ());
    //считаем площадь на основе векторов
    v3.setPoint("v3", 0, v1.getY()*v2.getZ() - v1.getZ()*v2.getY(),
                     -(v1.getX()*v2.getZ() - v1.getZ()*v2.getX()),
                       v1.getX()*v2.getY() - v1.getY()*v2.getX());
    return 0.5 * middleGeom3(v3);
}

double HelpFunction::lenghtLine(Point p1, Point p2)
{
    return sqrt((p1.getX() - p2.getX())*(p1.getX() - p2.getX()) +
                (p1.getY() - p2.getY())*(p1.getY() - p2.getY()) +
                (p1.getZ() - p2.getZ())*(p1.getZ() - p2.getZ()));
}

void HelpFunction::drawPoint(Point *p, double color[])
{
    GLuint n;
    GLUquadricObj *quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_FILL);

    n = glGenLists(1);
    glNewList(n, GL_COMPILE);
        glColor3f(color[0], color[1], color[2]);
        glTranslated(p->getX(), p->getY(), p->getZ());
        gluSphere(quadObj, 0.02, 7, 7);
        glTranslated(-p->getX(), -p->getY(), -p->getZ());
    glEndList();

    gluDeleteQuadric(quadObj);
    p->setNum(n);
}

void HelpFunction::drawLine(Linear *l)
{
    QList<QList<Point> > listOtr = l->getListOtr();
    GLuint n;
    GLUquadricObj *quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    n = glGenLists(1);
    glNewList(n, GL_COMPILE);
    for(int i = 0; i < listOtr.length(); i++){
        Point p1 = listOtr.at(i).at(0);
        Point p2 = listOtr.at(i).at(1);
        double width = lenghtLine(p1, p2);
        double degree, a, b, c;

        a = p2.getX()-p1.getX();
        b = p2.getY()-p1.getY();
        c = p2.getZ()-p1.getZ();

        glPushMatrix();
        glColor3f(HelpFunction::colorForLine[0], HelpFunction::colorForLine[1], HelpFunction::colorForLine[2]);
        glTranslated(p1.getX(), p1.getY(), p1.getZ());
        degree = acos( c / width)*180/3.14159265358979;
        glRotatef(degree, -b, a, 0);
        gluCylinder(quadObj, 0.01, 0.01, width, 6, 1);
        glPopMatrix();
    }
    glEndList();
    gluDeleteQuadric(quadObj);
    l->onlySetNum(n);
}

void HelpFunction::drawLineForColorFlat(Linear l, double rc, double gc, double bc)
{
    QList<QList<Point> > listOtr = l.getListOtr();
    GLUquadricObj *quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    for(int i = 0; i < listOtr.length(); i++){
        Point p1 = listOtr.at(i).at(0);
        Point p2 = listOtr.at(i).at(1);
        double width = lenghtLine(p1, p2);
        double degree, a, b, c;

        a = p2.getX()-p1.getX();
        b = p2.getY()-p1.getY();
        c = p2.getZ()-p1.getZ();

        glPushMatrix();
        glColor3f(rc, gc, bc);
        glTranslated(p1.getX(), p1.getY(), p1.getZ());
        degree = acos( c / width)*180/3.14159265358979;
        glRotatef(degree, -b, a, 0);
        gluCylinder(quadObj, 0.01, 0.01, width, 6, 1);
        glPopMatrix();
    }
    gluDeleteQuadric(quadObj);
}

GLuint HelpFunction::drawFigure(QList<Flat> listFlats, QList<ColorFlat> lColor)
{
    GLuint n;
    n = glGenLists(1);
    glNewList(n, GL_COMPILE);
    glColor4d(1, 1, 1, 0.66);

WriteLog::write("   1");
    QList<Point> list; //list
    for(int i = 0; i < listFlats.length(); i++){
        if(lColor.length() != 0){
            for(int lc = 0; lc < lColor.length(); lc++){
                if(i == lColor[lc].index){
                    glColor4d(lColor[lc].r, lColor[lc].g, lColor[lc].b, 0.80);
                    break;
                }
            }
        }
        list = listFlats[i].getListPoint();
        Point v3;
        v3.setPoint("", 0, -listFlats[i].getA(), -listFlats[i].getB(), -listFlats[i].getC());
WriteLog::write("   2");
       /* Алгоритм триангуляции:
        1. Берем три вершины A1, A2, A3
        2. Проверяем образуют ли вектора A1A3, A1A2 и их векторное произведение левую тройку векторов.
        3. Проверяем нет ли внутри треугольника A1A2A3 какой-либо из оставшихся вершин многоугольника.
        4. Если оба условия выполняются, то строим треугольник A1A2A3, а вершину A2 исключаем из многоугольника, не трогая вершину A1, сдвигаем вершины A2 (A2 на A3), A3 (A3 на A4)
        5. Если хоть одно условие не выполняется, переходим к следующим трем вершинам.
        6. Повторяем с 1 шага, пока не останется три вершины.*/
        bool f1 = false;
        bool isLeft = true;
        while(list.length() >= 3){
            for(int t = 0; t < 6 && list.length() >= 3; t++){
                for(int j = 0; j+2 < list.length(); j++){
                    if(f1) isLeft = !isLeftVectorThree(list[j], list[j+1], list[j+2], v3);
                    else isLeft = isLeftVectorThree(list[j], list[j+1], list[j+2], v3);
                    if(isLeft){
                        if(!isPointsInTriangle(list, j, j+1, j+2)){
                            glBegin(GL_TRIANGLES);
                                glVertex3f(list[j].getX(), list[j].getY(), list[j].getZ());
                                glVertex3f(list[j+1].getX(), list[j+1].getY(), list[j+1].getZ());
                                glVertex3f(list[j+2].getX(), list[j+2].getY(), list[j+2].getZ());
                            glEnd();
                            list.removeAt(j+1);
                            j--;
                        }
                    }
                }
            }
            f1 = !f1;
        }
WriteLog::write("   3");
        if(lColor.length() != 0){
            for(int lc = 0; lc < lColor.length(); lc++){
                if(i == lColor[lc].index){
                    //рисуем прямые вокруг плоскости
                    QList<Point> listPFL = listFlats[i].getListPoint();
                    Linear lPFL;
                    for(int j = 0; j+1 < listPFL.length(); j++){
                        lPFL.setLinear("", listPFL[j], listPFL[j+1], 0);
                        drawLineForColorFlat(lPFL, lColor[lc].r, lColor[lc].g, lColor[lc].b);
                    }
                    lPFL.setLinear("", listPFL[0], listPFL[listPFL.length()-1], 0);
                    drawLineForColorFlat(lPFL, lColor[lc].r, lColor[lc].g, lColor[lc].b);
                    //
                    lColor.removeAt(lc);
                    glColor4d(1, 1, 1, 0.66);
                    break;
                }
            }
        }
WriteLog::write("   4");
    }
    glEndList();
    return n;
}

GLuint HelpFunction::drawFigure(QList<Flat> listFlats)
{
    GLuint n;
    n = glGenLists(1);
    glNewList(n, GL_COMPILE);
    glColor4d(1, 1, 1, 0.66);

    QList<Point> list; //list
    for(int i = 0; i < listFlats.length(); i++){
        list = listFlats[i].getListPoint();
        Point v3;
        v3.setPoint("", 0, -listFlats[i].getA(), -listFlats[i].getB(), -listFlats[i].getC());

       /* Алгоритм триангуляции:
        1. Берем три вершины A1, A2, A3
        2. Проверяем образуют ли вектора A1A3, A1A2 и их векторное произведение левую тройку векторов.
        3. Проверяем нет ли внутри треугольника A1A2A3 какой-либо из оставшихся вершин многоугольника.
        4. Если оба условия выполняются, то строим треугольник A1A2A3, а вершину A2 исключаем из многоугольника, не трогая вершину A1, сдвигаем вершины A2 (A2 на A3), A3 (A3 на A4)
        5. Если хоть одно условие не выполняется, переходим к следующим трем вершинам.
        6. Повторяем с 1 шага, пока не останется три вершины.*/
        bool f1 = false;
        bool isLeft = true;
        while(list.length() >= 3){
            for(int t = 0; t < 6 && list.length() >= 3; t++){
                for(int j = 0; j+2 < list.length(); j++){
                    if(f1) isLeft = !isLeftVectorThree(list[j], list[j+1], list[j+2], v3);
                    else isLeft = isLeftVectorThree(list[j], list[j+1], list[j+2], v3);
                    if(isLeft){
                        if(!isPointsInTriangle(list, j, j+1, j+2)){
                            glBegin(GL_TRIANGLES);
                                glVertex3f(list[j].getX(), list[j].getY(), list[j].getZ());
                                glVertex3f(list[j+1].getX(), list[j+1].getY(), list[j+1].getZ());
                                glVertex3f(list[j+2].getX(), list[j+2].getY(), list[j+2].getZ());
                            glEnd();
                            list.removeAt(j+1);
                            j--;
                        }
                    }
                }
            }
            f1 = !f1;
        }
    }
    glEndList();
    return n;
}

double HelpFunction::abs(double num)
{
    if(num >= 0) return num;
    else return (-1)*num;
}

double HelpFunction::middleGeom3(double x, double y, double z)
{
    return sqrt(x*x + y*y + z*z);
}

double HelpFunction::middleGeom3(Point p)
{
    double d = p.getX()*p.getX() + p.getY()*p.getY() + p.getZ()*p.getZ();
    try{
        if(d != 0) return sqrt(d);
        else return 0;
    }
    catch(...){
        return 0;
    }
}

void HelpFunction::setColorForLine(double r, double g, double b)
{
    HelpFunction::colorForLine[0] = r;
    HelpFunction::colorForLine[1] = g;
    HelpFunction::colorForLine[2] = b;
}

double *HelpFunction::getColorForLine()
{
    double* color = new double[3];
    color[0] = HelpFunction::colorForLine[0];
    color[1] = HelpFunction::colorForLine[1];
    color[2] = HelpFunction::colorForLine[2];
    return color;
}

QString HelpFunction::one_of_line(QString str, int n)
{
    str.remove(0, n);
    str.remove(1, str.length()-1);
    return str;
}

QList<Point> HelpFunction::sotrPointConvexPolygon(QList<Point> pl)
{
    QList<Point> finaly;
    QList<Linear> ll;
    QList<Linear> listl;
    Linear l;
    Point p;
    for(int i = 1; i < pl.length(); i++){
        l.setLinear("", pl[0], pl[i], 0);
        ll.append(l);
    }
    bool flag = true;

    for(int i = 1; i < pl.length(); i++){
        for(int j = 1; j < pl.length(); j++){
            if(i != j){
                flag = true;
                l.setLinear("", pl[i], pl[j], 0);
                for(int k = 0; k < ll.length(); k++){
                    p = l.getPointIntersection(ll[k]);
                    if(p.getName() == "1"){
                        if(p.isEqual(pl[i]));
                        else if(p.isEqual(pl[j]));
                            else if(l.isPointBeInTheLine(p)) flag = false;
                    }
                }
                if(flag){
                    listl.append(l);
                }
            }
        }
    }
    int in = 0;
    Linear s;
    for(int i = 0; i < listl.length(); i++){
        in = 0;
        for(int j = 0; j < listl.length(); j++){
            if(listl[i].getP1().getName() == listl[j].getP1().getName()) in++;
        }
        if(in == 1){
            s = listl[i];
            break;
        }
    }

    finaly.append(pl[0]);
    finaly.append(s.getP1());

    while(listl.length()!=0){
        finaly.append(s.getP2());
        for(int i = 0; i < listl.length(); i++){
            if(listl[i].getP2().getName() == s.getP1().getName() && listl[i].getP1().getName() == s.getP2().getName()){
                listl.removeAt(i);
                break;
            }
        }
        for(int j = 0; j < listl.length(); j++){
            if(s.getP1().getName() == listl[j].getP1().getName() && s.getP2().getName() == listl[j].getP2().getName()){
                listl.removeAt(j);
                j--;
            }
        }
        for(int i = 0; i < listl.length(); i++){
            if(listl[i].getP1().getName() == s.getP2().getName()){
                s = listl[i];
                break;
            }
        }
    }
    return finaly;
}

QList<Point> HelpFunction::sotrPointInFigure(QList<Point> pl, QList<Flat> fl)
{
    if(pl.length() <= 3) return pl;

    QList<Point> finish;
    QList<Linear> lines;
    for(int i = 0; i < pl.length(); i++){
        for(int j = i+1; j < pl.length(); j++){
            Linear l;
            l.setLinear("", pl[i], pl[j], 0);
            for(int k = 0; k < fl.length(); k++){
                if(fl[k].isLineInThisPl(l)){
                    lines.append(l);
                    break; //что бы не было дубликатов
                }
            }
        }
    }

    //начинаем с плоскости, в которой только 1 отрезок
    QList<int> num;
    for(int i = 0; i < fl.length(); i++){
        num.clear();
        for(int j = 0; j < lines.length(); j++){
            if(fl[i].isLineInThisPl(lines[j])) num.append(j);
        }
        if(num.length() == 1){
            break;
        }
    }
    for(int var = 0; var < num[0]; var++){
        Linear li;
        li = lines.first();
        lines.removeFirst();
        lines.append(li);
    }
    //

    finish.append(lines[0].getP1());
    finish.append(lines[0].getP2());
    lines.removeAt(0);
    int l = pl.length()*pl.length();

    while(lines.length() > 1 && l != 0){
        //считаем количество возможных отрезков связанных с данной точкой, и помечаем сами отрезки
        QList<int> segWithPoint, segCopy;
        for(int j = 0; j < lines.length(); j++){
            if(finish.last().isEqual(lines[j].getP1()) || finish.last().isEqual(lines[j].getP2()))
                segWithPoint.append(j);
        }
        for(int j = 0; j < segWithPoint.length(); j++){
            segCopy.append(segWithPoint[j]);
        }

        //если они из разных плоскостей, то оставляем те, которые отличаются плоскостью от предыдущих
        //двух добавленных точек, а остальные удаляем
        Linear lLast, lCur;
        lLast.setLinear("", finish[finish.length()-2], finish[finish.length()-1], 0);
        for(int j = segWithPoint.length()-1; j >= 0; j--){
            lCur = lines[segWithPoint[j]];
            if(lLast.isOneLine(lCur)) {
                segWithPoint.removeAt(j);
            }
        }
        if(segWithPoint.length() == 0) segWithPoint.append(segCopy.first()); //так, навсякий

 //выбираем самый короткий
        double min = lines[segWithPoint[0]].getLenghtLine();
        int minI = segWithPoint[0];
        for(int j = 1; j < segWithPoint.length(); j++){
            if(lines[segWithPoint[j]].getLenghtLine() < min){
                min = lines[segWithPoint[j]].getLenghtLine();
                minI = segWithPoint[j];
            }
        }
        //самый которткий добавляем, остальные (все) удаляем
        if(finish.last().isEqual(lines[minI].getP1())) {
            finish.append(lines[minI].getP2());
        }
        else {
            finish.append(lines[minI].getP1());
        }

        for(int j = segCopy.length() - 1; j >= 0; j--){
            lines.removeAt(segCopy[j]);
        }
        l--;
    }

    if(finish.first().getName() == finish.last().getName()) finish.removeLast(); //костыль на случай дублирования первой точки в конце
    if(finish.length() < 3 || l == 0) {
        Point p;
        p.setName("error");
        finish.insert(0, p);
        return finish;
    }
    //подбираем, что бы первые три точки образовавали правую тройку
    for(int j = finish.length() - 1; j >= 0; j--){
        QList<Point> listP;
        listP.append(finish[0]);
        listP.append(finish[(finish.length()-1)/2]);
        listP.append(finish[finish.length()-1]);
        Flat f;
        f.setFlat(listP);
        Point point;
        point.setPoint("", 0, f.getA(), f.getB(), f.getC());
        if(isLeftVectorThree(finish[0], finish[1], finish[2], point)){
            point = finish.first();
            finish.removeFirst();
            finish.append(point);
        }
        else break;
    }
    return finish;
}

