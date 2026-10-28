#include "ui_display.h"
#include "oled.h"

void UI_Init(void) {
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0, 34, "CAN DIEN TU");
    OLED_ShowString(6, 40, "STABLE");
}

void UI_UpdateWeight(float weight) {
    unsigned int integer_part;
    unsigned int decimal_part;

    OLED_ClearPage(2);

    if (weight < 0.0f) {
        weight = 0.0f;
    }

    if (weight >= 1000.0f) {
        integer_part = (unsigned int)(weight / 1000.0f);
        decimal_part = (unsigned int)(weight - ((float)integer_part * 1000.0f));
        decimal_part = decimal_part / 100;

        OLED_ShowNumber(2, 30, integer_part);
        OLED_ShowChar(2, 36, '.');
        OLED_ShowNumber(2, 42, decimal_part);
        OLED_ShowString(2, 54, "kg");
    } 
    
    else {
        integer_part = (unsigned int)weight;
        decimal_part = (unsigned int)((weight - (float)integer_part) * 10.0f);

        OLED_ShowNumber(2, 24, integer_part);
        OLED_ShowChar(2, 42, '.');
        OLED_ShowNumber(2, 48, decimal_part);
        OLED_ShowString(2, 60, "g");
    }
}

void UI_ShowStable(void) {
    OLED_ClearPage(6);
    OLED_ShowString(6, 40, "STABLE");
}

void UI_ShowUnstable(void) {
    OLED_ClearPage(6);
    OLED_ShowString(6, 28, "UNSTABLE");
}

void UI_ShowCalib(void) {
    OLED_Clear();
    OLED_ShowString(0, 34, "CALIBRATE");
    OLED_ShowString(2, 22, "DAT VAT MAU");
    OLED_ShowString(3, 40, "1000 g");
    OLED_ShowString(5, 4, "CALIBRATE:1000");
}

void UI_ShowTare(void) {
    OLED_Clear();
    OLED_ShowString(0, 46, "TARE");
    OLED_ShowString(3, 22, "REMOVE LOAD");
    OLED_ShowString(5, 40, "WAIT");
}

void UI_ShowError(void) {
    OLED_Clear();
    OLED_ShowString(2, 40, "ERROR");
}