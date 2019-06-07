#ifndef QVISIONIMAGE_H
#define QVISIONIMAGE_H

#include <QImage>
#include <QVector>


class QVisionImage
{
public:
    QVisionImage();
    QVisionImage(QImage& image);
    virtual ~QVisionImage();
private:
    QImage *gray;
    QVector<QVector<uchar>> imageMatrix;
};

#endif // QVISIONIMAGE_H
