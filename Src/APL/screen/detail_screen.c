#include "screen_manager.h"
#include "lcd.h"

static void detailScreenInit(void) {
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
}

static void detailScreenRenderStatic(void) {
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
}

static void detailScreenUpdateDynamic(const AppState* uistate) {
    LCD_ShowSmartFloat(97, 2, uistate->voltage, 5, WHITE, ORANGE, 16, 1);
    LCD_ShowSmartFloat(97, 22, uistate->current, 5, WHITE, SGREEN, 16, 1);
    LCD_ShowSmartFloat(97, 42, uistate->power, 5, WHITE, GRAYBLUE, 16, 1);
    LCD_ShowChar(2, 34, uistate->flowDirChar, MY_GREEN, BLACK, 12, 1);
    LCD_ShowSmartFloat(5, 60, uistate->temperature, 4, RED, BLACK, 16, 1);
    LCD_ShowSmartFloat(97, 62, uistate->totalEnergy, 5, WHITE, BRRED, 16, 1);
    LCD_ShowStringFormat(5, 22, BLUE, BLACK, 12, 1, "%02d:%02d:%02d", uistate->chargingHour, uistate->chargingMinute, uistate->chargingSecond);
}

static void detailScreenCleanup(void) {
}

const ScreenInterface detailScreen = {
    .init = detailScreenInit,
    .renderStatic = detailScreenRenderStatic,
    .updateDynamic = detailScreenUpdateDynamic,
    .cleanup = detailScreenCleanup,
    .name = "Detail"
};
