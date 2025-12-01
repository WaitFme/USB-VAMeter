#ifndef POWER_UTIL_H
#define POWER_UTIL_H

#include "stdint.h"

double voltCalculate(uint16_t value);
double currentCalculate(uint16_t current, uint16_t offset);

#endif // !POWER_UTIL_H
