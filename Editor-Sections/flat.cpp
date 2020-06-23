#include "flat.h"

double Flat::getA()
{
    return A;
}

double Flat::getB()
{
    return B;
}

double Flat::getC()
{
    return C;
}

double Flat::getD()
{
    return D;
}

Flat::Flat()
{
    A = 0;
    B = 0;
    C = 0;
    D = 10;
}

void Flat::setFlat(QList<Point> p)
{
    plos = p;
    Point p1,p2,p3;
    p1 = p[0];
    p2 = p[1];
    p3 = p[2];
    //if(!isPointInTheArea(p1) || !isPointInTheArea(p2) || !isPointInTheArea(p3)){
        A = ((p2.getY()-p1.getY())*(p3.getZ()-p1.getZ()) - (p3.getY()-p1.getY())*(p2.getZ()-p1.getZ()));
        B = ((p3.getX()-p1.getX())*(p2.getZ()-p1.getZ()) - (p2.getX()-p1.getX())*(p3.getZ()-p1.getZ()));
        C = ((p2.getX()-p1.getX())*(p3.getY()-p1.getY()) - (p3.getX()-p1.getX())*(p2.getY()-p1.getY()));
        D = p1.getX()*( (p3.getY()-p1.getY())*(p2.getZ()-p1.getZ()) - (p2.getY()-p1.getY())*(p3.getZ()-p1.getZ()) ) +
            p1.getY()*( (p2.getX()-p1.getX())*(p3.getZ()-p1.getZ()) - (p3.getX()-p1.getX())*(p2.getZ()-p1.getZ()) ) +
            p1.getZ()*( (p3.getX()-p1.getX())*(p2.getY()-p1.getY()) - (p2.getX()-p1.getX())*(p3.getY()-p1.getY()) );
        name = p1.getName() + p2.getName() + p3.getName();
    //}
}

QString Flat::getName()
{
    return name;
}

void Flat::addPointToFlat(Point point)
{
    bool flag = true;
    for(int i = 0; i < plos.length(); i++){
        if(plos[i].getName() == point.getName()){
            flag = false;
            plos[i] = point;
        }
    }
    if(flag) plos.append(point);
    setFlat(plos);
}

QList<Point> Flat::getListPoint()
{
    return plos;
}

void Flat::removeFromListPoints(QString s)
{
    for(int i = 0; i < plos.length(); i++){
        if(s == plos[i].getName()){
            plos.removeAt(i);
            break;
        }
    }
}

Point Flat::getPointIntersection(Linear l)
{
    Point p;
    double k = A*(l.getP2().getX() - l.getP1().getX()) + B*(l.getP2().getY() - l.getP1().getY()) + C*(l.getP2().getZ() - l.getP1().getZ());
    double a = A*l.getP2().getX() + B*l.getP2().getY() + C*l.getP2().getZ() + D;
    if(k < 0.0001 && k > -0.0001 && a < 0.0001 && a > -0.0001) {
        p.setPoint("Прямая и плоскость совподают!", 0, 0, 0, 0);
        return p;
    }
    if(k < 0.0001 && k > -0.0001 && (a > 0.0001 || a < -0.0001)) {
        p.setPoint("Прямая и плоскость непересекаются!", 0, 0, 0, 0);
        return p;
    }
    if(k > 0.0001 || k < -0.0001){
        double x,y,z,v;
        v = (-a)/k;
        x = l.getP2().getX() + (l.getP2().getX() - l.getP1().getX())*v;
        y = l.getP2().getY() + (l.getP2().getY() - l.getP1().getY())*v;
        z = l.getP2().getZ() + (l.getP2().getZ() - l.getP1().getZ())*v;
        p.setPoint("1",0, x, y, z);
        return p;
    }
    p.setPoint("Ошибка поиска точки пересечения прямой и плоскости!", 0, 0, 0, 0);
    return p;
}

