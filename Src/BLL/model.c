#include "model.h"
#include "adc.h"
#include "tools.h"
#include "stdlib.h"
#include "ntc.h"
#include "tim.h"
#include "lcd.h"

#define CURRENT_THRESHOLD 5
#define SCREEN_COUNT      3

AdcData adcData;
AppState vstate;

uint16_t voltageBuffer;
uint16_t currentBuffer;
uint16_t offsetBuffer;
uint16_t ntcBuffer;

double voltage = 0.0;
double current = 0.0;
double power = 0.0;
double totalEnergy = 0.0;   // 电能(mWh)
uint32_t chargingtime = 0;  // 充电时间
uint8_t chargeState = 0;
uint8_t lastChargeState = 0;

static void cleaning() {
    // 使用均值滤波
    voltageBuffer = meanValueFilter(adcData.voltageBuffer, ADC_SAMPLE_SIZE);
    currentBuffer = meanValueFilter(adcData.currentBuffer, ADC_SAMPLE_SIZE);
    offsetBuffer = meanValueFilter(adcData.offsetBuffer, ADC_SAMPLE_SIZE);
    ntcBuffer = meanValueFilter(adcData.ntcBuffer, ADC_SAMPLE_SIZE);
}

/**
 * @brief 计算电压
 * @param value ADC采样值（范围：0-4095，对应12位ADC）
 * @return 实际电压值（单位：mV）
 */
static double voltCalculate(uint16_t value) {
    // 3.3V作参考进行运算
    return (double)value / 4095 * V_REF / R1 * (R1 + R2) + 5;
}

/**
 * @brief 计算电流值
 * @param current 电流ADC采样值（范围：0-4095，对应12位ADC）
 * @param offset 偏置ADC采样值（范围：0-4095，对应12位ADC）
 * @return 电流值 单位毫安（mA）
 */
static double currentCalculate(uint16_t current, uint16_t offset) {
    uint16_t temp = abs(current - offset);

    // 38mA对应的 0.038*0.005*50/3.3=0.00287878
    if (temp < (V_REF * 0.0029)) {
        return 0;
    } else {
        return (double)temp / 4095.0 * V_REF * 4;
    }
}

void modelInit() {
    initAdcData(&adcData);

    vstate.historyChargingtime = 0;
    vstate.historyTotalEnergy = 0;
    vstate.rotation = 0;
    vstate.initLock = 0;
    vstate.screenIndex = 0;
    vstate.rotationLock = true;
}

void model() {
    // 获取数据
    getAdcValue(&adcData);

    // 数据简单滤波
    cleaning();

    // 计算数据
    voltage = voltCalculate(voltageBuffer);
    current = currentCalculate(currentBuffer, offsetBuffer);
    power = voltage * current;

    vstate.voltage = voltage / 1000;
    vstate.current = current / 1000;
    vstate.power = power / 1000000;
    vstate.temperature = getNtcTemp(ntcBuffer);
    vstate.chargingtime = chargingtime;
    vstate.chargingSecond = chargingtime % 60;
    vstate.chargingMinute = chargingtime % 3600 / 60;
    vstate.chargingHour = chargingtime / 3600;
    vstate.totalEnergy = totalEnergy / 3600000;

    if (currentBuffer != offsetBuffer) {
        vstate.flowDIrChar = vstate.rotation ^ (currentBuffer > offsetBuffer) ? '>' : '<';
    } else {
        vstate.flowDIrChar = ' ';
    }

    chargeState = current > CURRENT_THRESHOLD;

    if (chargeState && !lastChargeState) {
        timer2Enable(ENABLE);
        lastChargeState = 1;
    } else if (lastChargeState && !chargeState) {
        timer2Enable(DISABLE);
        vstate.historyChargingtime = chargingtime;
        vstate.historyTotalEnergy = totalEnergy / 3600000;
        chargingtime = 0;
        totalEnergy = 0;
        lastChargeState = 0;
    }
}

const AppState* getAppState() {
    return &vstate;
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
    if (vstate.screenIndex >= SCREEN_COUNT) {
        vstate.screenIndex = 0;
    }
}

void changeRotation() {
    vstate.rotation = !vstate.rotation;
    vstate.initLock = 0;
    vstate.rotationLock = 0;
}

/**
 * @brief BTIM2定时器外部中断，频率10Hz
 */
void BTIM2_IRQHandler() {
    if (BTIM_GetITStatus(CW_BTIM2, BTIM_IT_UPDATE)) {
        BTIM_ClearITPendingBit(CW_BTIM2, BTIM_IT_UPDATE);

        // 100Hz计数器，每100次中断等于1秒
        static uint8_t interruptCounter = 0;
        interruptCounter++;

        // 梯形积分法：每100ms积分一次
        static double lastPower = 0;
        double currentPower = power / 1000;
        totalEnergy += (lastPower + currentPower) * 0.1 / 2.0;  // 0.1秒间隔
        lastPower = currentPower;

        // 每100次中断（1秒）更新一次时间显示
        if (interruptCounter >= 10) {
            interruptCounter = 0;
            chargingtime++;
        }
    }
}
