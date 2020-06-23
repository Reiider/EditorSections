#include "linear.h"

QString Linear::d_to_str(double d)
{
    QString temp;
    if(d > -0.0001 && d < 0.0001 ) temp = "0";
    else temp = temp.number(d);
    return temp;
}

void Linear::createListOtr()
{
    for(int i = 0; i < line.length() - 1; i++){
        QList<Point> list;
        list.append(line[i]);
        list.append(line[i+1]);
        listOtr.append(list);
    }
}

void Linear::sortLine()
{
    Point p;
    for(int i = 0; i < line.length() - 1; i++){
        for(int j = line.length() - 1; j > i; j--){
            if((line[j].getX()-pFGPFL2.getX())/(pFGPFL2.getX() - pFGPFL1.getX()) < (line[j-1].getX()-pFGPFL2.getX())/(pFGPFL2.getX() - pFGPFL1.getX())){
                p = line[j];
                line[j] = line[j-1];
                line[j-1] = p;
            }
            else if((line[j].getY()-pFGPFL2.getY())/(pFGPFL2.getY() - pFGPFL1.getY()) < (line[j-1].getY()-pFGPFL2.getY())/(pFGPFL2.getY() - pFGPFL1.getY())){
                p = line[j];
                line[j] = line[j-1];
                line[j-1] = p;
            }
            else if((line[j].getZ()-pFGPFL2.getZ())/(pFGPFL2.getZ() - pFGPFL1.getZ()) < (line[j-1].getZ()-pFGPFL2.getZ())/(pFGPFL2.getZ() - pFGPFL1.getZ())){
                p = line[j];
                line[j] = line[j-1];
                line[j-1] = p;
            }
        }
    }
}

bool Linear::isOtrExist(QString s1, QString s2)
{
    for(int i = 0; i < listOtr.length(); i++){
        if((listOtr.at(i).at(0).getName() == s1 && listOtr.at(i).at(1).getName() == s2) ||
           (listOtr.at(i).at(1).getName() == s1 && listOtr.at(i).at(0).getName() == s2)){
            return true;
        }
    }
    return false;
}


Linear::Linear(){
    num = 0;
}

Linear::~Linear()
{

}

void Linear::deleteNum()
{
    glDeleteLists(num, 1);
}

void Linear::setLinear(QString name, Point point1, Point point2, GLuint num)
{
    this->name = name;
    p1 = point1;
    p2 = point2;
    pFGPFL1 = p1;
    pFGPFL2 = p2;
    this->num = num;
    line.clear();
    line.append(p1);
    line.append(p2);
    listOtr.clear();
    createListOtr();
    /*length_line = sqrt((p1.getX() - p2.getX())*(p1.getX() - p2.getX()) +
                       (p1.getY() - p2.getY())*(p1.getY() - p2.getY()) +
                       (p1.getZ() - p2.getZ())*(p1.getZ() - p2.getZ()));*/
    if(WriteLog::isWrite){
        WriteLog::write("Linear::setLinear: "+name+", "+point1.getName()+", "+point2.getName());
    }
}

