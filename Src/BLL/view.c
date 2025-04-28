#include "view.h"
#include "lcd.h"
#include "icon.h"
#include "model.h"

viewState uistate;

void displayInit(uint8_t index) {
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);

    if (index == 0) {
        LCD_ShowImage(0, 0, 28, 80, (uint16_t *)head_icon);

        LCD_ShowString(8, 2, "U", WHITE, BLACK, 24, 0);
        LCD_ShowString(8, 28, "I", WHITE, BLACK, 24, 0);
        LCD_ShowString(8, 54, "P", WHITE, BLACK, 24, 0);

        LCD_ShowString(35, 2, "0.000", WHITE, BLACK, 32, 0);
        LCD_ShowString(35, 28, "0.000", WHITE, BLACK, 32, 0);
        LCD_ShowString(35, 54, "0.000", WHITE, BLACK, 32, 0);

        LCD_ShowCharCustom(121, 2, 'V', MY_YELLOW, BLACK, 16, 28, 0);
        LCD_ShowCharCustom(121, 28, 'A', MY_GREEN, BLACK, 16, 28, 0);
        LCD_ShowCharCustom(121, 54, 'W', MY_BLUE, BLACK, 16, 28, 0);
    } else if (index == 1) {
        LCD_ShowChinese(5, 5, 0, 12, BLUE, BLACK, 0);
        LCD_ShowChinese(17, 5, 1, 12, BLUE, BLACK, 0);
        LCD_ShowChinese(29, 5, 2, 12, BLUE, BLACK, 0);
        LCD_ShowChinese(41, 5, 3, 12, BLUE, BLACK, 0);

        LCD_ShowChinese(17, 44, 4, 12, RED, BLACK, 0);
        LCD_ShowChinese(29, 44, 5, 12, RED, BLACK, 0);

        LCD_Fill(60, 0, 160, 27, ORANGE);
        LCD_Fill(60, 20, 160, 54, SGREEN);
        LCD_Fill(60, 40, 160, 80, GRAYBLUE);
        LCD_Fill(60, 60, 160, 80, BRRED);
        LCD_ShowString(97, 2, "0.000", WHITE, ORANGE, 16, 0);
        LCD_ShowString(97, 22, "0.000", WHITE, SGREEN, 16, 0);
        LCD_ShowString(97, 42, "0.000", WHITE, GRAYBLUE, 16, 0);
        LCD_ShowString(97, 62, "0.000", WHITE, BRRED, 16, 0);
        LCD_ShowString(142, 2, "V", WHITE, ORANGE, 16, 0);
        LCD_ShowString(142, 22, "A", WHITE, SGREEN, 16, 0);
        LCD_ShowString(142, 42, "W", WHITE, GRAYBLUE, 16, 0);
        LCD_ShowString(142, 62, "Wh", WHITE, BRRED, 16, 0);

        LCD_ShowChinese(5 + 58, 2, 0, 16, WHITE, ORANGE, 0);
        LCD_ShowChinese(20 + 58, 2, 2, 16, WHITE, ORANGE, 0);
        LCD_ShowChinese(5 + 58, 22, 0, 16, WHITE, SGREEN, 0);
        LCD_ShowChinese(20 + 58, 22, 1, 16, WHITE, SGREEN, 0);
        LCD_ShowChinese(5 + 58, 42, 3, 16, WHITE, GRAYBLUE, 0);
        LCD_ShowChinese(20 + 58, 42, 4, 16, WHITE, GRAYBLUE, 0);
        LCD_ShowChinese(5 + 58, 62, 0, 16, WHITE, BRRED, 0);
        LCD_ShowChinese(20 + 58, 62, 5, 16, WHITE, BRRED, 0);

        LCD_ShowString(5, 60, "0.00", RED, BLACK, 16, 0);
        LCD_ShowString(41, 58, "o", RED, BLACK, 12, 0);
        LCD_ShowString(48, 60, "C", RED, BLACK, 16, 0);
    } else {
        // LCD_Fill(27, 2, 133, 78, MY_YELLOW);

        // LCD_Fill(56, 2, 80, 78, MY_GREEN);
        // LCD_Fill(80, 2, 133, 52, MY_GREEN);

        // LCD_Fill(85, 2, 109, 78, MY_BLUE);
        // LCD_Fill(85, 2, 133, 27, MY_BLUE);
    }
}

void screens(uint8_t index) {
    uint8_t x = uistate.rotation ^ uistate.currDirection;
    if (index == 0) {
        LCD_ShowSmartFloatEx(35, 2, uistate.voltage, 5, WHITE, BLACK, 16, 28, 1);
        LCD_ShowSmartFloatEx(35, 28, uistate.current, 5, WHITE, BLACK, 16, 28, 1);
        LCD_ShowSmartFloatEx(35, 54, uistate.power, 5, WHITE, BLACK, 16, 28, 1);
        if (uistate.currDirection != -1) {
            LCD_ShowString(150, 34, x ? ">" : "<", MY_GREEN, BLACK, 12, 0);
        }
    } else if (index == 1) {
        LCD_ShowSmartFloat(97, 2, uistate.voltage, 5, WHITE, ORANGE, 16, 1);
        LCD_ShowSmartFloat(97, 22, uistate.current, 5, WHITE, SGREEN, 16, 1);
        LCD_ShowSmartFloat(97, 42, uistate.power, 5, WHITE, GRAYBLUE, 16, 1);
        if (uistate.currDirection != -1) {
            LCD_ShowString(2, 34, x ? ">" : "<", MY_GREEN, BLACK, 12, 0);
        }
        LCD_ShowSmartFloat(5, 60, uistate.temperature, 5, RED, BLACK, 16, 1);
        LCD_ShowSmartFloat(97, 62, uistate.electricity, 4, WHITE, BRRED, 16, 1);
        LCD_ShowStringFormat(5, 22, BLUE, BLACK, 12, 1, "%02d:%02d:%02d", uistate.chargingtime / 3600, uistate.chargingtime % 3600 / 60, uistate.chargingtime % 60);
    }
}

void view() {
    uistate = getState();

    if (uistate.rotationLock == false) {
        lcd_rotation(uistate.rotation);
        setRotationLock(true);
    }

    if (uistate.initLock == false) {
        displayInit(uistate.screenIndex);
        setInitLock(true);
    }

    screens(uistate.screenIndex);
}
