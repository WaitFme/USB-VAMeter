#include "controller.h"
#include "view.h"
#include "model.h"
#include "key.h"
#include "lcd.h"

uint8_t ct_lock = 0;
uint8_t rt_lock = 0;

void controller(Display_typeDef *display, KeyConfig_typeDef *kct) {
    switch (kct->cStatus) {
        case KS_RELEASE:
            ct_lock = 0;
            rt_lock = 0;
            break;
        case KS_PRESS: {
            if (!ct_lock) {
                display->screen++;
                display->initLock = 0;
                if (display->screen >= 2) {
                    display->screen = 0;
                }
                ct_lock = 1;
            }
        } break;
        case KS_LONGPRESS: {
            if (!rt_lock) {
                display->event = ROTATION;
                rt_lock = 1;
            }
        } break;
        default:
            break;
    }
}

// void controller(ControllerEvent_typeDef ce) {
//     if (ce == ROTATIONX) {
//         setRotationss(0);
//     }
// }

// void setRotationss(uint8_t value) {
//     lcd_rotation(value);
// }

void controllerInit() {

}

void controller() {

}

void keyStatusScan(KeyConfig_typeDef *kc) {
    switch (kc->cStatus) {
        case KS_RELEASE: {
            if (KEY0 == 0) {
                kc->cStatus = KS_PRESS_SHAKE;
                kc->keyCount = 0;
            }
            break;
        }
        case KS_PRESS_SHAKE: {
            kc->keyCount++;
            if (KEY0 == 1) {
                kc->cStatus = KS_RELEASE;
            } else if (kc->keyCount == 3) {
                kc->cStatus = KS_WAIT_CHECK;
                kc->keyCount = 0;
            }
            break;
        }
        case KS_WAIT_CHECK: {
            kc->keyCount++;

            if (kc->keyCount >= 50) {
                kc->cStatus = KS_LONGPRESS;
                kc->keyCount = 0;
            } else if (KEY0 == 1) {
                kc->cStatus = KS_PRESS;
                kc->keyCount = 0;
            }
            break;
        }
        case KS_PRESS: {
            if (KEY0 == 1) {
                kc->cStatus = KS_RELEASE_SHAKE;
            }
            break;
        }
        case KS_LONGPRESS: {
            if (KEY0 == 1) {
                kc->cStatus = KS_RELEASE_SHAKE;
            }
            break;
        }
        case KS_DOUBLE_CLICK: {
            // Todo
            break;
        }
        case KS_RELEASE_SHAKE: {
            kc->keyCount++;
            if (KEY0 == 0) {
                kc->cStatus = KS_WAIT_CHECK;
            } else if (kc->keyCount == 3) {
                kc->cStatus = KS_RELEASE;
            }
            break;
        }
        default: {
            // Nothing
            break;
        }
    }
    if (kc->cStatus != kc->lStatus) {
        kc->lStatus = kc->cStatus;
    }
}
