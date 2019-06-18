#include "qbwimage.h"

QBWImage::QBWImage()
{}

QBWImage::QBWImage(int width, int height) :
    QImage(width,height,QImage::Format_Grayscale8)
{
    this->fill(0);
}

QBWImage::~QBWImage()
{}

/*QBWImage::concompResult *QBWImage::concomp()
{
    const uchar *imageData = this->constBits();
    for(int h = 0; h < this->height(); h++)
    {
        for(int w = 0; w < this->width(); w++)
        {
            this->constScanLine(h)[w];
        }
    }
}*/

QBWImage QBWImage::erode(int size)
{
    QBWImage newImage(this->width(),this->height());
    for(int h = 0; h < this->height(); h++)
    {
        for(int w = 0; w < this->width(); w++)
        {
            if(!erosionCheck(w,h,size))
            {
                newImage.scanLine(h)[w] = 255;
            }
        }
    }
    return newImage;
}

QBWImage QBWImage::dilate(int size)
{
    QBWImage newImage(this->width(),this->height());
    for(int h = 0; h < this->height(); h++)
    {
        for(int w = 0; w < this->width(); w++)
        {
            if(dilationCheck(w,h,size))
            {
                newImage.scanLine(h)[w] = 255;
            }
        }
    }
    return newImage;
}

QBWImage QBWImage::open(int size)
{
    QBWImage erodedImage = this->erode(size);
    QBWImage dilatedImage = erodedImage.dilate(size);
    return dilatedImage;
}

QBWImage QBWImage::close(int size)
{
    QBWImage dilatedImage = this->dilate(size);
    QBWImage erodedImage = dilatedImage.erode(size);
    return erodedImage;
}

bool QBWImage::erosionCheck(int x, int y, int size)
{
    int beginW = x-((size-1)/2);
    int beginH = y-((size-1)/2);
    int endW = x+((size-1)/2);
    int endH = y+((size-1)/2);
    if(beginW < 0 || beginH < 0 || endW >= this->width() || endH >= this->height())
    {
        return true;
    }

    for(int h = beginH; h <= endH; h++)
    {
        for(int w = beginW; w <= endW; w++)
        {
            if(this->constScanLine(h)[w] != 255)
            {
                return true;
            }
        }
    }
    return false;
}

bool QBWImage::dilationCheck(int x, int y, int size)
{
    int beginW = x-((size-1)/2);
    int beginH = y-((size-1)/2);
    int endW = x+((size-1)/2);
    int endH = y+((size-1)/2);


    for(int h = beginH; h <= endH; h++)
    {
        for(int w = beginW; w <= endW; w++)
        {
            if(!(w < 0 || h < 0 || w >= this->width() || h >= this->height()))
            {
                if(this->constScanLine(h)[w] == 255)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void QBWImage::cleanupImage(void *info)
{
    delete static_cast<QVector<uchar>*>(info);
}
