#include "plate.h"
#include <QPainter>
#include "qhsvimage.h"
#include "qgrayimage.h"
#include "qbwimage.h"

Plate::Plate()
{

}

Plate::Plate(const QImage& image) :
    originalImage(image)
{
    fillMap();
    process();
}

const QImage &Plate::getMarkedImage() const
{
    return this->markedImage;
}

const QImage &Plate::getPlate() const
{
    return this->plateImage;
}

const QString &Plate::getText() const
{
    return this->plateText;
}


void Plate::process()
{
    this->markedImage = this->originalImage.copy();
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
    QPainter painter(&this->markedImage);
    QPen pen(Qt::red, 10, Qt::SolidLine);
    QHSVImage hsvImage(this->originalImage);
    QBWImage processedImage;

    painter.setPen(pen);

    processedImage = hsvImage.toBW(isPlateColor);
    processedImage = processedImage.cleanBorder();
    processedImage = processedImage.areaopen(3000);
    QRect box = processedImage.boundingBox();
    this->plateImage = this->originalImage.copy(box);
    painter.drawRect(box);
}

void Plate::extractSymbols()
{
    QGrayImage grayPlate(this->plateImage);
    QBWImage bwPlate = ~grayPlate.toBW(50);
    QBWImage symbolImage;
    QBWImage::ObjectsVector symbols;
    const int area = static_cast<int>((bwPlate.height()/2.0)*(bwPlate.width()/100.0));

    this->plateText.clear();
    bwPlate = bwPlate.cleanBorder();
    bwPlate = bwPlate.areaopen(area);
    symbols = bwPlate.conncomp();

    for(auto& symbol : symbols)
    {
        symbolImage = bwPlate.copy(QBWImage::boundingBox(symbol));
        symbolImage = QGrayImage(symbolImage.scaled(24,42)).toBW();
        plateText.append(identifySymbol(symbolImage));
    }
}

QChar Plate::identifySymbol(const QBWImage &symbolImage)
{
    double correlationCoef;
    double maxCorrelationCoef = 0;
    QChar foundChar;
    for(auto& symbolChar : symbolMap.keys())
    {
        correlationCoef = symbolImage.corr2(*symbolMap[symbolChar]);
        if(maxCorrelationCoef < correlationCoef)
        {
            maxCorrelationCoef = correlationCoef;
            foundChar = symbolChar;
        }
    }
    return foundChar;
}
