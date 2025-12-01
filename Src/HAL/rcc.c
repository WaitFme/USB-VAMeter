#include "rcc.h"

void SystemClock_Init() {
    SystemInit();

    // 48M
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV1);
    SYSCTRL_SYSCLKSRC_Config(SYSCTRL_SYSCLKSRC_HSI);
    SYSCTRL_HCLKPRS_Config(SYSCTRL_HCLK_DIV1);
    SYSCTRL_PCLKPRS_Config(SYSCTRL_PCLK_DIV1);

    // flash
    REGBITS_SET(CW_SYSCTRL->AHBEN, (0x5A5A0000 | bv1));
}
