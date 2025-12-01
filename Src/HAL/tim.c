#include "tim.h"

void BTIM1_init() {
    __SYSCTRL_BTIM123_CLK_ENABLE();

    __disable_irq();
    NVIC_EnableIRQ(BTIM1_IRQn);
    NVIC_SetPriority(BTIM1_IRQn, 2);
    __enable_irq();

    BTIM_TimeBaseInitTypeDef tim1 = {
        .BTIM_Mode = BTIM_MODE_TIMER,
        .BTIM_Period = 500 - 1,  // 100
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
    NVIC_SetPriority(BTIM2_IRQn, 1);
    __enable_irq();

    BTIM_TimeBaseInitTypeDef tim2 = {
        .BTIM_Mode = BTIM_MODE_TIMER,
        .BTIM_Period = 4962 - 1,  // 50000-49580
        .BTIM_Prescaler = 960 - 1,
    };

    BTIM_TimeBaseInit(CW_BTIM2, &tim2);
    BTIM2_Cmd(ENABLE);
}

static volatile uint8_t btim2_state = 0;

/**
 * @brief  启用/禁用 BTIM2
 * @param  NewState: ENABLE 或 DISABLE
 * @note   ENABLE 时自动清零计数器
 */
void BTIM2_Cmd(FunctionalState NewState) {
    if ((NewState == ENABLE && btim2_state == 1) ||
        (NewState == DISABLE && btim2_state == 0)) {
        return;
    }

    if (NewState == ENABLE) {
        BTIM_SetCounter(CW_BTIM2, 0);
        BTIM_ClearITPendingBit(CW_BTIM2, BTIM_IT_UPDATE);
        BTIM_ITConfig(CW_BTIM2, BTIM_IT_UPDATE, ENABLE);
        BTIM_Cmd(CW_BTIM2, ENABLE);
        btim2_state = 1;
    } else {
        BTIM_Cmd(CW_BTIM2, DISABLE);
        BTIM_ITConfig(CW_BTIM2, BTIM_IT_UPDATE, DISABLE);
        BTIM_ClearITPendingBit(CW_BTIM2, BTIM_IT_UPDATE);
        btim2_state = 0;
    }
}

/**
 * @BTIM1_IRQHandler 5ms
 */
// void BTIM1_IRQHandler() {
//     if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_UPDATE)) {
//         BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_UPDATE);

//         button_ticks();
//     }
// }
