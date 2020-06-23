#ifndef FLAT_H
#define FLAT_H
#include <QList>
#include <point.h>
#include <linear.h>
#include <math.h>

#include <QMessageBox>

class Flat
{
    QList<Point> plos;
    //A*x + B*y + C*z + D = 0
    QString name;
    double A;
    double B;
    double C;
    double D;


public:
    double getA();
    double getB();
    double getC();
    double getD();
    Flat();
    void setFlat(QList<Point> p);
    QString getName();
    void addPointToFlat(Point point); //добавляет или заменяет точку на плоскости.
    QList<Point> getListPoint();
    void removeFromListPoints(QString s); // удалить точку из списка
    Point getPointIntersection(Linear l); //получить точку пересечения
    bool isPointBeInTheArea(Point p); //точка лежит в видимой области плоскости или нет
    bool isLineInThisPl(Linear l);//совпадение прямой и плоскости
    bool isPointInTheArea(Point p); //принадлежит ли точка плоскости (даже вне видимоcти (границ образующихся прямыми) плоскости)
    void renamePointInFlat(QString from, QString to); //для переименования точки

    friend QDataStream& operator <<( QDataStream &stream, const Flat &f );
    friend QDataStream& operator >>( QDataStream &stream, Flat &f );
};

#endif // FLAT_H
