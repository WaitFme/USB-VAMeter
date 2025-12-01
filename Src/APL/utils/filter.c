#include "filter.h"
#include <stdlib.h>
#include <string.h>

// 均值滤波
uint32_t AverageFilter(uint16_t* value, uint32_t size) {
    uint32_t sum = 0;
    uint16_t max = 0;
    uint16_t min = 0xffff;
    int i;

    for (i = 0; i < size; i++) {
        sum += value[i];
        if (value[i] > max) {
            max = value[i];
        }
        if (value[i] < min) {
            min = value[i];
        }
    }
    sum -= max + min;
    sum = sum / (size - 2);
    return sum;
}

/**
 * @brief 初始化均值递推滤波器状态
 *
 * @param filter 指向要初始化的状态结构体的指针
 * @param N 滤波窗口大小 (必须小于等于 MAX_FILTER_SIZE)
 * @return 1 成功, 0 失败
 */
uint8_t MovingAverageFilterInitialize(MovingAverageFilter* filter, uint16_t N) {
    if (N == 0 || N > MAX_FILTER_SIZE) {
        return 0;
    }

    // 清零历史缓冲区
    memset(filter->history, 0, sizeof(filter->history));

    filter->sum = 0;
    filter->windowSize = N;
    filter->head = 0;
    filter->count = 0;

    return 1;
}

/**
 * @brief 应用均值递推滤波函数 (整数版本)
 *
 * 注意：由于输入和输出都是整数，结果将是向下取整的整数平均值。
 *
 * @param filter 指向滤波器状态结构体的指针
 * @param newValue 当前的输入数据 (uint16_t)
 * @return 滤波后的输出值 (uint16_t)
 */
uint16_t MovingAverageFilterUpdate(MovingAverageFilter* filter, uint16_t newValue) {
    // 获取即将离开窗口的最旧数据
    uint16_t oldValue = filter->history[filter->head];

    // 更新总和
    filter->sum = filter->sum - oldValue + newValue;

    // 将新值存入历史缓冲区 (替换旧值)
    filter->history[filter->head] = newValue;

    // 更新循环缓冲区的头指针
    filter->head = (filter->head + 1) % filter->windowSize;

    uint32_t divisor;

    // 处理启动阶段 (当数据点不足 N 个时)
    if (filter->count < filter->windowSize) {
        filter->count++;
        divisor = filter->count;
    } else {
        divisor = filter->windowSize;
    }

    // 返回滤波后的结果 (整数除法，向下取整)
    // 如果需要四舍五入，可以改为 (state->sum + divisor / 2) / divisor
    return (uint16_t)(filter->sum / divisor);
}

void SlidAveFilterInit(SlidAveFilter* filter, uint16_t w_size) {
    if (w_size == 0) {
        return ;
    }

    filter->cache = (float*)malloc(sizeof(float) * w_size);
    filter->w_size = w_size;
    filter->sum = 0;
    filter->head = 0;
    filter->count = 0;
}

float SlidingAverageFilter(SlidAveFilter* filter, float value) {
    uint32_t divisor;

    float oldValue = filter->cache[filter->head];

    filter->sum = filter->sum - oldValue + value;
    filter->cache[filter->head] = value;
    filter->head = (filter->head + 1) % filter->w_size;

    if (filter->count < filter->w_size) {
        filter->count++;
        divisor = filter->count;
    } else {
        divisor = filter->w_size;
    }

    return (filter->sum + divisor / 2) / divisor;
}