void Linear::addPointToLine(Point p)
{
    if(WriteLog::isWrite){
        WriteLog::write("Linear::addPointToLine: "+p.getName()+", "+ d_to_str(p.getX())+", "+ d_to_str(p.getY())+", "+d_to_str(p.getZ()));
    }

    QString prevPoint = "";
    QString nextPoint = "";

    bool flag;
    flag = false;
    for(int i = 0; i < line.length(); i++){
        if(p.getName() == line[i].getName()){
            line[i].setPoint(p.getName(),p.getNum(),p.getX(),p.getY(),p.getZ());
            flag = true;
            //запоминаем между какими точками находилась точка ранее
            if(i > 0) prevPoint = line[i-1].getName();
            if(i < line.length() - 1) nextPoint = line[i+1].getName();
            break;
        }
    }

    if(flag){
    }
    else {
        line.append(p);
    }

    sortLine();

    int ind = 0; //текущее расположение точки на прямой
    for(ind = 0; ind < line.length(); ind++){
        if(line[ind].getName() == p.getName()){
            break;
        }
    }
    //Удаляем старые отрезки связанные с точкой
    if(prevPoint == "" && nextPoint == ""){ //если их нет, то нет и отрезков (тоесть точка добавилась, а не изменилась)

    }
    else if(prevPoint == "" && nextPoint != ""){ //точка была на одном конце линии
        for(int i = listOtr.length() - 1; i >= 0; i--){
            if((listOtr.at(i).at(0).getName() == nextPoint && listOtr.at(i).at(1).getName() == p.getName())
                    || (listOtr.at(i).at(1).getName() == nextPoint && listOtr.at(i).at(0).getName() == p.getName())){
                listOtr.removeAt(i);
                break;
            }
        }
    }
    else if(prevPoint != "" && nextPoint == ""){ //точка была на другом конце линии
        for(int i = listOtr.length() - 1; i >= 0; i--){
            if((listOtr.at(i).at(0).getName() == prevPoint && listOtr.at(i).at(1).getName() == p.getName())
                    || (listOtr.at(i).at(1).getName() == prevPoint && listOtr.at(i).at(0).getName() == p.getName())){
                listOtr.removeAt(i);
                break;
            }
        }
    }
    else { // точка была не на конце линии
        for(int i = listOtr.length() - 1; i >= 0; i--){
            if((listOtr.at(i).at(0).getName() == nextPoint && listOtr.at(i).at(1).getName() == p.getName())
                    || (listOtr.at(i).at(1).getName() == nextPoint && listOtr.at(i).at(0).getName() == p.getName())){
                listOtr.removeAt(i);
                break;
            }
        }
        for(int i = listOtr.length() - 1; i >= 0; i--){
            if((listOtr.at(i).at(0).getName() == prevPoint && listOtr.at(i).at(1).getName() == p.getName())
                    || (listOtr.at(i).at(1).getName() == prevPoint && listOtr.at(i).at(0).getName() == p.getName())){
                listOtr.removeAt(i);
                break;
            }
        }
    }

    //Удаляем отрезок в который добавилась точка
    if(ind > 0 && ind < line.length()-1){
        for(int i = listOtr.length() - 1; i >= 0; i--){
            if((listOtr.at(i).at(0).getName() == line[ind-1].getName() && listOtr.at(i).at(1).getName() == line[ind+1].getName())
                    || (listOtr.at(i).at(1).getName() == line[ind-1].getName() && listOtr.at(i).at(0).getName() == line[ind+1].getName())){

                listOtr.removeAt(i);
                break;
            }
        }
    }

    //
 // Добавляем новые
    QList<Point> list;
   //соединяем старый отрезок, с которого ушла точка (только если она была не на границе)
    //точка была не на границе и будет не на границе
    if(ind > 0 && ind < line.length()-1 && prevPoint != "" && nextPoint != ""){
        //если она будет не на том же участке
        if(((prevPoint != line[ind+1].getName()) && (nextPoint != line[ind+1].getName())) ||
                ((prevPoint != line[ind-1].getName()) && (nextPoint != line[ind-1].getName()))){
            for(int i = 0; i < line.length(); i++){
                if(line[i].getName() == prevPoint){
                    list.append(line[i]);
                }
                if(line[i].getName() == nextPoint){
                    list.append(line[i]);
                }
            }
            listOtr.append(list);
            list.clear();
        }
    } // если точка на границе, а была не на границе
    else if((ind == 0 || ind == line.length()-1) && prevPoint != "" && nextPoint != ""){
        for(int i = 0; i < line.length(); i++){
            if(line[i].getName() == prevPoint){
                list.append(line[i]);
            }
            if(line[i].getName() == nextPoint){
                list.append(line[i]);
            }
        }
        listOtr.append(list);
        list.clear();
    }
    //соединяем текущий отрезок
    // если точка не на границе
    if(ind != 0 && ind != line.length() - 1){
        list.append(line[ind - 1]);
        list.append(line[ind]);
        listOtr.append(list);
        list.clear();

        list.append(line[ind]);
        list.append(line[ind + 1]);
        listOtr.append(list);
        list.clear();
    }
    else if(ind == 0){
        list.append(line[ind + 1]);
        list.append(line[ind]);
        listOtr.append(list);
        list.clear();
    }
    else {
        list.append(line[ind - 1]);
        list.append(line[ind]);
        listOtr.append(list);
        list.clear();
    }

    p1 = line[0];
    p2 = line[line.length()-1];   


    if(WriteLog::isWrite){

        QString s = "";
        for(int i = 0; i < line.length(); i++){
            s += line[i].getName() + " ";
        }
        WriteLog::write("   line: "+s);
        s = "";
        for(int i = 0; i < listOtr.length(); i++){
            s += listOtr[i].at(0).getName() + listOtr[i].at(1).getName() +" ";
        }
        WriteLog::write("   listOtr: "+s);
    }
    /*length_line = sqrt((p1.getX() - p2.getX())*(p1.getX() - p2.getX()) +
                       (p1.getY() - p2.getY())*(p1.getY() - p2.getY()) +
                       (p1.getZ() - p2.getZ())*(p1.getZ() - p2.getZ()));*/
}

