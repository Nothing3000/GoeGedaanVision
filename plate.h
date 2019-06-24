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
    virtual ~Plate();
public:
    const QImage& getMarkedImage() const;
    const QImage& getPlate() const;
    const QString& getText() const;
    int getSerie() const;
private:
    void findSerie();
    void implementSeries();
    void process();
    void fillMap();
    void extractPlate();
    void extractSymbols();
    QChar identifySymbol(const QBWImage& symbol); 
private:
    QImage originalImage;
    QImage markedImage;
    QImage plateImage;
    QMap<QChar,QBWImage *> symbolMap;
    QString plateText;
    int serie;
};

#endif // PLATE_H
