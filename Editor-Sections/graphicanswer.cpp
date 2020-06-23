#include "graphicanswer.h"

GraphicAnswer::GraphicAnswer()
{
}

void GraphicAnswer::append(Flat flat, ColorFlat colorFlat)
{
    flats.append(flat);
    colorFlats.append(colorFlat);
}

void GraphicAnswer::setN(GLuint n)
{
    this->n = n;
}

QList<Flat> GraphicAnswer::getFlats()
{
    return flats;
}

QList<ColorFlat> GraphicAnswer::getColorFlats()
{
    return colorFlats;
}

GLuint GraphicAnswer::getN()
{
    return n;
}

void GraphicAnswer::clear()
{
    n = 0;
    flats.clear();
    colorFlats.clear();
}

void GraphicAnswer::updateColorFlat(int index, ColorFlat colorFlat)
{
    colorFlats.removeAt(index);
    colorFlats.insert(index, colorFlat);
}

void GraphicAnswer::setFlats(QList<Flat> f, QList<ColorFlat> c)
{
    flats = f;
    colorFlats = c;
}
