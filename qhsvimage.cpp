#include "qhsvimage.h"

QHSVImage::QHSVImage()
{

}

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

bool isPlateColor(const QColor &pixel)
{
    if(pixel.hue() > 40 && pixel.hue() < 60 && pixel.saturation() > 60 && pixel.value() > 50)
    {
        return true;
    }
    else
    {
        return false;
    }
}