QString Linear::getName(){
    return name;
}

Point Linear::getP1(){
    return p1;
}

Point Linear::getP2(){
    return p2;
}

Point Linear::getPFGPFL1()
{
    return pFGPFL1;
}

Point Linear::getPFGPFL2()
{
    return pFGPFL2;
}

GLuint Linear::getNum(){
    return num;
}

QList<Point> Linear::getLine()
{
    return line;
}

Point Linear::getPointForLine(double k)
{
    Point p;
    double x,y,z;
    x = pFGPFL2.getX() + (pFGPFL2.getX() - pFGPFL1.getX()) * k;
    y = pFGPFL2.getY() + (pFGPFL2.getY() - pFGPFL1.getY()) * k;
    z = pFGPFL2.getZ() + (pFGPFL2.getZ() - pFGPFL1.getZ()) * k;
    p.setPoint("1", 0, x, y, z);
    return p;
}

Point Linear::getPointIntersection(Linear l)
{
    QString nameL = l.getName();
    if(this->getName() != "endIntersec"){
        Point invers;
        l.setName("endIntersec");
        invers = l.getPointIntersection(*this);
        l.setName(nameL);
        if(invers.getName() == "1") {
            return invers;
        }
    }

    Point p, po1, po2, p1, p2;
    po1 = l.getPFGPFL1();
    po2 = l.getPFGPFL2();
    p1 = this->getPFGPFL1();
    p2 = this->getPFGPFL2();

    //решение методом гаусса с выбором главного элемента
    double mt[3][3];
    //система из параметрических уравнений, где предологается что одна точка лежит на двух прямых одновременно, исходя из этого приравниваем два параметрических уравнения
    mt[0][0] = p2.getX() - p1.getX();
    mt[1][0] = p2.getY() - p1.getY();
    mt[2][0] = p2.getZ() - p1.getZ();
    mt[0][1] = -(po2.getX() - po1.getX());
    mt[1][1] = -(po2.getY() - po1.getY());
    mt[2][1] = -(po2.getZ() - po1.getZ());

    mt[0][2] = po1.getX() - p1.getX();
    mt[1][2] = po1.getY() - p1.getY();
    mt[2][2] = po1.getZ() - p1.getZ();

    int ind = 0; //индекс максимального по модулю элемента в первом столбце
    double maxe = 0; // его значение

    for(int j = 0; j < 3; j++){ //поиск максимального элемента и его индекс
        if(fabs(mt[j][0]) > fabs(maxe)){
            ind = j;
            maxe = mt[j][0];
        }
    }
    if(0 != ind){ //перестановка местами строк уравнений (в которых используются различные координаты)
        for(int j = 0; j < 3; j++){
            mt[0][j] = mt[0][j] + mt[ind][j];
            mt[ind][j] = mt[0][j] - mt[ind][j];
            mt[0][j] = mt[0][j] - mt[ind][j];
        }
    }
    if(maxe!=0){ // делим первую строку на максимальное значение первого столбца
        for(int j = 0; j < 3; j++){
            mt[0][j] = mt[0][j]/maxe;
        }
    }
    double temp;
    for(int i = 1; i < 3; i++){
        temp = mt[i][0];  // запоменаем элемент в первом столбце
        for(int j = 0; j < 3; j++){
            mt[i][j] = mt[i][j] - (temp*mt[0][j]); //умножаем первую строку на элемент первого столбца и вычитаем это из остальных строк (что бы в первом столбце у остальных были нули)
        }
    }
    if((mt[1][1] == 0 && mt[1][2] != 0) || (mt[2][1] == 0 && mt[2][2] != 0)){
        p.setPoint("Прямые не пересекаются", 0, 0, 0, 0);
        return p;
    }
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if((mt[i][j] + 0.0001) > 0 && (mt[i][j] - 0.0001) < 0) mt[i][j] = 0;
        }
    }
    double k, t = 0, x, y, z,x1,y1,z1;
    if(mt[1][1] != 0){
        if(mt[1][2]/mt[1][1] != 0){ //выражаем вторую переменную
            t = mt[1][2]/mt[1][1];
        }
    }
    else if(mt[2][1] != 0){
        if(mt[2][2]/mt[2][1] != 0){ //выражаем вторую переменную
            t = mt[2][2]/mt[2][1];
        }
    }
    k = mt[0][2] - mt[0][1]*t; //подставляя все в первую строку получаем значение первой переменной

    x = p1.getX() + (p2.getX() - p1.getX())*k;
    y = p1.getY() + (p2.getY() - p1.getY())*k;
    z = p1.getZ() + (p2.getZ() - p1.getZ())*k;
    x1 = (po1.getX() + (po2.getX() - po1.getX()) * t);
    y1 = (po1.getY() + (po2.getY() - po1.getY()) * t);
    z1 = (po1.getZ() + (po2.getZ() - po1.getZ()) * t);
    if(x < 0.0001 && x > -0.0001) x = 0;
    if(y < 0.0001 && y > -0.0001) y = 0;
    if(z < 0.0001 && z > -0.0001) z = 0;
    if(((x + 0.0001) > x1) && ((x - 0.0001) < x1) &&
       ((y + 0.0001) > y1) && ((y - 0.0001) < y1) &&
       ((z + 0.0001) > z1) && ((z - 0.0001) < z1)){
        if((x > 10000 && x < -10000) || (y > 10000 && y < -10000) || ( z> 10000 && z < -10000)){
            p.setPoint("Прямые не пересекаются", 0, 0, 0, 0);
            return p;
        }
        p.setPoint("1", 0, x, y, z);
        return p;
    }

    p.setPoint("Прямые не пересекаются", 0, 0, 0, 0);
    return p;
}

