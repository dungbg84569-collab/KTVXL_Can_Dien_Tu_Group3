#ifndef __HX711_H
#define __HX711_H

#include "stm32f10x.h"
#include <stdint.h>

#define HX711_GPIO_PORT         GPIOB
#define HX711_GPIO_CLK          RCC_APB2Periph_GPIOB
#define HX711_PIN_DT            GPIO_Pin_0
#define HX711_PIN_SCK           GPIO_Pin_1

void HX711_Init(void);
uint8_t HX711_IsReady(void);
int32_t HX711_ReadRaw(void);
int32_t HX711_ReadAverage(uint8_t samples);

#endif /* __HX711_H */
