#include "qhsvimage.h"

/*!
 * \class QHSVImage
 * \brief The QHSVImage class is a class that provides a way of storing images in HSV format.
 *
 * QHSVImage provides a way of storing and filtering HSV based images.
 * It is mostly used to filter out a specific color.
 *
 * \sa QBWImage, QGrayImage, QImage
 */

/*!
 * Constructs a new instance of the class were imageData is null.
 */

QHSVImage::QHSVImage()
{

}

/*!
 * Constructs a new instance of the class were imageData contains the pixel data of \a image;
 */

QHSVImage::QHSVImage(const QImage &image)
{
    QColor pixel;

    for(int y = 0; y < image.height(); y++)
    {
        this->imageData.push_back(QVector<QColor>());
        for(int x = 0; x < image.width(); x++)
        {
            pixel = image.pixelColor(x,y);
            this->imageData[y].push_back(pixel.toHsv());
        }
    }

}

QHSVImage::~QHSVImage()
{

}

/*!
 * Returns a new \a QImage that is created according to the values in \a imageData.
 */

QImage QHSVImage::toImage() const
{
    QImage newImage(imageData[0].size(),imageData.size(),QImage::Format_RGB32);
    QColor pixel;

    if(imageData.isEmpty()) return newImage;

    for(int y = 0; y < imageData.size(); y++)
    {
        for(int x = 0; x < imageData[y].size(); x++)
        {
            pixel = this->imageData[y][x];
            newImage.setPixelColor(x,y,pixel.toRgb());
        }
    }

    return newImage;
}

/*!
 * Returns a new \a QBWImage using \a fn as a compare function.
 * (An example is included below)
 */

QBWImage QHSVImage::toBW(QHSVImage::compareFunction fn) const
{
    QBWImage newImage(imageData[0].size(),imageData.size());
    QColor pixel;
    for(int y = 0; y < imageData.size(); y++)
    {
        for(int x = 0; x < imageData[y].size(); x++)
        {
            pixel = this->imageData[y][x];
            if(fn(pixel))
            {
                newImage.scanLine(y)[x] = 255;
            }
        }
    }
    return newImage;
}

/*!
 * Returns \c true if the color of the pixel matches the color of a dutch license plate.
 */

bool isPlateColor(const QColor& pixel)
{
    if(pixel.hue() > 35 && pixel.hue() < 50 && pixel.saturation() > 150 && pixel.value() > 180)
    {
        return true;
    }
    else
    {
        return false;
    }
}
