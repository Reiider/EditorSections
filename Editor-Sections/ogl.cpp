#include "ogl.h"

OGL::OGL(QWidget* pwgt):QGLWidget(pwgt), m_xRotate(0), m_yRotate(0)
{
    f = 0.55;
    sizeFont = 18;
    figure = 0;
    colVershin = 0;
    colLine = 0;
    colFirstLine = 0;
    nnp = new QString*[26];
    for(int i = 0; i < 26; i++){
        nnp[i] = new QString[11];
    }
    for(int i = 0; i < 26; i++){
        nnp[i][10] = 'A'+i;
        for(int j = 0; j < 10; j++){
            nnp[i][j] = "free";
        }
    }
    lastPointSootnoshenie[0] = "0";
    lastPointSootnoshenie[1] = "0";

    redactTask = false;

    movePointName = "";

    colorNnp.setRgb(255,0,0);
    report = "";

    for(int i = 0; i < 5; i++){
        forLookAt[i] = 0;
    }
    forLookAt[2] = 1;
    forLookAt[4] = 1;
    rButtonPress = false;
    lButtonPress = false;
}

Point OGL::findPoint(QString s)
{
    for(int i = 0; i < listPoints.length(); i++){
        if(listPoints[i].getName() == s) return listPoints[i];
    }
    Point p;
    p.setPoint("error",0,0,0,0);
    return p;
}

Linear OGL::findLinear(QString p1, QString p2)
{
    Linear l;
    Point pp1, pp2;
    pp1 = findPoint(p1);
    pp2 = findPoint(p2);
    if(pp1.getName() == "error" || pp2.getName() == "error") {
        l.setLinear("error",pp1,pp2,0);
        return l;
    }
    for(int i = 0; i < listLines.length(); i++){
        if(listLines[i].isPointInListLine(pp1.getName()) && listLines[i].isPointInListLine(pp2.getName())) return listLines[i];
    }

    l.setLinear("error",pp1,pp2,0);
    return l;
}

QString OGL::isPointExist(Point p)
{
    for(int i = 0; i < listPoints.length(); i++){
        if(p.isEqual(listPoints[i])) return listPoints[i].getName();
    }
    return "false";
}

QList<QString> OGL::splitStringPoints(QString points)
{
    bool flagFindPoints;
    QStringList la = points.split(" ", QString::SkipEmptyParts);
    for(int i = 0; i < la.length(); i++){
        flagFindPoints = false;
        for(int k = 0; k < listPoints.length(); k++){
            if(la[i] == listPoints[k].getName()){
                flagFindPoints = true;
                break;
            }
        }
        if(!flagFindPoints) {
            QString returnStr = "Ошибка. Точка " + la[i] + " не найдена!";
            QRegExp rx(".*[А-Я]+.*");
            if(rx.exactMatch(la[i])){
                returnStr += "\r\nИспользуйте английский алфавит.";
            }
            rx.setPattern(".*[a-z]+.*");
            if(rx.exactMatch(la[i])){
                returnStr += "\r\nИспользуйте заглавные буквы.";
            }
            la.clear();
            la.append(returnStr);
            return la;
        }
    }
    return la;
}

void OGL::initializeGL()
{
    qglClearColor(Qt::white);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OGL::resizeGL(int nWidth, int nHeight)
{
    float w = (float)nWidth/ (float)nHeight;
    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w, w, -1.0, 1.0, 1.0, 10.0);
}

void OGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    glDisable(GL_CULL_FACE);
    glLoadIdentity();
    glTranslatef(forLookAt[0], forLookAt[1], -3.0);
    glRotatef(m_xRotate, 1.0, 0.0, 0.0);
    glRotatef(m_yRotate, 0.0, 1.0, 0.0);
    glScalef(f,f,f);


    glEnable(GL_MULTISAMPLE);
    for(int i = 0; i < listLines.length(); i++){
        glCallList(listLines[i].getNum());
    }

    for(int i = 0; i < listPoints.length(); i++){
        glCallList(listPoints[i].getNum());
    }

    if(redactTask){
        for(int i = 0; i < paintAnswer.length(); i++){ //отрисовка точек ответа при редактировании
            glCallList(paintAnswer[i].getNum());
        }
    }
    glDisable(GL_MULTISAMPLE);


    glCallList(graphicAnswer.getN());


    glCallList(figure);


    glDisable(GL_DEPTH_TEST);
    qglColor(colorNnp);
    for(int i = 0; i < listPoints.length(); i++){
        outText(listPoints[i].getX(), listPoints[i].getY(), listPoints[i].getZ(),  listPoints[i].getName());
    }

    if(redactTask){
        for(int i = 0; i < paintAnswer.length(); i++){
            outText(paintAnswer[i].getX(), paintAnswer[i].getY(), paintAnswer[i].getZ(),  paintAnswer[i].getName());
        }
    }
    glEnable(GL_DEPTH_TEST);
}

void OGL::mousePressEvent(QMouseEvent* pe)
{
    if(pe->buttons() == Qt::RightButton){
        rButtonPress = true;
    }
    if(pe->buttons() == Qt::LeftButton){
        lButtonPress = true;
    }
    m_ptPosition = pe->pos();
}

void OGL::mouseReleaseEvent(QMouseEvent *pe)
{
    if(pe->button() == Qt::RightButton){
        rButtonPress = false;
    }
    if(pe->button() == Qt::LeftButton){
        lButtonPress = false;
    }
}

void OGL::mouseMoveEvent(QMouseEvent* pe)
{
    if(rButtonPress){
        forLookAt[0] += (GLdouble)(pe->x()-m_ptPosition.x())/width();
        forLookAt[1] -= (GLdouble)(pe->y()-m_ptPosition.y())/height();
    }
    if(lButtonPress){
        m_xRotate += 180 *(GLfloat)(pe->y()-m_ptPosition.y())/height();
        m_yRotate +=180*(GLfloat)(pe->x()-m_ptPosition.x())/width();
    }
    updateGL();
    m_ptPosition = pe->pos();
}

void OGL::wheelEvent(QWheelEvent *ev)
{
    switch(ev->orientation())
    {
        case Qt::Vertical:{

                if(f >= 0.08 && f <= 1) f += ev->delta()/5000.;
                else if(f < 0.08 && ev->delta() > 0) f += ev->delta()/5000.;
                else if(f > 1 && ev->delta() < 0) f += ev->delta()/5000.;
                updateGL();
            }
            break;
        default:
            ev->ignore();
    }
}

void OGL::changeSizeFigure(int sizeF){
    f = (2*sizeF)/100.;
    updateGL();
}

QString OGL::getNameNextPoint(QString name){  //if name = "0" get name next point, else relese or set name point
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
        bool release = false;
        for(int i = 0; i < listPoints.length(); i++){
            if(listPoints[i].getName() == name) {
                release = true;
                break;
            }
        }
        int i,j;
        i = 0;
        j = 0;
        if(release){  //release
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
                for(i = 0; i < 26; i++){
                    if(s == nnp[i][10]) break;
                }
                s = name;
                s.remove(0,1);
                j = s.toInt(0, 10);
                if(j < 10) nnp[i][j] = "free";
            }
        }
        else{ //set
            if(s.length() > 2) return "Ошибка имени. Приемлимый формат: 'A-Z'['0-9']. \nПримеры: A, G, V0, M9.";
            s.remove(0,1);
            if(s == '\0'){
                if("A">name || "Z"<name) return "Ошибка имени. Приемлимый формат: 'A-Z'['0-9']. \nПримеры: A, G, V0, M9.";
                for(i = 0; i < 26; i++){
                    if(name == nnp[i][10]) break;
                }
                nnp[i][0] = "busy";
                return nnp[i][10];
            }
            else{
                s = name;
                s.remove(1,1);
                if("A">s || "Z"<s) return "Ошибка имени. Приемлимый формат: 'A-Z'['0-9']. \nПримеры: A, G, V0, M9.";
                for(i = 0; i < 26; i++){
                    if(s == nnp[i][10]) break;
                }
                s = name;
                s.remove(0,1);
                if("0">s || "9"<s) return "Ошибка имени. Приемлимый формат: 'A-Z'['0-9']. \nПримеры: A, G, V0, M9.";
                j = s.toInt(0, 10);
                if(j < 10) nnp[i][j] = "busy";

                char s[1];
                itoa(j, s, 10);
                return nnp[i][10]+s;
            }
        }
    }
    return "";
}

void OGL::outText(double x, double y, double z, QString s)
{
    QFont font("Arial", sizeFont);
    QColor col2;
    col2.setRgb(255, 255, 255);
    qglColor(col2);

    renderText(x + 0.02, y + 0.030, z, s, font);
    renderText(x + 0.030, y + 0.02, z, s, font);
    renderText(x + 0.010, y + 0.02, z, s, font);
    renderText(x + 0.02, y + 0.010, z, s, font);

    renderText(x + 0.010, y + 0.010, z, s, font);
    renderText(x + 0.010, y + 0.030, z, s, font);
    renderText(x + 0.030, y + 0.010, z, s, font);
    renderText(x + 0.030, y + 0.030, z, s, font);

    qglColor(colorNnp);
    renderText(x + 0.02, y + 0.02, z, s, font);

}

Point OGL::isPointInTheListPoint(Point p)
{
    for(int i = 0; i < listPoints.length(); i++){
        if(((p.getX() + 0.0001) > listPoints[i].getX()) && ((p.getX() - 0.0001) < listPoints[i].getX()) &&
           ((p.getY() + 0.0001) > listPoints[i].getY()) && ((p.getY() - 0.0001) < listPoints[i].getY()) &&
           ((p.getZ() + 0.0001) > listPoints[i].getZ()) && ((p.getZ() - 0.0001) < listPoints[i].getZ())){
            return listPoints[i];
        }
    }
    p.setPoint(getNameNextPoint("0"), 0, p.getX(), p.getY(), p.getZ());
    help.drawPoint(&p, p.getColor());

    listPoints.append(p);
    return p;
}

QString OGL::d_to_str(double d)
{
    QString temp;
    if(d > -0.0001 && d < 0.0001 ) temp = "0";
    else temp = temp.number(d);
    return temp;
}

QString OGL::one_of_line(QString str, int n)
{
    str.remove(0, n);
    str.remove(1, str.length()-1);
    return str;
}

QString OGL::points(QString point, QString isFlat)
{
WriteLog::write("points --- start");
    if(point == "") return "Ошибка. Введен пустой ответ.";
    QList<QString> list = splitStringPoints(point); // находим точки
    if(list[0].length() > 4){
        return list[0];
    }
    QList<Point> lp;
    for(int i = 0; i < list.length(); i++){
        Point p = findPoint(list[i]);
        if(p.getName() == "error") return "Ошибка при поиске точки.";
        lp.append(p);
    }
WriteLog::write("   1");
    if(list.length() >= 3 && isFlat != "no"){
        Flat f;
        f.setFlat(lp);
        listFlats.append(f);
        lp = help.sotrPointInFigure(lp, listFlats); //
    }

WriteLog::write("   2");
    if(isFlat == "no") Answer = "NoFlat ";
    else Answer = "";
    for(int i = 0; i < list.length(); i++){
        Answer += lp[i].getName() + " ";
    }
WriteLog::write("points --- end");
    return "";
}

QString OGL::intersecPlanWhithLine(QString line, QString plane)
{
    if(plane == "" || line == "") return "Ошибка. Введен пустой ответ.";
    QString a;
    a = cuttingPlane(plane);
    if(a != "") {
        Answer.clear();
        return a;
    }

    Point p1, p2;

    QList<QString> la = splitStringPoints(line);

    if(la.length() < 2){
        return la[0];
    }

    p1 = findPoint(la[0]);

    p2 = findPoint(la[1]);

    if(p1.getName() == "error"){
        QString returnStr = "Ошибка. Точка " + la[0] + " не найдена!";
        QRegExp rx(".*[А-Я]+.*");
        if(rx.exactMatch(la[0])){
            returnStr += "\r\nИспользуйте английский алфавит.";
        }
        rx.setPattern(".*[a-z]+.*");
        if(rx.exactMatch(la[0])){
            returnStr += "\r\nИспользуйте заглавные буквы.";
        }
        return returnStr;
    }
    if(p2.getName() == "error"){
        QString returnStr = "Ошибка. Точка " + la[1] + " не найдена!";
        QRegExp rx(".*[А-Я]+.*");
        if(rx.exactMatch(la[1])){
            returnStr += "\r\nИспользуйте английский алфавит.";
        }
        rx.setPattern(".*[a-z]+.*");
        if(rx.exactMatch(la[1])){
            returnStr += "\r\nИспользуйте заглавные буквы.";
        }
        return returnStr;
    }

    Linear l;
    l = findLinear(la[0], la[1]); // в поиске прямой было ИЛИ одна точка ИЛИ другая

    if(l.getName() == "error"){
        l.setLinear(p1.getName() + p2.getName(), p1, p2, 0);
        help.drawLine(&l);
        listLines.append(l);
    }

    Point p = listFlats.last().getPointIntersection(l);
    if(p.getName() != "1") return p.getName();

    if(isPointExist(p) == "false"){
        p.setName(getNameNextPoint("0"));
        help.drawPoint(&p, p.getColor());
        listPoints.append(p);
    }
    else{
        p.setPoint(isPointExist(p), p.getNum(), p.getX(), p.getY(), p.getZ());
    }

    Answer = p.getName();

    PossibleStep s; //save step
    s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);

    updateGL();

    return "";
}

