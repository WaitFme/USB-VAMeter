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
        controller();
    }
}
