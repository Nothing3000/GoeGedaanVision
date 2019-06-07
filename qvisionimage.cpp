#include "qvisionimage.h"

QVisionImage::QVisionImage()
{

}

QVisionImage::QVisionImage(QImage& image)
{
    gray = new QImage(image.convertToFormat(QImage::Format_Grayscale8));
}


QVisionImage::~QVisionImage()
{

}