QString OGL::intersecPlanWhithPlane(QString plane1, QString plane2)
{
    if(plane1 == "" || plane2 == "") return "Ошибка. Введен пустой ответ.";
    Flat f1, f2, f;
    QString willReturn;
    willReturn = cuttingPlane(plane1);
    if(willReturn != "") return willReturn;
    f1 = graphicAnswer.getFlats().last();

    willReturn = cuttingPlane(plane2);
    if(willReturn != "") return willReturn;
    f2 = graphicAnswer.getFlats().last();
    //находим пересечение плоскостей по формулам
    // х=х0+ха
    // у=у0+уа    это параметрическое уровнение а - параметр
    // z=а
    int countIntersecPoint = 0;
    double a, x, y, z;
    Point p;
    QList<Point> pl;
    double x0 = 0, xa = 0, y0 = 0, ya = 0, z0 = 0, za = 0;
    if((f1.getA()*f2.getB()-f2.getA()*f1.getB()) > 0 + 0.0001 || (f1.getA()*f2.getB()-f2.getA()*f1.getB()) < 0 - 0.0001){
        x0 = (f1.getB()*f2.getD()-f2.getB()*f1.getD())/(f1.getA()*f2.getB()-f2.getA()*f1.getB());
        xa = (f1.getB()*f2.getC()-f2.getB()*f1.getC())/(f1.getA()*f2.getB()-f2.getA()*f1.getB());

        y0 = (f2.getA()*f1.getD()-f1.getA()*f2.getD())/(f1.getA()*f2.getB()-f2.getA()*f1.getB());
        ya = (f2.getA()*f1.getC()-f1.getA()*f2.getC())/(f1.getA()*f2.getB()-f2.getA()*f1.getB());

        for(int i = 0; i < listFlats.length(); i++){
            f = listFlats[i];
            if((((f.getA()*xa + f.getB()*ya + f.getC()) + 0.0001 > 0 && (f.getA()*xa + f.getB()*ya + f.getC()) - 0.0001 > 0) || ((f.getA()*xa + f.getB()*ya + f.getC()) + 0.0001 < 0 && (f.getA()*xa + f.getB()*ya + f.getC()) - 0.0001 < 0)) ){
                a = (-f.getD()-f.getA()*x0-f.getB()*y0)/(f.getA()*xa + f.getB()*ya + f.getC());
                x = x0 + xa*a;
                y = y0 + ya*a;
                z = a;
                p.setPoint("", 0, x, y, z);
                countIntersecPoint++;
                if(f.isPointBeInTheArea(p)) pl.append(p);
            }
        }
    }
    else if((f1.getA()*f2.getC()-f2.getA()*f1.getC()) > 0 + 0.0001 || (f1.getA()*f2.getC()-f2.getA()*f1.getC()) < 0 - 0.0001){
        x0 = (f1.getC()*f2.getD()-f2.getC()*f1.getD())/(f1.getA()*f2.getC()-f2.getA()*f1.getC());
        xa = (f2.getB()*f1.getC()-f1.getB()*f2.getC())/(f1.getA()*f2.getC()-f2.getA()*f1.getC());

        z0 = (f2.getA()*f1.getD()-f1.getA()*f2.getD())/(f1.getA()*f2.getC()-f2.getA()*f1.getC());
        za = (f2.getA()*f1.getB()-f1.getA()*f2.getB())/(f1.getA()*f2.getC()-f2.getA()*f1.getC());

        for(int i = 0; i < listFlats.length(); i++){
            f = listFlats[i];
            if(((f.getA()*xa + f.getB() + f.getC()*za) > 0 + 0.0001 || (f.getA()*xa + f.getB() + f.getC()*za) < 0 - 0.0001) ){
                a = (-f.getD()-f.getA()*x0-f.getC()*z0)/(f.getA()*xa + f.getB() + f.getC()*za);
                x = x0 + xa*a;
                z = z0 + za*a;
                y = a;
                p.setPoint("", 0, x, y, z);
                countIntersecPoint++;
                if(f.isPointBeInTheArea(p)) pl.append(p);
            }
        }
    }
    else if((f1.getB()*f2.getC()-f2.getB()*f1.getC()) > 0 + 0.0001 || (f1.getB()*f2.getC()-f2.getB()*f1.getC()) < 0 - 0.0001){
        y0 = (f1.getC()*f2.getD()-f2.getC()*f1.getD())/(f1.getB()*f2.getC()-f2.getB()*f1.getC());
        ya = (f2.getA()*f1.getC()-f1.getA()*f2.getC())/(f1.getB()*f2.getC()-f2.getB()*f1.getC());

        z0 = (f2.getB()*f1.getD()-f1.getB()*f2.getD())/(f1.getB()*f2.getC()-f2.getB()*f1.getC());
        za = (f1.getA()*f2.getB()-f2.getA()*f1.getB())/(f1.getB()*f2.getC()-f2.getB()*f1.getC());

        for(int i = 0; i < listFlats.length(); i++){
            f = listFlats[i];
            if(((f.getA() + f.getB()*ya + f.getC()*za) > 0 + 0.0001 || (f.getA() + f.getB()*ya + f.getC()*za) < 0 - 0.0001) ){
                a = (-f.getD()-f.getB()*y0-f.getC()*z0)/(f.getA() + f.getB()*ya + f.getC()*za);
                y = y0 + ya*a;
                z = z0 + za*a;
                x = a;
                p.setPoint("", 0, x, y, z);
                countIntersecPoint++;
                if(f.isPointBeInTheArea(p)) pl.append(p);
            }
        }
    }
    //double z = 1;


    //ищем точки пересечения со всеми плоскостями, но что бы эти точки лежали в видимой части плоскости

    if(countIntersecPoint >= 2 && pl.length() < 2) return "Плоскости в фигуре не пересекаются";
    if(pl.length() < 2) return "Ошибка при построении прямой";
    //распознаем точки

    for(int i = 0; i < pl.length(); i++){
        for(int j = i + 1; j < pl.length(); ){
            if(pl[i].isEqual(pl[j])){
                pl.removeAt(j);
            }
            else j++;
        }
    }
    for(int i = 0; i < pl.length(); i++){
        if(isPointExist(pl[i]) == "false"){
            pl[i].setName(getNameNextPoint("0"));
            help.drawPoint(&pl[i], pl[i].getColor());
            listPoints.append(pl[i]);
        }
        else{
            pl[i] = findPoint(isPointExist(pl[i]));
        }
    }

    //строим прямую
    Linear l;

    l.setLinear("", pl[0], pl[1], 0);
    for(int i = 2; i < pl.length(); i++){
        l.addPointToLine(pl[i]);
    }
    l.setLinear(l.getP1().getName() + l.getP2().getName(), l.getP1(), l.getP2(), 0);
    help.drawLine(&l);
    listLines.append(l);

    glDeleteLists(graphicAnswer.getN(), 1);
    graphicAnswer.updateColorFlat(1, ColorFlat(1, 0.5, 0, 1));
    graphicAnswer.setN(help.drawFigure(graphicAnswer.getFlats(), graphicAnswer.getColorFlats()));

    Answer = l.getP1().getName() + " " + l.getP2().getName();

    PossibleStep s; //save step
    s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);

    updateGL();

    return "";
}

QString OGL::cuttingPlane(QString points)
{
WriteLog::write("cuttingPlane --- start");
    if(points == "") return "Ошибка. Введен пустой ответ.";
    QList<QString> la = splitStringPoints(points);
WriteLog::write("2");
    if(la[0].length() > 2) return la[0];
    QList<Point> ps;
    for(int i = 0; i < la.length(); i++){
        for(int j = 0; j < listPoints.length(); j++){
            if(la[i] == listPoints[j].getName()) ps.append(listPoints[j]);
        }
    }
    if(ps.length() != 3){
        return "Неверное количество введенных точек";
    }
    //проверка, лежат ли точки на одной прямой
WriteLog::write("4");
    Linear checkLine;
    checkLine.setLinear("", ps[0], ps[1], 0);
    if(checkLine.isPointInLine(ps[2])){
        return "Введенные точки лежат на одной прямой";
    }
    //
    Flat flat;
    flat.setFlat(ps);
    Point p;
    bool flag1 = true;
    ps.clear(); //в дальнейшем используется для построения плоскости
WriteLog::write("5");
    double colorP[3] = {0,0,0};
    for(int i = 0; i < listLines.length(); i++){ //31.05.2018 было for(int i = 0; i < colFirstLine; i++){
        p = flat.getPointIntersection(listLines[i]);
        if("1" == p.getName()){
            flag1 = true;
            for(int j = 0; j < listPoints.length(); j++){
                if(((p.getX() + 0.0001) > listPoints[j].getX()) && ((p.getX() - 0.0001) < listPoints[j].getX()) &&
                   ((p.getY() + 0.0001) > listPoints[j].getY()) && ((p.getY() - 0.0001) < listPoints[j].getY()) &&
                   ((p.getZ() + 0.0001) > listPoints[j].getZ()) && ((p.getZ() - 0.0001) < listPoints[j].getZ())){

                    flag1 = false;
                    p.setPoint(listPoints[j].getName(), p.getNum(), p.getX(), p.getY(), p.getZ());
                    bool bel = true;
                    for(int k = 0; k < ps.length(); k++){
                        if(p.getName() == ps[k].getName()) bel = false;
                    }
                    if(bel) ps.append(p);
                }
            }
            if(flag1 && listLines[i].isPointBeInTheLine(p)){ //отрисовка новых точек
                colorP[1] = 0.7;
                p.setName(getNameNextPoint("0"));
                help.drawPoint(&p, colorP);
                listPoints.append(p);
                ps.append(p);
            }
        }
    }
WriteLog::write("6");
    //отрисовка введенныйх точек
    for(int i = 0; i < la.length(); i++){
        for(int j = 0; j < listPoints.length(); j++){
            if(la[i] == listPoints[j].getName()){
                colorP[1] = 0;
                colorP[0] = 0.8;
                listPoints[j].deleteNum();
                help.drawPoint(&listPoints[j], colorP);
            }
        }
    }
    //удаляем точки не лежащие ни в одной из существующих плоскостей
    for(int i = ps.length()-1; i >= 0; i--){
        bool ok = true; //удалЯть
        for(int j = 0; j < listFlats.length(); j++){
            if(listFlats[j].isPointBeInTheArea(ps[i])){
                ok = false; //если точка лежит в одной из плоскостей, то не удалять
            }
        }
        if(ok){
            ps.removeAt(i);
        }
    }
WriteLog::write("7");
    if(ps.length() < 3) return "Ошибка, плоскость не пересекает фигуру.";
    QList<Point> pointL;
    pointL = help.sotrPointInFigure(ps, listFlats);
    for(int k = 0; k < pointL.length(); k++){
WriteLog::write("   pointL - " + pointL[k].getName());
    }
WriteLog::write("8");
    int returned = 0; //для определения ошибки
    if(pointL[0].getName() == "error") returned = 1;  // неудалось правильно распознать плоскость

    QString stAn = "";
    if(returned == 0){
        for(int i = 0; i < pointL.length(); i++){
            stAn += pointL[i].getName() + " ";
        }
    }
    else{
        for(int i = 0; i < ps.length(); i++){
            stAn += ps[i].getName() + " ";
        }
    }

    Flat lastF;
    if(returned == 0) lastF.setFlat(pointL);
    else lastF.setFlat(ps);
WriteLog::write("9");
    graphicAnswer.append(lastF, ColorFlat(0.5, 1, 0, 0));
    glDeleteLists(graphicAnswer.getN(), 1);
WriteLog::write("10");
    graphicAnswer.setN(help.drawFigure(graphicAnswer.getFlats(), graphicAnswer.getColorFlats()));
WriteLog::write("11");
    Answer = stAn;

    PossibleStep s; //save step
    s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);

    updateGL();
WriteLog::write("cuttingPlane --- end");
    if(returned == 0) return "";
    else return "1";
}

void OGL::setColVershin(int v){
    colVershin = v;
}

void OGL::setColLine(int l)
{
    colLine = l;
}

int OGL::getColVershin()
{
    return colVershin;
}

int OGL::getColLine()
{
    return colLine;
}

void OGL::createPyramid()
{
    clear();

    Pyramid pyramid;
    figure = pyramid.createPyramid(colVershin, &listPoints, &listLines, &listFlats, nnp);
    colVershin = colVershin + 1;
    colLine = listLines.length();
    colFirstLine = listLines.length();

    updateGL();

    PossibleStep s; //save step
    s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);
}

void OGL::createPrism(double lenBokReb)
{    
    clear();

    Prism prism;
    figure = prism.createPrism(colVershin, &listPoints, &listLines, &listFlats, lenBokReb, nnp);
    colVershin = colVershin * 2;
    colLine = listLines.length();
    colFirstLine = listLines.length();

    lastPointSootnoshenie[0] = "0";
    lastPointSootnoshenie[1] = "0";
    lastMoveLine = 0;
    updateGL();

    PossibleStep s; //save step
    s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);
}

void OGL::createCube()
{
    clear();

    Cube cube;
    figure = cube.createCube(&listPoints, &listLines, &listFlats, nnp);
    colVershin = 8;
    colLine = listLines.length();
    colFirstLine = listLines.length();

    lastPointSootnoshenie[0] = "0";
    lastPointSootnoshenie[1] = "0";
    lastMoveLine = 0;
    updateGL();

    PossibleStep s; //save step
    s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);
}

QList<Point> OGL::getListPoints()
{
    return listPoints;
}

QList<Linear> OGL::getListLines()
{
    return listLines;
}

QList<Flat> OGL::getListFlats()
{
    return listFlats;
}

