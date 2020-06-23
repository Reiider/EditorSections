#include "point.h"

QString Point::d_to_str(double d)
{
    QString temp;
    if(d > -0.0001 && d < 0.0001 ) temp = "0";
    else temp = temp.number(d);
    return temp;
}

Point::Point(){
    standartColor = true;
    r = rr;
    g = gg;
    b = bb;
    /*x = 0;
    y = 0;
    z = 0;
    num = 0;*/
}

Point::~Point()
{

}

void Point::setPoint(QString name, GLuint num, double x, double y, double z)
{
    this->name = name;
    this->num = num;
    this->x = x;
    this->y = y;
    this->z = z;
}

QString Point::getName() const{
    return name;
}

void Point::setName(QString name)
{
    this->name = name;
}

GLuint Point::getNum(){
    return num;
}

void Point::setNum(GLuint n)
{
    num = n;
}

double Point::getX() const{
    return x;
}

double Point::getY() const{
    return y;
}

double Point::getZ() const{
    return z;
}

bool Point::isEqual(Point p)
{
    if((this->getX() + 0.0001 > p.getX()) &&
       (this->getX() - 0.0001 < p.getX()) &&
       (this->getY() + 0.0001 > p.getY()) &&
       (this->getY() - 0.0001 < p.getY()) &&
       (this->getZ() + 0.0001 > p.getZ()) &&
       (this->getZ() - 0.0001 < p.getZ()) ) return true;
    else return false;
}

void Point::deleteNum()
{
    glDeleteLists(num, 1);
}

QString Point::toString()
{
    return "" + name+ ": (" + d_to_str(x) + ", " + d_to_str(y)+ ", " + d_to_str(z) + ") \n";
}

void Point::setColor(double r, double g, double b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    standartColor = false;
}

double *Point::getColor()
{
    double *color = new double[3];
    color[0] = r;
    color[1] = g;
    color[2] = b;
    return color;
}

void Point::setStandartColor()
{
    if(standartColor){
        r = rr;
        g = gg;
        b = bb;
    }
}
