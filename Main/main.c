#include "main.h"
#include "lcd.h"
#include "spi.h"
#include "rcc.h"
#include "key.h"
#include "tim.h"
#include "adc.h"
#include "controller.h"

void initial() {
    SystemClock_Init();

    Systick_Init();

    ADC_Initial();

    // BTIM1_init();

    BTIM2_init();

    SPI1_Init();

    LCD_Init();

    Key_Init();

    Controller_Init();
}

int main() {
    initial();

    while (1) {
        Controller();
    }
}
