#include "adc.h"

// uint16_t Volt_Buffer[ADC_SAMPLE_SIZE];
// uint16_t Curr_Buffer[ADC_SAMPLE_SIZE];
// uint16_t V12V_Buffer[ADC_SAMPLE_SIZE];
// uint16_t IPV1_Buffer[ADC_SAMPLE_SIZE];
// uint16_t NTC_Buffer[ADC_SAMPLE_SIZE];

uint16_t voltageAdcBuffer[ADC_SAMPLE_SIZE];
uint16_t currentAdcBuffer[ADC_SAMPLE_SIZE];
uint16_t offsetAdcBuffer[ADC_SAMPLE_SIZE];
uint16_t ntcAdcBuffer[ADC_SAMPLE_SIZE];

void ADC_Initial() {
    // 打开时钟
    __SYSCTRL_GPIOA_CLK_ENABLE();
    __SYSCTRL_GPIOB_CLK_ENABLE();
    __SYSCTRL_ADC_CLK_ENABLE();

    PB03_ANALOG_ENABLE();
    PB04_ANALOG_ENABLE();
    PB05_ANALOG_ENABLE();
    PB06_ANALOG_ENABLE();

    ADC_InitTypeDef adc = {
        .ADC_ClkDiv = ADC_Clk_Div4,
        .ADC_ConvertMode = ADC_ConvertMode_Continuous,  // 连续采样
        .ADC_SQREns = ADC_SqrEns0to4,                   // 单通道
        .ADC_IN0.ADC_InputChannel = ADC_InputCH11,      // 电流偏置电压
        .ADC_IN0.ADC_SampTime = ADC_SampTime6Clk,       // 15+6=21 12MHZ/21=571K
        .ADC_IN1.ADC_InputChannel = ADC_InputCH12,      // 电流
        .ADC_IN1.ADC_SampTime = ADC_SampTime6Clk,
        .ADC_IN2.ADC_InputChannel = ADC_InputCH13,      // 电压
        .ADC_IN2.ADC_SampTime = ADC_SampTime6Clk,
        .ADC_IN3.ADC_InputChannel = ADC_InputVref1P2,  // 1.2V参考电压
        .ADC_IN3.ADC_SampTime = ADC_SampTime6Clk,
        .ADC_IN4.ADC_InputChannel = ADC_InputCH10,      // NTC温度
        .ADC_IN4.ADC_SampTime = ADC_SampTime6Clk,
    };
    ADC_Init(&adc);

    ADC_ClearITPendingAll();           // 清除ADC所有中断状态
    ADC_Enable();                      // ADC使能
    ADC_SoftwareStartConvCmd(ENABLE);  // ADC转换软件启动命令
}

void getAdcValue() {
    static uint8_t cnt;

    ADC_GetSqr0Result(&offsetAdcBuffer[cnt]);
    ADC_GetSqr1Result(&currentAdcBuffer[cnt]);
    ADC_GetSqr2Result(&voltageAdcBuffer[cnt]);
    ADC_GetSqr4Result(&ntcAdcBuffer[cnt]);

    cnt++;
    if (cnt >= ADC_SAMPLE_SIZE) {
        cnt = 0;
    }
}