QString OGL::movePoint(QString namePoint, double x, double y, bool nearestAxis)
{
    namePoint = namePoint.toUpper().trimmed(); //переводим в верхний регистр
    //разбиваем на точки
    QList<QString> splitNamePoint = splitStringPoints(namePoint);
    if(splitNamePoint[0].length() > 2) return splitNamePoint[0];
    if(splitNamePoint.length() == 0) return "";
    if(splitNamePoint.length() > 1){
        // сохраняем точки и плоскости
        QList<Point> tempListPoints = listPoints;
        QList<Flat> tempListFlats = listFlats;

        //ищем точки и меняем их координаты
        for(int j = 0; j < splitNamePoint.length(); j++){
            for(int i = 0; i < listPoints.length(); i++){
                if(splitNamePoint[j] == listPoints[i].getName()){
                    double tX = x*cos(m_yRotate*grad) + y*sin(m_yRotate*grad)*sin(m_xRotate*grad);
                    double tY = y*cos(m_xRotate*grad);
                    double tZ = x*sin(m_yRotate*grad) + y*sin(-m_xRotate*grad)*cos(m_yRotate*grad);
                    if(nearestAxis){
                        if(help.abs(tX) / help.abs(tZ) > 0.6 && help.abs(tX)/help.abs(tZ) < 1.4){
                            if(help.abs(tX) > help.abs(tZ)) tZ = help.abs(tZ) / tZ * help.abs(tX);
                            else tX = help.abs(tX) / tX * help.abs(tZ);
                        }
                        else if(help.abs(tX) > help.abs(tZ)){
                            tZ = 0;
                        }
                        else tX = 0;
                    }
                    listPoints[i].setPoint(listPoints[i].getName(), listPoints[i].getNum(), listPoints[i].getX() + tX, listPoints[i].getY() + tY, listPoints[i].getZ() + tZ);
                }
            }
        }

        //проверяем возможно ли такое изменение координат
          //проверка на корректность плоскостей после сдвига
            //заменяем точки в плоскостях
        QList<Point> list;
        for(int i = 0; i < splitNamePoint.length(); i++){
            for(int j = 0; j < listFlats.length(); j++){
                list = listFlats[j].getListPoint();
                for(int k = 0; k < list.length(); k++){
                    if(splitNamePoint[i] == list[k].getName()){
                        int o = 0;
                        for(; o < listPoints.length(); o++){
                            if(listPoints[o].getName() == splitNamePoint[i]) break;
                        }
                        listFlats[j].addPointToFlat(listPoints[o]);
                        break;
                    }
                }
            }
        }
            //проверяем остались ли плоскости плоскостями (а не набором точек)
        for(int i = 0; i < listFlats.length(); i++){
            Flat newFlat;
            list = listFlats[i].getListPoint();
            QList<Point> newList; // создаем плоскость из трех точек и проверяем лежат ли остальные в этой плоскости
            newList.append(list.first());
            list.removeFirst();
            newList.append(list.first());
            list.removeFirst();
            newList.append(list.first());
            list.removeFirst();
            newFlat.setFlat(newList);
            while(list.length() != 0){
                if(newFlat.isPointInTheArea(list.first())) list.removeFirst();
                else{
                    listPoints = tempListPoints;
                    listFlats = tempListFlats;
                    return "Ошибка, при сдвиге точек плоскость '" + newFlat.getName() + "' перестаёт быть таковой. Разделите ее прямой.";
                }
            }
        }

        //проверка на выход за допустимые границы при передвижении
        //добавить бы...ДОДЕЛАТЬ

        //отрисовка точек
        for(int i = 0; i < splitNamePoint.length(); i++){
            for(int j = 0; j < listPoints.length(); j++){
                if(splitNamePoint[i] == listPoints[j].getName()){
                    listPoints[j].deleteNum();
                    help.drawPoint(&listPoints[j], listPoints[j].getColor());
                }
            }
        }

        //перерисовываем прямые
        for(int i = 0 ; i < splitNamePoint.length(); i++){
            int o = 0;
            for(; o < listPoints.length(); o++){
                if(splitNamePoint[i] == listPoints[o].getName()) break;
            }
            for(int j = 0; j < listLines.length(); j++){
                list = listLines[j].getLine();
                for(int k = 0; k < list.length(); k++){
                     if(listLines[j].isPointInLine(listPoints[o]) || listLines[j].getP1().getName() == splitNamePoint[i] || listLines[j].getP2().getName() == splitNamePoint[i]){
                        listLines[j].addPointToLine(listPoints[o]);
                        list = listLines[j].getLine();
                        listLines[j].setLinear(listLines[j].getName(), listLines[j].getP1(), listLines[j].getP2(), listLines[j].getNum());
                        for(int z = 0; z < list.length(); z++){
                            listLines[j].addPointToLine(list[z]);
                        }
                    }
                    break;
                }
            }
        }
        for(int i = 0 ; i < listLines.length(); i++){
            listLines[i].deleteNum();
            help.drawLine(&listLines[i]);
        }
        repaintAll();

    }
    else{

        //поиск передвигаемой точки и задание новых координат
        int indexPo = 0;
        Point po;
        for(int i = 0; i < listPoints.length(); i++){
            if(namePoint == listPoints[i].getName()){
                indexPo = i;
                double tX = x*cos(m_yRotate*grad) + y*sin(m_yRotate*grad)*sin(m_xRotate*grad);
                double tY = y*cos(m_xRotate*grad);
                double tZ = x*sin(m_yRotate*grad) + y*sin(-m_xRotate*grad)*cos(m_yRotate*grad);
                if(nearestAxis){
                    if(help.abs(tX) / help.abs(tZ) > 0.6 && help.abs(tX)/help.abs(tZ) < 1.4){
                        if(help.abs(tX) > help.abs(tZ)) tZ = help.abs(tZ) / tZ * help.abs(tX);
                        else tX = help.abs(tX) / tX * help.abs(tZ);
                    }
                    else if(help.abs(tX) > help.abs(tZ)){
                        tZ = 0;
                    }
                    else tX = 0;
                }
                po.setPoint(listPoints[i].getName(), listPoints[i].getNum(), listPoints[i].getX() + tX, listPoints[i].getY() + tY, listPoints[i].getZ() + tZ);
                break;
            }
        }

        //проверка на корректность плоскостей после сдвига
        QList<Point> list;
        for(int i = 0; i < listFlats.length(); i++){
            list = listFlats[i].getListPoint();
            bool isPITA = listFlats[i].isPointInTheArea(po);
            for(int j = 0; j < list.length(); j++){
                if((list.length() > 3) && (list[j].getName() == namePoint) && !isPITA)
                    return "Ошибка, при сдвиге точки одна из плоскостей перестаёт быть таковой. Разделите ее прямой.";
            }
        }

        //проверка на выход за допустимые границы при передвижении
        Linear lMoving; //отрезок передвижения точки
        lMoving.setLinear("", listPoints[indexPo], po, 0);
        list.clear();
        for(int i = 0; i < listFlats.length(); i++){
            list = listFlats[i].getListPoint();
            for(int j = 0; j < list.length(); j++){
                if(list[j].getName() == namePoint && listFlats[i].isPointInTheArea(po)){
                    Linear line1, line2; //прямые, которым принадлежит передвигаемая точка
                    Linear nearLine1, nearLine2;
                    if(j == 0) {
                        line1.setLinear("", list[1], po, 0);
                        line2.setLinear("", po, list[list.length()-1], 0);
                        if(list.length() >= 4){
                            nearLine1.setLinear("", list[2], list[1], 0);
                            nearLine2.setLinear("", list[list.length()-1], list[list.length()-2], 0);
                        }
                    }
                    else if(j == list.length() - 1){
                        line1.setLinear("", list[0], po, 0);
                        line2.setLinear("", po, list[list.length()-2], 0);
                        if(list.length() >= 4){
                            nearLine1.setLinear("", list[1], list[0], 0);
                            nearLine2.setLinear("", list[list.length()-2], list[list.length()-3], 0);
                        }
                    }
                    else {
                        line1.setLinear("", list[j+1], po, 0);
                        line2.setLinear("", po, list[j-1], 0);
                        if(list.length() >= 4){
                            if(j == 1){
                                nearLine1.setLinear("", list[3], list[2], 0);
                                nearLine2.setLinear("", list[0], list[list.length()-1], 0);
                            }
                            else if(j == list.length()-2){
                                nearLine1.setLinear("", list[list.length()-1], list[0], 0);
                                nearLine2.setLinear("", list[list.length()-3], list[list.length()-4], 0);
                            }
                            else{
                                nearLine1.setLinear("", list[j+2], list[j+1], 0);
                                nearLine2.setLinear("", list[j-1], list[j-2], 0);
                            }
                        }
                    }
                    if(list.length() == 3){//для случая с 3мя точками на грани
                        Linear line3case;
                        if(j == 0) line3case.setLinear("", list[1], list[2], 0);
                        else if(j == 2)line3case.setLinear("", list[0], list[1], 0);
                        else line3case.setLinear("", list[0], list[2], 0);
                        Point p3case = lMoving.getPointIntersection(line3case);
                        if(p3case.getName() == "1" && lMoving.isPointBeInTheLine(p3case)){
                            return "Ошибка передвижения. Точка не должна выходить за рамки определенной области.";
                        }
                    }
                    if(list.length() >= 4){ //проверка на пересечение линий соседствующих с line1 и line2
                        Point pI1 = line1.getPointIntersection(nearLine2);
                        Point pI2 = line2.getPointIntersection(nearLine1);
                        if((pI1.getName() == "1" && line1.isPointBeInTheLine(pI1) && nearLine2.isPointBeInTheLine(pI1)) ||
                           (pI2.getName() == "1" && line2.isPointBeInTheLine(pI2) && nearLine1.isPointBeInTheLine(pI2)))
                            return "Ошибка передвижения. Точка не должна выходить за рамки определенной области.";
                        if(list.length() == 4){
                            pI1 = lMoving.getPointIntersection(nearLine1);
                            pI2 = lMoving.getPointIntersection(nearLine2);
                            if((pI1.getName() == "1" && nearLine1.isPointBeInTheLine(pI1) && lMoving.isPointBeInTheLine(pI1)) ||
                               (pI2.getName() == "1" && nearLine2.isPointBeInTheLine(pI2) && lMoving.isPointBeInTheLine(pI2)))
                                return "Ошибка передвижения. Точка не должна выходить за рамки определенной области.";
                        }
                    }

                    //проверяем не пересекаются ли прямые образующие грань друг с другом
                    for(int k = j+2; k+1 < list.length()-1; k++){
                        Linear l = findLinear(list[k].getName(), list[k+1].getName());
                        Point pI1 = line1.getPointIntersection(l);
                        Point pI2 = line2.getPointIntersection(l);
                        if((pI1.getName() == "1" && line1.isPointBeInTheLine(pI1) && l.isPointBeInTheLine(pI1)) ||
                           (pI2.getName() == "1" && line2.isPointBeInTheLine(pI2) && l.isPointBeInTheLine(pI2)))
                            return "Ошибка передвижения. Точка не должна выходить за рамки определенной области.";
                    }
                    for(int k = 1; k+1 < j-1; k++){
                        Linear l = findLinear(list[k].getName(), list[k+1].getName());
                        Point pI1 = line1.getPointIntersection(l);
                        Point pI2 = line2.getPointIntersection(l);
                        if((pI1.getName() == "1" && line1.isPointBeInTheLine(pI1) && l.isPointBeInTheLine(pI1)) ||
                           (pI2.getName() == "1" && line2.isPointBeInTheLine(pI2) && l.isPointBeInTheLine(pI2)))
                            return "Ошибка передвижения. Точка не должна выходить за рамки определенной области.";
                    }
                    if(list.length() >= 5 && (j == 2 || j == list.length()-3)){
                        Linear l = findLinear(list[0].getName(), list[list.length()-1].getName());
                        Point pI1 = line1.getPointIntersection(l);
                        Point pI2 = line2.getPointIntersection(l);
                        if((pI1.getName() == "1" && line1.isPointBeInTheLine(pI1) && l.isPointBeInTheLine(pI1)) ||
                           (pI2.getName() == "1" && line2.isPointBeInTheLine(pI2) && l.isPointBeInTheLine(pI2)))
                            return "Ошибка передвижения. Точка не должна выходить за рамки определенной области.";
                    }
                }
            }
        }


        //отрисовка точки
        listPoints[indexPo].setPoint(po.getName(), po.getNum(), po.getX(), po.getY(), po.getZ());
        listPoints[indexPo].deleteNum();
        help.drawPoint(&listPoints[indexPo], listPoints[indexPo].getColor());

        list.clear();



        //заменяем точку в плоскостях
        for(int i = 0; i < listFlats.length(); i++){
            list = listFlats[i].getListPoint();
            for(int j = 0; j < list.length(); j++){
                if(list[j].getName() == namePoint){
                    listFlats[i].addPointToFlat(po);
                }
            }
        }

        //перерисовываем прямые
        bool needToRepaint;
        for(int i = 0; i < listLines.length(); i++){
            list = listLines[i].getLine();
            needToRepaint = false;
            for(int j = 0; j < list.length(); j++){
                if(namePoint == list[j].getName()) needToRepaint = true;
            }
            if(needToRepaint){
                if(listLines[i].isPointInLine(po) || listLines[i].getP1().getName() == po.getName() || listLines[i].getP2().getName() == po.getName()){ // если точка осталась на прямой после сдвига
                    listLines[i].addPointToLine(po);
                    // обновляем прямоую, что при добавлении точки и ее движении, она отрисовывалась по новой прямой, а не по старой
                    list = listLines[i].getLine();
                    listLines[i].setLinear(listLines[i].getName(), listLines[i].getP1(), listLines[i].getP2(), listLines[i].getNum());
                    for(int j = 0; j < list.length(); j++){
                        listLines[i].addPointToLine(list[j]);
                    }
                    //
                    if(namePoint == listLines[i].getP1().getName()  ||  namePoint == listLines[i].getP2().getName()){
                        listLines[i].deleteNum();
                        help.drawLine(&listLines[i]);
                    }
                }
                else { // если точка уже не на прямой
                    listLines[i].delPointWithLine(po);
                    listLines[i].deleteNum();
                    help.drawLine(&listLines[i]);
                }

            }
        }
    }

    glDeleteLists(figure,1);
    figure = help.drawFigure(listFlats);

    updateGL();


    if(movePointName == namePoint){
        steps.update(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    }
    else{
        PossibleStep st;
        st.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
        steps.append(st);
        movePointName = namePoint;
    }

    return "";
}

void OGL::clear()
{

    m_xRotate = 0;
    m_yRotate = 0;

    movePointName = "";

    lastPointSootnoshenie[0] = "0";
    lastPointSootnoshenie[1] = "0";
    lastMoveLine = 0;

    glDeleteLists(figure,1);
    for(int i = 0; i < 26; i++){
        for(int j = 0; j < 10; j++){
            nnp[i][j] = "free";
        }
    }
    for(int i = 0; i < listLines.length(); i++){
        listLines[i].deleteNum();
    }
    for(int i = 0; i < listPoints.length(); i++){
        listPoints[i].deleteNum();
    }
    for(int i = 0; i < paintAnswer.length(); i++){
        paintAnswer[i].deleteNum();
    }

    glDeleteLists(graphicAnswer.getN(), 1);

    graphicAnswer.clear();
    listLines.clear();
    listPoints.clear();
    paintAnswer.clear();
    listFlats.clear();
    listAnswer.clear();
    PointName.clear();
    PointX.clear();
    PointY.clear();
    PointZ.clear();
    LineName.clear();
    LineList.clear();
    LinePFL1.clear();
    LinePFL2.clear();
    TaskType.clear();
    FlatList.clear();
    Answer.clear();

    steps.clear();

    report = "";

    updateGL();
}

QString OGL::setRedactTask()
{
    redactTask = true;
    return "";
}

QString OGL::unsetRedactTask()
{
    redactTask = false;
    return "";
}

QString OGL::isTrueAnswer(QString answer)
{
    QString ans = answer;
    QString name;
    Point point;
    bool good = false;
    QStringList sl = answer.split(" ", QString::SkipEmptyParts);
    for(int k = 0; k < sl.length(); k++){
        name = sl[k];

        for(int i = 0; i < listPoints.length(); i++){
            if(name == listPoints[i].getName()) point = listPoints[i];
        }
        good = false;
        for(int i = 0; i < listAnswer.length(); i++){
            if(point.getX() + 0.0001 > listAnswer[i].getX() && point.getX() - 0.0001 < listAnswer[i].getX() &&
                    point.getY() + 0.0001 > listAnswer[i].getY() && point.getY() - 0.0001 < listAnswer[i].getY() &&
                    point.getZ() + 0.0001 > listAnswer[i].getZ() && point.getZ() - 0.0001 < listAnswer[i].getZ() &&
                    listAnswer[i].getName() == "may")
            {
                listAnswer[i].setPoint("dont", listAnswer[i].getNum(), listAnswer[i].getX(), listAnswer[i].getY(), listAnswer[i].getZ());
                good = true;
            }
        }
        if(!good){
            for(int j = 0; j < listAnswer.length(); j++){
                listAnswer[j].setPoint("may", listAnswer[j].getNum(), listAnswer[j].getX(), listAnswer[j].getY(), listAnswer[j].getZ());
            }
            report += "\r\nОтвет "+ans+" указан неверно.";

            bool regB = false;
            QString returnStr = "Не верно!";
            QRegExp rx(".*[А-Я]+.*");
            if(rx.exactMatch(ans)){
                returnStr += "\r\nИспользуйте английский алфавит.";
                regB = true;
            }
            rx.setPattern(".*[a-z]+.*");
            if(rx.exactMatch(ans)){
                returnStr += "\r\nИспользуйте заглавные буквы.";
                regB = true;
            }
            if(regB){
                returnStr += "\r\nПримеры: 'L'; 'A L1'; 'S1 B G2'.";
            }
            return returnStr;

        }
    }
    for(int k = 0; k < listAnswer.length(); k++){
        if(listAnswer[k].getName() == "may"){
            for(int j = 0; j < listAnswer.length(); j++){
                listAnswer[j].setPoint("may", listAnswer[j].getNum(), listAnswer[j].getX(), listAnswer[j].getY(), listAnswer[j].getZ());
            }
            report += "\r\nОтвет "+ans+" указан неверно";
            bool regB = false;
            QString returnStr = "Не верно!";
            QRegExp rx(".*[А-Я]+.*");
            if(rx.exactMatch(ans)){
                returnStr += "\r\nИспользуйте английский алфавит.";
                regB = true;
            }
            rx.setPattern(".*[a-z]+.*");
            if(rx.exactMatch(ans)){
                returnStr += "\r\nИспользуйте заглавные буквы.";
                regB = true;
            }
            if(regB){
                returnStr += "\r\nПримеры: 'L'; 'A L1'; 'S1 B G2'.";
            }
            return returnStr;
        }

    }
    for(int j = 0; j < listAnswer.length(); j++){
        listAnswer[j].setPoint("may", listAnswer[j].getNum(), listAnswer[j].getX(), listAnswer[j].getY(), listAnswer[j].getZ());
    }
    report += "\r\nОтвет "+ans+" указан верно";
    return "Верно!";
}

void OGL::openTask(QString PointName, QString PointX, QString PointY, QString PointZ, QString LineName, QString LineList, QString LinePFL1, QString LinePFL2, QString TaskType, QString FlatList, QString Answer)
{
    this->TaskType = TaskType;
    this->Answer = Answer;
    int kol;
    QString temp;
    //получаем число точек условия
    for(int i = 0; ;i++){
        if(one_of_line(PointName, i) == " "){
            break;
        }
        temp += one_of_line(PointName, i);
    }
    PointName.remove(0, temp.length() + 1);
    kol = temp.toInt();
    //

    QString name;
    double x;
    double y;
    double z;
    Point point;

    double color[3] = {0,0,0};
    int indexColor = 0;

    //получаем цвет имен точек

    for(int i = 0; ;i++){
        if(one_of_line(PointName, i) != "(" && one_of_line(PointName, i) != " "){
            colorNnp.setRgb(255,0,0);
            break;
        }
        if(one_of_line(PointName, i) == "("){ //получаем цвет
            i++;
            QString col = "";
            for(; i < PointName.length(); i++){
                if(one_of_line(PointName, i) == ";" || one_of_line(PointName, i) == ")"){
                    color[indexColor] = col.toDouble();
                    indexColor++;
                    col = "";
                    if(one_of_line(PointName, i) == ")"){
                        i++;
                        col.clear();
                        colorNnp.setRgb( (int)color[0], (int)color[1], (int)color[2]);
                        break;
                    }
                }
                else col += one_of_line(PointName, i);
            }
        }
        if(one_of_line(PointName, i) == " " && indexColor > 0){
            PointName.remove(0,i+1);
            break;
        }
    }

    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
    indexColor = 0;

    //заполнение listPoints
    for(int i = 0; i < kol; i++){
        name.clear();
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
        indexColor = 0;
        int indLength = 0;
        for(int j = 0; j < PointName.length(); j++){
            if(one_of_line(PointName, j) == "("){ //получаем цвет
                j++;
                QString col = "";
                for(; j < PointName.length(); j++){
                    if(one_of_line(PointName, j) == ";" || one_of_line(PointName, j) == ")"){
                        color[indexColor] = col.toDouble();
                        indexColor++;
                        col = "";
                        if(one_of_line(PointName, j) == ")"){
                            j++;
                            col.clear();
                            break;
                        }
                    }
                    else col += one_of_line(PointName, j);
                }
            }
            if(one_of_line(PointName, j) == " "){
                indLength = j;
                break;
            }
            name += one_of_line(PointName, j);
            indLength = j;
        }
        PointName.remove(0, indLength + 1);

        temp.clear();
        for(int j = 0; j < PointX.length(); j++){
            if(one_of_line(PointX, j) == " "){
                break;
            }
            temp += one_of_line(PointX, j);
        }
        PointX.remove(0, temp.length() + 1);
        x = temp.toDouble();

        temp.clear();
        for(int j = 0; j < PointY.length(); j++){
            if(one_of_line(PointY, j) == " "){
                break;
            }
            temp += one_of_line(PointY, j);
        }
        PointY.remove(0, temp.length() + 1);
        y = temp.toDouble();

        temp.clear();
        for(int j = 0; j < PointZ.length(); j++){
            if(one_of_line(PointZ, j) == " "){
                break;
            }
            temp += one_of_line(PointZ, j);
        }
        PointZ.remove(0, temp.length() + 1);
        z = temp.toDouble();

        point.setPoint(getNameNextPoint(name), 0, x, y, z);
        point.setColor(color[0], color[1], color[2]);
        help.drawPoint(&point, point.getColor());

        listPoints.append(point);
        temp.clear();
    }

    //получаем кол-во прямых условия
    for(int i = 0; ;i++){
        if(one_of_line(LineName, i) == " "){
            break;
        }
        temp += one_of_line(LineName, i);
    }
    LineName.remove(0, temp.length() + 1);
    kol = temp.toInt();

    //получаем цвет прямых
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
    indexColor = 0;
    for(int i = 0; ;i++){
        if(one_of_line(LineName, i) != "(" && one_of_line(LineName, i) != " "){
            help.setColorForLine(0,0,0);
            break;
        }
        if(one_of_line(LineName, i) == "("){ //получаем цвет
            i++;
            QString col = "";
            for(; i < LineName.length(); i++){
                if(one_of_line(LineName, i) == ";" || one_of_line(LineName, i) == ")"){
                    color[indexColor] = col.toDouble();
                    indexColor++;
                    col = "";
                    if(one_of_line(LineName, i) == ")"){
                        i++;
                        col.clear();
                        help.setColorForLine(color[0], color[1], color[2]);
                        break;
                    }
                }
                else col += one_of_line(LineName, i);
            }
        }
        if(one_of_line(LineName, i) == " " && indexColor > 0){
            LineName.remove(0,i+1);
            break;
        }
    }


    Point p1;
    Point p2;
    Linear l;
    // получаем кол-во первых прямых

    temp = "";
    for(int i = 0; ;i++){
        if(one_of_line(LineList, i) == " "){
            break;
        }
        temp += one_of_line(LineList, i);
    }
    LineList.remove(0, temp.length() + 1);
    colFirstLine = temp.toInt();

    //заполнение listLines
    for(int i = 0; i < kol; i++){
        name.clear();
        for(int j = 0; j < LineName.length(); j++){
            if(one_of_line(LineName, j) == " "){
                break;
            }
            name += one_of_line(LineName, j);
        }
        LineName.remove(0, name.length() + 1);
        //p1
        temp.clear();
        for(int j = 0; j < LinePFL1.length(); j++){
            if(one_of_line(LinePFL1, j) == " "){
                break;
            }
            temp += one_of_line(LinePFL1, j);
        }
        LinePFL1.remove(0, temp.length() + 1);
        p1 = findPoint(temp);
        //p2
        temp.clear();
        for(int j = 0; j < LinePFL2.length(); j++){
            if(one_of_line(LinePFL2, j) == " "){
                break;
            }
            temp += one_of_line(LinePFL2, j);
        }
        LinePFL2.remove(0, temp.length() + 1);
        p2 = findPoint(temp);

        l.setLinear(name, p1, p2, 0);
        help.drawLine(&l);
        listLines.append(l);

        temp.clear();
        while(one_of_line(LineList, 0) != ";"){
            temp.clear();
            for(int j = 0; j < LineList.length(); j++){
                if(one_of_line(LineList, j) == " ") break;
                temp += one_of_line(LineList, j);
            }
            LineList.remove(0, temp.length() + 1);
            for(int j = 0; j < listPoints.length(); j++){
                if(temp == listPoints[j].getName()){
                    listLines[i].addPointToLine(listPoints[j]);
                    break;
                }
            }
        }
        LineList.remove(0, 1);
        listLines[i].deleteNum();
        help.drawLine(&listLines[i]);
    }

    //получаем кол-во плоскостей условия
    temp.clear();
    for(int i = 0; ;i++){
        if(one_of_line(FlatList, i) == " "){
            break;
        }
        temp += one_of_line(FlatList, i);
    }
    FlatList.remove(0, temp.length() + 1);
    kol = temp.toInt();
    QList<Point> lp;
    Flat flat;
    //заполнение плоскостей
    for(int i = 0; i < kol; i++){
        lp.clear();
        while(one_of_line(FlatList, 0) != ";"){
            temp.clear();
            for(int j = 0; j < FlatList.length(); j++){
                if(one_of_line(FlatList, j) == " "){
                    break;
                }
                temp += one_of_line(FlatList, j);
            }
            FlatList.remove(0, temp.length() + 1);
            for(int j = 0; j < listPoints.length(); j++){
                if(temp == listPoints[j].getName()){
                    lp.append(listPoints[j]);
                    break;
                }
            }
        }
        FlatList.remove(0, 1);
        flat.setFlat(lp);
        listFlats.append(flat);
    }

    if(!redactTask) colVershin = listPoints.length();
    else colVershin = 0;
    if(!redactTask) colLine = listLines.length();
    else colLine = 0;
    figure = help.drawFigure(listFlats);

    //заполнение ответа

    while(true){
        if(PointName.length() == 0){
            break;
        }
        name.clear();
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
        indexColor = 0;
        int indLength = 0;
        for(int j = 0; j < PointName.length(); j++){
            if(one_of_line(PointName, j) == "("){ //получаем цвет
                j++;
                QString col = "";
                for(; j < PointName.length(); j++){
                    if(one_of_line(PointName, j) == ";" || one_of_line(PointName, j) == ")"){
                        color[indexColor] = col.toDouble();
                        indexColor++;
                        col = "";
                        if(one_of_line(PointName, j) == ")"){
                            j++;
                            col.clear();
                            break;
                        }
                    }
                    else col += one_of_line(PointName, j);
                }
            }
            if(one_of_line(PointName, j) == " "){
                indLength = j;
                break;
            }
            name += one_of_line(PointName, j);
            indLength = j;
        }
        PointName.remove(0, indLength + 1);
        if(color[0] == 0 && color[1] == 0 && color[2] == 0){ //стандартный цвет точки в ответе - зеленый
            color[1] = 1;
        }

        temp.clear();
        for(int j = 0; j < PointX.length(); j++){
            if(one_of_line(PointX, j) == " "){
                break;
            }
            temp += one_of_line(PointX, j);
        }
        PointX.remove(0, temp.length() + 1);
        x = temp.toDouble();

        temp.clear();
        for(int j = 0; j < PointY.length(); j++){
            if(one_of_line(PointY, j) == " "){
                break;
            }
            temp += one_of_line(PointY, j);
        }
        PointY.remove(0, temp.length() + 1);
        y = temp.toDouble();

        temp.clear();
        for(int j = 0; j < PointZ.length(); j++){
            if(one_of_line(PointZ, j) == " "){
                break;
            }
            temp += one_of_line(PointZ, j);
        }
        PointZ.remove(0, temp.length() + 1);
        z = temp.toDouble();

        point.setPoint("may", 0, x, y, z);
        listAnswer.append(point);
        if(findPoint(name).getName() == "error" && redactTask){
            point.setPoint(name, 0, x, y, z);
            help.drawPoint(&point, color);
            paintAnswer.append(point);
        }
    }

    updateGL();

    steps.clear();
    PossibleStep s; //save step
    s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);

    report = "Начало";
}

