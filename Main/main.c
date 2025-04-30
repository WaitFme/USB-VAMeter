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
#include "multi_button.h"

#define REFRESH_TICK_KEY     1
#define REFRESH_TICK_DATA    20
#define REFRESH_TICK_DISPLAY 100

MainState_TypeDef mainState = {
    .ViewRefreshLock = true,
    .ModelRefreshLock = true,
    .KeyScanLock = true,
};

void initial() {
    SYSCTRL_Configuration();

    InitTick(48000000);

    ADC_Initial();

    BTIM1_init();

    BTIM2_init();

    SPI1_Init();

    LCD_Init();

    Key_Init();

    controllerInit();
}

int main() {
    initial();

    while (1) {
        // controller();

        if (mainState.KeyScanLock == false) {
            // keyStatusScan();
            mainState.KeyScanLock = true;
        }

        if (mainState.ModelRefreshLock == false) {
            model();
            mainState.ModelRefreshLock = true;
        }

        if (mainState.ViewRefreshLock == false) {
            view();
            mainState.ViewRefreshLock = true;
        }
    }
}

void BTIM1_IRQHandler() {
    static uint32_t timecount = 0;

    if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_UPDATE)) {
        BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_UPDATE);

        timecount++;
            button_ticks();
        if (timecount % REFRESH_TICK_KEY == 0) {
            // button_ticks();
            mainState.KeyScanLock = false;
        }

        if (timecount % REFRESH_TICK_DATA == 0) {
            mainState.ModelRefreshLock = false;
        }

        if (timecount % REFRESH_TICK_DISPLAY == 0) {
            mainState.ViewRefreshLock = false;
        }

        if (timecount >= 1000) {
            timecount = 0;
        }
    }
}
