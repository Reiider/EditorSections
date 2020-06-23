#ifndef COLORFLAT_H
#define COLORFLAT_H

#include <QDataStream>

class ColorFlat
{
public:
    double r; //from 0 to 1
    double g;
    double b;
    int index;

    ColorFlat(double r = 0, double g = 0, double b = 0, int index = -1);

    friend QDataStream& operator <<( QDataStream &stream, const ColorFlat &p );
    friend QDataStream& operator >>( QDataStream &stream, ColorFlat &p );
};

#endif // COLORFLAT_H
