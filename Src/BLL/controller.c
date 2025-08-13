#include "multi_button.h"
#include "controller.h"
#include "model.h"
#include "view.h"
#include "key.h"
#include "lcd.h"

#define REFRESH_TICK_KEY     1
#define REFRESH_TICK_DATA    5
#define REFRESH_TICK_DISPLAY 25

ControlState_TypeDef ControlState;

struct Button button1;

void Callback_SINGLE_CLICK_Handler(void* btn) {
    changeScreen();
}

// void Callback_DOUBLE_Click_Handler(void* btn) { }

void Callback_LONG_PRESS_START_Handler(void* btn) {
    changeRotation();
}

void controllerInit() {
    button_init(&button1, readButton, 0, 0);
    button_attach(&button1, SINGLE_CLICK, Callback_SINGLE_CLICK_Handler);
    // button_attach(&button1, DOUBLE_CLICK, Callback_DOUBLE_Click_Handler);
    button_attach(&button1, LONG_PRESS_START, Callback_LONG_PRESS_START_Handler);
    button_start(&button1);

    ControlState.ViewRefreshLock = true;
    ControlState.ModelRefreshLock = true;
    ControlState.KeyScanLock = true;
}

void controller() {
    // if (ControlState.KeyScanLock == false) {
    //     button_ticks();
    //     ControlState.KeyScanLock = true;
    // }

    if (ControlState.ModelRefreshLock == false) {
        model();
        ControlState.ModelRefreshLock = true;
    }

    if (ControlState.ViewRefreshLock == false) {
        view();
        ControlState.ViewRefreshLock = true;
    }
}

// 5ms
void BTIM1_IRQHandler() {
    static uint32_t timecount = 0;

    if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_UPDATE)) {
        BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_UPDATE);

        timecount++;

        if (timecount % REFRESH_TICK_KEY == 0) {
            button_ticks();
            ControlState.KeyScanLock = false;
        }

        if (timecount % REFRESH_TICK_DATA == 0) {
            ControlState.ModelRefreshLock = false;
        }

        if (timecount % REFRESH_TICK_DISPLAY == 0) {
            ControlState.ViewRefreshLock = false;
        }

        if (timecount >= 1000) {
            timecount = 0;
        }
    }
}
