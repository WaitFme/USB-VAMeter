#ifndef __LCD_H__
#define __LCD_H__

#include "main.h"
#include "st7735s.h"

#define LCD_H      80
#define LCD_W      160

// RGB565
#define WHITE      0xFFFF
#define BLACK      0x0000
#define RED        0xF800
#define GREEN      0x0400
#define BLUE       0x001F
#define DARKRED    0x8800
#define LIGHTGREEN 0X9772
#define DARKGREEN  0X0320
#define DARKBLUE   0X0011
#define LIGHTBLUE  0XAEBC
#define FUCHSIA    0XF81F
#define YELLOW     0XFFE0
#define AQUA       0X07FF
#define LIME       0x07E0
#define BROWN      0XA145
#define PINK       0XFD09
#define GRAY       0X8410
#define LIGHTGRAY  0XD69A
#define DARKGRAY   0XAD55
#define ORANGE     0xFDE0
#define SGREEN     0x2644
#define GRAYBLUE   0X5458
#define BRRED      0XFC07
#define MY_YELLOW  0XFE00
#define MY_GREEN   0XA6A7
#define MY_BLUE    0X2C1C

void LCD_Init();
void LCD_Clear();
void lcd_rotation(uint8_t rotation);
void LCD_Fill_All(uint16_t color);
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawDottedLine(int x0, int y0, int x1, int y1, int dotLength, int space, uint16_t color);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
void LCD_ShowImage(uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height, uint16_t *pic);
void LCD_ShowChinese(uint16_t x, uint16_t y, uint16_t index, uint8_t size, uint16_t pointcolor, uint16_t backcolor, uint8_t mode);
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowCharCustom(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizex, uint8_t sizey, uint8_t mode);
void LCD_ShowString(uint16_t x, uint16_t y, const char *data, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowStringFormat(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode, const char *fmt, ...);
void LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowFloatNum(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowSmartFloat(uint16_t x, uint16_t y, double num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode);
void LCD_ShowSmartFloatEx(uint16_t x, uint16_t y, double num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizex, uint8_t sizey, uint8_t mode);

#endif
