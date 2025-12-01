#include "screen_manager.h"
#include "lcd.h"

static void debugScreenInit(void) {
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
}

static void debugScreenRenderStatic(void) {
    LCD_ShowString(0, 0, "ChargingTime:", WHITE, BLACK, 12, 1);
    LCD_ShowString(0, 12, "Power:", WHITE, BLACK, 12, 1);
    LCD_ShowString(36, 12, "0.000", WHITE, BLACK, 12, 1);
    LCD_ShowString(72, 12, "Wh", WHITE, BLACK, 12, 1);
    LCD_ShowString(0, 24, "RefreshLevel:", WHITE, BLACK, 12, 1);
}

static void debugScreenUpdateDynamic(const AppState* uistate) {
    LCD_ShowStringFormat(78, 0, WHITE, BLACK, 12, 1, "%02d:%02d:%02d", uistate->historyChargingtime / 3600, uistate->historyChargingtime % 3600 / 60, uistate->historyChargingtime % 60);
    LCD_ShowSmartFloat(36, 12, uistate->historyTotalEnergy, 5, WHITE, BLACK, 12, 1);
    LCD_ShowIntNum(78, 24, uistate->refreshLevel, 1, WHITE, BLACK, 12, 1);
}

static void debugScreenCleanup(void) {
}

const ScreenInterface debugScreen = {
    .init = debugScreenInit,
    .renderStatic = debugScreenRenderStatic,
    .updateDynamic = debugScreenUpdateDynamic,
    .cleanup = debugScreenCleanup,
    .name = "Debug"
};
