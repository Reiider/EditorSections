#ifndef POITER_H
#define POITER_H

#include <GL/glu.h>
#include <QString>
#include <math.h>

class Point
{
private:
    QString name;
    GLuint num;
    GLUquadricObj *quadObj;
    double x;
    double y;
    double z;

    double r,g,b;
    bool standartColor;

    QString d_to_str(double d);

public:
    static double rr, gg, bb;
    Point();

    void setPoint(QString name, GLuint num, double x, double y, double z);
    QString getName() const;
    void setName(QString name);
    GLuint getNum();
    void setNum(GLuint n);
    double getX() const;
    double getY() const;
    double getZ() const;
    bool isEqual(Point p); //проверка на совпадение точек
    void deleteNum();
    QString toString();
    void setColor(double r, double g, double b);
    double* getColor();
    void setStandartColor();
    ~Point();

    friend QDataStream& operator <<( QDataStream &stream, const Point &p );
    friend QDataStream& operator >>( QDataStream &stream, Point &p );
};



#endif // POITER_H
