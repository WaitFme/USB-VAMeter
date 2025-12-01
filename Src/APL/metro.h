#ifndef METRO_H
#define METRO_H

#include "main.h"

typedef struct {
    uint32_t previousTick;
    uint32_t intervalTick;
} Metro;

uint8_t MetroCheck(Metro* metro);

#endif  // !METRO_H
