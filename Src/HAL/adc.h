#ifndef ADC_H
#define ADC_H

#include "main.h"

#define ADC_SAMPLE_SIZE 100
#define ADC_REF_VALUE   1200
#define R2              82    // 220 KΩ
#define R1              10    // 43 Ω
#define V_REF           3300  // 3.3V

typedef struct {
    uint16_t voltageBuffer[ADC_SAMPLE_SIZE];
    uint16_t currentBuffer[ADC_SAMPLE_SIZE];
    uint16_t offsetBuffer[ADC_SAMPLE_SIZE];
    uint16_t ntcBuffer[ADC_SAMPLE_SIZE];
    uint8_t currentIndex;
} AdcData;

void ADC_Initial();
void initAdcData(AdcData* adcData);
void getAdcValue(AdcData* adcData);

#endif  // !ADC_H
