#include "view.h"
#include "screen_manager.h"

extern const ScreenInterface homeScreen;
extern const ScreenInterface detailScreen;
extern const ScreenInterface debugScreen;

void View_Init() {
    screenManagerInit();

    screenManagerRegisterScreen(SCREEN_HOME, &homeScreen);
    screenManagerRegisterScreen(SCREEN_DETAIL, &detailScreen);
    screenManagerRegisterScreen(SCREEN_DEBUG, &debugScreen);
}

void View(const AppState* uistate) {
    screenManagerUpdate(uistate);
}
