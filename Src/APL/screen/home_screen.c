#include "screen_manager.h"
#include "lcd.h"
#include "icon.h"

static void homeScreenInit(void) {
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
}

static void homeScreenRenderStatic(void) {
    // 绘制图标
    LCD_ShowImage(0, 0, 28, 80, (uint16_t*)head_icon);

    // 绘制标签
    LCD_ShowString(8, 2, "U", WHITE, BLACK, 24, 0);
    LCD_ShowString(8, 28, "I", WHITE, BLACK, 24, 0);
    LCD_ShowString(8, 54, "P", WHITE, BLACK, 24, 0);
    // 绘制占位符
    LCD_ShowSmartFloatBasic(35, 2, 0, 5, WHITE, BLACK, 16, 28, 0);
    LCD_ShowSmartFloatBasic(35, 28, 0, 5, WHITE, BLACK, 16, 28, 0);
    LCD_ShowSmartFloatBasic(35, 54, 0, 5, WHITE, BLACK, 16, 28, 0);

    // 绘制单位
    LCD_ShowCharBasic(121, 2, 'V', MY_YELLOW, BLACK, 16, 28, 0);
    LCD_ShowCharBasic(121, 28, 'A', MY_GREEN, BLACK, 16, 28, 0);
    LCD_ShowCharBasic(121, 54, 'W', MY_BLUE, BLACK, 16, 28, 0);
}

static void homeScreenUpdateDynamic(const AppState* uistate) {
    LCD_ShowSmartFloatBasic(35, 2, uistate->voltage, 5, WHITE, BLACK, 16, 28, 1);
    LCD_ShowSmartFloatBasic(35, 28, uistate->current, 5, WHITE, BLACK, 16, 28, 1);
    LCD_ShowSmartFloatBasic(35, 54, uistate->power, 5, WHITE, BLACK, 16, 28, 1);
    LCD_ShowChar(150, 34, uistate->flowDirChar, MY_GREEN, BLACK, 12, 1);
}

static void homeScreenCleanup(void) {
}

// 屏幕接口定义
const ScreenInterface homeScreen = {
    .init = homeScreenInit,
    .renderStatic = homeScreenRenderStatic,
    .updateDynamic = homeScreenUpdateDynamic,
    .cleanup = homeScreenCleanup,
    .name = "Home"
};
