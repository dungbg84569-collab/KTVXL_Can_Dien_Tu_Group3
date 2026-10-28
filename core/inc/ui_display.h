#ifndef __UI_DISPLAY_H
#define __UI_DISPLAY_H

#include "stm32f10x.h"

void UI_Init(void);
void UI_UpdateWeight(float weight);
void UI_ShowStable(void);
void UI_ShowUnstable(void);
void UI_ShowCalib(void);
void UI_ShowTare(void);
void UI_ShowError(void);

#endif /* __UI_DISPLAY_H */
