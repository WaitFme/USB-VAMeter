#ifndef SPI_H
#define SPI_H

#include "main.h"

#define SPI1_Port		CW_GPIOA
#define SPI1_PIN_SCLK	GPIO_PIN_0
#define SPI1_PIN_MOSI	GPIO_PIN_4

void SPI1_Init();
void SPI1_Transmit(uint16_t value);

#endif
