#include "dice.h"
#include "qgrayimage.h"
#include <QPainter>

/*!
 * \class Dice
 * \brief The Dice class is a class that provides a way of counting the dots per die given in the image
 *
 * Dice provides a way of recognizing and extracting dice from an image.
 * When extracted, the dots per die can be counted and the total dots on all dice.
 *
 * \sa Plate
 */

/*!
 * Constructs a new instance of the class were QImage is null.
 */

Dice::Dice()
{}

/*!
 * Constructs a new \a Dice of image \a QImage
 * The dice is immediately processed.
 */

Dice::Dice(const QImage& image) :
    originalImage(image.copy())
{
    process();
}

Dice::~Dice()
{}

/*!
 * Returns an \a int with the ammount of dice.
 */

int Dice::amountOfDice() const
{
    return dice.size();
}

/*!
 * Returns an \a int with the ammount of dots on the die, given by \a dieIndex.
 */

int Dice::dotsOnDice(int dieIndex) const
{
    return dice[dieIndex].dots;
}

/*!
 * Returns an \a QImage that contains the dice marked with a rectangle and a number.
 */

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

