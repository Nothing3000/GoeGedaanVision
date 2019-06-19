#ifndef RGBPIXEL_H
#define RGBPIXEL_H

#include <QVector>

class HSVPixel;

class RGBPixel
{
public:
    RGBPixel();
    RGBPixel(int32_t r, int32_t g, int32_t b);
    RGBPixel(const HSVPixel& hsvPixel);
    virtual ~RGBPixel();
public:
    int32_t& operator[](int index);
    const int32_t& operator[](int index) const;
private:
    QVector<int32_t> pixelData;
};

#endif // RGBPIXEL_H
