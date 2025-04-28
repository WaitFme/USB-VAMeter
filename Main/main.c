#include "main.h"
#include "lcd.h"
#include "spi.h"
#include "sysctrl.h"
#include "key.h"
#include "tim.h"
#include "adc.h"
#include "model.h"
#include "view.h"
#include "controller.h"

#define REFRESH_TICK_DATA    2
#define REFRESH_TICK_DISPLAY 50

ModelState_typeDef dataRefresh = {
    .event = ME_NONE,
    .initLock = 1,
    .refreshLock = 0,
};

ViewState_typeDef display = {
    .event = NONE,
    .initLock = 0,
    .refreshLock = 0,
};

KeyConfig_typeDef kct = {
    .cStatus = KS_RELEASE,
    .lStatus = KS_RELEASE,
    .keyCount = 0,
};

MainState_TypeDef mainState = {
    .ViewRefreshLock = 1,
    .ModelRefreshLock = 1,
};

// uint8_t ct_lock = 0;
// uint8_t rt_lock = 0;

void initial() {
    SYSCTRL_Configuration();

    InitTick(48000000);

    ADC_Initial();

    BTIM1_init();

    BTIM2_init();

    SPI1_Init();

    LCD_Init();

    Key_Init();
}

int main() {
    initial();

    while (1) {
        controller(&display, &kct);

        // switch (kct.cStatus) {
        //     case KS_RELEASE:
        //         ct_lock = 0;
        //         rt_lock = 0;
        //         break;
        //     case KS_PRESS: {
        //         if (!ct_lock) {
        //             display.screen++;
        //             display.initLock = 0;
        //             if (display.screen >= 2) {
        //                 display.screen = 0;
        //             }
        //             ct_lock = 1;
        //         }
        //     } break;
        //     case KS_LONGPRESS: {
        //         if (!rt_lock) {
        //             display.event = ROTATION;
        //             rt_lock = 1;
        //         }
        //     } break;
        //     default:
        //         break;
        // }
        if (mainState.ModelRefreshLock == false) {
            model(&dataRefresh);
            mainState.ModelRefreshLock = true;
        }

        if (mainState.ViewRefreshLock == false) {
            view(&display);
            mainState.ViewRefreshLock = true;
        }

    }
}

void BTIM1_IRQHandler() {
    static uint32_t timecount = 0;

    if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_UPDATE)) {
        BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_UPDATE);

        timecount++;

        if (timecount % KEY_DEBOUNCE_TIME == 0) {
            keyStatusScan(&kct);
        }

        if (timecount % REFRESH_TICK_DATA == 0) {
            dataRefresh.refreshLock = 0;

            mainState.ModelRefreshLock = 0;
        }

        if (timecount % REFRESH_TICK_DISPLAY == 0) {
            display.refreshLock = 0;

            mainState.ViewRefreshLock = 0;
        }

        if (timecount >= 100) {
            timecount = 0;
        }
    }
}
