#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "main.h"

typedef enum {
    NONEX,
    ROTATIONX,
    REFRESHX,
} ControllerEvent_typeDef;

typedef struct {
    uint8_t ViewRefreshLock;
    uint8_t ModelRefreshLock;
    uint8_t KeyScanLock;
} ControlState_TypeDef;

void controllerInit();
void controller();

#endif  // !CONTROLLER_H
