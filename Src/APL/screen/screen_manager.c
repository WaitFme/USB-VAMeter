#include "screen_manager.h"
#include "lcd.h"

ScreenManager screenManager;

void screenManagerInit() {
    screenManager.currentScreen = SCREEN_HOME;
    screenManager.targetScreen = SCREEN_HOME;
    screenManager.state = SCREEN_STATE_INIT;
    screenManager.staticRendered = false;
    screenManager.lastUpdateTime = 0;

    for (int i = 0; i < SCREEN_COUNT; i++) {
        screenManager.screens[i] = NULL;
    }
}

void screenManagerUpdate(const AppState* state) {
    const ScreenInterface* currentInterface = screenManager.screens[screenManager.currentScreen];

    switch (screenManager.state) {
        case SCREEN_STATE_INIT:
            if (currentInterface && currentInterface->init) {
                currentInterface->init();
            }
            screenManager.state = SCREEN_STATE_STATIC_RENDER;
            screenManager.staticRendered = false;
            break;

        case SCREEN_STATE_STATIC_RENDER:
            if (!screenManager.staticRendered && currentInterface) {
                if (currentInterface->renderStatic) {
                    currentInterface->renderStatic();
                }
                screenManager.staticRendered = true;
                screenManager.lastUpdateTime = GetTick();
            }
            screenManager.state = SCREEN_STATE_ACTIVE;
            break;

        case SCREEN_STATE_ACTIVE:
            if (currentInterface && currentInterface->updateDynamic) {
                currentInterface->updateDynamic(state);
            }
            // 检查是否需要切换屏幕
            if (screenManager.currentScreen != screenManager.targetScreen) {
                screenManager.state = SCREEN_STATE_CLEANUP;
            }
            break;

        case SCREEN_STATE_CLEANUP:
            if (currentInterface && currentInterface->cleanup) {
                currentInterface->cleanup();
            }
            screenManager.currentScreen = screenManager.targetScreen;
            screenManager.state = SCREEN_STATE_INIT;
            break;

        default:
            break;
    }
}

void resetStateMachine() {
    if (screenManager.state == SCREEN_STATE_ACTIVE) {
        screenManager.state = SCREEN_STATE_INIT;
    }
}

void screenManagerSwitchTo(screen_id_t screenId) {
    if (screenId < SCREEN_COUNT && screenId != screenManager.currentScreen) {
        screenManager.targetScreen = screenId;
    }
}

void screenManagerRegisterScreen(screen_id_t id, const ScreenInterface* interface) {
    if (id < SCREEN_COUNT && interface) {
        screenManager.screens[id] = interface;
    }
}

/* void onDataUpdate(ModelEvent_TypeDef event, const void* data) {
    if (screenManager.state == SCREEN_STATE_ACTIVE) {
        const ScreenInterface* currentInterface =
            screenManager.screens[screenManager.currentScreen];

        if (currentInterface && currentInterface->updateDynamic) {
            currentInterface->updateDynamic((const AppState*)data);
        }
    }
} */
