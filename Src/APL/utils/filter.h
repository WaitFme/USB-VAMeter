#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

#define MAX_FILTER_SIZE 32

typedef struct {
    uint16_t history[MAX_FILTER_SIZE];
    uint32_t sum;
    uint16_t windowSize;
    uint16_t head;
    uint16_t count;
} MovingAverageFilter;

typedef struct {
    uint16_t w_size;
    uint32_t head;
    float sum;
    float *cache;
    uint16_t count;
} SlidAveFilter;

uint32_t AverageFilter(uint16_t* value, uint32_t size);
uint8_t MovingAverageFilterInitialize(MovingAverageFilter* filter, uint16_t N);
uint16_t MovingAverageFilterUpdate(MovingAverageFilter* filter, uint16_t newValue);

void SlidAveFilterInit(SlidAveFilter* filter, uint16_t w_size);
float SlidingAverageFilter(SlidAveFilter* filter, float value);

#endif  // !FILTER_H
