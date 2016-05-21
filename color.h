#ifndef COLOR_H
#define COLOR_H
#include <stdint.h>
#include <QColor>


typedef struct lab
{
    float L;
    float a;
    float b;
} lab_t;

typedef struct xyz
{
    float x;
    float y;
    float z;
} xyz_t;


xyz_t rgb2xyz(QRgba64 rgb);
QRgba64 xyz2rgb(xyz_t xyz);

lab_t xyz2lab(xyz_t xyz);
xyz_t lab2xyz(lab_t lab);

lab_t rgb2lab(QRgba64 rgb);
QRgba64 lab2rgb(lab_t lab);

#endif // COLOR_H
