#include "plate.h"
#include "qhsvimage.h"
#include "qgrayimage.h"
#include "qbwimage.h"

Plate::Plate()
{

}

Plate::Plate(const QImage& image) :
    originalImage(image)
{
    process();
}

const QImage &Plate::getPlate() const
{
    return this->plateImage;
}


void Plate::process()
{
    extractPlate();
    extractSymbols();
}

void Plate::fillMap()
{
    const QString alphanum = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123567890";
    QBWImage *symbolImage;
    QString filePath;

    for(auto symbolChar : alphanum)
    {
        filePath = QString("templates/")+symbolChar+QString(".bmp");
        symbolImage = new QBWImage(QGrayImage(QImage(filePath)).toBW());
        symbolMap[symbolChar] = symbolImage;
    }
}

void Plate::extractPlate()
{
    QHSVImage hsvImage(this->originalImage);
    QBWImage processedImage;

    processedImage = hsvImage.toBW(isPlateColor);
    processedImage = processedImage.cleanBorder();
    processedImage = processedImage.areaopen(3000);
    QRect box = processedImage.boundingBox();
    this->plateImage = this->originalImage.copy(box);
}

void Plate::extractSymbols()
{
    QGrayImage grayPlate(this->plateImage);
    QBWImage bwPlate = ~grayPlate.toBW(50);
    QBWImage::ObjectsVector symbols;
    const int area = static_cast<int>((bwPlate.height()/2.0)*(bwPlate.width()/100.0));

    bwPlate = bwPlate.cleanBorder();
    bwPlate = bwPlate.areaopen(area);
    symbols = bwPlate.conncomp();

    this->plateImage = bwPlate;
}
