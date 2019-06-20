#ifndef QHSVIMAGE_H
#define QHSVIMAGE_H

#include <QImage>
#include <QColor>
#include <QVector>

#include "qbwimage.h"

class QHSVImage
{
public:
    typedef bool (*compareFunction)(const QColor& pixel);
public:
    QHSVImage();
    QHSVImage(const QImage& image);
    virtual ~QHSVImage();
public:
    QImage toImage() const;
    QBWImage toBW(compareFunction fn) const;
private:
    QVector<QVector<QColor>> imageData;
};

bool isPlateColor(const QColor& pixel);

#endif // QHSVIMAGE_H
