#include "model.h"
#include "adc.h"
#include "ntc.h"
#include "tim.h"
#include "lcd.h"
#include "filter.h"
#include "power_util.h"
#include <stdlib.h>

#define CURRENT_THRESHOLD 10

static MovingAverageFilter voltageFilter;
static MovingAverageFilter currentFilter;
static MovingAverageFilter offsetFilter;
static MovingAverageFilter ntcFilter;

static uint16_t voltageBuffer;
static uint16_t currentBuffer;
static uint16_t offsetBuffer;
static uint16_t ntcBuffer;

static ChargeDirect_TypeDef mChargeDirect = CHARGE_DIRECT_NONE;
static ChargeState_TypeDef mChargeState = CHARGE_STATE_IDLE;
static double mVoltage = 0.0;
static double mCurrent = 0.0;
static double mPower = 0.0;
static double mTotalEnergy = 0.0;
static double mHistoryTotalEnergy = 0;
static double mTemperature = 0.0;
static uint32_t mChargingtime = 0;
static uint32_t mHistoryChargingtime = 0;
static uint8_t mInterruptCounter = 0;
static double mLastPower = 0;

static void dataProcessing();
static void updateAppState(AppState* uistate);
static void ChargeStateMachine();

void Model_Init() {
    MovingAverageFilterInitialize(&voltageFilter, 16);
    MovingAverageFilterInitialize(&currentFilter, 16);
    MovingAverageFilterInitialize(&offsetFilter, 16);
    MovingAverageFilterInitialize(&ntcFilter, 16);
}

void Model(AppState* uistate) {
    // 数据处理
    dataProcessing();

    // 数据更新
    updateAppState(uistate);
}

static void dataProcessing() {
    offsetBuffer = MovingAverageFilterUpdate(&offsetFilter, getChannelOffset());
    currentBuffer = MovingAverageFilterUpdate(&currentFilter, getChannelCurrent());
    voltageBuffer = MovingAverageFilterUpdate(&voltageFilter, getChannelVoltage());
    ntcBuffer = MovingAverageFilterUpdate(&ntcFilter, getChannelNtc());

    mVoltage = voltCalculate(voltageBuffer);
    mCurrent = currentCalculate(currentBuffer, offsetBuffer);
    mTemperature = getNtcTemp(ntcBuffer);
    // 电能(mWh)
    mPower = mVoltage * mCurrent;

    if (currentBuffer != offsetBuffer && abs(currentBuffer - offsetBuffer) > CURRENT_THRESHOLD) {
        mChargeDirect = getRotation() ^ (currentBuffer > offsetBuffer) ? CHARGE_DIRECT_NEGATIVE : CHARGE_DIRECT_POSITIVE;
    } else {
        mChargeDirect = CHARGE_DIRECT_NONE;
    }
}

static void updateAppState(AppState* uistate) {
    uistate->voltage = mVoltage / 1000;
    uistate->current = mCurrent / 1000;
    uistate->power = mPower / 1000000;
    uistate->totalEnergy = mTotalEnergy / 3600000;
    uistate->temperature = mTemperature;
    uistate->chargeState = mChargeState;

    switch (mChargeDirect) {
        case CHARGE_DIRECT_NEGATIVE:
            uistate->flowDirChar = '>';
            break;
        case CHARGE_DIRECT_POSITIVE:
            uistate->flowDirChar = '<';
            break;
        default:
            uistate->flowDirChar = ' ';
            break;
    }

    switch (mChargeState) {
        case CHARGE_STATE_CHARGING: {
            uistate->chargingtime = mChargingtime;
            uistate->chargingSecond = mChargingtime % 60;
            uistate->chargingMinute = mChargingtime % 3600 / 60;
            uistate->chargingHour = mChargingtime / 3600;
            break;
        }
        case CHARGE_STATE_IDLE: {
            uistate->historyChargingtime = mHistoryChargingtime;
            uistate->historyTotalEnergy = mHistoryTotalEnergy;
            break;
        }

        default:
            break;
    }
}

static void ChargeStateMachine() {
    ChargeState_TypeDef prev_state = mChargeState;

    switch (mChargeState) {
        case CHARGE_STATE_IDLE: {
            if (mCurrent > CURRENT_THRESHOLD) {
                mChargeState = CHARGE_STATE_STARTING;
            }
            break;
        }

        case CHARGE_STATE_STARTING: {
            if (mCurrent > CURRENT_THRESHOLD) {
                mChargeState = CHARGE_STATE_CHARGING;

                mChargingtime = 0;
                mTotalEnergy = 0;
                mInterruptCounter = 0;
                mLastPower = 0;
            } else {
                mChargeState = CHARGE_STATE_IDLE;
            }
            break;
        }

        case CHARGE_STATE_CHARGING: {
            mInterruptCounter++;

            if (mInterruptCounter >= 10) {
                mInterruptCounter = 0;
                mChargingtime++;
            }

            double currentPower = mPower / 1000;
            mTotalEnergy += (mLastPower + currentPower) * 0.1 / 2.0;  // 0.1秒间隔
            mLastPower = currentPower;

            if (mCurrent <= CURRENT_THRESHOLD) {
                mChargeState = CHARGE_STATE_COMPLETED;
            }
            break;
        }

        case CHARGE_STATE_COMPLETED: {
            if (mCurrent > CURRENT_THRESHOLD) {
                mChargeState = CHARGE_STATE_CHARGING;
            } else {
                mChargeState = CHARGE_STATE_IDLE;

                if (prev_state != CHARGE_STATE_IDLE) {
                    mHistoryChargingtime = mChargingtime;
                    mHistoryTotalEnergy = mTotalEnergy / 3600000;
                }
            }
            break;
        }

        default: {
            mChargeState = CHARGE_STATE_IDLE;
            break;
        }
    }
}

/**
 * @brief BTIM2定时器外部中断，频率10Hz
 */
void BTIM2_IRQHandler() {
    if (BTIM_GetITStatus(CW_BTIM2, BTIM_IT_UPDATE)) {
        BTIM_ClearITPendingBit(CW_BTIM2, BTIM_IT_UPDATE);

        ChargeStateMachine();
    }
}
