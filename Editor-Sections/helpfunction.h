#ifndef HELPFUNCTION_H
#define HELPFUNCTION_H

#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glaux.h>
#include <QList>
#include "point.h"
#include "linear.h"
#include "flat.h"
#include "string.h"
#include <string>
#include <QMessageBox>
#include <QInputDialog>
#include <math.h>
#include "colorflat.h"

class HelpFunction
{

private:
    static double* colorForLine;
    void drawLineForColorFlat(Linear l, double rc, double gc, double bc);

public:
    HelpFunction();

    bool isLeftVectorThree(Point p1, Point p2, Point p3, Point v3);
    bool isPointsInTriangle(QList<Point> list, int p1, int p2, int p3);//есть ли одна из точек списка в треугольнике

    double rounding(double d);//округление числа
    double areaOfTriangle(Point p1, Point p2, Point p3); //площадь треугольника по трем точкам
    void drawPoint(Point *p, double color[3]);  //рисует точку
    void drawLine(Linear *l); //рисует прямую
    GLuint drawFigure(QList<Flat> listFlats, QList<ColorFlat> lColor); //рисует прозрачные грани фигуры выделяя указанные цветом
    GLuint drawFigure(QList<Flat> listFlats); //рисует прозрачные грани фигуры
    double abs(double num); //модуль числа
    double middleGeom3(double x, double y, double z); //среднее геометрическое 3х чисел (длина вектора в пространстве)
    double middleGeom3(Point p);
    void setColorForLine(double r, double g, double b);
    double* getColorForLine();
    QString one_of_line(QString str, int n);
    double lenghtLine(Point p1, Point p2);

    template <typename T> QString itoqs(T n){ //number to QString
        QString temp;
        if(n > -0.0001 && n < 0.0001 ) temp = "0";
        else temp = temp.number(n);
        return temp;
    }

    QList<Point> sotrPointConvexPolygon(QList<Point> pl); //сортировка точек плоскости выпуклого многоугольника
    QList<Point> sotrPointInFigure(QList<Point> pl, QList<Flat> fl); //сортировка точек для правильного отображения
};

#endif // HELPFUNCTION_H