bool Linear::isPointBeInTheLine(Point p)
{
    if(((p.getX()-pFGPFL2.getX())/(pFGPFL2.getX() - pFGPFL1.getX()) < (p1.getX()-pFGPFL2.getX())/(pFGPFL2.getX() - pFGPFL1.getX())) &&
       ((p.getX()-pFGPFL2.getX())/(pFGPFL2.getX() - pFGPFL1.getX()) < (p2.getX()-pFGPFL2.getX())/(pFGPFL2.getX() - pFGPFL1.getX())))
       return false;
    if(((p.getY()-pFGPFL2.getY())/(pFGPFL2.getY() - pFGPFL1.getY()) < (p1.getY()-pFGPFL2.getY())/(pFGPFL2.getY() - pFGPFL1.getY())) &&
       ((p.getY()-pFGPFL2.getY())/(pFGPFL2.getY() - pFGPFL1.getY()) < (p2.getY()-pFGPFL2.getY())/(pFGPFL2.getY() - pFGPFL1.getY())))
       return false;
    if(((p.getZ()-pFGPFL2.getZ())/(pFGPFL2.getZ() - pFGPFL1.getZ()) < (p1.getZ()-pFGPFL2.getZ())/(pFGPFL2.getZ() - pFGPFL1.getZ())) &&
       ((p.getZ()-pFGPFL2.getZ())/(pFGPFL2.getZ() - pFGPFL1.getZ()) < (p2.getZ()-pFGPFL2.getZ())/(pFGPFL2.getZ() - pFGPFL1.getZ())))
       return false;

    if(((p.getX()-pFGPFL2.getX())/(pFGPFL2.getX() - pFGPFL1.getX()) > (p1.getX()-pFGPFL2.getX())/(pFGPFL2.getX() - pFGPFL1.getX())) &&
       ((p.getX()-pFGPFL2.getX())/(pFGPFL2.getX() - pFGPFL1.getX()) > (p2.getX()-pFGPFL2.getX())/(pFGPFL2.getX() - pFGPFL1.getX())))
       return false;
    if(((p.getY()-pFGPFL2.getY())/(pFGPFL2.getY() - pFGPFL1.getY()) > (p1.getY()-pFGPFL2.getY())/(pFGPFL2.getY() - pFGPFL1.getY())) &&
       ((p.getY()-pFGPFL2.getY())/(pFGPFL2.getY() - pFGPFL1.getY()) > (p2.getY()-pFGPFL2.getY())/(pFGPFL2.getY() - pFGPFL1.getY())))
       return false;
    if(((p.getZ()-pFGPFL2.getZ())/(pFGPFL2.getZ() - pFGPFL1.getZ()) > (p1.getZ()-pFGPFL2.getZ())/(pFGPFL2.getZ() - pFGPFL1.getZ())) &&
       ((p.getZ()-pFGPFL2.getZ())/(pFGPFL2.getZ() - pFGPFL1.getZ()) > (p2.getZ()-pFGPFL2.getZ())/(pFGPFL2.getZ() - pFGPFL1.getZ())))
       return false;
    return true;
}

