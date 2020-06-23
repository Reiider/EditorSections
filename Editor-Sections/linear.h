#ifndef LINEAR_H
#define LINEAR_H

#include <GL/glu.h>
#include <QString>
#include <point.h>
#include <QList>
#include <math.h>
#include <QMessageBox>
#include "writelog.h"

class Linear //одна линия на группу точек
{
private:
    QString name;
    QList<Point> line;
    Point p1; //крайниe точки
    Point p2; //
    GLuint num;
    //double length_line;
    Point pFGPFL1; // точки на основе которых расчитывается уравнение прямой (первоначально добавленные точки)
    Point pFGPFL2;

    QString d_to_str(double d);

    //new
    QList<QList<Point> > listOtr; //список пар отрезков
    void createListOtr();
    void sortLine();
    bool isOtrExist(QString s1, QString s2);//существует ли такой отрезок


public:
    Linear();
    ~Linear();
    void deleteNum(); //освободить номер отрисовки
    void setLinear(QString name, Point point1, Point point2, GLuint num);
    void addPointToLine(Point p);
    QString getName();
    Point getP1();
    Point getP2();
    Point getPFGPFL1();
    Point getPFGPFL2();
    GLuint getNum();
    QList<Point> getLine();
    Point getPointForLine(double k); // получение точки на прямой из коэфициента
    Point getPointIntersection(Linear l); // получить точку пересечения
    bool isPointBeInTheLine(Point p); //лежит ли эта точка между самыми дальними точками прямой
    bool isPointInLine(Point p); //проверка, лежит ли точка на прямой
    bool isPointInListLine(QString nameP);//проверка есть ли точка с таким именем в списке точек
    void setNum(GLuint n); //устанавливает новый номер отрисовки и удаляет старый
    void setName(QString name); //задает имя прямой
    void onlySetNum(GLuint n); //устанавливает номер отрисовки
    void delPointWithLine(Point p);
    int getLineLength(); //получение кол-ва точек на прямой
    bool isOneLine(Linear l);//проверка совпадения прямых

    void renamePointInLine(QString from, QString to); //для переименования точек

    QString toString();

    friend QDataStream& operator <<( QDataStream &stream, const Linear &l );
    friend QDataStream& operator >>( QDataStream &stream, Linear &l );

    //new
    QList<QList<Point> > getListOtr(); //возвращает список отрезков, которые необходимо отрисовать
    void deleteOtr(QString p1, QString p2); //удаление отображения части прямой
    void appendListOtr(QList<QList<Point> > list); // добавляет новые отрезки к текущему списку
    void addPointsToLine(QList<Point> list);//добавляет на прямую список точек без добавления связей между ними
    void addOtr(Point p1, Point p2);
    void setPFGPFL(Point p1, Point p2);
    double getLenghtLine(); // возвращает длину прямой (от p1 до p2)
};

#endif // LINEAR_H
