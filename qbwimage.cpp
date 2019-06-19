#include "qbwimage.h"
#include <QStack>

QBWImage::QBWImage()
{}

QBWImage::QBWImage(int width, int height) :
    QImage(width,height,QImage::Format_Grayscale8)
{
    this->fill(0);
}

QBWImage::QBWImage(int width, int height, const objectCoords object) :
    QBWImage(width,height)
{
    for(auto coord : object)
    {
        this->scanLine(coord.y())[coord.x()] = 255;
    }
}

QBWImage::QBWImage(int width, int height, const objectsVector objects) :
    QBWImage(width,height)
{
    for(auto object : objects)
    {
        for(auto coord : object)
        {
            this->scanLine(coord.y())[coord.x()] = 255;
        }
    }
}

QBWImage::~QBWImage()
{}

QBWImage::objectsVector QBWImage::conncomp() const
{
    QImage tempImage = this->copy();
    coord currentCoord;
    objectsVector result;
    for(currentCoord.setY(0); currentCoord.y() < this->height(); currentCoord.ry()++)
    {
        for(currentCoord.setX(0); currentCoord.x() < this->width(); currentCoord.rx()++)
        {
            if(tempImage.constScanLine(currentCoord.y())[currentCoord.x()] == 255)
            {
                floodFillAdd(currentCoord,result,tempImage);
            }
        }
    }
    return result;
}

QBWImage QBWImage::areaopen(int size) const
{
    objectsVector objects = conncomp();
    objectsVector::iterator object = objects.begin();
    while(object != objects.end())
    {
        if(object->size() < size)
        {
            objects.erase(object);
        }
        else
        {
            object++;
        }
    }
    return QBWImage(this->width(), this->height(), objects);
}

QBWImage QBWImage::erode(int size) const
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

QBWImage QBWImage::dilate(int size) const
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

QBWImage QBWImage::open(int size) const
{
    QBWImage erodedImage = this->erode(size);
    QBWImage dilatedImage = erodedImage.dilate(size);
    return dilatedImage;
}

QBWImage QBWImage::close(int size) const
{
    QBWImage dilatedImage = this->dilate(size);
    QBWImage erodedImage = dilatedImage.erode(size);
    return erodedImage;
}

QBWImage QBWImage::operator~() const
{
    QBWImage newImage(this->width(), this->height());
    for(int h = 0; h < this->height(); h++)
    {
        for(int w = 0; w < this->width(); w++)
        {
            newImage.scanLine(h)[w] = ~this->constScanLine(h)[w];
        }
    }
    return newImage;
}

void QBWImage::floodFillAdd(coord coord, objectsVector &result, QImage& labelImage) const
{
    QLinkedList<QBWImage::coord> coordsToCheck;
    QBWImage::coord currentCoords;
    coordsToCheck.push_back(coord);

    result.push_back(QBWImage::objectCoords());
    auto& connectedCoords = *result.rbegin();
    while(!coordsToCheck.isEmpty())
    {
        currentCoords = coordsToCheck.back();
        coordsToCheck.pop_back();
        if(currentCoords.x() < 0 || currentCoords.x() >= this->width() ||
           currentCoords.y() < 0 || currentCoords.y() >= this->height());
        else if(labelImage.constScanLine(currentCoords.y())[currentCoords.x()] == 255)
        {
            connectedCoords.push_back(currentCoords);
            labelImage.scanLine(currentCoords.y())[currentCoords.x()] = 0;

            coordsToCheck.push_back(QBWImage::coord(currentCoords.x(),currentCoords.y()-1));
            coordsToCheck.push_back(QBWImage::coord(currentCoords.x(),currentCoords.y()+1));

            coordsToCheck.push_back(QBWImage::coord(currentCoords.x()+1,currentCoords.y()));
            coordsToCheck.push_back(QBWImage::coord(currentCoords.x()+1,currentCoords.y()-1));
            coordsToCheck.push_back(QBWImage::coord(currentCoords.x()+1,currentCoords.y()+1));

            coordsToCheck.push_back(QBWImage::coord(currentCoords.x()-1,currentCoords.y()));
            coordsToCheck.push_back(QBWImage::coord(currentCoords.x()-1,currentCoords.y()-1));
            coordsToCheck.push_back(QBWImage::coord(currentCoords.x()-1,currentCoords.y()+1));
        }
    }
}

bool QBWImage::erosionCheck(int x, int y, int size) const
{
    coord begin(x-((size-1)/2),y-((size-1)/2));
    coord end(x+((size-1)/2),y+((size-1)/2));
    if(begin.x() < 0 || begin.y() < 0 || end.x() >= this->width() || end.y() >= this->height())
    {
        return true;
    }

    for(int h = begin.y(); h <= end.y(); h++)
    {
        for(int w = begin.x(); w <= end.x(); w++)
        {
            if(this->constScanLine(h)[w] != 255)
            {
                return true;
            }
        }
    }
    return false;
}

bool QBWImage::dilationCheck(int x, int y, int size) const
{
    coord begin(x-((size-1)/2),y-((size-1)/2));
    coord end(x+((size-1)/2),y+((size-1)/2));

    for(int h = begin.y(); h <= end.y(); h++)
    {
        for(int w = begin.x(); w <= end.x(); w++)
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
