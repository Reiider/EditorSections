#include "paintlogo.h"

void PaintLogo::drawLine(Linear *l)
{
    Point p1 = l->getP1();
    Point p2 = l->getP2();

    GLuint n;
    GLUquadricObj *quadObj;
    quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    n = glGenLists(1);
    glNewList(n, GL_COMPILE);
        glLineWidth(width);
        glBegin(GL_LINE_LOOP);
            glColor3f(r, g, b);
            glVertex3f(p1.getX(), p1.getY(), p1.getZ());
            glVertex3f(p2.getX(), p2.getY(), p2.getZ());
        glEnd();
    glEndList();

    l->setNum(n);
}

void PaintLogo::setColor(double r, double g, double b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

void PaintLogo::setWidthLine(double width)
{
    this->width = width;
}

PaintLogo::PaintLogo()
{
    r = 0;
    g = 0;
    b = 0;
    width = 3;
}

void PaintLogo::paintLogo(QList<Linear> *listLine)
{
    QList<Linear> lL = *listLine;
//Расстовляем точки
    QList<Point> lP = setPoints();
//Рисуем по точкам
    Linear l;
    //Editor-Sections

  //E
    l.setLinear("", lP[0], lP[1], 0);
    drawLine(&l);
    lL.append(l);
    l.setLinear("", lP[2], lP[3], 0);
    drawLine(&l);
    lL.append(l);
    l.setLinear("", lP[4], lP[5], 0);
    drawLine(&l);
    lL.append(l);
    l.setLinear("", lP[1], lP[5], 0);
    drawLine(&l);
    lL.append(l);
  //d
    l.setLinear("", lP[6], lP[7], 0);
    drawLine(&l);
    lL.append(l);
    int i;
    for(i = 8; i < 22; i++){
        l.setLinear("", lP[i], lP[i+1], 0);
        drawLine(&l);
        lL.append(l);
    }
  //i
    l.setLinear("", lP[23], lP[24], 0);
    drawLine(&l);
    lL.append(l);
    l.setLinear("", lP[25], lP[26], 0);
    drawLine(&l);
    lL.append(l);
  //t
    l.setLinear("", lP[27], lP[30], 0);
    drawLine(&l);
    lL.append(l);
    l.setLinear("", lP[28], lP[29], 0);
    drawLine(&l);
    lL.append(l);
    for(i = 30; i < 33; i++){
        l.setLinear("", lP[i], lP[i+1], 0);
        drawLine(&l);
        lL.append(l);
    }
  //o
    for(i = 34; i < 49; i++){
        l.setLinear("", lP[i], lP[i+1], 0);
        drawLine(&l);
        lL.append(l);
    }
    l.setLinear("", lP[49], lP[34], 0);
    drawLine(&l);
    lL.append(l);
  //r
    l.setLinear("", lP[50], lP[51], 0);
    drawLine(&l);
    lL.append(l);
    for(i = 52; i < 57; i++){
        l.setLinear("", lP[i], lP[i+1], 0);
        drawLine(&l);
        lL.append(l);
    }
  //-
    l.setLinear("", lP[58], lP[59], 0);
    drawLine(&l);
    lL.append(l);
  //S
    for(i = 60; i < 75; i++){
        l.setLinear("", lP[i], lP[i+1], 0);
        drawLine(&l);
        lL.append(l);
    }
  //e
    for(i = 76; i < 90; i++){
        l.setLinear("", lP[i], lP[i+1], 0);
        drawLine(&l);
        lL.append(l);
    }
    l.setLinear("", lP[90], lP[82], 0);
    drawLine(&l);
    lL.append(l);
  //c
    for(i = 91; i < 103; i++){
        l.setLinear("", lP[i], lP[i+1], 0);
        drawLine(&l);
        lL.append(l);
    }
  //t
    l.setLinear("", lP[104], lP[107], 0);
    drawLine(&l);
    lL.append(l);
    l.setLinear("", lP[105], lP[106], 0);
    drawLine(&l);
    lL.append(l);
    for(i = 107; i < 110; i++){
        l.setLinear("", lP[i], lP[i+1], 0);
        drawLine(&l);
        lL.append(l);
    }
  //i
    l.setLinear("", lP[111], lP[112], 0);
    drawLine(&l);
    lL.append(l);
    l.setLinear("", lP[113], lP[114], 0);
    drawLine(&l);
    lL.append(l);
  //o
    for(i = 115; i < 130; i++){
        l.setLinear("", lP[i], lP[i+1], 0);
        drawLine(&l);
        lL.append(l);
    }
    l.setLinear("", lP[115], lP[130], 0);
    drawLine(&l);
    lL.append(l);
  //n
    l.setLinear("", lP[131], lP[132], 0);
    drawLine(&l);
    lL.append(l);
    for(i = 133; i < 140; i++){
        l.setLinear("", lP[i], lP[i+1], 0);
        drawLine(&l);
        lL.append(l);
    }
  //s
    for(i = 141; i < 156; i++){
        l.setLinear("", lP[i], lP[i+1], 0);
        drawLine(&l);
        lL.append(l);
    }

    *listLine = lL;
}


QList<Point> PaintLogo::setPoints()
{
    QList<Point> lP;
    double c = 1/23.; //одна 23-я часть
    Point s;
//Editor-Sections   z = 1 для этой грани
  //E
    s.setPoint("", 0, c*(-18), c*4, 1.1);
    lP.append(s); // 0
    s.setPoint("", 0, c*(-20), c*4, 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-18), c*1, 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-20), c*1, 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-18), c*(-3), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-20), c*(-3), 1.1);
    lP.append(s); //5
  //d
    s.setPoint("", 0, c*(-14.5), c*(4.1), 1.1);
    lP.append(s); // 6
    s.setPoint("", 0, c*(-14.5), c*(-3), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-14.5), c*(0.5), 1.1);
    lP.append(s); // 8
    s.setPoint("", 0, c*(-14.8), c*(1.1), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-15.2), c*(1.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-15.7), c*(2), 1.1);
    lP.append(s); // 11
    s.setPoint("", 0, c*(-16.2), c*(1.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-16.5), c*(1.1), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-16.6), c*(0.5), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-16.7), c*(-0.5), 1.1);
    lP.append(s); // 15
    s.setPoint("", 0, c*(-16.6), c*(-1.5), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-16.5), c*(-2.1), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-16.2), c*(-2.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-15.7), c*(-3), 1.1);
    lP.append(s); // 19
    s.setPoint("", 0, c*(-15.2), c*(-2.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-14.8), c*(-2.1), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-14.5), c*(-0.5), 1.1);
    lP.append(s); // 22
  //i
    s.setPoint("", 0, c*(-13), c*(3.8), 1.1);
    lP.append(s); // 23
    s.setPoint("", 0, c*(-13), c*(3.3), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-13), c*(2), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-13), c*(-3), 1.1);
    lP.append(s); // 26
  //t
    s.setPoint("", 0, c*(-11.1), c*(3.5), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-11.8), c*(1.9), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-10.2), c*(1.9), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-11.1), c*(-2.2), 1.1);
    lP.append(s); //30
    s.setPoint("", 0, c*(-11), c*(-2.6), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-10.8), c*(-2.9), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-10.3), c*(-3), 1.1);
    lP.append(s); // 33
  //o
    s.setPoint("", 0, c*(-7.1), c*(0.5), 1.1);
    lP.append(s); // 34
    s.setPoint("", 0, c*(-7.2), c*(1.1), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-7.5), c*(1.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-8.1), c*(2), 1.1);
    lP.append(s); // 37
    s.setPoint("", 0, c*(-8.7), c*(1.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-9), c*(1.1), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-9.1), c*(0.5), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-9.2), c*(-0.5), 1.1);
    lP.append(s); // 41
    s.setPoint("", 0, c*(-9.1), c*(-1.5), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-9), c*(-2.1), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-8.7), c*(-2.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-8.1), c*(-3), 1.1);
    lP.append(s); // 45
    s.setPoint("", 0, c*(-7.5), c*(-2.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-7.2), c*(-2.1), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-7.1), c*(-1.5), 1.1);
    lP.append(s); // 48
    s.setPoint("", 0, c*(-7), c*(-0.5), 1.1);
    lP.append(s); // 49
  //r
    s.setPoint("", 0, c*(-5.5), c*(2), 1.1);
    lP.append(s); //50
    s.setPoint("", 0, c*(-5.5), c*(-3), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-5.5), c*(0.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-4.9), c*(1.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-4.6), c*(1.9), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-4.5), c*(2), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-4.4), c*(1.9), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-4.2), c*(1.6), 1.1);
    lP.append(s); //57
  //-
    s.setPoint("", 0, c*(-3.8), c*(-0.5), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-2.4), c*(-0.5), 1.1);
    lP.append(s); // 59
  //S
    s.setPoint("", 0, c*(-1.7), c*(-2), 1.1);
    lP.append(s); //60
    s.setPoint("", 0, c*(-1.3), c*(-2.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-0.5), c*(-3), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(0.2), c*(-2.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(0.5), c*(-2), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(0.7), c*(-1.2), 1.1);
    lP.append(s); // 65
    s.setPoint("", 0, c*(0.5), c*(-0.4), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(0.1), c*(0.4), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-0.9), c*(1), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-1.5), c*(1.6), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-1.7), c*(2.5), 1.1);
    lP.append(s); //70
    s.setPoint("", 0, c*(-1.5), c*(3.4), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-1.2), c*(4), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(-0.5), c*(4.2), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(0.2), c*(4), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(0.5), c*(3.4), 1.1);
    lP.append(s); // 75
  //e
    s.setPoint("", 0, c*(4), c*(-2.6), 1.1);
    lP.append(s); //76
    s.setPoint("", 0, c*(3.4), c*(-2.9), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(3.2), c*(-3), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(2.6), c*(-2.9), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(2.1), c*(-2.6), 1.1);
    lP.append(s); //80
    s.setPoint("", 0, c*(1.8), c*(-1.9), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(1.6), c*(-0.3), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(1.7), c*(0.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(1.9), c*(1.5), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(2.2), c*(1.9), 1.1);
    lP.append(s); //85
    s.setPoint("", 0, c*(2.9), c*(2.1), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(3.4), c*(1.9), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(3.7), c*(1.5), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(3.9), c*(0.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(4), c*(-0.3), 1.1);
    lP.append(s); //90
  //c
    s.setPoint("", 0, c*(7.2), c*(-2.4), 1.1);
    lP.append(s); //91
    s.setPoint("", 0, c*(6.6), c*(-2.9), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(6.4), c*(-3), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(5.8), c*(-2.9), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(5.3), c*(-2.6), 1.1);
    lP.append(s); //95
    s.setPoint("", 0, c*(5), c*(-1.9), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(4.8), c*(-0.3), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(4.9), c*(0.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(5.1), c*(1.5), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(5.4), c*(1.9), 1.1);
    lP.append(s); //100
    s.setPoint("", 0, c*(6.1), c*(2.1), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(6.6), c*(1.9), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(7.1), c*(1.5), 1.1);
    lP.append(s);//103
  //t
    s.setPoint("", 0, c*(8.4), c*(3.5), 1.1);
    lP.append(s); //104
    s.setPoint("", 0, c*(7.7), c*(1.9), 1.1);
    lP.append(s); //105
    s.setPoint("", 0, c*(9.3), c*(1.9), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(8.4), c*(-2.2), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(8.5), c*(-2.6), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(8.6), c*(-2.9), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(9.1), c*(-3), 1.1);
    lP.append(s); // 110
  //i
    s.setPoint("", 0, c*(10.3), c*(3.8), 1.1);
    lP.append(s); // 111
    s.setPoint("", 0, c*(10.3), c*(3.3), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(10.3), c*(2), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(10.3), c*(-3), 1.1);
    lP.append(s); // 114
  //o
    s.setPoint("", 0, c*(13.7), c*(0.5), 1.1);
    lP.append(s); // 115
    s.setPoint("", 0, c*(13.6), c*(1.1), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(13.3), c*(1.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(12.7), c*(2), 1.1);
    lP.append(s); // 37
    s.setPoint("", 0, c*(12.1), c*(1.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(11.8), c*(1.1), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(11.7), c*(0.5), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(11.6), c*(-0.5), 1.1);
    lP.append(s); // 41
    s.setPoint("", 0, c*(11.7), c*(-1.5), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(11.8), c*(-2.1), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(12.1), c*(-2.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(12.7), c*(-3), 1.1);
    lP.append(s); // 45
    s.setPoint("", 0, c*(13.3), c*(-2.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(13.6), c*(-2.1), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(13.7), c*(-1.5), 1.1);
    lP.append(s); // 48
    s.setPoint("", 0, c*(13.8), c*(-0.5), 1.1);
    lP.append(s); // 130
  //n
    s.setPoint("", 0, c*(14.8), c*(2), 1.1);
    lP.append(s); // 131
    s.setPoint("", 0, c*(14.8), c*(-3), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(14.8), c*(0.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(15.1), c*(1.6), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(15.7), c*(1.9), 1.1);
    lP.append(s); //135
    s.setPoint("", 0, c*(16), c*(2), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(16.3), c*(1.9), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(16.7), c*(1.6), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(17), c*(1), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(17), c*(-3), 1.1);
    lP.append(s); //140
  //s
    s.setPoint("", 0, c*(17.9), c*(-2.3), 1.1);
    lP.append(s); //141
    s.setPoint("", 0, c*(18.3), c*(-2.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(18.9), c*(-3), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(19.5), c*(-2.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(19.8), c*(-2.3), 1.1);
    lP.append(s); //145
    s.setPoint("", 0, c*(19.9), c*(-1.5), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(19.6), c*(-0.7), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(19.2), c*(-0.5), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(18.6), c*(-0.1), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(18.1), c*(0.1), 1.1);
    lP.append(s); //150
    s.setPoint("", 0, c*(17.9), c*(0.6), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(18), c*(1.3), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(18.3), c*(1.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(18.9), c*(2), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(19.5), c*(1.8), 1.1);
    lP.append(s);
    s.setPoint("", 0, c*(19.8), c*(1.3), 1.1);
    lP.append(s); // 156


    return lP;
}