bool OGL::saveFigure()
{
    PointName.clear();
    PointX.clear();
    PointY.clear();
    PointZ.clear();
    LineName.clear();
    LineList.clear();
    LinePFL1.clear();
    LinePFL2.clear();
    TaskType.clear();
    FlatList.clear();

    char a[5];
    itoa(listPoints.length(), a, 10);
    PointName = a;
    PointName += " ";
    PointName += "("+d_to_str(colorNnp.red()/1.)+";"+d_to_str(colorNnp.green()/1.)+";"+d_to_str(colorNnp.blue()/1.)+")"+ " ";
    for(int i = 0; i < listPoints.length(); i++){
        double *color = listPoints[i].getColor();
        PointName += listPoints[i].getName() +
                "("+d_to_str(color[0])+";"+d_to_str(color[1])+";"+d_to_str(color[2])+")"+ " ";
        PointX += d_to_str(listPoints[i].getX()) + " ";
        PointY += d_to_str(listPoints[i].getY()) + " ";
        PointZ += d_to_str(listPoints[i].getZ()) + " ";
    }

    itoa(listLines.length(), a, 10);
    LineName = a;
    LineName += " ";
    double *color = help.getColorForLine();
    LineName += "("+d_to_str(color[0])+";"+d_to_str(color[1])+";"+d_to_str(color[2])+")"+ " ";
    LineList += LineList.number(colFirstLine) + " ";
    QList<Point> l;
    for(int i = 0; i < listLines.length(); i++){
        l = listLines[i].getLine();
        LineName += listLines[i].getName() + " ";
        for(int j = 0; j < l.length(); j++){
            LineList += l[j].getName() + " ";
        }
        LineList += ";";
        LinePFL1 += listLines[i].getPFGPFL1().getName() + " ";
        LinePFL2 += listLines[i].getPFGPFL2().getName() + " ";
    }
    l.clear();
    itoa(listFlats.length(), a, 10);
    FlatList = a;
    FlatList += " ";
    for(int i = 0; i < listFlats.length(); i++){
        l = listFlats[i].getListPoint();
        for(int j = 0; j < l.length(); j++){
            FlatList += l[j].getName() + " ";
        }
        FlatList += ";";
    }
    if(listPoints.length() == 0) return false;
    else return true;
}

