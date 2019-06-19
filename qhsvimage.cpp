#include "qhsvimage.h"
#include "rgbpixel.h"

QHSVImage::QHSVImage()
{

}

QHSVImage::QHSVImage(const QImage &image) :
    imageData(image.height())
{
    const QImage imageCopy = image.convertToFormat(QImage::Format_RGB32);
    RGBPixel pixel;
    const int32_t *pixelData;
    const int pixelSize = sizeof(int32_t)*3;

    for(int y = 0; y < imageCopy.height(); y++)
    {
        this->imageData.push_back(QVector<HSVPixel>(image.width()));
        for(int x = 0; x < imageCopy.bytesPerLine(); x += pixelSize)
        {
            pixelData = reinterpret_cast<const int32_t *>(&imageCopy.constScanLine(y)[x]);
            pixel = RGBPixel(pixel[0],pixel[1],pixel[2]);
            this->imageData[y].push_back(pixel);
        }
    }

}

QHSVImage::~QHSVImage()
{

}

QImage QHSVImage::convertToImage() const
{
    QImage newImage(imageData.size(),imageData[0].size(),QImage::Format_RGB32);
    RGBPixel pixel;
    int32_t *pixelData;
    const int pixelSize = sizeof(int32_t)*3;

    if(imageData.isEmpty()) return newImage;

    for(int y = 0; y < imageData.size(); y++)
    {
        for(int x = 0; x < imageData[y].size(); x++)
        {
            pixel = this->imageData[y][x];
            pixelData = reinterpret_cast<int32_t *>(newImage.scanLine(y)[x*pixelSize]);

            pixelData[0] = pixel[0];
            pixelData[1] = pixel[1];
            pixelData[2] = pixel[2];
        }
    }

    return newImage;
}
