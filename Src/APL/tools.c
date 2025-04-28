#include "tools.h"

char charTemp[20];

/**
 * @brief 计算幂函数
 *
 * @param m 基数
 * @param n 幂数
 * @return uint32_t
 */
uint32_t mypow(uint8_t m, uint8_t n) {
    uint32_t result = 1;

    while (n--) {
        result *= m;
    }

    return result;
}

// 角度转弧度
float angleToRad(float angle) {
    return angle * PI / 180;
}

// 矩阵向量乘法
void multiplyMatrixVector(float* m, float* v, float* out) {
    for (int i = 0; i < 4; i++) {
        out[i] = 0.0f;
        for (int j = 0; j < 4; j++) {
            out[i] += v[j] * m[j * 4 + i];
        }
    }
}

// 矩阵乘算
void multiply(float* A, float* B, int m, int p, int n, float* C) {
    int i, j, k;
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            C[n * i + j] = 0;
            for (k = 0; k < p; k++) {
                C[n * i + j] = C[n * i + j] + A[p * i + k] * B[n * k + j];
            }
        }
    }
}

// 均值滤波
uint32_t meanValueFilter(uint16_t* value, uint32_t size) {
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