QString OGL::getSaveTask(QString *PointName_, QString *PointX_, QString *PointY_, QString *PointZ_, QString *LineName_, QString *LineList_, QString *LinePFL1_, QString *LinePFL2_, QString *TaskType_, QString *FlatList_, QString *Answer_)
{
    QStringList la = Answer.split(" ", QString::SkipEmptyParts);
    if(la.length() > 0 && la[0] == "NoFlat") la.removeFirst();
    //проверка ответа
    int count = 0;
    bool namePointIsUsed = false;
    for(int j = 0; j < la.length(); j++){
        namePointIsUsed = false;
        for(int i = 0; i < listPoints.length(); i++){
           if(la[j] == listPoints[i].getName()){
               count++;
               namePointIsUsed = true;
           }
        }
        if(redactTask){
            for(int i = 0; i < paintAnswer.length(); i++){
                if(la[j] == paintAnswer[i].getName() && !namePointIsUsed) count++;
                if(la[j] == paintAnswer[i].getName() && namePointIsUsed){
                    if(QMessageBox::Ok == QMessageBox::information(this, " ", "Точка "+la[j]+" ранее была одной из точек ответа. \nТеперь же в ответ попадет новая точка, добавленная при текущем редактировании. \nПродолжить сохранение?", QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel));
                    else {
                        return "processed";
                    }
                }
            }
        }
    }

    if(count != la.length()) return "Проверьте правильность введенных точек!";

    for(int j = 0; j < la.length(); j++){
        namePointIsUsed = false;
        for(int i = 0; i < listPoints.length(); i++){
            if(la[j] == listPoints[i].getName()){
                double *color = listPoints[i].getColor();
                PointName += listPoints[i].getName() +
                        "("+d_to_str(color[0])+";"+d_to_str(color[1])+";"+d_to_str(color[2])+")"+ " ";
                PointX += d_to_str(listPoints[i].getX()) + " ";
                PointY += d_to_str(listPoints[i].getY()) + " ";
                PointZ += d_to_str(listPoints[i].getZ()) + " ";
                namePointIsUsed = true;
            }
        }
        if(redactTask){
            for(int i = 0; i < paintAnswer.length(); i++){
                if(la[j] == paintAnswer[i].getName() && !namePointIsUsed){
                    PointName += paintAnswer[i].getName() + " ";
                    PointX += d_to_str(paintAnswer[i].getX()) + " ";
                    PointY += d_to_str(paintAnswer[i].getY()) + " ";
                    PointZ += d_to_str(paintAnswer[i].getZ()) + " ";
                }
            }
        }
    }

    *PointName_ = PointName;
    *PointX_ = PointX;
    *PointY_ = PointY;
    *PointZ_ = PointZ;
    *LineName_ = LineName;
    *LineList_ = LineList;
    *LinePFL1_ = LinePFL1;
    *LinePFL2_ = LinePFL2;
    *TaskType_ = TaskType;
    *FlatList_ = FlatList;
    *Answer_ = Answer;
    return "";
}

QString OGL::getAnswer()
{
    if(Answer.indexOf("NoFlat") == -1) return Answer;
    else{
        QString s = Answer;
        s.remove("NoFlat");
        return s;
    }
}

QString OGL::setAnswer(QString typeAnswer, QString answer1, QString answer2)
{
    lastPointSootnoshenie[0] = "0";
    lastPointSootnoshenie[1] = "0";
    lastMoveLine = -1;
    if("Авторский ответ по точкам" == typeAnswer){
        return points(answer1, answer2);
    }
    if("Пересечение прямой и плоскости" == typeAnswer){
        QString str = intersecPlanWhithLine(answer1, answer2);
        if(str == ""){
            PossibleStep s; //save step
            s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
            steps.append(s);
        }
        return str;
    }
    if("Пересечение двух плоскостей" == typeAnswer){
        QString str = intersecPlanWhithPlane(answer1, answer2);
        if(str == ""){
            PossibleStep s; //save step
            s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
            steps.append(s);
        }
        return str;
    }
    if("Секущая плоскость" == typeAnswer){
        QString str = cuttingPlane(answer1);
        if(str == ""){
            PossibleStep s; //save step
            s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
            steps.append(s);
        }
        return str;
    }
    return "Ошибка";
}

QString OGL::updateAnswerCuttingPlane(QString answer)
{
    QList<QString> la = splitStringPoints(answer);
    if(la[0].length() > 2) return la[0];
    QList<Point> ps;
    for(int i = 0; i < la.length(); i++){
        for(int j = 0; j < listPoints.length(); j++){
            if(la[i] == listPoints[j].getName()) ps.append(listPoints[j]);
        }
    }

    //отрисовка плоскости
    Flat f;
    f.setFlat(ps);

    glDeleteLists(graphicAnswer.getN(), 1);
    graphicAnswer.clear();
    graphicAnswer.append(f, ColorFlat(0.5, 1, 0, 0));
    graphicAnswer.setN(help.drawFigure(graphicAnswer.getFlats(), graphicAnswer.getColorFlats()));

    Answer = answer;

    PossibleStep s; //save step
    s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);

    updateGL();
    return "";
}

QString OGL::updateIntersecTwoFlats(QString answer1, QString answer2)
{
    QList<QString> la1 = splitStringPoints(answer1);
    if(la1[0].length() > 2) return la1[0];
    QList<Point> ps1;
    for(int i = 0; i < la1.length(); i++){
        for(int j = 0; j < listPoints.length(); j++){
            if(la1[i] == listPoints[j].getName()) ps1.append(listPoints[j]);
        }
    }

    QList<QString> la2 = splitStringPoints(answer2);
    if(la2[0].length() > 2) return la2[0];
    QList<Point> ps2;
    for(int i = 0; i < la2.length(); i++){
        for(int j = 0; j < listPoints.length(); j++){
            if(la2[i] == listPoints[j].getName()) ps2.append(listPoints[j]);
        }
    }

    //отрисовка плоскости
    Flat f1, f2;
    f1.setFlat(ps1);
    f2.setFlat(ps2);

    glDeleteLists(graphicAnswer.getN(), 1);
    graphicAnswer.clear();
    graphicAnswer.append(f1, ColorFlat(0.5, 1, 0, 0));
    graphicAnswer.append(f2, ColorFlat(1, 0.5, 0, 1));
    graphicAnswer.setN(help.drawFigure(graphicAnswer.getFlats(), graphicAnswer.getColorFlats()));

    PossibleStep s; //save step
    s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);

    updateGL();
    return "";
}

QString OGL::previousStep()
{
    if(steps.previousStep(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer)){
        report += "\r\nДействие отменено.";
    }

    lastPointSootnoshenie[0] = "0";
    lastPointSootnoshenie[1] = "0";
    lastMoveLine = -1;

    movePointName = "";
    repaintAll();
    updateGL();

    return "";
}

QString OGL::nextStep()
{
    if(steps.nextStep(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer)){
        report += "\r\nДействие возобновлено.";
    }

    lastPointSootnoshenie[0] = "0";
    lastPointSootnoshenie[1] = "0";
    lastMoveLine = -1;

    movePointName = "";
    repaintAll();
    updateGL();

    return "";
}

void OGL::firstStep()
{
    steps.firstStep(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);

    lastPointSootnoshenie[0] = "0";
    lastPointSootnoshenie[1] = "0";
    lastMoveLine = -1;

    movePointName = "";
    repaintAll();
    updateGL();
}

void OGL::lastStep()
{
    steps.lastStep(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);

    lastPointSootnoshenie[0] = "0";
    lastPointSootnoshenie[1] = "0";
    lastMoveLine = -1;

    movePointName = "";
    repaintAll();
    updateGL();
}

QString OGL::setColorOfPoint(QString name, double r, double g, double b)
{
    int i = 0;
    for(i = 0; i < listPoints.length(); i++){
        if(listPoints[i].getName() == name){
            listPoints[i].setColor(r,g,b);
            listPoints[i].deleteNum();
            help.drawPoint(&listPoints[i], listPoints[i].getColor());
        }
    }
    updateGL();
    if(i == listPoints.length()) return "Ошибка 000001";
    return "";
}

QString OGL::setStandartColorToPoint(double r, double g, double b)
{
    Point::rr = r;
    Point::gg = g;
    Point::bb = b;

    for(int i = 0; i < listPoints.length(); i++){
        listPoints[i].setStandartColor();
        listPoints[i].deleteNum();
        help.drawPoint(&listPoints[i], listPoints[i].getColor());
    }
    updateGL();
    return "";
}

QString OGL::setStandartColorToLine(double r, double g, double b)
{
    help.setColorForLine(r, g, b);
    for(int i = 0; i < listLines.length(); i++){
        listLines[i].deleteNum();
        help.drawLine(&listLines[i]);
    }
    updateGL();
    return "";
}

QString OGL::setStandartColorToText(QColor color)
{
    colorNnp = color;
    updateGL();
    return "";
}

void OGL::rotateSplash()
{
    static double x = 0.9;
    static double y = 0.78;
    m_xRotate += 180 *(GLfloat)(y)/height();
    m_yRotate +=180*(GLfloat)(x)/width();
    updateGL();
}

QString OGL::tiltPrism(int degree, QString os, double height)
{   
    lastPointSootnoshenie[0] = "0";
    lastPointSootnoshenie[1] = "0";
    lastMoveLine = -1;

    double x = 0, y = 0, z = 0;
    double xHeight = 0;
    for(int i = 0 ; i < listPoints.length(); i++){
        if(listPoints[i].getY() < -0.00001 || listPoints[i].getY() > 0.00001){
            xHeight = height/listPoints[i].getY();
            break;
        }
    }
    if(height == -1) xHeight = 1;
    QList<Point> list;
    for(int k = 0; k < listPoints.length(); k++){
        y = help.abs(listPoints[k].getY())*xHeight;
        if(os == "x"){
            x = y*sin(degree*grad);
            y = y*cos(degree*grad);
        }
        else {
            z = y*sin(degree*grad);
            y = y*cos(degree*grad);
        }
        if(listPoints[k].getY() < 0) listPoints[k].setPoint(listPoints[k].getName(), listPoints[k].getNum(), listPoints[k].getX() + x, - y, listPoints[k].getZ() + z);
        else listPoints[k].setPoint(listPoints[k].getName(), listPoints[k].getNum(), listPoints[k].getX() - x,  y, listPoints[k].getZ() - z);

        listPoints[k].deleteNum();
        help.drawPoint(&listPoints[k], listPoints[k].getColor());


        //заменяем точку в плоскостях
        for(int i = 0; i < listFlats.length(); i++){
            list = listFlats[i].getListPoint();
            for(int j = 0; j < list.length(); j++){
                if(list[j].getName() == listPoints[k].getName()){
                    listFlats[i].addPointToFlat(listPoints[k]);
                }
            }
        }

        //перерисовываем прямые
        bool needToRepaint;
        for(int i = 0; i < listLines.length(); i++){
            list = listLines[i].getLine();
            needToRepaint = false;
            for(int j = 0; j < list.length(); j++){
                if(listPoints[k].getName() == list[j].getName()) {
                    needToRepaint = true;
                    break;
                }
            }
            if(needToRepaint){
                listLines[i].addPointToLine(listPoints[k]);
                listLines[i].deleteNum(); //
                help.drawLine(&listLines[i]); //
                list.clear();
                list = listLines[i].getLine();
                listLines[i].setLinear(listLines[i].getName(), listLines[i].getP1(), listLines[i].getP2(), listLines[i].getNum());
                for(int j = 0; j < list.length(); j++){
                    listLines[i].addPointToLine(list[j]);
                }
            }
        }
    }

    glDeleteLists(figure,1);


    figure = help.drawFigure(listFlats);

    if(height != -1){
        steps.update(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    }
    else{
        PossibleStep st;
        st.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
        steps.append(st);
    }

    updateGL();
    return "";
}

QString OGL::getReport()
{
    return report;
}

void OGL::addToReport(QString text)
{
    report += text;
}

void OGL::repaintAll()
{
    glDeleteLists(figure, 1);
    for(int i = 0; i < listPoints.length(); i++){
        listPoints[i].deleteNum();
    }
    for(int i = 0; i < listLines.length(); i++){
        listLines[i].deleteNum();
    }

    for(int i = 0; i < listPoints.length(); i++){
        help.drawPoint(&listPoints[i], listPoints[i].getColor());
    }
    for(int i = 0; i < listLines.length(); i++){
        help.drawLine(&listLines[i]);
    }

    graphicAnswer.setN(help.drawFigure(graphicAnswer.getFlats(), graphicAnswer.getColorFlats()));

    figure = help.drawFigure(listFlats);
    updateGL();
}

void OGL::setFromDigest(DigestOgl d)
{
    colVershin = d.colVershin;
    colLine = d.colLine;
    colFirstLine = d.colFirstLine;
    colorNnp = d.colorNnp;
    Answer = d.Answer;
    listAnswer = d.listAnswer;
    steps = d.steps;
    steps.getCurrentStep(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);

    repaintAll();
}

DigestOgl OGL::getDigest()
{
    DigestOgl d;

    d.colVershin = colVershin;
    d.colLine = colLine;
    d.colFirstLine = colFirstLine;
    d.colorNnp = colorNnp;
    d.Answer = Answer;
    d.listAnswer = listAnswer;
    d.steps = steps;
    d.report = report;

    return d;
}

void OGL::hidePointName()
{
    for(int i = 0; i < listPoints.length(); i++){
        listPoints[i].setName("");
    }
}

void OGL::paintLogo()
{
    PaintLogo pl;
    pl.paintLogo(&listLines);
}

void OGL::lookingForIntersecWithLineIn(Linear line)
{
    int l1 = -1;

    for(int i = 0; i < listLines.length(); i++){
        if(line.getName() == listLines[i].getName()){
            l1 = i;
            break;
        }
    }

    if(l1 == -1) return;
    Point p;
    for(int i = 0; i < listLines.length(); i++){
        p = listLines[l1].getPointIntersection(listLines[i]);
        if(p.getName() == "1" && listLines[i].isPointBeInTheLine(p) && listLines[l1].isPointBeInTheLine(p)){
            bool flag;
            flag = true; //такой точки нет
            for(int j = 0; j < listPoints.length(); j++){
                if(p.isEqual(listPoints[j])){
                    flag = false; //такая точка есть
                    break;
                }
            }
            if(flag){
                p.setName(getNameNextPoint("0"));
                help.drawPoint(&p, p.getColor());
                listPoints.append(p);
                listLines[l1].addPointToLine(p);
                listLines[i].addPointToLine(p);
                listLines[l1].deleteNum();
                help.drawLine(&listLines[l1]);
                listLines[i].deleteNum();
                help.drawLine(&listLines[i]);
            }
        }
    }
}

QString OGL::addLS(QString p, QString from, QString to, QString p1, QString p2)
{
    if(from == to) return "Указана одна и та же точка ("+from+").";
    if(p1 == p2) return "Указана одна и та же точка ("+p1+").";
    Point pP, pFrom, pTo, pP1, pP2;
    for(int i = 0; i < listPoints.length(); i++){
        if(listPoints[i].getName() == p) pP = listPoints[i];
        if(listPoints[i].getName() == from) pFrom = listPoints[i];
        if(listPoints[i].getName() == to) pTo = listPoints[i];
        if(listPoints[i].getName() == p1) pP1 = listPoints[i];
        if(listPoints[i].getName() == p2) pP2 = listPoints[i];
    }
    Linear l;
    bool pointInLine = false;
    int currLine = 0;
    for(int i = 0; i < listLines.length(); i++){
        if(listLines[i].isPointInLine(pFrom) && listLines[i].isPointInLine(pTo)){
            listLines[i].addPointToLine(pFrom);
            listLines[i].addPointToLine(pTo);
            l = listLines[i];
            currLine = i;
            pointInLine = true;
            break;
        }
    }
    if(!pointInLine) l.setLinear(pFrom.getName()+pTo.getName(), pFrom, pTo, 0);
    if(!l.isPointInLine(pP)) return "Точка " + p + " не лежит на прямой " + from + " " + to;

    lastPointSootnoshenie[0] = "0";
    lastPointSootnoshenie[1] = "0";
    lastMoveLine = -1;

    if(!pointInLine){
        help.drawLine(&l);
        listLines.append(l);
    }
    Point vFromTo, vP1P2;
    vFromTo.setPoint("", 0, pTo.getX()-pFrom.getX(), pTo.getY()-pFrom.getY(), pTo.getZ()-pFrom.getZ());
    vP1P2.setPoint("", 0, pP2.getX()-pP1.getX(), pP2.getY()-pP1.getY(), pP2.getZ()-pP1.getZ());
    double lenVFT = help.middleGeom3(vFromTo.getX(), vFromTo.getY(), vFromTo.getZ());
    double lenVP1P2 = help.middleGeom3(vP1P2.getX(), vP1P2.getY(), vP1P2.getZ());
    vFromTo.setPoint("", 0, vFromTo.getX()/lenVFT*lenVP1P2, vFromTo.getY()/lenVFT*lenVP1P2, vFromTo.getZ()/lenVFT*lenVP1P2);

    Point pNew;
    pNew.setPoint(getNameNextPoint("0"), 0, pP.getX()+vFromTo.getX(), pP.getY()+vFromTo.getY(), pP.getZ()+vFromTo.getZ());

    QString isEx = isPointExist(pNew);
    if(isEx != "false") return "В данной позиции уже имеется точка " + isEx + ".";

    help.drawPoint(&pNew, pNew.getColor());
    listPoints.append(pNew);
    listLines[currLine].addPointToLine(pNew);
    help.drawLine(&listLines[currLine]);

    PossibleStep s;
    s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);

    updateGL();
    report += "\r\nДобавлена точка (построение отрезка равного заданному) " + pNew.getName();

    return "";
}

