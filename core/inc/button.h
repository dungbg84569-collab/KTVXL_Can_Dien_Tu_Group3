#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f10x.h"
#include <stdint.h>

#define BUTTON_TARE_PIN     GPIO_Pin_0
#define BUTTON_CALIB_PIN    GPIO_Pin_1
#define BUTTON_TARE_CLK     RCC_APB2Periph_GPIOA

void Button_Init(void);
uint8_t Button_IsTarePressed(void);
uint8_t Button_IsCalibPressed(void);

#endif /* __BUTTON_H */