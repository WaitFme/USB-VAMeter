#include "metro.h"

uint8_t MetroCheck(Metro* metro) {
    uint32_t now = GetTick();

    if (metro->intervalTick == 0) {
        metro->previousTick = now;
        return 1;
    }

    if ((now - metro->previousTick) >= metro->intervalTick) {
	    metro->previousTick += metro->intervalTick;
        return 1;
    }

    return 0;
}