bool Linear::isPointInLine(Point p)
{
    double k,k1,k2,k3;
    if((pFGPFL2.getX() - pFGPFL1.getX()) != 0) k1 = (p.getX() - pFGPFL2.getX())/(pFGPFL2.getX() - pFGPFL1.getX());
        else k1 = 0;
    if((pFGPFL2.getY() - pFGPFL1.getY()) != 0) k2 = (p.getY() - pFGPFL2.getY())/(pFGPFL2.getY() - pFGPFL1.getY());
        else k2 = 0;
    if((pFGPFL2.getZ() - pFGPFL1.getZ()) != 0) k3 = (p.getZ() - pFGPFL2.getZ())/(pFGPFL2.getZ() - pFGPFL1.getZ());
        else k3 = 0;
    if(k1 != 0) k = k1;
        else if(k2 != 0) k = k2;
        else if(k3 != 0) k = k3;
        else k = 0;
    if((p.getX() + 0.0001 > pFGPFL2.getX() + (pFGPFL2.getX() - pFGPFL1.getX()) * k) &&
       (p.getX() - 0.0001 < pFGPFL2.getX() + (pFGPFL2.getX() - pFGPFL1.getX()) * k) &&
       (p.getY() + 0.0001 > pFGPFL2.getY() + (pFGPFL2.getY() - pFGPFL1.getY()) * k) &&
       (p.getY() - 0.0001 < pFGPFL2.getY() + (pFGPFL2.getY() - pFGPFL1.getY()) * k) &&
       (p.getZ() + 0.0001 > pFGPFL2.getZ() + (pFGPFL2.getZ() - pFGPFL1.getZ()) * k) &&
       (p.getZ() - 0.0001 < pFGPFL2.getZ() + (pFGPFL2.getZ() - pFGPFL1.getZ()) * k))
           return true;
    else return false;
}

