#include <assert.h>
#include <stdlib.h>
#include "lut3d.h"
#include "QDebug"


Lut3D::Lut3D(uint16_t interval, uint16_t nodes)
{
    this->interval = interval;
    this->nodes = nodes;
    // Generate identity 3D LUT
    for (uint8_t b = 0; b < nodes; b++)
    {
        for (uint8_t g = 0; g < nodes; g++)
        {
            for (uint8_t r = 0; r < nodes; r++)
            {
                entry[b][g][r].setRed(interval * r);
                entry[b][g][r].setGreen(interval * g);
                entry[b][g][r].setBlue(interval * b);
            }
        }
    }
}

QString Lut3D::getString(void)
{
    uint16_t x;
    QString retString;
    QString entryString;
    for (int i = 0; i < MAX_INDEX; i++)
    {
        x = INTERVAL * i;
        entryString = QString("%1").arg(x, 0, 10, QChar(' '));
        retString.append(entryString);
        retString.append(' ');
    }
    retString.append('\n');
    //retString.append("lut_3d =\n;U12F\n");
    for (int b = 0; b < MAX_INDEX; b++)
    {
        for (int g = 0; g < MAX_INDEX; g++)
        {
            for (int r = 0; r < MAX_INDEX; r++)
            {
                entryString = QString("%1").arg(entry[b][g][r].red(), 0, 10, QChar(' '));
                retString.append(entryString);
                retString.append(' ');

                entryString = QString("%1").arg(entry[b][g][r].green(), 0, 10, QChar(' '));
                retString.append(entryString);
                retString.append(' ');

                entryString = QString("%1").arg(entry[b][g][r].blue(), 0, 10, QChar(' '));
                retString.append(entryString);
                retString.append('\n');
            }
        }
    }
    return retString;
}

QRgba64 Lut3D::getColor64(uint16_t red, uint16_t green, uint16_t blue)
{
    uint8_t redIndex, greenIndex, blueIndex;
    float redDif, greenDif, blueDif;
    float c8[2][2][2], c4[2][2], c2[2], c1;
    QRgba64 colorPoint;

    // Retrieve 3D LUT index
    redIndex   = getIndex(red);
    greenIndex = getIndex(green);
    blueIndex  = getIndex(blue);

    redDif   = (float)(red   - (redIndex   * INTERVAL)) / INTERVAL;
    greenDif = (float)(green - (greenIndex * INTERVAL)) / INTERVAL;
    blueDif  = (float)(blue  - (blueIndex  * INTERVAL)) / INTERVAL;

    // Interpolate red channel
    c8[0][0][0] = entry[blueIndex  ][greenIndex  ][redIndex  ].red();
    c8[0][0][1] = entry[blueIndex  ][greenIndex  ][redIndex+1].red();
    c8[0][1][0] = entry[blueIndex  ][greenIndex+1][redIndex  ].red();
    c8[0][1][1] = entry[blueIndex  ][greenIndex+1][redIndex+1].red();
    c8[1][0][0] = entry[blueIndex+1][greenIndex  ][redIndex  ].red();
    c8[1][0][1] = entry[blueIndex+1][greenIndex  ][redIndex+1].red();
    c8[1][1][0] = entry[blueIndex+1][greenIndex+1][redIndex  ].red();
    c8[1][1][1] = entry[blueIndex+1][greenIndex+1][redIndex+1].red();

    c4[0][0] = c8[0][0][0] * (1 - redDif) + c8[0][0][1] * redDif;
    c4[0][1] = c8[0][1][0] * (1 - redDif) + c8[0][1][1] * redDif;
    c4[1][0] = c8[1][0][0] * (1 - redDif) + c8[1][0][1] * redDif;
    c4[1][1] = c8[1][1][0] * (1 - redDif) + c8[1][1][1] * redDif;

    c2[0] = c4[0][0] * (1 - greenDif) + c4[0][1] * greenDif;
    c2[1] = c4[1][0] * (1 - greenDif) + c4[1][1] * greenDif;

    c1 = c2[0] * (1 - blueDif) + c2[1] * blueDif;
    colorPoint.setRed(c1);

    // Interpolate green channel
    c8[0][0][0] = entry[blueIndex  ][greenIndex  ][redIndex  ].green();
    c8[0][0][1] = entry[blueIndex  ][greenIndex  ][redIndex+1].green();
    c8[0][1][0] = entry[blueIndex  ][greenIndex+1][redIndex  ].green();
    c8[0][1][1] = entry[blueIndex  ][greenIndex+1][redIndex+1].green();
    c8[1][0][0] = entry[blueIndex+1][greenIndex  ][redIndex  ].green();
    c8[1][0][1] = entry[blueIndex+1][greenIndex  ][redIndex+1].green();
    c8[1][1][0] = entry[blueIndex+1][greenIndex+1][redIndex  ].green();
    c8[1][1][1] = entry[blueIndex+1][greenIndex+1][redIndex+1].green();

    c4[0][0] = c8[0][0][0] * (1 - redDif) + c8[0][0][1] * redDif;
    c4[0][1] = c8[0][1][0] * (1 - redDif) + c8[0][1][1] * redDif;
    c4[1][0] = c8[1][0][0] * (1 - redDif) + c8[1][0][1] * redDif;
    c4[1][1] = c8[1][1][0] * (1 - redDif) + c8[1][1][1] * redDif;

    c2[0] = c4[0][0] * (1 - greenDif) + c4[0][1] * greenDif;
    c2[1] = c4[1][0] * (1 - greenDif) + c4[1][1] * greenDif;

    c1 = c2[0] * (1 - blueDif) + c2[1] * blueDif;
    colorPoint.setGreen(c1);

    // Interpolate blue channel
    c8[0][0][0] = entry[blueIndex  ][greenIndex  ][redIndex  ].blue();
    c8[0][0][1] = entry[blueIndex  ][greenIndex  ][redIndex+1].blue();
    c8[0][1][0] = entry[blueIndex  ][greenIndex+1][redIndex  ].blue();
    c8[0][1][1] = entry[blueIndex  ][greenIndex+1][redIndex+1].blue();
    c8[1][0][0] = entry[blueIndex+1][greenIndex  ][redIndex  ].blue();
    c8[1][0][1] = entry[blueIndex+1][greenIndex  ][redIndex+1].blue();
    c8[1][1][0] = entry[blueIndex+1][greenIndex+1][redIndex  ].blue();
    c8[1][1][1] = entry[blueIndex+1][greenIndex+1][redIndex+1].blue();

    c4[0][0] = c8[0][0][0] * (1 - redDif) + c8[0][0][1] * redDif;
    c4[0][1] = c8[0][1][0] * (1 - redDif) + c8[0][1][1] * redDif;
    c4[1][0] = c8[1][0][0] * (1 - redDif) + c8[1][0][1] * redDif;
    c4[1][1] = c8[1][1][0] * (1 - redDif) + c8[1][1][1] * redDif;

    c2[0] = c4[0][0] * (1 - greenDif) + c4[0][1] * greenDif;
    c2[1] = c4[1][0] * (1 - greenDif) + c4[1][1] * greenDif;

    c1 = c2[0] * (1 - blueDif) + c2[1] * blueDif;
    colorPoint.setBlue(c1);
    return colorPoint;
}