bool Flat::isPointBeInTheArea(Point p)
{
    double areaWithoutPoint = 0;
    double areaWithPoint = 0;

    double areaActualTriangle;
    double pper, a, b, c;
    double temp = 0;

    for(int i = 1; i < plos.length() - 1; i++){
        a = sqrt((plos[0].getX()-plos[i].getX())*(plos[0].getX()-plos[i].getX()) +
                (plos[0].getY()-plos[i].getY())*(plos[0].getY()-plos[i].getY()) +
                (plos[0].getZ()-plos[i].getZ())*(plos[0].getZ()-plos[i].getZ()));
        b = sqrt((plos[i].getX()-plos[i+1].getX())*(plos[i].getX()-plos[i+1].getX()) +
                (plos[i].getY()-plos[i+1].getY())*(plos[i].getY()-plos[i+1].getY()) +
                (plos[i].getZ()-plos[i+1].getZ())*(plos[i].getZ()-plos[i+1].getZ()));
        c = sqrt((plos[0].getX()-plos[i+1].getX())*(plos[0].getX()-plos[i+1].getX()) +
                (plos[0].getY()-plos[i+1].getY())*(plos[0].getY()-plos[i+1].getY()) +
                (plos[0].getZ()-plos[i+1].getZ())*(plos[0].getZ()-plos[i+1].getZ()));
        pper = (a+b+c)/2;
        temp = pper*(pper-a)*(pper-b)*(pper-c);
        if(temp <= 0){
            areaActualTriangle = 0;
        }
        else areaActualTriangle = sqrt(temp);
        areaWithoutPoint += areaActualTriangle;
    }

    for(int i = 0; i < plos.length() - 1; i++){
        a = sqrt((p.getX()-plos[i].getX())*(p.getX()-plos[i].getX()) +
                (p.getY()-plos[i].getY())*(p.getY()-plos[i].getY()) +
                (p.getZ()-plos[i].getZ())*(p.getZ()-plos[i].getZ()));
        b = sqrt((plos[i].getX()-plos[i+1].getX())*(plos[i].getX()-plos[i+1].getX()) +
                (plos[i].getY()-plos[i+1].getY())*(plos[i].getY()-plos[i+1].getY()) +
                (plos[i].getZ()-plos[i+1].getZ())*(plos[i].getZ()-plos[i+1].getZ()));
        c = sqrt((p.getX()-plos[i+1].getX())*(p.getX()-plos[i+1].getX()) +
                (p.getY()-plos[i+1].getY())*(p.getY()-plos[i+1].getY()) +
                (p.getZ()-plos[i+1].getZ())*(p.getZ()-plos[i+1].getZ()));
        pper = (a+b+c)/2;
        temp = pper*(pper-a)*(pper-b)*(pper-c);
        if(temp <= 0){
            areaActualTriangle = 0;
        }
        else areaActualTriangle = sqrt(temp);
        areaWithPoint += areaActualTriangle;
    }
        a = sqrt((p.getX()-plos[0].getX())*(p.getX()-plos[0].getX()) +
                (p.getY()-plos[0].getY())*(p.getY()-plos[0].getY()) +
                (p.getZ()-plos[0].getZ())*(p.getZ()-plos[0].getZ()));
        b = sqrt((plos[0].getX()-plos[plos.length() - 1].getX())*(plos[0].getX()-plos[plos.length() - 1].getX()) +
                (plos[0].getY()-plos[plos.length() - 1].getY())*(plos[0].getY()-plos[plos.length() - 1].getY()) +
                (plos[0].getZ()-plos[plos.length() - 1].getZ())*(plos[0].getZ()-plos[plos.length() - 1].getZ()));
        c = sqrt((p.getX()-plos[plos.length() - 1].getX())*(p.getX()-plos[plos.length() - 1].getX()) +
                (p.getY()-plos[plos.length() - 1].getY())*(p.getY()-plos[plos.length() - 1].getY()) +
                (p.getZ()-plos[plos.length() - 1].getZ())*(p.getZ()-plos[plos.length() - 1].getZ()));
        pper = (a+b+c)/2;
        temp = pper*(pper-a)*(pper-b)*(pper-c);
        if(temp <= 0){
            areaActualTriangle = 0;
        }
        else areaActualTriangle = sqrt(temp);
        areaWithPoint += areaActualTriangle;
    if(areaWithoutPoint + 0.0001 > areaWithPoint) return true;
    else return false;
}

bool Flat::isLineInThisPl(Linear l)
{
    Point p1, p2;
    p1 = l.getP1();
    p2 = l.getP2();
    double s1 = A*p1.getX() + B*p1.getY() + C*p1.getZ() + D;
    double s2 = A*p2.getX() + B*p2.getY() + C*p2.getZ() + D;
    if(s1 + 0.0001 > 0 && s1 - 0.0001 < 0 &&
       s2 + 0.0001 > 0 && s2 - 0.0001 < 0){
        return true;
    }
    else return false;
}

bool Flat::isPointInTheArea(Point p)
{
    if((A*p.getX()+B*p.getY()+C*p.getZ()+D + 0.0001 > 0) &&
      ((A*p.getX()+B*p.getY()+C*p.getZ()+D - 0.0001 < 0)))
        return true;
    else return false;
}

void Flat::renamePointInFlat(QString from, QString to)
{
    for(int i = 0; i < plos.length(); i++){
        if(plos[i].getName() == from) plos[i].setName(to);
    }
    name = plos[0].getName() + plos[1].getName() + plos[2].getName();
}
