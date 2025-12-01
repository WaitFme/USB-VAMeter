#include "power_util.h"
#include <stdlib.h>

#define R2              82    // 220 KΩ
#define R1              10    // 43 Ω
#define V_REF           3300  // 3.3V

/**
 * @brief 计算电压
 * @param value ADC采样值（范围：0-4095，对应12位ADC）
 * @return 实际电压值（单位：mV）
 */
double voltCalculate(uint16_t value) {
    // 3.3V作参考进行运算
    return (double)value / 4095 * V_REF / R1 * (R1 + R2) + 5;
}

/**
 * @brief 计算电流值
 * @param current 电流ADC采样值（范围：0-4095，对应12位ADC）
 * @param offset 偏置ADC采样值（范围：0-4095，对应12位ADC）
 * @return 电流值 单位毫安（mA）
 */
double currentCalculate(uint16_t current, uint16_t offset) {
    uint16_t temp = abs(current - offset);

    // 38mA对应的 0.038*0.005*50/3.3=0.00287878
    if (temp < (V_REF * 0.0029)) {
        return 0;
    } else {
        return (double)temp / 4095.0 * V_REF * 4;
    }
}