bool Linear::isPointInListLine(QString nameP)
{
    for(int i = 0; i < line.length(); i++){
        if(line[i].getName() == nameP) return true;
    }
    return false;
}

void Linear::setNum(GLuint n)
{
    if(num != 0) glDeleteLists(num, 1);
    num = n;
}

void Linear::setName(QString name)
{
    this->name = name;
}

void Linear::onlySetNum(GLuint n)
{
    num = n;
}

void Linear::delPointWithLine(Point p)
{
    if(WriteLog::isWrite){
        WriteLog::write("Linear::delPointWithLine: "+p.getName());
    }

    Point prev, next;
    prev.setName("");
    next.setName("");
    bool flag = false;
    for(int i = 0; i < line.length(); i++){
        if(p.getName() == line[i].getName()){
            if(i > 0) prev = line[i-1];
            if(i < line.length()-1) next = line[i+1];
            flag = true;
            line.removeAt(i);
            break;
        }
    }
    if(line.length() >= 2 && flag){
        for(int i = listOtr.length() - 1; i >= 0; i--){
            if(listOtr.at(i).at(0).getName() == p.getName() || listOtr.at(i).at(1).getName() == p.getName()){
                listOtr.removeAt(i);
            }
        }
        if(prev.getName() != "" && next.getName() != ""){
            QList<Point> list;
            list.append(prev);
            list.append(next);
            listOtr.append(list);
        }

        p1 = line[0];
        p2 = line[line.length()-1];

        this->name = p1.getName() + p2.getName();
    }

    if(WriteLog::isWrite){

        QString s = "";
        for(int i = 0; i < line.length(); i++){
            s += line[i].getName() + " ";
        }
        WriteLog::write("   line: "+s);
        s = "";
        for(int i = 0; i < listOtr.length(); i++){
            s += listOtr[i].at(0).getName() + listOtr[i].at(1).getName() +" ";
        }
        WriteLog::write("   listOtr: "+s);
    }

}

int Linear::getLineLength()
{
    return line.length();
}

bool Linear::isOneLine(Linear l)
{
    if(this->isPointInLine(l.getPFGPFL1()) && this->isPointInLine(l.getPFGPFL2()))
        return true;
    else return false;
}

void Linear::renamePointInLine(QString from, QString to)
{
    for(int j = 0; j < line.length(); j++){
        if(line[j].getName() == from){
            line[j].setName(to);
        }
    }
    if(p1.getName() == from) p1.setName(to);
    if(p2.getName() == from) p2.setName(to);

    if(pFGPFL1.getName() == from) pFGPFL1.setName(to);
    if(pFGPFL2.getName() == from) pFGPFL2.setName(to);
    this->name = p1.getName()+p2.getName();
}

QString Linear::toString()
{
    return "" + name + ": \n     " + p1.toString() + "\n     " + p2.toString() + "\n";
}

QList<QList<Point> > Linear::getListOtr()
{
    return listOtr;
}

