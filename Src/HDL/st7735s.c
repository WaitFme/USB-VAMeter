/**
 * @file ST7735S.c
 * @author anpe (waitfme@outlook.com)
 * @brief st7735s 驱动
 * @version 0.1
 * @date 2023-09-13
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "st7735s.h"
#include "spi.h"

void GPIO_Initial();
void ST7735S_RESET();
void ST7735S_PARAMETER();

/**
 * @brief 初始化ST7735S
 *
 */
void ST7735S_Init() {
    // GOIO初始化
    GPIO_Initial();

    // ST7735S重置
    ST7735S_RESET();

    // ST7735S配置参数
    ST7735S_PARAMETER();
}

/**
 * @brief GPIO初始化
 *
 */
void GPIO_Initial() {
    REGBITS_SET(CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk);

    GPIO_InitTypeDef gpio = {
        gpio.IT = GPIO_IT_NONE,
        gpio.Mode = GPIO_MODE_OUTPUT_PP,
        gpio.Pins = ST7735S_Pin_RES | ST7735S_Pin_DC | ST7735S_Pin_CS,
    };
    GPIO_Init(ST7735S_Port, &gpio);
}

/**
 * @brief 重置st7735s
 *
 */
void ST7735S_RESET() {
    ST7735S_RES_WB(0);
    SysTickDelay(1);
    ST7735S_RES_WB(1);
    SysTickDelay(120);
}

/**
 * @brief 配置ST7735s屏幕参数
 *
 */
void ST7735S_PARAMETER() {
    // SLEEP EXIT
    ST7735S_WriteCMD(0x11);

    // ST7735S Frame Rate
    ST7735S_WriteCMD(0xB1);
    ST7735S_WriteData(0x05);
    ST7735S_WriteData(0x3C);
    ST7735S_WriteData(0x3C);

    ST7735S_WriteCMD(0xB2);
    ST7735S_WriteData(0x05);
    ST7735S_WriteData(0x3C);
    ST7735S_WriteData(0x3C);

    ST7735S_WriteCMD(0xB3);
    ST7735S_WriteData(0x05);
    ST7735S_WriteData(0x3C);
    ST7735S_WriteData(0x3C);
    ST7735S_WriteData(0x05);
    ST7735S_WriteData(0x3C);
    ST7735S_WriteData(0x3C);

    ST7735S_WriteCMD(0xB4);
    ST7735S_WriteData(0x03);

    // ST7735S Power Sequence
    ST7735S_WriteCMD(0xC0);
    ST7735S_WriteData(0x28);
    ST7735S_WriteData(0x08);
    ST7735S_WriteData(0x04);

    ST7735S_WriteCMD(0xC1);
    ST7735S_WriteData(0XC0);
    ST7735S_WriteCMD(0xC2);
    ST7735S_WriteData(0x0D);
    ST7735S_WriteData(0x00);

    ST7735S_WriteCMD(0xC3);
    ST7735S_WriteData(0x8D);
    ST7735S_WriteData(0x2A);

    ST7735S_WriteCMD(0xC4);
    ST7735S_WriteData(0x8D);
    ST7735S_WriteData(0xEE);

    ST7735S_WriteCMD(0xC5);
    ST7735S_WriteData(0x1A);

    // MX, MY, RGB mode
    ST7735S_WriteCMD(0x36);
    ST7735S_WriteData(0xA8);

    // ST7735S Gamma Sequence
    ST7735S_WriteCMD(0xE0);
    ST7735S_WriteData(0x04);
    ST7735S_WriteData(0x22);
    ST7735S_WriteData(0x07);
    ST7735S_WriteData(0x0A);
    ST7735S_WriteData(0x2E);
    ST7735S_WriteData(0x30);
    ST7735S_WriteData(0x25);
    ST7735S_WriteData(0x2A);
    ST7735S_WriteData(0x28);
    ST7735S_WriteData(0x26);
    ST7735S_WriteData(0x2E);
    ST7735S_WriteData(0x3A);
    ST7735S_WriteData(0x00);
    ST7735S_WriteData(0x01);
    ST7735S_WriteData(0x03);
    ST7735S_WriteData(0x13);

    ST7735S_WriteCMD(0xE1);
    ST7735S_WriteData(0x04);
    ST7735S_WriteData(0x16);
    ST7735S_WriteData(0x06);
    ST7735S_WriteData(0x0D);
    ST7735S_WriteData(0x2D);
    ST7735S_WriteData(0x26);
    ST7735S_WriteData(0x23);
    ST7735S_WriteData(0x27);
    ST7735S_WriteData(0x27);
    ST7735S_WriteData(0x25);
    ST7735S_WriteData(0x2D);
    ST7735S_WriteData(0x3B);
    ST7735S_WriteData(0x00);
    ST7735S_WriteData(0x01);
    ST7735S_WriteData(0x04);
    ST7735S_WriteData(0x13);

    ST7735S_WriteCMD(0x21);

    // End ST7735S Gamma Sequence
    ST7735S_WriteCMD(0x3A);   // 65k mode
    ST7735S_WriteData(0x05);  // RGB565 16bit
}

/**
 * @brief 往st7735寄存器写命令
 *
 * @param cmd 要写入的8bit命令
 */
void ST7735S_WriteCMD(uint8_t cmd) {
    ST7735S_DC_WB(0);
    SPI_NSSInternalSoftwareConfig(CW_SPI, SPI_NSSInternalSoft_Reset);
    SPI1_Transmit(cmd << 8 | cmd);
    SPI_NSSInternalSoftwareConfig(CW_SPI, SPI_NSSInternalSoft_Set);
}

/**
 * @brief 往st7735寄存器写数据
 *
 * @param data	要写入的8bit数据
 */
void ST7735S_WriteData(uint8_t data) {
    ST7735S_DC_WB(1);
    SPI_NSSInternalSoftwareConfig(CW_SPI, SPI_NSSInternalSoft_Reset);
    SPI1_Transmit(data << 8 | data);
    SPI_NSSInternalSoftwareConfig(CW_SPI, SPI_NSSInternalSoft_Set);
}

/**
 * @brief 往st7735寄存器写数据
 *
 * @param data	要写入的16bit数据
 */
void ST7735S_WriteData_16Bit(uint16_t data) {
    ST7735S_DC_WB(1);
    SPI_NSSInternalSoftwareConfig(CW_SPI, SPI_NSSInternalSoft_Reset);
    SPI1_Transmit(data);
    SPI_NSSInternalSoftwareConfig(CW_SPI, SPI_NSSInternalSoft_Set);
}

/**
 * @brief 往st7735s设定显存范围
 *
 * @param x_start	起始x轴
 * @param y_start	起始y轴
 * @param x_end		结束x轴
 * @param y_end		结束y轴
 */
void ST7735S_Address_Set(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end) {
    ST7735S_WriteCMD(0x2a);
    ST7735S_WriteData_16Bit(x_start + 1);
    ST7735S_WriteData_16Bit(x_end + 1);
    ST7735S_WriteCMD(0x2b);
    ST7735S_WriteData_16Bit(y_start + 26);
    ST7735S_WriteData_16Bit(y_end + 26);
    ST7735S_WriteCMD(0x2c);
}

/**
 * @brief st7735s颜色显存设定
 *
 * @param color 颜色
 */
void ST7735S_Address_Color_Set(uint16_t color) {
    ST7735S_WriteData_16Bit(color);
}
