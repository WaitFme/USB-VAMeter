#ifndef ST7735S_H
#define ST7735S_H

#include "main.h"

#define ST7735S_Port			CW_GPIOA
#define ST7735S_Pin_RES			GPIO_PIN_2
#define ST7735S_Pin_DC			GPIO_PIN_3
#define ST7735S_Pin_CS			GPIO_PIN_1

#define ST7735S_RES_WB(x)       GPIO_WritePin(ST7735S_Port, ST7735S_Pin_RES, (GPIO_PinState)(x))
#define ST7735S_DC_WB(x)        GPIO_WritePin(ST7735S_Port, ST7735S_Pin_DC, (GPIO_PinState)(x))

void ST7735S_Init();
void ST7735S_WriteCMD(uint8_t cmd);
void ST7735S_WriteData(uint8_t data);
void ST7735S_WriteData_16Bit(uint16_t data);
void ST7735S_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void ST7735S_Address_Color_Set(uint16_t color);

#endif //!ST7735S_H
