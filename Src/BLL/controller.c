#include "controller.h"
#include "view.h"
#include "model.h"
#include "key.h"
#include "lcd.h"
#include "multi_button.h"

uint8_t ct_lock = false;
uint8_t rt_lock = false;

struct Button button1;

KeyConfig_typeDef kc = {
    .cStatus = KS_RELEASE,
    .lStatus = KS_RELEASE,
    .keyCount = 0,
};

void Callback_SINGLE_CLICK_Handler(void* btn) {
    changeScreen();
}

void Callback_DOUBLE_Click_Handler(void* btn) {
}

void Callback_LONG_PRESS_START_Handler(void* btn) {
    changeRotation();
}

void controllerInit() {
    button_init(&button1, readButton, 0, 0);
    button_attach(&button1, SINGLE_CLICK, Callback_SINGLE_CLICK_Handler);
    button_attach(&button1, DOUBLE_CLICK, Callback_DOUBLE_Click_Handler);
    button_attach(&button1, LONG_PRESS_START, Callback_LONG_PRESS_START_Handler);
    button_start(&button1);
}

void controller() {
    switch (kc.cStatus) {
        case KS_RELEASE: {
            ct_lock = false;
            rt_lock = false;
        } break;
        case KS_PRESS: {
            if (!ct_lock) {
                changeScreen();
                ct_lock = true;
            }
        } break;
        case KS_LONGPRESS: {
            if (!rt_lock) {
                changeRotation();
                rt_lock = true;
            }
        } break;
        default:
            break;
    }
}

void keyStatusScan() {
    switch (kc.cStatus) {
        case KS_RELEASE: {
            if (KEY0 == 0) {
                kc.cStatus = KS_PRESS_SHAKE;
                kc.keyCount = 0;
            }
            break;
        }
        case KS_PRESS_SHAKE: {
            kc.keyCount++;
            if (KEY0 == 1) {
                kc.cStatus = KS_RELEASE;
            } else if (kc.keyCount == 3) {
                kc.cStatus = KS_WAIT_CHECK;
                kc.keyCount = 0;
            }
            break;
        }
        case KS_WAIT_CHECK: {
            kc.keyCount++;

            if (kc.keyCount >= 50) {
                kc.cStatus = KS_LONGPRESS;
                kc.keyCount = 0;
            } else if (KEY0 == 1) {
                kc.cStatus = KS_PRESS;
                kc.keyCount = 0;
            }
            break;
        }
        case KS_PRESS: {
            if (KEY0 == 1) {
                kc.cStatus = KS_RELEASE_SHAKE;
            }
            break;
        }
        case KS_LONGPRESS: {
            if (KEY0 == 1) {
                kc.cStatus = KS_RELEASE_SHAKE;
            }
            break;
        }
        case KS_DOUBLE_CLICK: {
            // Todo
            break;
        }
        case KS_RELEASE_SHAKE: {
            kc.keyCount++;
            if (KEY0 == 0) {
                kc.cStatus = KS_WAIT_CHECK;
            } else if (kc.keyCount == 3) {
                kc.cStatus = KS_RELEASE;
            }
            break;
        }
        default: {
            // Nothing
            break;
        }
    }
    if (kc.cStatus != kc.lStatus) {
        kc.lStatus = kc.cStatus;
    }
}
