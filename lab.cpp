#include "lab.h"
#include "color.h"
#include "QDebug"
void processLab(int Lvalue, int Avalue, int Bvalue, Lut3D *inputLut, Lut3D *outputLut)
{
    for (uint8_t b = 0; b < MAX_INDEX; b++)
    {
        for (uint8_t g = 0; g < MAX_INDEX; g++)
        {
            for (uint8_t r = 0; r < MAX_INDEX; r++)
            {
                QRgba64 rgb = inputLut->getEntry(r, g, b);
                lab_t lab = rgb2lab(rgb);
                lab.L += Lvalue;
                if(lab.L > 100)
                {
                    lab.L = 100;
                }
                lab.a += Avalue;
                lab.b += Bvalue;
                rgb = lab2rgb(lab);
                outputLut->setEntry(r,g,b,rgb);
            }
        }
    }
}
