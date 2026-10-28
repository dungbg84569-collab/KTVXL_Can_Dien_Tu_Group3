#include "hx711.h"

static void HX711_DelayUs(uint32_t us) {
    volatile uint32_t i;
    for (i = 0; i < us * 12; i++) {
        __NOP();
    }
}

void HX711_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(HX711_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = HX711_PIN_SCK;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HX711_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = HX711_PIN_DT;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(HX711_GPIO_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(HX711_GPIO_PORT, HX711_PIN_SCK);
}

uint8_t HX711_IsReady(void) {
    return (GPIO_ReadInputDataBit(HX711_GPIO_PORT, HX711_PIN_DT) == Bit_RESET);
}

int32_t HX711_ReadRaw(void) {
    uint32_t count = 0;
    uint8_t i;

    while (GPIO_ReadInputDataBit(HX711_GPIO_PORT, HX711_PIN_DT) == Bit_SET);

    for (i = 0; i < 24; i++) {
        GPIO_SetBits(HX711_GPIO_PORT, HX711_PIN_SCK);
        HX711_DelayUs(2);
        
        count = count << 1;
        
        GPIO_ResetBits(HX711_GPIO_PORT, HX711_PIN_SCK);
        HX711_DelayUs(2);

        if (GPIO_ReadInputDataBit(HX711_GPIO_PORT, HX711_PIN_DT)) {
            count++;
        }
    }

    GPIO_SetBits(HX711_GPIO_PORT, HX711_PIN_SCK);
    HX711_DelayUs(2);
    GPIO_ResetBits(HX711_GPIO_PORT, HX711_PIN_SCK);
    HX711_DelayUs(2);

    if (count & 0x800000) {
        count |= 0xFF000000;
    }

    return (int32_t)count;
}
