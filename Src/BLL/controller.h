#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "main.h"

typedef enum {
    CHARGE_DIRECT_NONE = 0,
    CHARGE_DIRECT_POSITIVE,
    CHARGE_DIRECT_NEGATIVE
} ChargeDirect_TypeDef;

typedef enum {
    CHARGE_STATE_IDLE = 0,
    CHARGE_STATE_STARTING,
    CHARGE_STATE_CHARGING,
    CHARGE_STATE_COMPLETED
} ChargeState_TypeDef;

typedef enum {
    SCREEN_DIRECT_POSITIVE = 0,
    SCREEN_DIRECT_NEGATIVE,
} ScreenDirect_TypeDef;

typedef struct {
    // 测量数据
    double voltage;
    double current;
    double power;
    double totalEnergy;
    double historyTotalEnergy;
    double temperature;
    char flowDirChar;
    // 时间数据
    uint32_t chargingtime;
    uint32_t historyChargingtime;
    uint16_t chargingHour;
    uint8_t chargingMinute;
    uint8_t chargingSecond;
    // 状态数据
    uint8_t flowDirect;
    ChargeState_TypeDef chargeState;
    ChargeDirect_TypeDef chargeDirect;
    uint8_t refreshLevel;
} AppState;

void Controller_Init();
void Controller();
uint8_t getRotation();

#endif  // !CONTROLLER_H
