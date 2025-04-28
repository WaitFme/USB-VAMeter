#include "key.h"

void Key_Init() {
    __SYSCTRL_GPIOB_CLK_ENABLE();
    GPIO_ConfigFilter(CW_GPIOB, GPIO_PIN_0, GPIO_FLTCLK_LSI);

    GPIO_InitTypeDef gpio = {
        .IT = GPIO_IT_FALLING,
        .Mode = GPIO_MODE_INPUT_PULLUP,
        .Pins = KEY_PIN_0,
    };
    GPIO_Init(CW_GPIOB, &gpio);
}

/* void GPIOB_IRQHandler() {
    if (CW_GPIOB->ISR & GPIO_PIN_0) {
        CW_GPIOB->ICR &= ~GPIO_PIN_0;
    }
} */
