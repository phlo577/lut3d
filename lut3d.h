#ifndef LUT3D_H
#define LUT3D_H
#include <stdint.h>
#include <QString>
#include <QColor>

#define MAX_INDEX 16
#define INTERVAL  273

class Lut3D
{
public:

    // Generate 3D LUT
    Lut3D(uint16_t interval, uint16_t nodes);

    // Get string representation of the 3D LUT
    QString  getString(void);

    // Get processed color
    QRgba64  getColor64(uint16_t red, uint16_t green, uint16_t blue);
    QRgba64  getColor64(QRgba64 rgb);

    QRgb  getColor(uint8_t red, uint8_t green, uint8_t blue);
    QRgb  getColor(QRgb rgb);

    // Get 3D LUT entry[blue][green[red]
    QRgba64  getEntry(uint16_t red, uint16_t green, uint16_t blue);

    // Set 3D LUT entry[blue][green[red] value with color
    void setEntry(uint16_t red, uint16_t green, uint16_t blue, QRgba64 color);
private:
    QRgba64 entry[16][16][16];
    QRgba64 interval;
    QRgba64 nodes;

    uint16_t getIndex(uint16_t x);
};

#endif // LUT3D_H
