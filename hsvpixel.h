#ifndef HSVPIXEL_H
#define HSVPIXEL_H

#include <QVector>

class RGBPixel;

class HSVPixel
{
public:
    HSVPixel();
    HSVPixel(double h, double s, double v);
    HSVPixel(const RGBPixel& rgbPixel);
    virtual ~HSVPixel();
public:
    double& operator[](int index);
    const double& operator[](int index) const;
private:
    QVector<double> pixelData;
};

#endif // HSVPIXEL_H