void OGL::drawTrueAnswer()
{
    //отрисовка плоскости
    if(listAnswer.length() < 3 || Answer.indexOf("NoFlat") != -1) return;
    Flat f;
    f.setFlat(listAnswer);

    graphicAnswer.clear();
    graphicAnswer.append(f, ColorFlat(0.5, 1, 0, 0));
    graphicAnswer.setN(help.drawFigure(graphicAnswer.getFlats(), graphicAnswer.getColorFlats()));

    PossibleStep s; //save step
    s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);

    updateGL();
}

void OGL::setSizeFont(int size)
{
    sizeFont = size;
    updateGL();
}

void OGL::createImgBMP(QString path)
{
    this->grabFrameBuffer().save(path);
}

QString OGL::addPoint(QString s1, QString s2){
    lastPointSootnoshenie[0] = s1;
    lastPointSootnoshenie[1] = s2;
    if(s1 == s2) return "Указана одна и та же точка.";

    Point p, p1, p2;

    p1 = findPoint(s1);
    p2 = findPoint(s2);
    if(p1.getName() == "error" || p2.getName() == "error"){
        return "Ошибка поиска точек.";
    }
    bool findLine = false;
    for(int i = 0; i < listLines.length(); i++){
        if(listLines[i].isPointInLine(p1) && listLines[i].isPointInLine(p2)) {
            lastMoveLine = i;
            findLine = true;
            break;
        }
    }
    if(!findLine){
        addLine(p1.getName(), p2.getName());
        lastMoveLine = listLines.length() - 1;
    }
    p.setPoint(getNameNextPoint("0"), 0,(p1.getX() + p2.getX())/2,
               (p1.getY() + p2.getY())/2,
               (p1.getZ() + p2.getZ())/2);
    help.drawPoint(&p, p.getColor());
    listPoints.append(p);
    listLines[lastMoveLine].addPointToLine(p);
//
    Linear l;
    l.setLinear(p1.getName()+p2.getName(), p1, p2, 0);
    //найти точки в пространстве, которые лежат на ОТРЕЗКЕ sp1-sp2
    for(int j = 0; j < listPoints.length(); j++){
        if(l.isPointInLine(listPoints[j])){
            l.addPointToLine(listPoints[j]);
        }
    }
    bool del = true;
    QList<Point> list = l.getLine();
    for(int j = 0; j < list.length(); j++){
        if(list[j].getName() == p1.getName() || list[j].getName() == p2.getName()){
            del = !del;
            if(del == true){
                if(j < list.length() - 1) j++;
                else break;
            }
        }
        if(del){
            l.delPointWithLine(list[j]);
        }
    }

    //     и аккуратно добаить их на прямую, без лишних связей (линий)
    //слить в одну кучу точки на прямых  и их связи (том могут быть только одинвковые, но они не должны добавиться)
    listLines[lastMoveLine].addPointsToLine(l.getLine());
    listLines[lastMoveLine].appendListOtr(l.getListOtr());
    //найти точки пересечения этой прямой с другими прямыми

    for(int k = 0; k < listLines.length()-1; k++){
        p = listLines[k].getPointIntersection(l);//получение точки пересечения
        if(p.getName() == "1"){
            //сравнение координат точки с существующими точками
            bool flag2;
            flag2 = true; //такой точки нет
            for(int j = 0; j < listPoints.length(); j++){
                if(p.isEqual(listPoints[j])){
                    flag2 = false; //такая точка есть
                    break;
                }
            }
            if(flag2 && listLines[k].isPointBeInTheLine(p)){ //сравнение с концами прямой, лежит ли точка в пределах отрезка
                p.setName(getNameNextPoint("0"));
                help.drawPoint(&p, p.getColor());
                listPoints.append(p);
                listLines[lastMoveLine].addPointToLine(p);
                listLines[k].addPointToLine(p);
            }
        }
    }
 //

    listLines[lastMoveLine].setPFGPFL(p1, p2);
    listLines[lastMoveLine].deleteNum();
    help.drawLine(&listLines[lastMoveLine]);

    PossibleStep s;
        s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);

    updateGL();
    report += "\r\nДобавлена точка "+p.getName();
    return "";
}

QString OGL::renamePoint(QString from, QString to)
{
    bool noP1 = true;
    bool noP2 = true;
    for(int i = 0; i < listPoints.length(); i++){
        if(listPoints[i].getName() == from){
            noP1 = false;
        }
        if(listPoints[i].getName() == to){
            noP2 = false;
        }
    }
    if(noP1) return "Ошибка 000002";
    if(!noP2) return "Имя "+to+" занято.";

    QString err = getNameNextPoint(from);
    if(err.length() > 2) return err;

    QString name = getNameNextPoint(to);
    if(name.length() > 2) return name;

    for(int i = 0; i < listPoints.length(); i++){
        if(listPoints[i].getName() == from){
            listPoints[i].setName(to);
            break;
        }
    }
    for(int i = 0; i < listLines.length(); i++){
        listLines[i].renamePointInLine(from, to);
    }

    for(int i = 0; i < listFlats.length(); i++){
        listFlats[i].renamePointInFlat(from, to);
    }
    updateGL();

    PossibleStep s; //save step
        s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);

    return "";
}

void OGL::delPoint(QString s)
{
    getNameNextPoint(s);
    lastPointSootnoshenie[0] = "0";
    lastPointSootnoshenie[1] = "0";
    Point p;
    for(int i = 0; i < listPoints.length(); i++){
        if(s == listPoints[i].getName()){
            p = listPoints[i];
            listPoints[i].deleteNum();
            listPoints.removeAt(i);
            break;
        }
    }

    for(int i = 0; i < listLines.length(); i++){
        listLines[i].delPointWithLine(p);
        if(listLines[i].getLineLength() <= 1){
            listLines.removeAt(i);
            i--;
        }
        else {
            listLines[i].deleteNum();
            help.drawLine(&listLines[i]);
        }
    }
    updateGL();

    PossibleStep st; //save step
    st.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(st);

    report += "\r\nУдалена точка "+s;
}

