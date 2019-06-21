#ifndef PLATE_H
#define PLATE_H

#include <QImage>
#include "qbwimage.h"
#include <QMap>

class Plate
{
public:
    Plate();
    Plate(const QImage& image);
public:
    const QImage& getPlate() const;
private:
    void process();
    void fillMap();
    void extractPlate();
    void extractSymbols();
private:
    QImage originalImage;
    QImage plateImage;
    QMap<QChar,QBWImage *> symbolMap;
};

#endif // PLATE_H
