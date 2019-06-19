#ifndef QGRAYIMAGE_H
#define QGRAYIMAGE_H

#include <QImage>
#include "qbwimage.h"


class QGrayImage : public QImage
{
public:
    QGrayImage();
    QGrayImage(const QImage& image);
    QGrayImage(QImage&& image);
    virtual ~QGrayImage();
public:
    QBWImage toBW(uchar threshold = 127);
};

#endif // QGRAYIMAGE_H
