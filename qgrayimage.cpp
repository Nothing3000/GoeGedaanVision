#include "qgrayimage.h"
#include <QVector>

QGrayImage::QGrayImage() :
    QImage()
{

}

QGrayImage::QGrayImage(const QImage& image) :
    QImage(image.convertToFormat(QImage::Format_Grayscale8))
{}

QGrayImage::QGrayImage(QImage&& image) :
    QImage(image.convertToFormat(QImage::Format_Grayscale8))
{}


QGrayImage::~QGrayImage()
{

}

QBWImage QGrayImage::toBW(uchar threshold)
{
    QBWImage newImage(this->width(),this->height());
    for(int h = 0; h < this->height(); h++)
    {
        for(int w = 0; w < this->width(); w++)
        {
            newImage.scanLine(h)[w] = (this->constScanLine(h)[w] >= threshold) ? 255 : 0;
        }
    }
    return newImage;
}
