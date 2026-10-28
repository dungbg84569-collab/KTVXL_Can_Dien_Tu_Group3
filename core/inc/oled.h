#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ClearPage(unsigned char page);
void OLED_ShowChar(unsigned char page, unsigned char column, char chr);
void OLED_ShowString(unsigned char page, unsigned char column, const char *str);
void OLED_ShowNumber(unsigned char page, unsigned char column, unsigned int number);

#endif /* __OLED_H */