void OGL::moveLastPoint(int v, QDoubleSpinBox *s1, QDoubleSpinBox *s2, QComboBox *box)
{
    if((listPoints.length() > colVershin) && (lastMoveLine != -1) && (listLines.length() > lastMoveLine)){
        double f = 5*v/500.;
        Point p;
        p = listLines[lastMoveLine].getPointForLine(f);
        listPoints[listPoints.length() - 1].deleteNum();
        p.setName(listPoints[listPoints.length() - 1].getName());
        help.drawPoint(&p, p.getColor());
      //задаем соотношение для спинбоксов
        Point p1, p2;
        p1 = listLines[lastMoveLine].getPFGPFL1();
        p2 = listLines[lastMoveLine].getPFGPFL2();

        if(((help.abs(p.getX()-p1.getX()) < help.abs(p1.getX()-p2.getX())) && (help.abs(p.getX()-p2.getX()) <= help.abs(p1.getX()-p2.getX()))) &&
           ((help.abs(p.getY()-p1.getY()) < help.abs(p1.getY()-p2.getY())) && (help.abs(p.getY()-p2.getY()) <= help.abs(p1.getY()-p2.getY()))) &&
           ((help.abs(p.getZ()-p1.getZ()) < help.abs(p1.getZ()-p2.getZ())) && (help.abs(p.getZ()-p2.getZ()) <= help.abs(p1.getZ()-p2.getZ()))) ){
            if(p1.getX() != p2.getX()) s2->setValue((p1.getX()-p2.getX())/(p1.getX()-p.getX()));
            else if(p1.getY() != p2.getY()) s2->setValue((p1.getY()-p2.getY())/(p1.getY()-p.getY()));
            else if(p1.getZ() != p2.getZ()) s2->setValue((p1.getZ()-p2.getZ())/(p1.getZ()-p.getZ()));
            else s2->setValue(1.);
            s1->setValue(1.);
            if(help.middleGeom3(help.abs(p.getX()-p1.getX()), help.abs(p.getY()-p1.getY()), help.abs(p.getZ()-p1.getZ()))
                    > help.middleGeom3(help.abs(p.getX()-p2.getX()), help.abs(p.getY()-p2.getY()), help.abs(p.getZ()-p2.getZ())))
                box->setCurrentText(p1.getName() + p.getName());
            else box->setCurrentText(p2.getName() + p.getName());
        }
        else if((help.abs(p.getX()-p1.getX()) >= help.abs(p.getX()-p2.getX())) &&
                (help.abs(p.getY()-p1.getY()) >= help.abs(p.getY()-p2.getY())) &&
                (help.abs(p.getZ()-p1.getZ()) >= help.abs(p.getZ()-p2.getZ()))  ){
            s2->setValue(1);
            if(p1.getX() != p2.getX()) s1->setValue(help.abs((p.getX()-p1.getX())/(p2.getX()-p1.getX())));
            else if(p1.getY() != p2.getY()) s1->setValue(help.abs((p.getY()-p1.getY())/(p2.getY()-p1.getY())));
            else if(p1.getZ() != p2.getZ()) s1->setValue(help.abs((p.getZ()-p1.getZ())/(p2.getZ()-p1.getZ())));
            else s1->setValue(0);
            if(help.middleGeom3(help.abs(p.getX()-p1.getX()), help.abs(p.getY()-p1.getY()), help.abs(p.getZ()-p1.getZ()))
                    > help.middleGeom3(help.abs(p.getX()-p2.getX()), help.abs(p.getY()-p2.getY()), help.abs(p.getZ()-p2.getZ())))
                box->setCurrentText(p1.getName() + p.getName());
            else box->setCurrentText(p2.getName() + p.getName());
        }
        else if((help.abs(p.getX()-p1.getX()) <= help.abs(p.getX()-p2.getX())) &&
                (help.abs(p.getY()-p1.getY()) <= help.abs(p.getY()-p2.getY())) &&
                (help.abs(p.getZ()-p1.getZ()) <= help.abs(p.getZ()-p2.getZ()))  ){
            s2->setValue(1);
            if(p1.getX() != p2.getX()) s1->setValue(help.abs((p.getX()-p2.getX())/(p1.getX()-p2.getX())));
            else if(p1.getY() != p2.getY()) s1->setValue(help.abs((p.getY()-p2.getY())/(p1.getY()-p2.getY())));
            else if(p1.getZ() != p2.getZ()) s1->setValue(help.abs((p.getZ()-p2.getZ())/(p1.getZ()-p2.getZ())));
            else s1->setValue(0);
            if(help.middleGeom3(help.abs(p.getX()-p1.getX()), help.abs(p.getY()-p1.getY()), help.abs(p.getZ()-p1.getZ()))
                    > help.middleGeom3(help.abs(p.getX()-p2.getX()), help.abs(p.getY()-p2.getY()), help.abs(p.getZ()-p2.getZ())))
                box->setCurrentText(p1.getName() + p.getName());
            else box->setCurrentText(p2.getName() + p.getName());
        }
//
        p1 = listLines[lastMoveLine].getP1();
        p2 = listLines[lastMoveLine].getP2();
        Linear l;
        l.setLinear("temp", p1, p2, 0);
        //найти точки в пространстве, которые лежат на ОТРЕЗКЕ sp1-sp2
        for(int j = 0; j < listPoints.length(); j++){
            if(l.isPointInLine(listPoints[j])){
                l.addPointToLine(listPoints[j]);
            }
        }
        bool del = true;
        QList<Point> list = l.getLine();
        for(int j = 0; j < list.length(); j++){
            if(list[j].getName() == p1.getName() || list[j].getName() == p2.getName()){
                del = !del;
                if(del == true){
                    if(j < list.length() - 1) j++;
                    else break;
                }
            }
            if(del){
                l.delPointWithLine(list[j]);
            }
        }
        QList<Point> lLine = l.getLine();
        for(int i = 0; i < lLine.length(); i++){
            listLines[lastMoveLine].addPointToLine(lLine[i]);
        }
//


        listPoints[listPoints.length() - 1].setPoint(p.getName(), p.getNum(), p.getX(), p.getY(), p.getZ());
        listLines[lastMoveLine].addPointToLine(p);

        listLines[lastMoveLine].deleteNum();
        help.drawLine(&listLines[lastMoveLine]);
        updateGL();
        steps.update(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    }
}

QString OGL::setSootnoshenie(QString otr, double s1, double s2)
{
    int p1 = 0;
    int p2 = 0;
    if(lastPointSootnoshenie[0] == "0" || lastPointSootnoshenie[1] == "0"){
        return "Для задания соотношения необходимо поставить точку.";
    }
    else {
        for(int i = 0; i < listPoints.length(); i++){
            if(listPoints[i].getName() == lastPointSootnoshenie[0]) p1 = i;
            if(listPoints[i].getName() == lastPointSootnoshenie[1]) p2 = i;
        }
    }
    QString tempToReport = "";
    if((listPoints.length() > colVershin) && (lastMoveLine != -1) && (listLines.length() > lastMoveLine)){
        if(listPoints[p1].getName() + getNameLastPoint() == otr){
            double x, y, z;
            x = listPoints[p2].getX() - listPoints[p1].getX();
            y = listPoints[p2].getY() - listPoints[p1].getY();
            z = listPoints[p2].getZ() - listPoints[p1].getZ();
            x = (x/s2)*s1;
            y = (y/s2)*s1;
            z = (z/s2)*s1;
            x = x + listPoints[p1].getX();
            y = y + listPoints[p1].getY();
            z = z + listPoints[p1].getZ();

            listPoints[listPoints.length() - 1].setPoint(listPoints[listPoints.length() - 1].getName(), 0, x, y, z);
            help.drawPoint(&listPoints[listPoints.length() - 1], listPoints[listPoints.length() - 1].getColor());

            listLines[lastMoveLine].addPointToLine(listPoints[listPoints.length() - 1]);
            listLines[lastMoveLine].deleteNum();
            help.drawLine(&listLines[lastMoveLine]);

            tempToReport = listPoints[p1].getName() + listPoints[p2].getName();
        }
        else {
            double x, y, z;
            x = listPoints[p1].getX() - listPoints[p2].getX();
            y = listPoints[p1].getY() - listPoints[p2].getY();
            z = listPoints[p1].getZ() - listPoints[p2].getZ();
            x = (x/s2)*s1;
            y = (y/s2)*s1;
            z = (z/s2)*s1;
            x = x + listPoints[p2].getX();
            y = y + listPoints[p2].getY();
            z = z + listPoints[p2].getZ();

            listPoints[listPoints.length() - 1].setPoint(listPoints[listPoints.length() - 1].getName(), 0, x, y, z);
            help.drawPoint(&listPoints[listPoints.length() - 1], listPoints[listPoints.length() - 1].getColor());

            listLines[lastMoveLine].addPointToLine(listPoints[listPoints.length() - 1]);
            listLines[lastMoveLine].deleteNum();
            help.drawLine(&listLines[lastMoveLine]);

            tempToReport = listPoints[p2].getName() + listPoints[p1].getName();
        }
    }

    PossibleStep st; //save step
    st.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(st);

    report += "\r\nЗадано соотношение "+otr+"/"+tempToReport+" = "+otr.number(s1)+"/"+otr.number(s2);
    updateGL();
    return "";
}

QString OGL::getNameLastPoint()
{
    return listPoints[listPoints.length()-1].getName();
}

QString OGL::addLine(QString p1, QString p2)
{    
    lastPointSootnoshenie[0] = "0";
    lastPointSootnoshenie[1] = "0";
    lastMoveLine = -1;
    if(p1 == p2) return "Указана одна и та же точка.";

    Point sp1,sp2;
    sp1 = findPoint(p1);
    sp2 = findPoint(p2);
// 1 если это не новая прямая, а только линия на уже сужествющей, то ничего лишнего не нужно,+ добавить точки к прямой, которые встретяться на отрезке
    int oldLine = -1;
    for(int i = 0; i < listLines.length(); i++){
        if(listLines[i].isPointInLine(sp1) && listLines[i].isPointInLine(sp2)){
            oldLine = i;
            //перейти в конец, минуя дробление плоскостей. Но при этом могут появиться новые пересечения с другими прямыми
            break;
        }
    }

    Linear l;
    Point p;

    if(oldLine == -1){//если прямой не существовало
//дробление плоскостей
        QList<Point> lPoint;
        QList<Point> plist;
        int flagToTwoPoint = 0;
        Flat fl;
        for(int i = 0; i < listFlats.length(); i++){
            lPoint = listFlats[i].getListPoint();
            flagToTwoPoint = 0;
            for(int j = 0; j < lPoint.length(); j++){
                if(lPoint[j].getName() == p1) flagToTwoPoint++;
                if(lPoint[j].getName() == p2) flagToTwoPoint++;
            }
            if(flagToTwoPoint == 2){
                listFlats.removeAt(i);
                int k;
                plist.clear();
                for(k = 0; k < lPoint.length(); k++){
                    if(lPoint[k].getName() == p1 || lPoint[k].getName() == p2){
                        plist.append(lPoint[k]);
                        break;
                    }
                    else plist.append(lPoint[k]);
                }
                for(k = k + 1; k < lPoint.length(); k++){
                    if(lPoint[k].getName() == p1 || lPoint[k].getName() == p2){
                        plist.append(lPoint[k]);
                        break;
                    }
                }
                for(k = k + 1; k < lPoint.length(); k++){
                    plist.append(lPoint[k]);
                }
                fl.setFlat(plist);
                listFlats.append(fl);
                plist.clear();
                for(k = 0; k < lPoint.length(); k++){
                    if(lPoint[k].getName() == p1 || lPoint[k].getName() == p2){
                        plist.append(lPoint[k]);
                        break;
                    }
                }
                for(k = k + 1; k < lPoint.length(); k++){
                    if(lPoint[k].getName() == p1 || lPoint[k].getName() == p2){
                        plist.append(lPoint[k]);
                        break;
                    }
                    else plist.append(lPoint[k]);
                }
                fl.setFlat(plist);
                listFlats.append(fl);
                break;
            }
        }
    }

    l.setLinear(sp1.getName()+sp2.getName(), sp1, sp2, 0);
    //найти точки в пространстве, которые лежат на ОТРЕЗКЕ sp1-sp2
    for(int j = 0; j < listPoints.length(); j++){
        if(l.isPointInLine(listPoints[j])){
            l.addPointToLine(listPoints[j]);
        }
    }
    bool del = true;
    QList<Point> list = l.getLine();
    for(int j = 0; j < list.length(); j++){
        if(list[j].getName() == sp1.getName() || list[j].getName() == sp2.getName()){
            del = !del;
            if(del == true){
                if(j < list.length() - 1) j++;
                else break;
            }
        }
        if(del){
            l.delPointWithLine(list[j]);
        }
    }
    //     и аккуратно добаить их на прямую, без лишних связей (линий), если прямая ранее существовала
    //слить в одну кучу точки на прямых  и их связи (том могут быть только одинвковые, но они не должны добавиться)
    if(oldLine == -1){
        listLines.append(l);
        oldLine = listLines.length()-1;
    }
    listLines[oldLine].addPointsToLine(l.getLine());
    listLines[oldLine].appendListOtr(l.getListOtr());
    //перерисовать прямую

    //найти точки пересечения этой прямой с другими прямыми
    for(int k = 0; k < listLines.length()-1; k++){
        p = listLines[k].getPointIntersection(l);//получение точки пересечения
        if(p.getName() == "1"){
            //сравнение координат точки с существующими точками
            bool flag2;
            flag2 = true; //такой точки нет
            for(int j = 0; j < listPoints.length(); j++){
                if(p.isEqual(listPoints[j])){
                    flag2 = false; //такая точка есть
                    break;
                }
            }
            if(flag2 && listLines[k].isPointBeInTheLine(p)){ //сравнение с концами прямой, лежит ли точка в пределах отрезка
                p.setName(getNameNextPoint("0"));
                help.drawPoint(&p, p.getColor());
                listPoints.append(p);
                listLines[oldLine].addPointToLine(p);
                listLines[k].addPointToLine(p);
            }
        }
    }

    listLines[oldLine].deleteNum();
    help.drawLine(&listLines[oldLine]);
        //

        /*//нахождение точек пересечения прямой и плоскости

        for(int i = 0; i < listFlats.length(); i++){
            p = listFlats[i].getPointIntersection(l); // получить точку пересечения, если такое возможно
            if(p.getName() == "1"){

                //сравнение координат точки с существующими точками
                bool flag2;
                flag2 = true; //такой точки нет
                for(int j = 0; j < listPoints.length(); j++){
                    if(p.isEqual(listPoints[j])){
                        flag2 = false; //такая точка есть
                        break;
                    }
                }
                //сравнение с точками проскости
                if(flag2 && listFlats[i].isPointBeInTheArea(p)){ //точка лежит в области плоскости?
                    p.setName(getNameNextPoint("0"));
                    help.drawPoint(&p, p.getColor());

                    listPoints.append(p);
                    listLines[listLines.length() - 1].addPointToLine(p);
                    listLines[listLines.length() - 1].deleteNum();
                    help.drawLine(&listLines[listLines.length() - 1]);
                }
            }
        }
    */


    updateGL();

    PossibleStep s; //save step
        s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);

    report += "\r\nДобавлена прямая "+p1+p2;

    return "";
}

QString OGL::delLine(Point pp1, Point pp2)
{
    Linear line;
    if(pp1.getName() == pp2.getName()) return "Указана одна и та же точка.";
    bool tr = false;
    int numlin = -1;

    for(int i = 0; i < listLines.length(); i++){ // поиск прямой
        QList<Point> lPL = listLines[i].getLine();
        bool bp1 = false, bp2 = false;
        for(int j = 0; j < lPL.length(); j++){
            if(lPL[j].getName() == pp1.getName()) bp1 = true;
            if(lPL[j].getName() == pp2.getName()) bp2 = true;
        }
        if(bp1 && bp2){ //если на одной прямой одновременно есть точки
            line = listLines[i];
            numlin = i;
            tr = true;
        }
    }
    if(!tr) return "Ошибка в выборе точек."; // такой прямой не сущетвует
    if(numlin > -1 && numlin < colLine){ // всю прямую удалять нельзя, но в некоторых случаях часть прямой можно удалить
    // можно удалять когда :
    // одна точка неудаляемая, а другая лежит в противоположной стороне от неудаляемого отрезка
    // обе точки удаляемые, и лежат вне неудаляемого отрезка по одну сторону
        //обе точки неудаляемые - удалять нельзя
        int numpp1 = 0, numpp2 = 0;
        for(int i = 0; i < listPoints.length(); i++){
            if(pp1.getName() == listPoints[i].getName()) numpp1 = i;
            if(pp2.getName() == listPoints[i].getName()) numpp2 = i;
        }
        if(numpp1 < colVershin && numpp2 < colVershin){
            return "Данную прямую удалить невозможно.";
        }
      //одна точка неудаляемая, а другая лежит в противоположной стороне от неудаляемого отрезка
        else if(numpp1 < colVershin || numpp2 < colVershin){
            QList<Point> lP = line.getLine();
            int numP1 = 0, numP2 = 0; // номера точек
            for(int i = 0; i < lP.length(); i++){
                if(pp1.getName() == lP[i].getName()) numP1 = i;
                if(pp2.getName() == lP[i].getName()) numP2 = i;
            }
            int numPNotDel = 0; //номер второй неудаляемой точки

            if(numpp1 < colVershin){
                for(int i = 0; i < colVershin; i++){
                    for(int j = 0; j < lP.length(); j++){
                        if(listPoints[i].getName() == lP[j].getName() && lP[j].getName() != pp1.getName()){
                            numPNotDel = j;
                        }
                    }
                }
                if(((numP1 - numPNotDel < 0) && (numP1 - numP2) >= 0) || ((numP1 - numPNotDel >= 0) && (numP1 - numP2) < 0)){
                    //можно удалять
                }
                else{
                    // нельзя удалять
                    return "Данную прямую удалить невозможно.";
                }
            }
            else{
                for(int i = 0; i < colVershin; i++){
                    for(int j = 0; j < lP.length(); j++){
                        if(listPoints[i].getName() == lP[j].getName() && lP[j].getName() != pp2.getName()){
                            numPNotDel = j;
                        }
                    }
                }
                if(((numP2 - numPNotDel < 0) && (numP2 - numP1) >= 0) || ((numP2 - numPNotDel >= 0) && (numP2 - numP1) < 0)){
                    //можно удалять
                }
                else{
                    // нельзя удалять
                    return "Данную прямую удалить невозможно.";
                }
            }
        }
      // обе точки удаляемые, и лежат вне неудаляемого отрезка по одну сторону
        else{
            QList<Point> lP = line.getLine();
            int numP1 = 0, numP2 = 0; // номера точек
            for(int i = 0; i < lP.length(); i++){
                if(pp1.getName() == lP[i].getName()) numP1 = i;
                if(pp2.getName() == lP[i].getName()) numP2 = i;
            }
            int numPNotDel1 = 0, numPNotDel2 = 0; //номера неудаляемых точек
            bool flag = true;
            for(int i = 0; i < colVershin; i++){
                for(int j = 0; j < lP.length(); j++){
                    if(listPoints[i].getName() == lP[j].getName() && flag){
                        numPNotDel1 = j;
                        flag = false;
                    }
                    else if(listPoints[i].getName() == lP[j].getName() && !flag){
                        numPNotDel2 = j;
                    }
                }
            }
            if(((numPNotDel1-numPNotDel2 < 0) && (numPNotDel1 - numP1 >= 0) && (numPNotDel1 - numP2 >= 0)) ||
                ((numPNotDel1-numPNotDel2 >= 0) && (numPNotDel1 - numP1 < 0) && (numPNotDel1 - numP2 < 0))){
                //можно удалять
            }
            else{
                //нельзя удалять
                return "Данную прямую удалить невозможно.";
            }
        }
    }
    //удаление
    if(!(pp1.getName() == line.getP1().getName() && pp2.getName() == line.getP2().getName()) &&   //то удаляем НЕ всю прямую
            !(pp1.getName() == line.getP2().getName() && pp2.getName() == line.getP1().getName())){
        listLines[numlin].deleteOtr(pp1.getName(), pp2.getName());
        listLines[numlin].deleteNum();
        help.drawLine(&listLines[numlin]);
    }
    else{//иначе ... (дальше по коду)
        lastPointSootnoshenie[0] = "0";
        lastPointSootnoshenie[1] = "0";
        for(int i = 0; i < listLines.length(); i++){
            if(listLines[i].getName() == line.getName()){
                listLines[i].deleteNum();
                listLines.removeAt(i);
            }
        }
        QString p1;
        QString p2;
        p1 = line.getP1().getName();
        p2 = line.getP2().getName();
        //соеденение плоскостей
        QList<Point> lPoint;
        QList<Point> lPoint1;
        QList<Point> lPoint2;
        QList<Point> plist;
        int flagToTwoPoint = 0;
        int intP1 = -1;
        int intP2 = -1;
        Flat fl;
        for(int i = 0; i < listFlats.length(); i++){
            lPoint = listFlats[i].getListPoint();
            flagToTwoPoint = 0;
            for(int j = 0; j < lPoint.length(); j++){
                if(lPoint[j].getName() == p1) flagToTwoPoint++;
                if(lPoint[j].getName() == p2) flagToTwoPoint++;
            }
            if(flagToTwoPoint == 2 && intP1 == -1) intP1 = i;
            else if(flagToTwoPoint == 2 && intP2 == -1) intP2 = i;

            if(intP1 != -1 && intP2 != -1){
                lPoint1 = listFlats[intP1].getListPoint();
                lPoint2 = listFlats[intP2].getListPoint();
                listFlats.removeAt(intP2); //удаляем сначала р2 т.к. он по индексу больше, что бы вторая плоскость по индексу не сместилась
                listFlats.removeAt(intP1);
                int k;
                int l;
                plist.clear();
                for(k = 0; k < lPoint1.length(); k++){
                    if(lPoint1[k].getName() == p1 || lPoint1[k].getName() == p2){
                        plist.append(lPoint1[k]);
                        break;
                    }
                    else plist.append(lPoint1[k]);
                }
                for(l = 1; l < lPoint2.length(); l++){
                    if(lPoint2[l].getName() == p1 || lPoint2[l].getName() == p2){
                        break;
                    }
                    else plist.append(lPoint2[l]);
                }
                for(k = k + 1; k < lPoint1.length(); k++){
                    plist.append(lPoint1[k]);
                }
                fl.setFlat(plist);
                listFlats.append(fl);
                break;
            }
        }
    }
//
    updateGL();

    PossibleStep s; //save step
        s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);

    report += "\r\nУдалена прямая "+pp1.getName()+pp2.getName();

    return "";
}

