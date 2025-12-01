#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "main.h"
#include "controller.h"

// 屏幕ID枚举
typedef enum {
    SCREEN_HOME = 0,
    SCREEN_DETAIL,
    SCREEN_DEBUG,
    SCREEN_COUNT
} screen_id_t;

// 屏幕状态枚举
typedef enum {
    SCREEN_STATE_INACTIVE = 0,
    SCREEN_STATE_INIT,
    SCREEN_STATE_STATIC_RENDER,
    SCREEN_STATE_ACTIVE,
    SCREEN_STATE_CLEANUP
} screen_state_t;

// 屏幕接口结构体
typedef struct {
    void (*init)(void);
    void (*renderStatic)(void);
    void (*updateDynamic)(const AppState* state);
    void (*cleanup)(void);
    const char* name;
} ScreenInterface;

typedef struct {
    screen_id_t currentScreen;
    screen_id_t targetScreen;
    screen_state_t state;
    bool staticRendered;
    uint32_t lastUpdateTime;
    const ScreenInterface* screens[SCREEN_COUNT];
} ScreenManager;

// 屏幕管理器函数
void screenManagerInit(void);
void screenManagerUpdate(const AppState* state);
void resetStateMachine();
void screenManagerSwitchTo(screen_id_t screenId);
void screenManagerRegisterScreen(screen_id_t id, const ScreenInterface* interface);

#endif // !SCREEN_MANAGER_H
