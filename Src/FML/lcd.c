/**
 * @file lcd.c
 * @author anpe (waitfme@outlook.com)
 * @brief st7735s Lcd 显示屏驱动
 * @version 0.1
 * @date 2023-09-13
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "lcd.h"
#include "lcdfont.h"
#include "math.h"
#include "tools.h"
#include <string.h>
#include <stdarg.h>

const uint8_t charToIndex[128] = {
    [' '] = 0,   // 空格 -> 索引 0
    ['.'] = 1,   // '.' -> 索引 1
    ['0'] = 2,   // '0' -> 索引 2
    ['1'] = 3,   // '1' -> 索引 3
    ['2'] = 4,
    ['3'] = 5,
    ['4'] = 6,
    ['5'] = 7,
    ['6'] = 8,
    ['7'] = 9,
    ['8'] = 10,
    ['9'] = 11,
    ['A'] = 12,  // 'A' -> 索引 33
    ['V'] = 13,  // 'V' -> 索引 54
    ['W'] = 14,  // 'W' -> 索引 55
};

/**
 * @brief 初始化LCD
 *
 */
void LCD_Init() {
    ST7735S_Init();

    LCD_Clear();

    // Display on
    ST7735S_WriteCMD(0x29);
}

void LCD_Clear() {
    LCD_Fill(0, 0, LCD_W - 1, LCD_H - 1, BLACK);
}

void LCD_Fill_All(uint16_t color) {
    LCD_Fill(0, 0, LCD_W - 1, LCD_H - 1, color);
}

void LCD_Rotation(uint8_t rotation) {
    LCD_Fill(0, 0, LCD_W - 1, LCD_H - 1, BLACK);

    ST7735S_WriteCMD(0x36);
    if (rotation) {
        ST7735S_WriteData(0x78);
    } else {
        ST7735S_WriteData(0xA8);
    }
}

/**
 * @brief 填充指定区域颜色
 *
 * @param x_start
 * @param y_start
 * @param x_end
 * @param y_end
 * @param color
 */
void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color) {
    ST7735S_Address_Set(x_start, y_start, x_end, y_end);
    for (uint8_t i = y_start; i <= y_end; i++) {
        for (uint8_t j = x_start; j <= x_end; j++) {
            ST7735S_Address_Color_Set(color);
        }
    }
}

/**
 * @brief 绘制点
 *
 * @param x	x轴
 * @param y	y轴
 * @param color	颜色
 */
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color) {
    ST7735S_Address_Set(x, y, x, y);
    ST7735S_Address_Color_Set(color);
}

/**
 * @brief 绘制线
 *
 * @param x1	起始点x轴
 * @param y1	起始点y轴
 * @param x2	截止点x轴
 * @param y2	截止点y轴
 * @param color	填充颜色
 */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;

    // 计算坐标增量
    delta_x = x2 - x1;
    delta_y = y2 - y1;

    // 画线起点坐标
    uRow = x1;
    uCol = y1;

    if (delta_x > 0) {
        // 设置单步方向
        incx = 1;
    } else if (delta_x == 0) {
        // 垂直线
        incx = 0;
    } else {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0) {
        incy = 1;
    } else if (delta_y == 0) {
        // 水平线
        incy = 0;
    } else {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y) {
        // 选取基本增量坐标轴
        distance = delta_x;
    } else {
        distance = delta_y;
    }

    for (uint16_t t = 0; t < distance + 1; t++) {
        // 画点
        LCD_DrawPoint((uint16_t)uRow, (uint16_t)uCol, color);

        xerr += delta_x;
        yerr += delta_y;

        if (xerr > distance) {
            xerr -= distance;
            uRow += incx;
        }

        if (yerr > distance) {
            yerr -= distance;
            uCol += incy;
        }
    }
}

/**
 * @brief 绘制虚线
 *
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param dotLength
 * @param space
 * @param color
 */
void LCD_DrawDottedLine(int x0, int y0, int x1, int y1, int dotLength, int space, uint16_t color) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int d = sqrt(dx * dx + dy * dy);
    float stepX = dx / (float)d * (dotLength + space);
    float stepY = dy / (float)d * (dotLength + space);

    for (int i = 0; i < d; i += (dotLength + space)) {
        // 计算每个点的起始位置
        int startX = x0 + stepX * i / (dotLength + space);
        int startY = y0 + stepY * i / (dotLength + space);

        // 绘制虚线段
        LCD_DrawLine(startX, startY, startX + (dotLength * dx / d), startY + (dotLength * dy / d), color);
    }
}

/**
 * @brief 		绘制长方形
 *
 * @param x1	起始x轴
 * @param y1	起始y轴
 * @param x2	截止x轴
 * @param y2	截止y轴
 * @param color	填充颜色
 */
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    LCD_DrawLine(x1, y1, x2, y1, color);
    LCD_DrawLine(x1, y1, x1, y2, color);
    LCD_DrawLine(x1, y2, x2, y2, color);
    LCD_DrawLine(x2, y1, x2, y2, color);
}

/**
 * @brief 		绘制圆形
 *
 * @param x0	圆心x轴
 * @param y0	圆心y轴
 * @param r		圆半径
 * @param color	圆颜色
 */
