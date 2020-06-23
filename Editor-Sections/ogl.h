#ifndef OGLPYRAMID_H
#define OGLPYRAMID_H

#include "helpfunction.h"
#include <QGLWidget>
#include <QtOpenGL>
#include <QtOpenGL/QGLBuffer>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glaux.h>
#include <QList>
#include <point.h>
#include "linear.h"
#include "flat.h"
#include "pyramid.h"
#include "cube.h"
#include "string.h"
#include <string>
#include "prism.h"
#include <QMessageBox>
#include <QInputDialog>
#include "arraystep.h"
#include "digestogl.h"
#include "paintlogo.h"
#include "writelog.h"
#include "colorflat.h"
#include "graphicanswer.h"
#include <vector>

#include <fstream>
#include <QtMath>

class OGL : public QGLWidget {
private:


    GLfloat m_xRotate;
    GLfloat m_yRotate;
    QPoint m_ptPosition;
    GLfloat f;
    int sizeFont;

    static const double grad = 0.017453292519943;
    GLuint figure;
    QList<Point> listPoints;
    QList<Linear> listLines;
    QList<Flat> listFlats;

    GraphicAnswer graphicAnswer;


    Point findPoint(QString s); //поиск точки. В случае отсутствия getName() == "error"
    Linear findLinear(QString p1, QString p2);  //поиск прямой. В случае отсутствия getName() == "error"
    QString isPointExist(Point p); // если такая точка уже есть получить ее имя, иначе == "false"
    QList<QString> splitStringPoints(QString points); //разбивает строку с точками через пробел в массив

    bool redactTask;
    QList<Point> paintAnswer; //отображение точек ответа

    int colVershin;//для пирамиды указание кол-ва вершин в основании (без S)
    int colLine; // количество первых прямых, недоступных для редактирования
    int colFirstLine; //кол-во первых прямых,(для редактирования задач)

    QString **nnp; //имена задействованых точек
    QColor colorNnp; //цвет текста точек

    int lastMoveLine;
    QString lastPointSootnoshenie[2]; //последняя прямая для установления отсчета при построении точки по соотношению

    //for task
    QString PointName;
    QString PointX;
    QString PointY;
    QString PointZ;
    QString LineName;
    QString LineList;
    QString LinePFL1;
    QString LinePFL2;
    QString TaskType;
    QString FlatList;
    QString Answer;
    QList<Point> listAnswer;

    //for work with steps
    ArrayStep steps;

    HelpFunction help;

    QString movePointName;

    QString report;

    GLdouble forLookAt[5];
    bool rButtonPress;
    bool lButtonPress;


protected:

    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();
    void mousePressEvent(QMouseEvent* pe);
    void mouseMoveEvent(QMouseEvent* pe);
    void wheelEvent(QWheelEvent* ev);
    void mouseReleaseEvent(QMouseEvent* pe);

    QString getNameNextPoint(QString name);

    void outText(double x, double y, double z, QString s);
    Point isPointInTheListPoint(Point p); //эта точка есть в списке точек (по координатам) если нет то создает, добавляет и отрисовывает
    QString d_to_str(double d);
    QString one_of_line(QString str, int n);

    QString points(QString point, QString isFlat);
    QString intersecPlanWhithLine(QString line, QString plane);
    QString intersecPlanWhithPlane(QString plane1, QString plane2);
    QString cuttingPlane(QString point);

public:

    OGL (QWidget* pwgt=0);

    void changeSizeFigure(int sizeF);

    QString addPoint(QString s1, QString s2);
    QString renamePoint(QString from, QString to);
    void delPoint(QString s);
    void moveLastPoint(int v, QDoubleSpinBox *s1, QDoubleSpinBox *s2, QComboBox *box);
    QString setSootnoshenie(QString otr, double s1, double s2); //установить последнюю точку по заданному соотношению.
    QString getNameLastPoint();

    QString addLine(QString sp1, QString sp2);
    QString delLine(Point p1, Point p2);
    QString addParallelLine(QString p, QString l1, QString l2); //добавление прямой через точку параллельно данной прямой
    QString createPerpendicularLine(QString point, QString PtoLine1, QString PtoLine2);//провести перпендикуляр через точку к прямой

    QString addPointIntersection_LineWithLine(QString point11, QString point12, QString point21, QString point22);
    QString addPointIntersection_LineWithFlat(QString line, QString line2, QString flat);

    void setColVershin(int v);
    void setColLine(int l);
    int getColVershin();
    int getColLine();

    void createPyramid();

    void createPrism(double lengthBokovogoRebra);

    void createCube();

    QList<Point> getListPoints();
    QList<Linear> getListLines();
    QList<Flat> getListFlats();

    QString movePoint(QString namePoint, double x, double y, bool nearestAxis);

    void clear();

    //for task
    QString setRedactTask();
    QString unsetRedactTask();
    QString isTrueAnswer(QString answer);
    void openTask(  QString PointName,
                    QString PointX,
                    QString PointY,
                    QString PointZ,
                    QString LineName,
                    QString LineList,
                    QString LinePFL1,
                    QString LinePFL2,
                    QString TaskType,
                    QString FlatList,
                    QString Answer);

    bool saveFigure();
    QString getSaveTask(   QString *PointName_,
                        QString *PointX_,
                        QString *PointY_,
                        QString *PointZ_,
                        QString *LineName_,
                        QString *LineList_,
                        QString *LinePFL1_,
                        QString *LinePFL2_,
                        QString *TaskType_,
                        QString *FlatList_,
                        QString *Answer_);

    QString getAnswer();
    QString setAnswer(QString typeAnswer, QString answer1, QString answer2);
    QString updateAnswerCuttingPlane(QString answer);
    QString updateIntersecTwoFlats(QString answer1, QString answer2);

    //for work with steps
    QString previousStep();
    QString nextStep();
    void firstStep();
    void lastStep();

    QString setColorOfPoint(QString name, double r, double g, double b);
    QString setStandartColorToPoint(double r, double g, double b);
    QString setStandartColorToLine(double r, double g, double b);
    QString setStandartColorToText(QColor color);

    void rotateSplash();
    QString tiltPrism(int degree, QString os, double height);
    QString getReport();
    void addToReport(QString text);
    void repaintAll();
    void setFromDigest(DigestOgl d);
    DigestOgl getDigest();
    void hidePointName();
    void paintLogo();
    void lookingForIntersecWithLineIn(Linear line);
    QString addLS(QString p, QString from, QString to, QString p1, QString p2);

    void drawTrueAnswer(); // рисует плоскость и прямые, если ответ содержит более 3 точек
    void setSizeFont(int size);

    void createImgBMP(QString path); //path = path + .bmp
};



#endif // OGLPYRAMID_H
