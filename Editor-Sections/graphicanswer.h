#ifndef GRAPHICANSWER_H
#define GRAPHICANSWER_H

#include <QList>
#include "point.h"
#include "linear.h"
#include "flat.h"
#include "string.h"
#include <string>
#include "writelog.h"
#include "colorflat.h"

class GraphicAnswer
{
private:
    QList<Flat> flats;
    QList<ColorFlat> colorFlats;
    GLuint n;

public:
    GraphicAnswer();
    void append(Flat flat, ColorFlat colorFlat);
    void setN(GLuint n);
    QList<Flat> getFlats();
    QList<ColorFlat> getColorFlats();
    GLuint getN();
    void clear();
    void updateColorFlat(int index, ColorFlat colorFlat);
    void setFlats(QList<Flat> f, QList<ColorFlat> c);

    friend QDataStream& operator <<( QDataStream &stream, const GraphicAnswer &p );
    friend QDataStream& operator >>( QDataStream &stream, GraphicAnswer &p );
};

#endif // GRAPHICANSWER_H
