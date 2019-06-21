#include "dice.h"
#include "qgrayimage.h"
#include <QPainter>

Dice::Dice()
{

}

Dice::Dice(const QImage& image) :
    originalImage(image.copy())
{
    process();
}

Dice::~Dice()
{}

int Dice::amountOfDice() const
{
    return dice.size();
}

int Dice::dotsOnDice(int dieIndex) const
{
    return dice[dieIndex].dots;
}

const QImage& Dice::getMarkedImage() const
{
    return this->markedImage;
}

void Dice::process()
{
    findDice();
    markDice();
}

void Dice::findDice()
{
    QGrayImage grayImage;
    QBWImage processedImage;
    QBWImage::ObjectsVector dice;
    QBWImage dieImage;

    if(!this->originalImage.isNull())
    {
        grayImage = QGrayImage(this->originalImage);
        processedImage = grayImage.toBW(202);
        processedImage = processedImage.close(7);
        processedImage = processedImage.areaopen(1900);

        dice = processedImage.conncomp();

        for(auto& die : dice)
        {
            this->dice.push_back(Dice::die());
            dieImage = ~(processedImage.copy(QBWImage::boundingBox(die)));
            dieImage = dieImage.cleanBorder();
            this->dice.rbegin()->dots = dieImage.conncomp().size();
            this->dice.rbegin()->coords = die;
        }
    }
}

void Dice::markDice()
{
    this->markedImage = originalImage.copy();
    QPainter painter(&this->markedImage);
    QPen pen(Qt::red, 10, Qt::SolidLine);
    QRect rectangle;
    QPointF textPoint;
    QString number = "";
    painter.setPen(pen);

    QFont font("Kenteken", 100, QFont::Bold);
    painter.setFont (font);

    int dieCount = 1;
    int topX = 0, topY = 0, width = 0, height = 0;

    for(auto& die : this->dice)
    {
        rectangle = QBWImage::boundingBox(die.coords);
        painter.drawRect(rectangle);
        rectangle.getRect (&topX, &topY, &width, &height);
        textPoint.setX (topX + (width/2));
        textPoint.setY (topY);
        number = QString::number (dieCount++);
        painter.drawText (textPoint, number);
    }
}

