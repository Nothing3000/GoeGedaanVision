#ifndef QHSVIMAGE_H
#define QHSVIMAGE_H

#include <QImage>
#include <QVector>

#include "hsvpixel.h"

class QHSVImage
{
public:
    QHSVImage();
    QHSVImage(const QImage& image);
    virtual ~QHSVImage();
public:
    QImage convertToImage() const;
private:
    void addPixel(const int32_t& red, const int32_t& green, const int32_t& blue);
private:
    QVector<QVector<HSVPixel>> imageData;
};

#endif // QHSVIMAGE_H
