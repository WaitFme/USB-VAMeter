#include "tim.h"

void BTIM1_init() {
    __SYSCTRL_BTIM123_CLK_ENABLE();

    __disable_irq();
    NVIC_EnableIRQ(BTIM1_IRQn);
    __enable_irq();

    BTIM_TimeBaseInitTypeDef tim1 = {
        .BTIM_Mode = BTIM_MODE_TIMER,
        .BTIM_Period = 500 - 1, // 100
        .BTIM_Prescaler = 480 - 1,
    };
    BTIM_TimeBaseInit(CW_BTIM1, &tim1);

    BTIM_ITConfig(CW_BTIM1, BTIM_IT_UPDATE, ENABLE);

    BTIM_Cmd(CW_BTIM1, ENABLE);
}

// 定时器初始化函数
void BTIM2_init() {
    __SYSCTRL_BTIM123_CLK_ENABLE();

    __disable_irq();
    NVIC_EnableIRQ(BTIM2_IRQn);
    __enable_irq();

    BTIM_TimeBaseInitTypeDef tim2 = {
        .BTIM_Mode = BTIM_MODE_TIMER,
        .BTIM_Period = 50000 - 1,
        .BTIM_Prescaler = 960 - 1,
    };

    BTIM_TimeBaseInit(CW_BTIM2, &tim2);
    BTIM_Cmd(CW_BTIM2, ENABLE);
}

void timer2Enable(FunctionalState NewState) {
    // 使能BTIMx的溢出中断
    BTIM_ITConfig(CW_BTIM2, BTIM_IT_UPDATE, NewState);
}
