#include "spi.h"
#include "cw32l010_spi.h"

/**
 * @brief SPI1初始化函数
 *
 */
void SPI1_Init() {
    REGBITS_SET(CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk);
    SYSCTRL_APBPeriphClk_Enable1(SYSCTRL_APB1_PERIPH_SPI, ENABLE);

    GPIO_InitTypeDef gpio = {
        .IT   = GPIO_IT_NONE,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pins = SPI1_PIN_SCLK | SPI1_PIN_MOSI,
    };

    GPIO_Init(SPI1_Port, &gpio);

    PA00_AFx_SPI1SCK();
    PA04_AFx_SPI1MOSI();
    PA01_AFx_SPI1NCS();

    SPI_InitTypeDef spi = {
        .SPI_Direction         = SPI_Direction_1Line_TxOnly,
        .SPI_Mode              = SPI_Mode_Master,
        .SPI_DataSize          = SPI_DataSize_16b,
        .SPI_CPOL              = SPI_CPOL_High,
        .SPI_CPHA              = SPI_CPHA_2Edge,
        .SPI_NSS               = SPI_NSS_Soft,
        .SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2,
        .SPI_FirstBit          = SPI_FirstBit_MSB,
        .SPI_Speed             = SPI_Speed_High,
    };
    SPI_Init(CW_SPI, &spi);

    SPI_Cmd(CW_SPI, ENABLE);
}

/**
 * @brief SPI1发送函数
 *
 * @param transmitData 要发送的8bit数据
 */
void SPI1_Transmit(uint16_t value) {
    while (SPI_GetFlagStatus(CW_SPI, SPI_FLAG_TXE) == RESET);
    SPI_SendData(CW_SPI, value);
    while (SPI_GetFlagStatus(CW_SPI, SPI_FLAG_BUSY) == SET);
}
