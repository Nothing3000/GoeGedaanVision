#ifndef DICE_H
#define DICE_H

#include <QImage>
#include <QPixmap>
#include "qbwimage.h"

class Dice
{
private:
    struct die
    {
        int dots;
        QBWImage::ObjectCoords coords;
    };

public:
    Dice();
    Dice(const QImage& image);
    virtual ~Dice();
public:
    int amountOfDice() const;
    int dotsOnDice(int dieIndex) const;
    const QImage& getMarkedImage() const;

private:
    void process();
    void findDice();
    void markDice();
private:
    QImage originalImage;
    QImage markedImage;
    QVector<die> dice;
};

#endif // DICE_H
