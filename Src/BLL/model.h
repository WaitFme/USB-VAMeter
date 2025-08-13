#ifndef MODEL_H
#define MODEL_H

#include "main.h"

typedef enum {
    ME_NONE,
    ME_ROTATION,
} ModelEvent_typeDef;

typedef struct {
    ModelEvent_typeDef event;
    uint8_t refreshLock;
    uint8_t initLock;
} ModelState_typeDef;

typedef struct {
    double voltage;
    double current;
    double power;
    double totalEnergy;
    double historyTotalEnergy;
    double temperature;
    uint32_t chargingtime;
    uint32_t historyChargingtime;
    uint8_t rotation;
    bool rotationLock;
    bool initLock;
    uint8_t screenIndex;
    char flowDIrChar;
    uint16_t chargingHour;
    uint8_t chargingMinute;
    uint8_t chargingSecond;
} AppState;

void model();
const AppState* getAppState();
void setRotationLock(bool value);
void setInitLock(bool value);
void changeScreen();
void changeRotation();

#endif // !MODEL_H
