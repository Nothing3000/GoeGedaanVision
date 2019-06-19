#include "hsvpixel.h"
#include "rgbpixel.h"

HSVPixel::HSVPixel() :
    pixelData(3)
{}

HSVPixel::HSVPixel(double h, double s, double v) :
    pixelData(3)
{
    this->pixelData[0] = h;
    this->pixelData[1] = s;
    this->pixelData[2] = v;
}

HSVPixel::HSVPixel(const RGBPixel &rgbPixel) :
    pixelData(3)
{
    double deltaR,deltaG,deltaB;

    double r = rgbPixel[0]/255;
    double g = rgbPixel[1]/255;
    double b = rgbPixel[2]/255;

    double min = qMin(qMin(r,g),b);
    double max = qMax(qMax(r,g),b);
    double delta = max - min;


    if(delta == 0)
    {
        this->pixelData[0] = 0;
        this->pixelData[1] = 0;
    }
    else
    {
        this->pixelData[1] = delta/max;
        deltaR = (((max - r) / 6) + ( delta / 2 ) ) / delta;
        deltaG = (((max - g) / 6) + ( delta / 2 ) ) / delta;
        deltaB = (((max - b) / 6) + ( delta / 2 ) ) / delta;

        if(r == max) this->pixelData[0] = deltaB - deltaG;
        else if(g == max) this->pixelData[1] = (1 / 3) + deltaR - deltaB;
        else if(b == max) this->pixelData[2] = (2 / 3) + deltaG - deltaR;

        if(this->pixelData[0] < 0) this->pixelData[0]++;
        if(this->pixelData[0] > 1) this->pixelData[0]--;
    }
}

HSVPixel::~HSVPixel()
{

}

double& HSVPixel::operator[](int index)
{
    return this->pixelData[index];
}

const double& HSVPixel::operator[](int index) const
{
    return this->pixelData[index];
}
