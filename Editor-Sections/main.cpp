#include "mainwindow.h"

#include <QApplication>

double Point::rr = 0;
double Point::gg = 0;
double Point::bb = 0;

int main(int argc, char *argv[])
{
    //для сглажиания
    QGLFormat fmt;
    fmt.setSampleBuffers(true);
    fmt.setSamples(8); //2, 4, 8, 16
    QGLFormat::setDefaultFormat(fmt);

    QApplication a(argc, argv);
    a.setStyle("fusion");
    MainWindow w;
    w.showMaximized();

    return a.exec();
}

QDataStream &operator<<( QDataStream &stream, const Point &p ){
    stream << p.name << QString::number(p.x) << QString::number(p.y) << QString::number(p.z)
           << QString::number(p.r) << QString::number(p.g) << QString::number(p.b);
    return stream;
}

QDataStream &operator>>( QDataStream &stream, Point &p ){
    QString s;
    stream >> p.name;
    stream >> s;
        p.x = s.toDouble();
        s.clear();
    stream >> s;
        p.y = s.toDouble();
        s.clear();
    stream >> s;
        p.z = s.toDouble();
        s.clear();
    stream >> s;
        p.r = s.toDouble();
        s.clear();
    stream >> s;
        p.g = s.toDouble();
        s.clear();
    stream >> s;
        p.b = s.toDouble();
        s.clear();
    return stream;
}

QDataStream& operator<<( QDataStream& stream, const Linear& line ) {
    stream << line.name << line.line << line.listOtr << line.p1 << line.p2 << line.pFGPFL1 << line.pFGPFL2;
    return stream;
}

QDataStream& operator>>( QDataStream& stream, Linear& line ) {
    stream >> line.name >> line.line >> line.listOtr >> line.p1 >> line.p2 >> line.pFGPFL1 >> line.pFGPFL2;
    return stream;
}

QDataStream& operator<<( QDataStream &stream, const Flat &f ) {
    stream << f.plos << f.name << QString::number(f.A) << QString::number(f.B)
           << QString::number(f.C) << QString::number(f.D);
    return stream;
}

QDataStream& operator>>( QDataStream &stream, Flat &f ) {
    stream >> f.plos >> f.name;
    QString s;
    stream >> s;
        f.A = s.toDouble();
        s.clear();
    stream >> s;
        f.B = s.toDouble();
        s.clear();
    stream >> s;
        f.C = s.toDouble();
        s.clear();
    stream >> s;
        f.D = s.toDouble();
        s.clear();
    return stream;
}

QDataStream& operator<<( QDataStream &stream, const PossibleStep &ps ) {
    stream << ps.points << ps.lines << ps.flats << ps.graphicAnswer;
    for(int j = 0; j < 10; j++){
        for(int i = 0; i < 26; i++){
            stream << ps.nnp[i][j];
        }
    }
    return stream;
}

QDataStream& operator>>( QDataStream &stream, PossibleStep &ps ) {
    stream >> ps.points >> ps.lines >> ps.flats >> ps.graphicAnswer;
    for(int j = 0; j < 10; j++){
        for(int i = 0; i < 26; i++){
            stream >> ps.nnp[i][j];
        }
    }
    return stream;
}

QDataStream& operator<<( QDataStream &stream, const ArrayStep &arr ) {
    stream << arr.array << arr.stepNumber;
    return stream;
}

QDataStream& operator>>( QDataStream &stream, ArrayStep &arr ) {
    stream >> arr.array >> arr.stepNumber;
    return stream;
}

QDataStream& operator<<( QDataStream &stream, const DigestOgl &d ) {
    stream << d.colVershin << d.colLine
      << d.colFirstLine << d.colorNnp << d.Answer << d.listAnswer << d.steps <<d.report;
    return stream;
}

QDataStream& operator>>( QDataStream &stream, DigestOgl &d ) {
    stream >> d.colVershin >> d.colLine
      >> d.colFirstLine >> d.colorNnp >> d.Answer >> d.listAnswer >> d.steps >> d.report;
    return stream;
}

QDataStream& operator<<( QDataStream &stream, const ListTasksLog &ltl ) {
    stream << ltl.listTasks << ltl.listConditionTask << ltl.login << ltl.test << ltl.variant;
    return stream;
}

QDataStream& operator>>( QDataStream &stream, ListTasksLog &ltl ) {
    stream >> ltl.listTasks >> ltl.listConditionTask >> ltl.login >> ltl.test >> ltl.variant;
    return stream;
}

QDataStream &operator<<( QDataStream &stream, const ColorFlat &p ){
    stream << p.index << p.r << p.g << p.b;
    return stream;
}

QDataStream &operator>>( QDataStream &stream, ColorFlat &p ){
    stream >> p.index >> p.r >> p.g >> p.b;
    return stream;
}

QDataStream &operator<<( QDataStream &stream, const GraphicAnswer &p ){
    stream << p.flats << p.colorFlats;
    return stream;
}

QDataStream &operator>>( QDataStream &stream, GraphicAnswer &p ){
    stream >> p.flats >> p.colorFlats;
    return stream;
}
