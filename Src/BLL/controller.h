#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "main.h"

typedef enum {
    NONEX,
    ROTATIONX,
    REFRESHX,
} ControllerEvent_typeDef;

// 状态
typedef enum {
    KS_RELEASE,
    KS_PRESS_SHAKE,
    KS_WAIT_CHECK,
    KS_PRESS,
    KS_LONGPRESS,
    KS_DOUBLE_CLICK,
    KS_RELEASE_SHAKE,
} KS_TypeDef;

typedef struct {
    KS_TypeDef lStatus;
    KS_TypeDef cStatus;
    uint16_t keyCount;
} KeyConfig_typeDef;

void controllerInit();
void controller();
void keyStatusScan();

#endif  // !CONTROLLER_H