QRgba64 Lut3D::getColor64(QRgba64 rgb)
{
    QRgba64 color = this->getColor64(rgb.red(), rgb.green(), rgb.red());
    return color;
}

QRgb Lut3D::getColor(uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t redIndex, greenIndex, blueIndex;
    float redDif, greenDif, blueDif;
    float c8[2][2][2], c4[2][2], c2[2], c1;
    QRgb colorPoint;

    // Retrieve 3D LUT index
    redIndex   = getIndex(red);
    greenIndex = getIndex(green);
    blueIndex  = getIndex(blue);

    redDif   = (float)(red   - (redIndex   * INTERVAL)) / INTERVAL;
    greenDif = (float)(green - (greenIndex * INTERVAL)) / INTERVAL;
    blueDif  = (float)(blue  - (blueIndex  * INTERVAL)) / INTERVAL;

    // Interpolate red channel
    c8[0][0][0] = entry[blueIndex  ][greenIndex  ][redIndex  ].red();
    c8[0][0][1] = entry[blueIndex  ][greenIndex  ][redIndex+1].red();
    c8[0][1][0] = entry[blueIndex  ][greenIndex+1][redIndex  ].red();
    c8[0][1][1] = entry[blueIndex  ][greenIndex+1][redIndex+1].red();
    c8[1][0][0] = entry[blueIndex+1][greenIndex  ][redIndex  ].red();
    c8[1][0][1] = entry[blueIndex+1][greenIndex  ][redIndex+1].red();
    c8[1][1][0] = entry[blueIndex+1][greenIndex+1][redIndex  ].red();
    c8[1][1][1] = entry[blueIndex+1][greenIndex+1][redIndex+1].red();

    c4[0][0] = c8[0][0][0] * (1 - redDif) + c8[0][0][1] * redDif;
    c4[0][1] = c8[0][1][0] * (1 - redDif) + c8[0][1][1] * redDif;
    c4[1][0] = c8[1][0][0] * (1 - redDif) + c8[1][0][1] * redDif;
    c4[1][1] = c8[1][1][0] * (1 - redDif) + c8[1][1][1] * redDif;

    c2[0] = c4[0][0] * (1 - greenDif) + c4[0][1] * greenDif;
    c2[1] = c4[1][0] * (1 - greenDif) + c4[1][1] * greenDif;

    c1 = c2[0] * (1 - blueDif) + c2[1] * blueDif;
    colorPoint = (uint8_t)c1 << 16;

    // Interpolate green channel
    c8[0][0][0] = entry[blueIndex  ][greenIndex  ][redIndex  ].green();
    c8[0][0][1] = entry[blueIndex  ][greenIndex  ][redIndex+1].green();
    c8[0][1][0] = entry[blueIndex  ][greenIndex+1][redIndex  ].green();
    c8[0][1][1] = entry[blueIndex  ][greenIndex+1][redIndex+1].green();
    c8[1][0][0] = entry[blueIndex+1][greenIndex  ][redIndex  ].green();
    c8[1][0][1] = entry[blueIndex+1][greenIndex  ][redIndex+1].green();
    c8[1][1][0] = entry[blueIndex+1][greenIndex+1][redIndex  ].green();
    c8[1][1][1] = entry[blueIndex+1][greenIndex+1][redIndex+1].green();

    c4[0][0] = c8[0][0][0] * (1 - redDif) + c8[0][0][1] * redDif;
    c4[0][1] = c8[0][1][0] * (1 - redDif) + c8[0][1][1] * redDif;
    c4[1][0] = c8[1][0][0] * (1 - redDif) + c8[1][0][1] * redDif;
    c4[1][1] = c8[1][1][0] * (1 - redDif) + c8[1][1][1] * redDif;

    c2[0] = c4[0][0] * (1 - greenDif) + c4[0][1] * greenDif;
    c2[1] = c4[1][0] * (1 - greenDif) + c4[1][1] * greenDif;

    c1 = c2[0] * (1 - blueDif) + c2[1] * blueDif;
    // Set green
    colorPoint += (uint8_t)c1 << 8;

    // Interpolate blue channel
    c8[0][0][0] = entry[blueIndex  ][greenIndex  ][redIndex  ].blue();
    c8[0][0][1] = entry[blueIndex  ][greenIndex  ][redIndex+1].blue();
    c8[0][1][0] = entry[blueIndex  ][greenIndex+1][redIndex  ].blue();
    c8[0][1][1] = entry[blueIndex  ][greenIndex+1][redIndex+1].blue();
    c8[1][0][0] = entry[blueIndex+1][greenIndex  ][redIndex  ].blue();
    c8[1][0][1] = entry[blueIndex+1][greenIndex  ][redIndex+1].blue();
    c8[1][1][0] = entry[blueIndex+1][greenIndex+1][redIndex  ].blue();
    c8[1][1][1] = entry[blueIndex+1][greenIndex+1][redIndex+1].blue();

    c4[0][0] = c8[0][0][0] * (1 - redDif) + c8[0][0][1] * redDif;
    c4[0][1] = c8[0][1][0] * (1 - redDif) + c8[0][1][1] * redDif;
    c4[1][0] = c8[1][0][0] * (1 - redDif) + c8[1][0][1] * redDif;
    c4[1][1] = c8[1][1][0] * (1 - redDif) + c8[1][1][1] * redDif;

    c2[0] = c4[0][0] * (1 - greenDif) + c4[0][1] * greenDif;
    c2[1] = c4[1][0] * (1 - greenDif) + c4[1][1] * greenDif;

    c1 = c2[0] * (1 - blueDif) + c2[1] * blueDif;
    // Set blue
    colorPoint += (uint8_t)c1;
    //qDebug() << "colorPoint" << colorPoint;
    return colorPoint;
}

QRgb Lut3D::getColor(QRgb rgb)
{
    QRgb color = this->getColor(rgb);
    return color;
}

QRgba64 Lut3D::getEntry(uint16_t red, uint16_t green, uint16_t blue)
{
    return entry[blue][green][red];
}

void Lut3D::setEntry(uint16_t red, uint16_t green, uint16_t blue, QRgba64 color)
{
    entry[blue][green][red].setRed(color.red());
    entry[blue][green][red].setGreen(color.green());
    entry[blue][green][red].setBlue(color.blue());
}

uint16_t Lut3D::getIndex(uint16_t value)
{
    uint8_t index = (uint8_t)(value >> 8);
    if(value < (INTERVAL * index))
    {
        index--;
    }
    return index;
}
