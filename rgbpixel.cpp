#include "rgbpixel.h"
#include "hsvpixel.h"

RGBPixel::RGBPixel() :
    pixelData(3)
{

}

RGBPixel::RGBPixel(int32_t r, int32_t g, int32_t b) :
    pixelData(3)
{
    this->pixelData[0] = r;
    this->pixelData[1] = g;
    this->pixelData[2] = b;
}

RGBPixel::RGBPixel(const HSVPixel& hsvPixel) :
    pixelData(3)
{
    const double& h = hsvPixel[0];
    const double& s = hsvPixel[1];
    const double& v = hsvPixel[2];

    double var_h,var_1,var_2,var_3;
    double var_r,var_g,var_b;
    int var_i;

    if ( s == 0 )
    {
       this->pixelData[0] = static_cast<int32_t>(v * 255);
       this->pixelData[1] = static_cast<int32_t>(v * 255);
       this->pixelData[2] = static_cast<int32_t>(v * 255);
    }
    else
    {
       var_h = h * 6;
       if ( var_h == 6 ) var_h = 0;      //H must be < 1
       var_i = int( var_h );             //Or ... var_i = floor( var_h )
       var_1 = v * ( 1 - s );
       var_2 = v * ( 1 - s * ( var_h - var_i ) );
       var_3 = v * ( 1 - s * ( 1 - ( var_h - var_i ) ) );

       if      ( var_i == 0 ) { var_r = v     ; var_g = var_3 ; var_b = var_1 ;}
       else if ( var_i == 1 ) { var_r = var_2 ; var_g = v     ; var_b = var_1 ;}
       else if ( var_i == 2 ) { var_r = var_1 ; var_g = v     ; var_b = var_3 ;}
       else if ( var_i == 3 ) { var_r = var_1 ; var_g = var_2 ; var_b = v     ;}
       else if ( var_i == 4 ) { var_r = var_3 ; var_g = var_1 ; var_b = v     ;}
       else                   { var_r = v     ; var_g = var_1 ; var_b = var_2 ;}

       this->pixelData[0] = static_cast<int32_t>(var_r * 255);
       this->pixelData[1] = static_cast<int32_t>(var_g * 255);
       this->pixelData[2] = static_cast<int32_t>(var_b * 255);
    }
}

RGBPixel::~RGBPixel()
{

}

int32_t& RGBPixel::operator[](int index)
{
    return pixelData[index];
}

const int32_t& RGBPixel::operator[](int index) const
{
    return pixelData[index];
}