QString OGL::addParallelLine(QString pp, QString l1, QString l2)
{
    lastPointSootnoshenie[0] = "0";
    lastPointSootnoshenie[1] = "0";
    Point p, p1;
    Point line1, line2;
    Linear l;
    lastMoveLine = -1;
    for(int i = 0; i < listPoints.length(); i++){
        if(listPoints[i].getName() == pp) p1 = listPoints[i];
        if(listPoints[i].getName() == l1) line1 = listPoints[i];
        if(listPoints[i].getName() == l2) line2 = listPoints[i];
    }

    Linear tempLine;
    tempLine.setLinear("", line1, line2, 0);
    if(tempLine.isPointInLine(p1)) return "Указанная точка принадлежит указанной прямой";

    double x, y, z;
    x = p1.getX() + (line2.getX() - line1.getX());
    y = p1.getY() + (line2.getY() - line1.getY());
    z = p1.getZ() + (line2.getZ() - line1.getZ());

    Point p2;
    p2.setPoint("", 0, x, y, z);
    p2 = isPointInTheListPoint(p2);

    l.setLinear(p1.getName() + p2.getName(), p1, p2, 0);
    help.drawLine(&l);
    listLines.append(l);

    //нахождение точек пересечения прямой и плоскости
    for(int i = 0; i < listFlats.length(); i++){
        p = listFlats[i].getPointIntersection(l); // получить точку пересечения, если такое возможно
        if(p.getName() == "1"){

            //сравнение координат точки с существующими точками
            bool flag2;
            flag2 = true; //такой точки нет
            for(int j = 0; j < listPoints.length(); j++){
                if(p.isEqual(listPoints[j])){
                    flag2 = false; //такая точка есть
                    break;
                }
            }
            //сравнение с точками проскости
            if(flag2 && listFlats[i].isPointBeInTheArea(p)){ //точка лежит в области плоскости?
                p.setName(getNameNextPoint("0"));
                help.drawPoint(&p, p.getColor());
                listPoints.append(p);
                listLines[listLines.length() - 1].addPointToLine(p);
                listLines[listLines.length() - 1].deleteNum();
                help.drawLine(&listLines[listLines.length() - 1]);
            }
        }
    }

    //нахождение точек пересечения прямой c другими прямыми
    lookingForIntersecWithLineIn(listLines[listLines.length()-1]);

    updateGL();

    PossibleStep s; //save step
        s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);

    report += "\r\nПроведена прямая через точку "+pp+" параллельно "+l1+l2;

    return "";
}

QString OGL::createPerpendicularLine(QString point, QString PtoLine1, QString PtoLine2)
{
    Point p; //также является первой точкой при построении перпендикуляра
    Linear line; //пряма к которой нужно провести перпендикуляр
    if(PtoLine1 == PtoLine2) return "Для прямой указана одна и та же точка.";
    bool fline = false;
    for(int i = 0; i < listPoints.length(); i++){
        if(listPoints[i].getName() == point) p = listPoints[i];
    }
    //searches for line
    for(int i = 0; i < listLines.length(); i++){  //поиск прямой
        if(listLines[i].getName() == PtoLine1 + PtoLine2 || listLines[i].getName() == PtoLine2 + PtoLine1){
            fline = true;
            line = listLines[i];
        }
    }
//создание прямой
    if(!fline){
        Linear l;
        Point poin1;
        Point poin2;
        for(int i = 0; i < listPoints.length(); i++){
            if(PtoLine1 == listPoints[i].getName()) poin1 = listPoints[i];
            if(PtoLine2 == listPoints[i].getName()) poin2 = listPoints[i];
        }

        l.setLinear(poin1.getName() + poin2.getName(), poin1, poin2, 0);
        help.drawLine(&l);
        listLines.append(l);
        line = listLines[listLines.length() - 1];
    }

    if(line.isPointInLine(p)) return "Перпендикуляр построить невозможно, так как точка лежит на прямой.";

    double ax, ay, az,   bx, by, bz;

    ax = line.getP2().getX() - line.getP1().getX();
    ay = line.getP2().getY() - line.getP1().getY();
    az = line.getP2().getZ() - line.getP1().getZ();

    bx = p.getX() - line.getP1().getX();
    by = p.getY() - line.getP1().getY();
    bz = p.getZ() - line.getP1().getZ();

    double t0 = (ax*bx + ay*by + az*bz)/(ax*ax + ay*ay + az*az);

    Point pp;
    pp.setPoint("", 0, ax*t0 + line.getP1().getX(), ay*t0 + line.getP1().getY(), az*t0 + line.getP1().getZ());

    Linear li; //прямая по которой проходит перпендикуляр
    li.setLinear("", p, pp, 0);

    bool flag;
    flag = true; //такой точки нет
    for(int j = 0; j < listPoints.length(); j++){
        if(pp.isEqual(listPoints[j])){
            flag = false; //такая точка есть
            break;
        }
    }
    if(flag){
        pp.setName(getNameNextPoint("0"));
        help.drawPoint(&pp, pp.getColor());
        listPoints.append(pp);

        li.setLinear(p.getName() + pp.getName(), p, pp, 0);
        help.drawLine(&li);
        listLines.append(li);
    }
    else{
        int j1 = 0;
        //рисуем прямую
        for(int j = 0; j < listPoints.length(); j++){
            if(pp.isEqual(listPoints[j])){
                j1 = j;
                break;
            }
        }
        li.setLinear(p.getName() + listPoints[j1].getName(), p, listPoints[j1], 0);
        help.drawLine(&li);
        listLines.append(li);
    }

    lookingForIntersecWithLineIn(line);
    lookingForIntersecWithLineIn(li);


    updateGL();

    PossibleStep s; //save step
        s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);

    report += "\r\nПроведен перпендикуляр через точку "+point+" к прямой "+PtoLine1+PtoLine2;

    return "";
}

QString OGL::addPointIntersection_LineWithLine(QString point11, QString point12, QString point21, QString point22)
{     
    bool fline1 = false;
    bool fline2 = false;
    lastPointSootnoshenie[0] = "0";
    lastPointSootnoshenie[1] = "0";
    int l1 = 0;
    int l2 = 0;
    for(int i = 0; i < listLines.length(); i++){  //поиск прямых
        if(listLines[i].getName() == point11 + point12 || listLines[i].getName() == point12 + point11){
            fline1 = true;
        }
        if(listLines[i].getName() == point21 + point22 || listLines[i].getName() == point22 + point21){
            fline2 = true;
        }
    }
    if(point11 + point12 == point21 + point22 || point12 + point11 == point21 + point22 || point11 + point12 == point22 + point21){
        return "Указана одна и та же прямая.";
    }
    if(point11 == point12 || point21 == point22){
        return "Указана одна и та же точка.";
    }
//создание прямой
    if(!fline1){
        Linear l;
        Point poin1;
        Point poin2;
        for(int i = 0; i < listPoints.length(); i++){
            if(point11 == listPoints[i].getName()) poin1 = listPoints[i];
            if(point12 == listPoints[i].getName()) poin2 = listPoints[i];
        }
        l.setLinear(poin1.getName() + poin2.getName(), poin1, poin2, 0);
        help.drawLine(&l);
        listLines.append(l);
    }
    if(!fline2){
        Linear l;
        Point poin1;
        Point poin2;
        for(int i = 0; i < listPoints.length(); i++){
            if(point21 == listPoints[i].getName()) poin1 = listPoints[i];
            if(point22 == listPoints[i].getName()) poin2 = listPoints[i];
        }
        l.setLinear(poin1.getName() + poin2.getName(), poin1, poin2, 0);
        help.drawLine(&l);
        listLines.append(l);
    }
//
    for(int i = 0; i < listLines.length(); i++){  //поиск прямых
        if(listLines[i].getName() == point11 + point12 || listLines[i].getName() == point12 + point11){
            fline1 = true;
            l1 = i;
        }
        if(listLines[i].getName() == point21 + point22 || listLines[i].getName() == point22 + point21){
            fline2 = true;
            l2 = i;
        }
    }
    if(listLines[l1].isOneLine(listLines[l2])) return "Прямые совпадают.";
    Point p;
    p = listLines[l1].getPointIntersection(listLines[l2]);
    if(p.getName() != "1"){
        return p.getName();
    }
    else {
        bool flag;
        flag = true; //такой точки нет
        for(int j = 0; j < listPoints.length(); j++){
            if(p.isEqual(listPoints[j])){
                flag = false; //такая точка есть
                break;
            }
        }
        if(flag){
            p.setName(getNameNextPoint("0"));
            help.drawPoint(&p, p.getColor());
            listPoints.append(p);
            listLines[l1].addPointToLine(p);
            listLines[l2].addPointToLine(p);
            listLines[l1].deleteNum();
            help.drawLine(&listLines[l1]);
            listLines[l2].deleteNum();
            help.drawLine(&listLines[l2]);

            report += "\r\nДобавлена точка "+p.getName()+" как точка пересечения прямых "+point11+point12+" и "+point21+point22;
        }
        else{
            //нарисовать прямую
            listLines[l1].addPointToLine(p);
            listLines[l2].addPointToLine(p);
            listLines[l1].deleteNum();
            help.drawLine(&listLines[l1]);
            listLines[l2].deleteNum();
            help.drawLine(&listLines[l2]);
        }
    }

    lookingForIntersecWithLineIn(listLines[l1]);
    lookingForIntersecWithLineIn(listLines[l2]);

    updateGL();

    PossibleStep s; //save step
        s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);

    return "";
}

QString OGL::addPointIntersection_LineWithFlat(QString line, QString line2, QString flat)
{
    lastPointSootnoshenie[0] = "0";
    lastPointSootnoshenie[1] = "0";
    int l = -1;
    int f = 0;
    for(int i = 0; i < listLines.length(); i++){
        if(listLines[i].getName() == line + line2 || listLines[i].getName() == line2 + line) {
            l = i;
            break;
        }
    }
    if(l == -1){
        Linear lin;
        Point poin1;
        Point poin2;
        for(int i = 0; i < listPoints.length(); i++){
            if(line == listPoints[i].getName()) poin1 = listPoints[i];
            if(line2 == listPoints[i].getName()) poin2 = listPoints[i];
        }
        lin.setLinear(poin1.getName() + poin2.getName(), poin1, poin2, 0);
        help.drawLine(&lin);
        listLines.append(lin);
        l = listLines.length() - 1;

    }
    for(int i = 0; i < listFlats.length(); i++){
        if(listFlats[i].getName() == flat) {
            f = i;
            break;
        }
    } 
    Point p;
    p = listFlats[f].getPointIntersection(listLines[l]);
    if(p.getName() != "1") return p.getName();
    else{
        bool flag;
        flag = true; //такой точки нет
        for(int j = 0; j < listPoints.length(); j++){
            if(p.isEqual(listPoints[j])){
                flag = false; //такая точка есть
                break;
            }
        }
        if(flag){
            p.setName(getNameNextPoint("0"));
            help.drawPoint(&p, p.getColor());
            listPoints.append(p);
            listLines[l].addPointToLine(p);
            listLines[l].deleteNum();
            help.drawLine(&listLines[l]);

            report += "\r\nДобавлена точка "+p.getName()+" как точка пересечения прямой "+line+line2+ " и плоскости "+flat;
        }
    }
    updateGL();

    PossibleStep s; //save step
        s.set(&listPoints, &listLines, &listFlats, nnp, &graphicAnswer);
    steps.append(s);

    return "";
}