void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color) {
    // int a = 0, b = r;
    uint16_t a = 0, b = r;

    while (a <= b) {
        LCD_DrawPoint(x0 - b, y0 - a, color);
        LCD_DrawPoint(x0 + b, y0 - a, color);
        LCD_DrawPoint(x0 - a, y0 + b, color);
        LCD_DrawPoint(x0 - a, y0 - b, color);
        LCD_DrawPoint(x0 + b, y0 + a, color);
        LCD_DrawPoint(x0 + a, y0 - b, color);
        LCD_DrawPoint(x0 + a, y0 + b, color);
        LCD_DrawPoint(x0 - b, y0 + a, color);

        a++;

        if ((a * a + b * b) > (r * r)) {
            b--;
        }
    }
}

void LCD_ShowImage(uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height, uint16_t *pic) {
    uint16_t x_end = x_start + width - 1;
    uint16_t y_end = y_start + height - 1;
    uint16_t *data = (uint16_t *)pic;
    ST7735S_Address_Set(x_start, y_start, x_end, y_end);
    for (uint8_t i = y_start; i <= y_end; i++) {
        for (uint8_t j = x_start; j <= x_end; j++) {
            LCD_DrawPoint(j, i, *data++);
        }
    }
}

void LCD_ShowChinese(uint16_t x, uint16_t y, uint16_t index, uint8_t size, uint16_t pointcolor, uint16_t backcolor, uint8_t mode) {
    uint8_t temp, t, t1;
    uint16_t y0 = y;
    uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size);
    for (t = 0; t < csize; t++) {
        if (size == 12) {
            temp = hz12[index][t];
        } else if (size == 16) {
            temp = hz16[index][t];
        } else if (size == 24) {
            temp = hz24[index][t];
        }

        for (t1 = 0; t1 < 8; t1++) {
            if (temp & 0x80) {
                LCD_DrawPoint(x, y, pointcolor);
            } else if (mode == 0) {
                LCD_DrawPoint(x, y, backcolor);
            }
            temp <<= 1;
            y++;
            if ((y - y0) == size) {
                y = y0;
                x++;
                break;
            }
        }
    }
}

/**
 * @brief 		显示单字符
 *
 * @param x		起始x轴
 * @param y		起始y轴
 * @param num	字符
 * @param fc	字符颜色
 * @param bc	背景颜色
 * @param sizex	字符宽度
 * @param sizey	字符大小 -> 12, 16, 24, 32
 * @param mode	显示模式 -> 1:显示背景颜色; 0:不显示背景颜色
 */
void LCD_ShowCharBasic(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizex, uint8_t sizey, uint8_t mode) {
    uint8_t temp;
    uint8_t m = 0;

    uint16_t x0 = x;
    uint16_t typefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
    uint16_t index = charToIndex[num];
    num = num - ' ';

    if (mode) {
        ST7735S_Address_Set(x, y, x + sizex - 1, y + sizey - 1);
    }

    for (uint16_t i = 0; i < typefaceNum; i++) {
        if (sizey == 12) {
            temp = ascii_1206[num][i];
        } else if (sizey == 16) {
            temp = ascii_1608[num][i];
        } else if (sizey == 24) {
            temp = ascii_2412_jetbra_mono[num][i];
        } else if (sizey == 28) {
            temp = ascii_2816_helvetica[index][i];
        } else if (sizey == 32) {
            temp = ascii_3216[num][i];
        } else {
            return;
        }

        for (uint16_t j = 0; j < 8; j++) {
            if (mode) {
                if (temp & (0x01 << j)) {
                    ST7735S_WriteData_16Bit(fc);
                } else {
                    ST7735S_WriteData_16Bit(bc);
                }

                m++;

                if (m % sizex == 0) {
                    m = 0;
                    break;
                }
            } else {
                if (temp & (0x01 << j)) {
                    LCD_DrawPoint(x, y, fc);
                }

                x++;

                if ((x - x0) == sizex) {
                    x = x0;
                    y++;
                    break;
                }
            }
        }
    }
}

/**
 * @brief 		显示单字符
 *
 * @param x		起始x轴
 * @param y		起始y轴
 * @param num	字符
 * @param fc	字符颜色
 * @param bc	背景颜色
 * @param sizey	字符大小 -> 12, 16, 24, 32
 * @param mode	显示模式 -> 1:显示背景颜色; 0:不显示背景颜色
 */
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode) {
    LCD_ShowCharBasic(x, y, num, fc, bc, sizey / 2, sizey, mode);
}

/**
 * @brief 		显示字符串
 *
 * @param x		起始x轴
 * @param y		起始y轴
 * @param data	字符串
 * @param fc	字符颜色
 * @param bc	背景颜色
 * @param sizey	字符大小 -> 12, 16, 24, 32
 * @param mode	显示模式 -> 0:显示背景颜色; 1:不显示背景颜色
 */
void LCD_ShowString(uint16_t x, uint16_t y, const char *data, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode) {
    while (*data != '\0') {
        LCD_ShowChar(x, y, *data, fc, bc, sizey, mode);
        x += sizey / 2;
        data++;
    }
}

