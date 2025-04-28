#ifndef VIEW_H
#define VIEW_H

#include "main.h"

typedef enum {
    NONE,
    ROTATION,
    REFRESH,
} ViewEvent_typeDef;

typedef struct {
    ViewEvent_typeDef event;
    uint8_t refreshLock;
    uint8_t initLock;
    uint8_t screen;
} ViewState_typeDef;

void view(ViewState_typeDef *dt);

#endif  // !VIEW_H
