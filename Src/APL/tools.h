#ifndef TOOLS_H
#define TOOLS_H

#include "main.h"

#define PI 3.1415926

uint32_t mypow(uint8_t m, uint8_t n);
float angleToRad(float angle);
void multiplyMatrixVector(float* m, float* v, float* out);
void multiply(float* A, float* B, int m, int p, int n, float* C);
uint32_t meanValueFilter(uint16_t* value, uint32_t size);

#endif // !__TOOLS_H
