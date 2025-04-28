#ifndef ADC_H
#define ADC_H

#include "main.h"

#define ADC_SAMPLE_SIZE (100)
#define ADC_REF_VALUE   (1200)
#define R2              (82)  // 220 KΩ
#define R1              (10)  // 43 Ω
#define V_REF           3300 // 3.3V

void ADC_Initial();
void getAdcValue();

#endif // !ADC_H
