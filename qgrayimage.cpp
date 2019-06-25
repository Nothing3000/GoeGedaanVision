#include "qgrayimage.h"
#include <QVector>

/*!
 * \class QGrayImage
 * \brief The QGrayImage class stores images in grayscale format.
 *
 * This class is used as a in between step for converting QImages to QBWImages.
 *
 * \sa QBWImage, QHSVImage, QImage
 */

/*!
 * Constructs an empty QGrayImage
 */

QGrayImage::QGrayImage() :
    QImage()
{

}

/*!
 * Constructs a \a QGrayImage based on a reference to a \a QImage.
 */

QGrayImage::QGrayImage(const QImage& image) :
    QImage(image.convertToFormat(QImage::Format_Grayscale8))
{}

/*!
 * Constructs a \a QGrayImage based on a rvalue reference to a \a QImage.
 */
QGrayImage::QGrayImage(QImage&& image) :
    QImage(image.convertToFormat(QImage::Format_Grayscale8))
{}


QGrayImage::~QGrayImage()
{

}

/*!
 * Returns a new \a QBWImage. Pixels with a value above \a threshold become white and values below \a threshold become black.
 * The default \a threshold is 127.
 */

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