/**
 * @brief       格式化显示字符串（类似printf）
 * @param x     起始x坐标
 * @param y     起始y坐标
 * @param fc    字体颜色
 * @param bc    背景颜色
 * @param sizey 字体大小
 * @param mode  显示模式 (0:有背景, 1:无背景)
 * @param fmt   格式化字符串
 * @param ...   可变参数
 */
void LCD_ShowStringFormat(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode, const char *fmt, ...) {
    char buffer[64];  // 根据需要调整缓冲区大小
    va_list args;

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    LCD_ShowString(x, y, buffer, fc, bc, sizey, mode);
}

/**
 * @brief		显示整型数字
 *
 * @param x		起始x轴
 * @param y		起始y轴
 * @param num	显示的整型
 * @param len	整型的长度
 * @param fc	字符颜色
 * @param bc	背景颜色
 * @param sizey	字符大小 -> 12, 16, 24, 32
 */
void LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode) {
    uint8_t t, temp;
    uint8_t enshow = 0;
    uint8_t sizex = sizey / 2;
    for (t = 0; t < len; t++) {
        temp = (num / mypow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                LCD_ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, mode);
                continue;
            } else {
                enshow = 1;
            }
        }
        LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, mode);
    }
}

/**
 * @brief		显示浮点数
 *
 * @param x		起始x轴
 * @param y		起始y轴
 * @param num	显示的浮点数
 * @param len	浮点数的显示长度
 * @param fontColor	字符颜色
 * @param backgroundColor	背景颜色
 * @param fontSize	字符大小 -> 12, 16, 24, 32
 */
void LCD_ShowFloatNum(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode) {
    uint8_t sizex = sizey / 2;
    uint16_t num1 = num * 100;

    for (uint8_t index = 0; index < len; index++) {
        uint8_t temp = (num1 / mypow(10, len - index - 1)) % 10;

        if (index == (len - 2)) {
            LCD_ShowChar(x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
            index++;
            len += 1;
        }

        LCD_ShowChar(x + index * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}

/**
 * @brief 智能动态小数点显示（支持double和指定长度）
 * @param x 起始x坐标
 * @param y 起始y坐标
 * @param num 要显示的数字
 * @param len 总显示长度（如5→"1.234"）
 * @param fColor 字体颜色
 * @param bColor 背景颜色
 * @param fontSize 字体大小
 */
void LCD_ShowSmartFloatBasic(uint16_t x, uint16_t y, double num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizex, uint8_t sizey, uint8_t mode) {
    char buffer[16] = {0};
    uint8_t decimal_places = 0;
    bool show_decimal = true;

    // 根据数值范围确定显示格式
    if (num < 10.0) {
        decimal_places = len - 2;  // 如length=5→3位小数
    } else if (num < 100.0) {
        decimal_places = len - 3;  // 如length=5→2位小数
    } else if (num < 1000.0) {
        decimal_places = len - 4;  // 如length=5→1位小数
    } else {
        show_decimal = false;  // 超过1000转为整数模式
    }

    // 特殊处理纯整数模式
    if (!show_decimal) {
        snprintf(buffer, sizeof(buffer), "%0*d", len, (int)num);
    } else {
        // 处理带小数的情况
        // 计算需要的格式字符串（如"%.3f"）
        char format_str[8];
        snprintf(format_str, sizeof(format_str), "%%.%df", decimal_places);

        // 先格式化为标准小数
        snprintf(buffer, sizeof(buffer), format_str, num);

        // 移除多余的0（如1.230→1.23）
        // for (int i = strlen(buffer) - 1; i >= 0; --i) {
        //     if (buffer[i] == '0' && buffer[i - 1] != '.') {
        //         buffer[i] = '\0';
        //     } else {
        //         break;
        //     }
        // }

        // 补前导0（如.5→0.5）
        if (buffer[0] == '.') {
            memmove(buffer + 1, buffer, strlen(buffer) + 1);
            buffer[0] = '0';
        }
    }

    // 显示字符（自动对齐）
    for (uint8_t i = 0, pos = 0; i < strlen(buffer) && pos < len; ++i) {
        if (buffer[i] == '.') {
            LCD_ShowCharBasic(x + pos * sizex, y, '.', fc, bc, sizex, sizey, mode);
            pos++;
            continue;
        }
        LCD_ShowCharBasic(x + pos * sizex, y, buffer[i], fc, bc, sizex, sizey, mode);
        LCD_ShowCharBasic(x + pos * sizex, y, buffer[i], fc, bc, sizex, sizey, mode);
        pos++;
    }
}

/**
 * @brief 智能动态小数点显示（支持double和指定长度）
 * @param x 起始x坐标
 * @param y 起始y坐标
 * @param num 要显示的数字
 * @param len 总显示长度（如5→"1.234"）
 * @param fColor 字体颜色
 * @param bColor 背景颜色
 * @param fontSize 字体大小
 */
void LCD_ShowSmartFloat(uint16_t x, uint16_t y, double num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode) {
    LCD_ShowSmartFloatBasic(x, y, num, len, fc, bc, sizey / 2, sizey, mode);
}

