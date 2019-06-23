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

int Plate::getSeries() const
{
    if(!this->plateIndex.isEmpty ())
    {
        return this->series ();
    }

    return 0;
}

int Plate::series() const
{
    bool* seriesList = new bool[14];

    seriesList[0] = (this->plateIndex[0] <= 25) & (this->plateIndex[1] <= 25) & (this->plateIndex[2] > 25) & (this->plateIndex[3] > 25) & (this->plateIndex[4] > 25) & (this->plateIndex[5] > 25);
    seriesList[1] = (this->plateIndex[0] > 25) & (this->plateIndex[1] > 25) & (this->plateIndex[2] > 25) & (this->plateIndex[3] > 25) & (this->plateIndex[4] <= 25) & (this->plateIndex[5] <= 25);
    seriesList[2] = (this->plateIndex[0] > 25) & (this->plateIndex[1] > 25) & (this->plateIndex[2] <= 25) & (this->plateIndex[3] <= 25) & (this->plateIndex[4] > 25) & (this->plateIndex[5] > 25);
    seriesList[3] = (this->plateIndex[0] <= 25) & (this->plateIndex[1] <= 25) & (this->plateIndex[2] > 25) & (this->plateIndex[3] > 25) & (this->plateIndex[4] <= 25) & (this->plateIndex[5] <= 25);
    seriesList[4] = (this->plateIndex[0] <= 25) & (this->plateIndex[1] <= 25) & (this->plateIndex[2] <= 25) & (this->plateIndex[3] <= 25) & (this->plateIndex[4] > 25) & (this->plateIndex[5] > 25);
    seriesList[5] = (this->plateIndex[0] > 25) & (this->plateIndex[1] > 25) & (this->plateIndex[2] <= 25) & (this->plateIndex[3] <= 25) & (this->plateIndex[4] <= 25) & (this->plateIndex[5] <= 25);

    seriesList[6] = (this->plateIndex[0] > 25) & (this->plateIndex[1] > 25) & (this->plateIndex[2] <= 25) & (this->plateIndex[3] <= 25) & (this->plateIndex[4] <= 25) & (this->plateIndex[5] > 25);
    seriesList[7] = (this->plateIndex[0] > 25) & (this->plateIndex[1] <= 25) & (this->plateIndex[2] <= 25) & (this->plateIndex[3] <= 25) & (this->plateIndex[4] > 25) & (this->plateIndex[5] > 25);
    seriesList[8] = (this->plateIndex[0] <= 25) & (this->plateIndex[1] <= 25) & (this->plateIndex[2] > 25) & (this->plateIndex[3] > 25) & (this->plateIndex[4] > 25) & (this->plateIndex[5] <= 25);
    seriesList[9] = (this->plateIndex[0] <= 25) & (this->plateIndex[1] > 25) & (this->plateIndex[2] > 25) & (this->plateIndex[3] > 25) & (this->plateIndex[4] <= 25) & (this->plateIndex[5] <= 25);

    seriesList[10] = (this->plateIndex[0] <= 25) & (this->plateIndex[1] <= 25) & (this->plateIndex[2] <= 25) & (this->plateIndex[3] > 25) & (this->plateIndex[4] > 25) & (this->plateIndex[5] <= 25);
    seriesList[11] = (this->plateIndex[0] <= 25) & (this->plateIndex[1] > 25) & (this->plateIndex[2] > 25) & (this->plateIndex[3] <= 25) & (this->plateIndex[4] <= 25) & (this->plateIndex[5] <= 25);
    seriesList[12] = (this->plateIndex[0] > 25) & (this->plateIndex[1] <= 25) & (this->plateIndex[2] <= 25) & (this->plateIndex[3] > 25) & (this->plateIndex[4] > 25) & (this->plateIndex[5] > 25);
    seriesList[13] = (this->plateIndex[0] > 25) & (this->plateIndex[1] > 25) & (this->plateIndex[2] > 25) & (this->plateIndex[3] <= 25) & (this->plateIndex[4] <= 25) & (this->plateIndex[5] > 25);

    for(int i = 0; i < 14; i++)
    {
        if(seriesList[i])
        {
            return i+1;
        }
    }

    return 0;
}

void Plate::implementSeries()
{
    QString newPlate = this->getText ();

    int series = this->getSeries ();

    if((newPlate.size () - 1) == 6)
    {
        if(series == 1 || series == 2 || series == 3 || series == 4 || series == 5 || series == 6)
        {
            newPlate.insert(2, '-');
            newPlate.insert(5, '-');
        }
        else if (series == 7 || series == 9)
        {
            newPlate.insert(2, '-');
            newPlate.insert(6, '-');
        }
        else if (series == 8 || series == 10)
        {
            newPlate.insert(1, '-');
            newPlate.insert(6, '-');
        }
        else if (series == 11 || series == 14)
        {
            newPlate.insert(3, '-');
            newPlate.insert(6, '-');
        }
        else if (series == 12 || series == 13)
        {
            newPlate.insert(1, '-');
            newPlate.insert(4, '-');
        }
    }

    this->plateText.clear ();

    this->plateText = newPlate;
}

void Plate::process()
{
    this->markedImage = this->originalImage.copy();
    this->extractPlate();
    this->extractSymbols();
    this->implementSeries();
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
        this->plateText.append(identifySymbol(symbolImage));
    }
}

QChar Plate::identifySymbol(const QBWImage &symbolImage)
{
    int indexCount = 0;
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
            this->plateIndex.push_back (indexCount);
        }
        else {
            indexCount++;
        }
    }
    return foundChar;
}
