#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <QByteArray>
#include <QImage>
#include "lut3d.h"


typedef QImage    Image8bit;
typedef uint8_t* Image12bit;

class Image
{
public:
    Image(uint16_t width, uint16_t height, Image12bit imgBuffer);
    Image(uint16_t width, uint16_t height, Image8bit image);
    Image8bit   getImage8bit();
    Image12bit  getImage12bit();
    void   process(Lut3D *lut3d);
private:
    uint16_t width;
    uint16_t height;
    Image8bit image8bit;
    Image12bit image12bit;
};

#endif // IMAGE_H
