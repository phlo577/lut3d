#include "color.h"
#include <math.h>
#include <QDebug>

#define MAX_RGB64   4095
#define X_REF     95.047
#define Y_REF    100.000
#define Z_REF    108.883

const float rgb_xyz[3][3] = {0.4124564,  0.3575761,  0.1804375,
                             0.2126729,  0.7151522,  0.0721750,
                             0.0193339,  0.1191920,  0.9503041};


const float xyz_rgb[3][3] = { 3.2406255, -1.537208, -0.4986286,
                             -0.9689307,  1.8757561, 0.0415175,
                              0.0557101, -0.2040211, 1.0569959};

uint16_t limitRgbChannel(uint16_t channel);


uint16_t limitRgbChannel(uint16_t channel)
{
    uint16_t retChannel;
    if ((int16_t)channel > MAX_RGB64)
    {
        retChannel = MAX_RGB64;
    }
    else
    {
        if ((int16_t)channel < 0)
        {
            retChannel = 0;
        }
        else
        {
            retChannel = channel;
        }
    }
    return retChannel;
}

xyz_t rgb2xyz(QRgba64 rgb)
{
    xyz_t xyz;
    xyz.x = (rgb_xyz[0][0] * rgb.red() + rgb_xyz[0][1] * rgb.green() + rgb_xyz[0][2] * rgb.blue())/MAX_RGB64;
    xyz.y = (rgb_xyz[1][0] * rgb.red() + rgb_xyz[1][1] * rgb.green() + rgb_xyz[1][2] * rgb.blue())/MAX_RGB64;
    xyz.z = (rgb_xyz[2][0] * rgb.red() + rgb_xyz[2][1] * rgb.green() + rgb_xyz[2][2] * rgb.blue())/MAX_RGB64;
    return xyz;
}

QRgba64 xyz2rgb(xyz_t xyz)
{
    QRgba64 rgb;
    uint16_t red, green, blue;
    red   = (xyz_rgb[0][0] * xyz.x + xyz_rgb[0][1] * xyz.y + xyz_rgb[0][2] * xyz.z) * MAX_RGB64 + 0.5;
    green = (xyz_rgb[1][0] * xyz.x + xyz_rgb[1][1] * xyz.y + xyz_rgb[1][2] * xyz.z) * MAX_RGB64 + 0.5;
    blue  = (xyz_rgb[2][0] * xyz.x + xyz_rgb[2][1] * xyz.y + xyz_rgb[2][2] * xyz.z) * MAX_RGB64 + 0.5;

    rgb.setRed(limitRgbChannel(red));
    rgb.setGreen(limitRgbChannel(green));
    rgb.setBlue(limitRgbChannel(blue));
    return rgb;
}

lab_t xyz2lab(xyz_t xyz)
{
    lab_t lab;
    float x_rel;
    float y_rel;
    float z_rel;
    float exp = 0.3333;

    // Calculate relative X
    x_rel = xyz.x * 100/X_REF;
    if (x_rel > 0.008856)
    {
        x_rel = pow(x_rel, exp);
    }
    else
    {
        x_rel = (7.78706896552 * x_rel) + 0.137931;
    }

    // Calculate relative Y
    y_rel = xyz.y * 100/Y_REF;
    if (y_rel > 0.008856)
    {
        y_rel = pow(y_rel, exp);
    }
    else
    {
        y_rel = (7.78706896552 * y_rel) + 0.137931;
    }

    // Calculate relative Z
    z_rel = xyz.z * 100/Z_REF;
    if (z_rel > 0.008856)
    {
        z_rel = pow(z_rel, exp);
    }
    else
    {
        z_rel = (7.78706896552 * z_rel) + 0.137931;
    }

    // Calucalte L*a*b* values
    lab.L = (116 * y_rel) - 16;
    lab.a = 500 * (x_rel - y_rel);
    lab.b = 200 * (y_rel - z_rel);

    return lab;
}

xyz_t lab2xyz(lab_t lab)
{
    xyz_t xyz;
    float x_rel;
    float y_rel;
    float z_rel;
    float fx;
    float fy;
    float fz;

    if (lab.L > 100)
    {
        lab.L = 100;
    }
    else
    {
        if (lab.L < 0)
        {
            lab.L = 0;
        }
    }

    fy = (lab.L + 16)/116;
    fx = fy + (lab.a/500);
    fz = fy - (lab.b/200);

    // Calculate relative X
    if(pow(fx,3) >  0.008856)
    {
        x_rel = pow(fx,3);
    }
    else
    {
        x_rel = (116*fx - 16) / 903.3;
    }

    // Calculate relative Y
    if(lab.L > 8)
    {
        y_rel = pow(fy,3);
    }
    else
    {
        y_rel = lab.L/903.3;
    }

    // Calculate relative Z
    if(pow(fz,3) >  0.008856)
    {
        z_rel = pow(fz,3);
    }
    else
    {
        z_rel = (116*fz - 16) / 903.3;
    }

    // Calculate XYZ values
    xyz.x = x_rel*X_REF/100;
    xyz.y = y_rel*Y_REF/100;
    xyz.z = z_rel*Z_REF/100;
    return xyz;
}

lab_t rgb2lab(QRgba64 rgb)
{
    lab_t lab;
    xyz_t xyz;
    xyz = rgb2xyz(rgb);
    lab = xyz2lab(xyz);
    return lab;
}

QRgba64 lab2rgb(lab_t lab)
{
    QRgba64 rgb;
    xyz_t xyz;
    xyz = lab2xyz(lab);
    rgb = xyz2rgb(xyz);
    return rgb;
}
