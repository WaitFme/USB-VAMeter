#ifndef KEY_H
#define KEY_H

#include "main.h"

#define KEY_PIN_0             GPIO_PIN_0

#define KEY_DEBOUNCE_TIME     10    // 消抖时间
#define KEY_LONG_PRESS_TIME   500  // 长按判定时间
#define KEY_QUICK_CLICK_TIME  100  // 连按时间间隔
#define KEY_DOUBLE_CLICK_TIME 200  // 双击判定时间
#define KEY_PRESSED_LEVEL     0    // 按键被按下时的电平

#define KEY0                  GPIO_ReadPin(CW_GPIOB, GPIO_PIN_0)

void Key_Init();
uint8_t readButton();

#endif  // !KEY_H
