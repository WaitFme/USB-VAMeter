#include "model.h"
#include "adc.h"
#include "tools.h"
#include "stdlib.h"
#include "ntc.h"
#include "tim.h"
#include "lcd.h"

// extern uint16_t Volt_Buffer[ADC_SAMPLE_SIZE];
// extern uint16_t Curr_Buffer[ADC_SAMPLE_SIZE];
// extern uint16_t V12V_Buffer[ADC_SAMPLE_SIZE];
// extern uint16_t IPV1_Buffer[ADC_SAMPLE_SIZE];
// extern uint16_t NTC_Buffer[ADC_SAMPLE_SIZE];

extern uint16_t voltageAdcBuffer[ADC_SAMPLE_SIZE];
extern uint16_t currentAdcBuffer[ADC_SAMPLE_SIZE];
extern uint16_t offsetAdcBuffer[ADC_SAMPLE_SIZE];
extern uint16_t ntcAdcBuffer[ADC_SAMPLE_SIZE];

uint16_t voltageBuffer, currentBuffer, referenceBuffer, ntcBuffer, offsetBuffer;

short direction = -1;       // 电流方向 <0：正方向；>0：反方向
uint32_t voltage = 0;       // 电压值(mV)
uint32_t current = 0;       // 电流值(mA)
uint32_t power = 0;         // 功率值(mW)
double electricity = 0.0;   // 电能(mWh)
uint32_t temperature = 0;   // 温度值摄氏度
uint32_t chargingtime = 0;  // 充电时间

viewState vstate = {
    .rotation = 0,
    .initLock = 0,
    .screenIndex = 0,
    .rotationLock = true,
};

void cleaning() {
    // 使用均值滤波
    voltageBuffer = meanValueFilter(voltageAdcBuffer, ADC_SAMPLE_SIZE);
    currentBuffer = meanValueFilter(currentAdcBuffer, ADC_SAMPLE_SIZE);
    offsetBuffer = meanValueFilter(offsetAdcBuffer, ADC_SAMPLE_SIZE);
    ntcBuffer = meanValueFilter(ntcAdcBuffer, ADC_SAMPLE_SIZE);
}

/**
 * @brief 计算电压
 * @param value ADC采样值（范围：0-4095，对应12位ADC）
 * @return 实际电压值（单位：mV）
 */
uint32_t voltCalculate(uint16_t value) {
    // 3.3V作参考进行运算
    return ((double)value / 4095 * V_REF / R1 * (R1 + R2) + 5);
}

/**
 * @brief 计算电流方向
 * @param current 电流ADC采样值（范围：0-4095，对应12位ADC）
 * @param offset 偏置ADC采样值（范围：0-4095，对应12位ADC）
 * @return 电流方向
 */
short currentDirection(uint16_t current, uint16_t offset) {
    if (current > offset) {
        return 1;
    } else if (current < offset) {
        return 0;
    } else {
        return -1;
    }
}

// 计算电流值 单位毫安（mA）
uint32_t currentCalculate(uint16_t cBuffer, uint16_t oBuffer) {
    uint16_t temp = abs(cBuffer - oBuffer);

    // 38mA对应的 0.038*0.005*50/3.3=0.00287878
    if (temp < (V_REF * 0.0029)) {
        return 0;
    } else {
        return temp / 4095.0 * V_REF * 4;
    }
}

void modelInit() {
}

void model() {
    // 获取数据
    getAdcValue();

    // 数据简单滤波
    cleaning();

    // 计算数据
    voltage = voltCalculate(voltageBuffer);
    current = currentCalculate(currentBuffer, offsetBuffer);
    power = voltage * current / 1000;
    direction = currentDirection(currentBuffer, offsetBuffer);
    temperature = GetNTCTable_Temp(ntcBuffer);

    if (current > 5) {
        timer2Enable(ENABLE);
    } else {
        timer2Enable(DISABLE);
        chargingtime = 0;
        electricity = 0;
    }

    vstate.voltage = (double)voltage / 1000;
    vstate.current = (double)current / 1000;
    vstate.power = (double)power / 1000;
    vstate.temperature = (double)temperature / 10;
    vstate.currDirection = direction;

    if (direction != -1) {
        vstate.directionChar = vstate.rotation ^ vstate.currDirection ? '>' : '<';
    } else {
        vstate.directionChar = ' ';
    }
}

viewState getState() {
    return vstate;
}

void setInitLock(bool value) {
    vstate.initLock = value;
}

void setRotationLock(bool value) {
    vstate.rotationLock = value;
}

void changeScreen() {
    vstate.initLock = 0;
    vstate.screenIndex++;
    if (vstate.screenIndex >= 2) {
        vstate.screenIndex = 0;
    }
}

void changeRotation() {
    vstate.rotation = !vstate.rotation;
    vstate.initLock = 0;
    vstate.rotationLock = 0;
}

// 1hz 中断
void BTIM2_IRQHandler() {
    if (BTIM_GetITStatus(CW_BTIM2, BTIM_IT_UPDATE)) {
        BTIM_ClearITPendingBit(CW_BTIM2, BTIM_IT_UPDATE);
        chargingtime++;
        electricity += (double)power / 1000;
        vstate.chargingtime = chargingtime;
        vstate.chargingSecond = chargingtime % 60;
        vstate.chargingMinute = chargingtime % 3600 / 60;
        vstate.chargingHour = chargingtime / 3600;
        vstate.electricity = electricity / 3600;
    }
}
