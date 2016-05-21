#include "image.h"
#include <QtWidgets>
#include <QDebug>

Image::Image(uint16_t w, uint16_t h, uint8_t* imgBuffer)
{
    QRgba64 color;
    uint16_t x, y;
    uint16_t red, green, blue;
    uint32_t i;
    width = w;
    height = h;
    image12bit = (Image12bit)imgBuffer;
    image8bit  = (Image8bit)QImage(width, height,  QImage::Format_RGB888);

    // PLANARY!
    // Convert 12bit image to 8bit image
    for(int y = 0; y < height;y++)
    {
        for(int x = 0; x < width; x++)
        {
            i = 2*(y*width + x);
            red = (uint16_t)(((uint8_t)image12bit[i] + ((uint8_t)image12bit[i+1] << 8)) << 4);
            color.setRed(red);

            i += 2*height*width;
            green = (uint16_t)(((uint8_t)image12bit[i] + ((uint8_t)image12bit[i+1] << 8)) << 4);
            color.setGreen(green);

            i += 2*height*width;
            blue = (uint16_t)(((uint8_t)image12bit[i] + ((uint8_t)image12bit[i+1] << 8)) << 4);
            color.setBlue(blue);

            image8bit.setPixelColor(x, y, color);
        }
    }
}

Image::Image(uint16_t w, uint16_t h, uint8_t* imgBuffer, bool useless)
{
    QColor color;
    uint16_t x, y;
    uint8_t red, green, blue;
    uint32_t i;
    width = w;
    height = h;
    image8bit  = (Image8bit)QImage(width, height,  QImage::Format_RGB888);

    // PLANARY!
    // Convert 12bit image to 8bit image
    for(int y = 0; y < height;y++)
    {
        for(int x = 0; x < width; x++)
        {
            i = y*width + x;
            red = imgBuffer[i];
            color.setRed(red);

            i += height*width;
            green = imgBuffer[i];
            color.setGreen(green);

            i += height*width;
            blue = imgBuffer[i];
            color.setBlue(blue);

            image8bit.setPixelColor(x, y, color);
        }
    }
}

Image::Image(uint16_t w, uint16_t h, Image8bit image)
{
    width = w;
    height = h;
    image8bit = image;
}


Image8bit Image::getImage8bit()
{
    return image8bit;
}


Image12bit Image::getImage12bit()
{
    return image12bit;
}

void Image::process(Lut3D *lut3d)
{
    uint16_t x, y;
    uint8_t red, green, blue;
    QRgb rgb;
    QColor color;
    for(y = 0; y < height; y++)
    {
        for(x = 0; x < width; x++)
        {
            red =   image8bit.pixelColor(x,y).red();
            green = image8bit.pixelColor(x,y).green();
            blue =  image8bit.pixelColor(x,y).blue();
            rgb = lut3d->getColor(red, green, blue);
            image8bit.setPixelColor(x,y,color.fromRgb(rgb));
        }
    }
}
