#ifndef ADC_H
#define ADC_H

#include "main.h"

#define ADC_REF_VALUE   1200

void ADC_Initial();

uint16_t getChannelOffset();
uint16_t getChannelCurrent();
uint16_t getChannelVoltage();
uint16_t getChannelNtc();

#endif  // !ADC_H