void Linear::deleteOtr(QString p1, QString p2)
{
    if(WriteLog::isWrite){
        WriteLog::write("Linear::deleteOtr: "+p1+p2);
    }
    //удаляем отрезок
    bool del = false;
    for(int i = 0; i < line.length()-1; i++){
        if(line[i].getName() == p1 || line[i].getName() == p2){
            del = !del;
            if(line[i+1].getName() != p1 && line[i+1].getName() != p2)i++;
            else {
                for(int j = listOtr.length() - 1; j >= 0; j--){
                    if((listOtr.at(j).at(0).getName() == line[i].getName() && listOtr.at(j).at(1).getName() == line[i+1].getName())
                        || (listOtr.at(j).at(0).getName() == line[i+1].getName() && listOtr.at(j).at(1).getName() == line[i].getName())){
                        listOtr.removeAt(j);
                        break;
                    }
                }
                break;
            }
        }
        if(del){
            for(int j = listOtr.length() - 1; j >= 0; j--){
                if(listOtr.at(j).at(0).getName() == line[i].getName() || listOtr.at(j).at(1).getName() == line[i].getName()){
                    listOtr.removeAt(j);
                }
            }
        }
    }
    //если точка не соединена одним отрезком, то удаляем ее с прямой, что бы при движении новой (добавленной) точки, отрезки не поялялись самопроизвольно раньше времени
    QString s = "";
    for(int i = 0; i < listOtr.length(); i++){
        s += listOtr.at(i).at(0).getName()+listOtr.at(i).at(1).getName();
    }
    for(int i = line.length() - 1; i >= 0; i--){
        if(s.indexOf(line[i].getName()) == -1){
            line.removeAt(i);
        }
    }

    if(WriteLog::isWrite){

        QString s = "";
        for(int i = 0; i < line.length(); i++){
            s += line[i].getName() + " ";
        }
        WriteLog::write("   line: "+s);
        s = "";
        for(int i = 0; i < listOtr.length(); i++){
            s += listOtr[i].at(0).getName() + listOtr[i].at(1).getName() +" ";
        }
        WriteLog::write("   listOtr: "+s);
    }
}

void Linear::appendListOtr(QList<QList<Point> > list)
{
    bool canAdd = true;
    for(int i = 0; i < list.length(); i++){
        canAdd = true;
        for(int j = 0; j < listOtr.length(); j++){
            if((list[i].at(0).getName() == listOtr[j].at(0).getName() && list[i].at(1).getName() == listOtr[j].at(1).getName()) ||
               (list[i].at(0).getName() == listOtr[j].at(1).getName() && list[i].at(1).getName() == listOtr[j].at(0).getName())){
                canAdd = false;
                break;
            }
        }
        if(canAdd){
            listOtr.append(list[i]);
        }
    }
}

void Linear::addPointsToLine(QList<Point> list)
{
    bool canAdd = true;
    for(int i = 0; i < list.length(); i++){
        canAdd = true;
        for(int j = 0; j < line.length(); j++){
            if(list[i].getName() == line[j].getName()){
                canAdd = false;
                break;
            }
        }
        if(canAdd){
            line.append(list[i]);
        }
    }
    sortLine();
    p1 = line[0];
    p2 = line[line.length()-1];
}

void Linear::addOtr(Point p1, Point p2)
{
    QList<Point> list;
    list.append(p1);
    list.append(p2);
    addPointsToLine(list);
    bool canAdd = false;
    for(int i = 0; i < line.length() - 1; i++){
        if(line[i].getName() == p1.getName() || line[i].getName() == p2.getName()){
            canAdd = !canAdd;
        }
        if(canAdd){
            if(!isOtrExist(line[i].getName(), line[i+1].getName())){
                QList<Point> l;
                l.append(line[i]);
                l.append(line[i+1]);
                listOtr.append(l);
            }
        }
    }
}

void Linear::setPFGPFL(Point p1, Point p2)
{
    pFGPFL1 = p1;
    pFGPFL2 = p2;
}

double Linear::getLenghtLine()
{
    double a = (p1.getX() - p2.getX())*(p1.getX() - p2.getX()) +
               (p1.getY() - p2.getY())*(p1.getY() - p2.getY()) +
               (p1.getZ() - p2.getZ())*(p1.getZ() - p2.getZ());
    if (a > 0) return sqrt(a);
    else return 0;
}


