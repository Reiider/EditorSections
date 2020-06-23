#ifndef DIGESTOGL_H
#define DIGESTOGL_H

#include "linear.h"
#include "flat.h"
#include <QColor>
#include "arraystep.h"

class DigestOgl
{
public:    
    DigestOgl();

    int colVershin;
    int colLine;
    int colFirstLine;
    QColor colorNnp;
    QString Answer;
    QList<Point> listAnswer;
    ArrayStep steps;
    QString report;

    void setBeginPosition();

    friend QDataStream& operator <<( QDataStream &stream, const DigestOgl &d );
    friend QDataStream& operator >>( QDataStream &stream, DigestOgl &d );
};

#endif // DIGESTOGL_H
