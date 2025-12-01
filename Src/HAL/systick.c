#include "systick.h"

void Systick_Init() {
    InitTick(48000000);
}

// 延时函数(ms)
void delay(uint32_t ms) {
    uint32_t tickstart = GetTick();
    uint32_t wait = ms;

    // 等待时间到达
    while ((GetTick() - tickstart) < wait);
}
