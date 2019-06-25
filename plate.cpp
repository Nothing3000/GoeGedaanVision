#include "plate.h"
#include <QPainter>
#include <QStack>
#include "qhsvimage.h"
#include "qgrayimage.h"
#include "qbwimage.h"

/*!
 * \class Plate
 * \brief The Plate class is a class that provides a way of telling which characters are on a Dutch license plate.
 *
 * Plate provides a way of recognizing and extracting a numberplate from an image.
 * When extracted, the characters can be extracted and recognized with the use of templates.
 * The class is also able to recognize all the possible series of the Dutch license plates.
 *
 * \sa Dice
 */

/*!
 * Constructs a new instance of the class were QImage is null.
 */

Plate::Plate()
{}

/*!
 * Constructs a new \a Plate of image \a QImage
 * The character templates are extracted and placed in the map.
 * The plate is immediately processed after that.
 */

Plate::Plate(const QImage& image) :
    originalImage(image)
{
    fillMap();
    process();
}

/*!
 * Deconstructs the \a Plate and deletes each \a QBWImage in the \a symbolMap.
 */

Plate::~Plate()
{
    for(auto image : symbolMap)
    {
        delete image;
    }
}

/*!
 * Returns an \a QImage that contains the input image where the license plate marked with a rectangle.
 */

const QImage &Plate::getMarkedImage() const
{
    return this->markedImage;
}

/*!
 * Returns an \a QImage that contains the license plate on its own.
 */

const QImage &Plate::getPlate() const
{
    return this->plateImage;
}

/*!
 * Returns an \a QString that contains the text that is extracted from the license plate.
 */

const QString &Plate::getText() const
{
    return this->plateText;
}

/*!
 * Returns an \a int that contains the serie of the license plate.
 */

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

    symbolTypeList.resize(3);

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
    const int plateSize = (originalImage.width()/10 * originalImage.height()/5)/2;

    painter.setPen(pen);

    processedImage = hsvImage.toBW(isPlateColor);
    processedImage = processedImage.cleanBorder();
    processedImage = processedImage.areaopen(plateSize);
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
