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
        this->scanLine(coord.second)[coord.first] = 255;
    }
}

QBWImage::QBWImage(int width, int height, const objectsVector objects) :
    QBWImage(width,height)
{
    for(auto object : objects)
    {
        for(auto coord : object)
        {
            this->scanLine(coord.second)[coord.first] = 255;
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
    for(currentCoord.second = 0; currentCoord.second < this->height(); currentCoord.second++)
    {
        for(currentCoord.first = 0; currentCoord.first < this->width(); currentCoord.first++)
        {
            if(tempImage.constScanLine(currentCoord.second)[currentCoord.first] == 255)
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
        if(currentCoords.first < 0 || currentCoords.first >= this->width() ||
           currentCoords.second < 0 || currentCoords.second >= this->height());
        else if(labelImage.constScanLine(currentCoords.second)[currentCoords.first] == 255)
        {
            connectedCoords.push_back(currentCoords);
            labelImage.scanLine(currentCoords.second)[currentCoords.first] = 0;

            coordsToCheck.push_back(QPair<int,int>(currentCoords.first,currentCoords.second-1));
            coordsToCheck.push_back(QPair<int,int>(currentCoords.first,currentCoords.second+1));

            coordsToCheck.push_back(QPair<int,int>(currentCoords.first+1,currentCoords.second));
            coordsToCheck.push_back(QPair<int,int>(currentCoords.first+1,currentCoords.second-1));
            coordsToCheck.push_back(QPair<int,int>(currentCoords.first+1,currentCoords.second+1));

            coordsToCheck.push_back(QPair<int,int>(currentCoords.first-1,currentCoords.second));
            coordsToCheck.push_back(QPair<int,int>(currentCoords.first-1,currentCoords.second-1));
            coordsToCheck.push_back(QPair<int,int>(currentCoords.first-1,currentCoords.second+1));
        }
    }
}

bool QBWImage::erosionCheck(int x, int y, int size) const
{
    coord begin(x-((size-1)/2),y-((size-1)/2));
    coord end(x+((size-1)/2),y+((size-1)/2));
    if(begin.first < 0 || begin.second < 0 || end.first >= this->width() || end.second >= this->height())
    {
        return true;
    }

    for(int h = begin.second; h <= end.second; h++)
    {
        for(int w = begin.first; w <= end.first; w++)
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

    for(int h = begin.second; h <= end.second; h++)
    {
        for(int w = begin.first; w <= end.first; w++)
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
