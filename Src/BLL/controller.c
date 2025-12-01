#include "controller.h"
#include "model.h"
#include "view.h"
#include "key.h"
#include "lcd.h"
#include "metro.h"
#include "multi_button.h"
#include "screen_manager.h"
#include "image_xzm.h"

static AppState vstate;
static struct Button button1;

static Metro key_metro = {.intervalTick = 5};
static Metro model_metro = {.intervalTick = 25};
static Metro view_metro = {.intervalTick = 200};

static screen_id_t screenIndex = SCREEN_HOME;
static uint8_t rotation = 0;
static uint8_t view_refresh_mode = 0;

void Callback_SINGLE_CLICK_Handler(void* btn) {
    if (view_refresh_mode < 3) {
        view_metro.intervalTick = 200 - (view_refresh_mode * 50);
        vstate.refreshLevel = view_refresh_mode;
        view_refresh_mode++;
    } else {
        view_refresh_mode = 0;
    }
}

void Callback_DOUBLE_Click_Handler(void* btn) {
    screenIndex++;
    if (screenIndex >= SCREEN_COUNT) {
        screenIndex = SCREEN_HOME;
    }
    screenManagerSwitchTo(screenIndex);
}

void Callback_LONG_PRESS_START_Handler(void* btn) {
    rotation = !rotation;
    resetStateMachine();
    LCD_Rotation(rotation);
}

void Controller_Init() {
    button_init(&button1, readButton, 0, 0);

    button_attach(&button1, SINGLE_CLICK, Callback_SINGLE_CLICK_Handler);
    button_attach(&button1, DOUBLE_CLICK, Callback_DOUBLE_Click_Handler);
    button_attach(&button1, LONG_PRESS_START, Callback_LONG_PRESS_START_Handler);

    button_start(&button1);

    Model_Init();

    View_Init();

    vstate.historyChargingtime = 0;
    vstate.historyTotalEnergy = 0;

    LCD_ShowImage(50, 10, 60, 60, (uint16_t*)gImage_xzm);

    delay(500);
}

void Controller() {
    if (MetroCheck(&key_metro) == 1) {
        button_ticks();
    }

    if (MetroCheck(&model_metro) == 1) {
        Model(&vstate);
    }

    if (MetroCheck(&view_metro) == 1) {
        View((const AppState*)&vstate);
    }
}

uint8_t getRotation() {
    return rotation;
}
