#include "plate.h"
#include <QPainter>
#include <QStack>
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

Plate::~Plate()
{
    for(auto image : symbolMap)
    {
        delete image;
    }
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

int Plate::getSerie() const
{
    return this->serie;
}

void Plate::findSerie()
{
    QVector<QPair<int, int>> symbolTypeList;
    QChar prevSymbol;
    symbolTypeList.push_back(QPair<int,int>(0,0));

    for(auto& symbol : this->getText())
    {
        if(symbol.isDigit() && (prevSymbol.isDigit() || prevSymbol.isNull()))
        {
            symbolTypeList.last().first++;
        }
        else if(symbol.isDigit () && prevSymbol.isLetter())
        {
            symbolTypeList.push_back(QPair<int,int>(1,0));
        }
        else if(symbol.isLetter() && prevSymbol.isDigit())
        {
            symbolTypeList.push_back(QPair<int,int>(0,1));
        }
        else if(symbol.isLetter() && (prevSymbol.isLetter() || prevSymbol.isNull()))
        {
            symbolTypeList.last().second++;
        }
        prevSymbol = symbol;
    }

    if(symbolTypeList[0].second == 2 && symbolTypeList[1].first == 4) this->serie = 1;
    else if(symbolTypeList[0].first == 4 && symbolTypeList[1].second == 2) this->serie = 2;
    else if(symbolTypeList[0].second == 4 && symbolTypeList[1].first == 2) this->serie = 5;
    else if(symbolTypeList[0].first == 2 && symbolTypeList[1].second == 4) this->serie = 6;
    else if(symbolTypeList[0].first == 2 && symbolTypeList[1].second == 2 && symbolTypeList[2].first == 2) this->serie = 3;
    else if(symbolTypeList[0].second == 2 && symbolTypeList[1].first == 2 && symbolTypeList[2].second == 2) this->serie = 4;
    else if(symbolTypeList[0].first == 2 && symbolTypeList[1].second == 3 && symbolTypeList[2].first == 1) this->serie = 7;
    else if(symbolTypeList[0].first == 1 && symbolTypeList[1].second == 3 && symbolTypeList[2].first == 2) this->serie = 8;
    else if(symbolTypeList[0].second == 2 && symbolTypeList[1].first == 3 && symbolTypeList[2].second == 1) this->serie = 9;
    else if(symbolTypeList[0].second == 1 && symbolTypeList[1].first == 3 && symbolTypeList[2].second == 2) this->serie = 10;
    else if(symbolTypeList[0].second == 3 && symbolTypeList[1].first == 2 && symbolTypeList[2].second == 1) this->serie = 11;
    else if(symbolTypeList[0].second == 1 && symbolTypeList[1].first == 2 && symbolTypeList[2].second == 3) this->serie = 12;
    else if(symbolTypeList[0].first == 1 && symbolTypeList[1].second == 2 && symbolTypeList[2].first == 3) this->serie = 13;
    else if(symbolTypeList[0].first == 3 && symbolTypeList[1].second == 2 && symbolTypeList[2].first == 1) this->serie = 14;
}

void Plate::implementSeries()
{
    QString& newPlate = this->plateText;

    if((newPlate.size()) == 6)
    {
        if (this->serie == 7 || this->serie == 9)
        {
            newPlate.insert(2, '-');
            newPlate.insert(6, '-');
        }
        else if (this->serie == 8 || this->serie == 10)
        {
            newPlate.insert(1, '-');
            newPlate.insert(5, '-');
        }
        else if (this->serie == 11 || this->serie == 14)
        {
            newPlate.insert(3, '-');
            newPlate.insert(6, '-');
        }
        else if (this->serie == 12 || this->serie == 13)
        {
            newPlate.insert(1, '-');
            newPlate.insert(4, '-');
        }
        else
        {
            newPlate.insert(2, '-');
            newPlate.insert(5, '-');
        }
    }
}

void Plate::process()
{
    this->markedImage = this->originalImage.copy();
    this->extractPlate();
    this->extractSymbols();
    this->findSerie();
    this->implementSeries();
}

void Plate::fillMap()
{
    const QString alphanum = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123567890";
    QBWImage *symbolImage;
    QString filePath;

    for(auto symbolChar : alphanum)
    {
        filePath = QString(":/templatePrefix/templates/")+symbolChar+QString(".bmp");
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
