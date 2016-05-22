#ifndef LAB_H
#define LAB_H
#include "stdint.h"
#include "lut3d.h"

void processLab(int Lvalue, int Avalue, int Bvalue, Lut3D *inputLut, Lut3D *outputLut);

#endif // LAB_H
