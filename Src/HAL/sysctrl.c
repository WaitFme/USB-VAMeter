#include "sysctrl.h"

void SYSCTRL_Configuration() {
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV1);                // 48M
    SYSCTRL_SYSCLKSRC_Config(SYSCTRL_SYSCLKSRC_HSI);
    SYSCTRL_HCLKPRS_Config(SYSCTRL_HCLK_DIV1);
    SYSCTRL_PCLKPRS_Config(SYSCTRL_PCLK_DIV1);

    REGBITS_SET(CW_SYSCTRL->AHBEN, (0x5A5A0000 | bv1));     // flash
}
