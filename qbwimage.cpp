#include "qbwimage.h"
#include <QStack>
#include <math.h>

QBWImage::QBWImage()
{}

QBWImage::QBWImage(int width, int height) :
    QImage(width,height,QImage::Format_Grayscale8)
{
    this->fill(0);
}

QBWImage::QBWImage(int width, int height, const ObjectCoords object) :
    QBWImage(width,height)
{
    for(auto coord : object)
    {
        this->scanLine(coord.y())[coord.x()] = 255;
    }
}

QBWImage::QBWImage(int width, int height, const ObjectsVector objects) :
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

QBWImage::ObjectsVector QBWImage::conncomp() const
{
    QImage tempImage = this->copy();
    Coord currentCoord;
    ObjectsVector result;
    for(currentCoord.setX(0); currentCoord.x() < this->width(); currentCoord.rx()++)
    {
        for(currentCoord.setY(0); currentCoord.y() < this->height(); currentCoord.ry()++)
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
    ObjectsVector objects = conncomp();
    ObjectsVector::iterator object = objects.begin();
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
    for(int y = 0; y < this->height(); y++)
    {
        for(int x = 0; x < this->width(); x++)
        {
            if(!erosionCheck(x,y,size))
            {
                newImage.scanLine(y)[x] = 255;
            }
        }
    }
    return newImage;
}

QBWImage QBWImage::dilate(int size) const
{
    QBWImage newImage(this->width(),this->height());
    for(int y = 0; y < this->height(); y++)
    {
        for(int x = 0; x < this->width(); x++)
        {
            if(dilationCheck(x,y,size))
            {
                newImage.scanLine(y)[x] = 255;
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

QRect QBWImage::boundingBox() const
{
    QPoint topLeft;
    QPoint bottomRight;

    topLeft.setX(find(left).x());
    topLeft.setY(find(top).y());

    bottomRight.setX(find(right).x());
    bottomRight.setY(find(bottom).y());

    return QRect(topLeft,bottomRight);
}

QRect QBWImage::boundingBox(const QBWImage::ObjectCoords &object)
{
    QPoint topLeft(INT_MAX,INT_MAX);
    QPoint bottomRight;

    for(auto coord : object)
    {
        topLeft.setX(qMin(topLeft.x(),coord.x()));
        topLeft.setY(qMin(topLeft.y(),coord.y()));

        bottomRight.setX(qMax(bottomRight.x(),coord.x()));
        bottomRight.setY(qMax(bottomRight.y(),coord.y()));
    }
    return QRect(topLeft,bottomRight);
}

QBWImage::Coord QBWImage::find(findType type) const
{
    Coord coord;

    if(type == top || type == bottom)
    {
        for(int y = 0; y < this->height(); y++)
        {
            for(int x = 0; x < this->width(); x++)
            {
                if(type == top && this->constScanLine(y)[x] == 255)
                {
                    coord.setX(x);
                    coord.setY(y);
                    return coord;
                }
                else if(type == bottom && this->constScanLine(this->height()-y-1)[this->width()-x-1] == 255)
                {
                    coord.setX(this->width()-x-1);
                    coord.setY(this->height()-y-1);
                    return coord;
                }
            }
        }
    }
    else if(type == left || type == right)
    {
        for(int x = 0; x < this->width(); x++)
        {
            for(int y = 0; y < this->height(); y++)
            {
                if(type == left && this->constScanLine(y)[x] == 255)
                {
                    coord.setX(x);
                    coord.setY(y);
                    return coord;
                }
                else if(type == right && this->constScanLine(this->height()-y-1)[this->width()-x-1] == 255)
                {
                    coord.setX(this->width()-x-1);
                    coord.setY(this->height()-y-1);
                    return coord;
                }
            }
        }
    }
    return coord;
}

QBWImage QBWImage::cleanBorder() const
{
    QBWImage newImage = this->copy();
    const QVector<int> verticalPosition = {0,this->height()-1};
    const QVector<int> horizontalPosition = {0,this->width()-1};
    for(auto y : verticalPosition)
    {
        for(int x = 0; x < newImage.width(); x++)
        {
            if(newImage.constScanLine(y)[x] == 255)
            {
                newImage.floodFillRemove(Coord(x,y));
            }
        }
    }
    for(auto x : horizontalPosition)
    {
        for(int y = 0; y < this->height(); y++)
        {
            if(newImage.constScanLine(y)[x] == 255)
            {
                newImage.floodFillRemove(Coord(x,y));
            }
        }
    }

    return newImage;

}

QBWImage QBWImage::copy() const
{
    QBWImage newImage(this->width(),this->height());

    for(int y = 0; y < this->height(); y++)
    {
        for(int x = 0; x < this->width(); x++)
        {
            newImage.scanLine(y)[x] = this->constScanLine(y)[x];
        }
    }
    return newImage;
}

QBWImage QBWImage::copy(const QRect &rect) const
{
    QBWImage newImage(rect.width(),rect.height());

    for(int y = 0; y < rect.height(); y++)
    {
        for(int x = 0; x < rect.width(); x++)
        {
            newImage.scanLine(y)[x] = this->constScanLine(y+rect.y())[x+rect.x()];
        }
    }
    return newImage;
}

double QBWImage::corr2(const QBWImage &compareImage) const
{
    if(this->width() != compareImage.width() || this->height() != compareImage.height())
    {
        return 0.0;
    }

    const int aMean = this->mean2();
    const int bMean = compareImage.mean2();
    int aDev,bDev;
    long devSum = 0;
    long aDevSquareSum = 0;
    long bDevSquareSum = 0;

    for(int y = 0; y < this->height(); y++)
    {
        for(int x = 0; x < this->width(); x++)
        {
            aDev = this->constScanLine(y)[x] - aMean;
            bDev = compareImage.constScanLine(y)[x] - bMean;

            devSum += aDev*bDev;
            aDevSquareSum += aDev*aDev;
            bDevSquareSum += bDev*bDev;
        }
    }

    return devSum/sqrt(aDevSquareSum*bDevSquareSum);

}

int QBWImage::mean2() const
{
    long totalValue = 0;
    int amountOfPixels = this->height()*this->width();
    for(int y = 0; y < this->height(); y++)
    {
        for(int x = 0; x < this->width(); x++)
        {
            totalValue += this->constScanLine(y)[x];
        }
    }
    return static_cast<int>(totalValue/amountOfPixels);
}

void QBWImage::floodFillAdd(Coord coord, ObjectsVector &result, QImage& labelImage) const
{
    QStack<QBWImage::Coord> coordsToCheck;
    QBWImage::Coord currentCoords;
    coordsToCheck.push_back(coord);

    result.push_back(QBWImage::ObjectCoords());
    auto& connectedCoords = *result.rbegin();
    while(!coordsToCheck.isEmpty())
    {
        currentCoords = coordsToCheck.pop();
        //coordsToCheck.pop_back();
        if(currentCoords.x() < 0 || currentCoords.x() >= this->width() ||
           currentCoords.y() < 0 || currentCoords.y() >= this->height());
        else if(labelImage.constScanLine(currentCoords.y())[currentCoords.x()] == 255)
        {
            connectedCoords.push_back(currentCoords);
            labelImage.scanLine(currentCoords.y())[currentCoords.x()] = 0;

            coordsToCheck.push(QBWImage::Coord(currentCoords.x(),currentCoords.y()-1));
            coordsToCheck.push(QBWImage::Coord(currentCoords.x(),currentCoords.y()+1));

            coordsToCheck.push(QBWImage::Coord(currentCoords.x()+1,currentCoords.y()));
            coordsToCheck.push(QBWImage::Coord(currentCoords.x()+1,currentCoords.y()-1));
            coordsToCheck.push(QBWImage::Coord(currentCoords.x()+1,currentCoords.y()+1));

            coordsToCheck.push(QBWImage::Coord(currentCoords.x()-1,currentCoords.y()));
            coordsToCheck.push(QBWImage::Coord(currentCoords.x()-1,currentCoords.y()-1));
            coordsToCheck.push(QBWImage::Coord(currentCoords.x()-1,currentCoords.y()+1));
        }
    }
}

void QBWImage::floodFillRemove(Coord coord)
{
    QLinkedList<QBWImage::Coord> coordsToCheck;
    QBWImage::Coord currentCoords;
    coordsToCheck.push_back(coord);

    while(!coordsToCheck.isEmpty())
    {
        currentCoords = coordsToCheck.back();
        coordsToCheck.pop_back();
        if(currentCoords.x() < 0 || currentCoords.x() >= this->width() ||
           currentCoords.y() < 0 || currentCoords.y() >= this->height());
        else if(this->constScanLine(currentCoords.y())[currentCoords.x()] == 255)
        {
            this->scanLine(currentCoords.y())[currentCoords.x()] = 0;

            coordsToCheck.push_back(QBWImage::Coord(currentCoords.x(),currentCoords.y()-1));
            coordsToCheck.push_back(QBWImage::Coord(currentCoords.x(),currentCoords.y()+1));

            coordsToCheck.push_back(QBWImage::Coord(currentCoords.x()+1,currentCoords.y()));
            coordsToCheck.push_back(QBWImage::Coord(currentCoords.x()+1,currentCoords.y()-1));
            coordsToCheck.push_back(QBWImage::Coord(currentCoords.x()+1,currentCoords.y()+1));

            coordsToCheck.push_back(QBWImage::Coord(currentCoords.x()-1,currentCoords.y()));
            coordsToCheck.push_back(QBWImage::Coord(currentCoords.x()-1,currentCoords.y()-1));
            coordsToCheck.push_back(QBWImage::Coord(currentCoords.x()-1,currentCoords.y()+1));
        }
    }
}

bool QBWImage::erosionCheck(int x, int y, int size) const
{
    Coord begin(x-((size-1)/2),y-((size-1)/2));
    Coord end(x+((size-1)/2),y+((size-1)/2));
    if(begin.x() < 0 || begin.y() < 0 || end.x() >= this->width() || end.y() >= this->height())
    {
        return true;
    }

    for(int y = begin.y(); y <= end.y(); y++)
    {
        for(int x = begin.x(); x <= end.x(); x++)
        {
            if(this->constScanLine(y)[x] != 255)
            {
                return true;
            }
        }
    }
    return false;
}

bool QBWImage::dilationCheck(int x, int y, int size) const
{
    Coord begin(x-((size-1)/2),y-((size-1)/2));
    Coord end(x+((size-1)/2),y+((size-1)/2));

    for(int y = begin.y(); y <= end.y(); y++)
    {
        for(int x = begin.x(); x <= end.x(); x++)
        {
            if(!(x < 0 || y < 0 || x >= this->width() || y >= this->height()))
            {
                if(this->constScanLine(y)[x] == 255)
                {
                    return true;
                }
            }
        }
    }
    return false;
}
