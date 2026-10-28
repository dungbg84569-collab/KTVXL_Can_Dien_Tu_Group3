#include "app_scale.h"
#include "button.h"
#include "uart_cmd.h"
#include "hx711.h"
#include "storage.h"
#include "ui_display.h"
#include <stdio.h>
#include "oled.h"
ScaleSystem_t g_scale;
static MovingAverageFilter_t adc_filter;
static int32_t s_last_filtered_adc = 0;

void App_Scale_Init(void) {
    g_scale.state = SCALE_STATE_INIT;
    
    Filter_Init(&adc_filter);
    Button_Init();
    HX711_Init();
    Storage_Init();
    UI_Init();

    Storage_LoadConfig(&g_scale.zero_offset, &g_scale.scale_factor);
    
    g_scale.current_weight = 0.0f;
    g_scale.is_stable = 1;
    g_scale.state = SCALE_STATE_NORMAL;
}

void App_Scale_SetTare(void) {
    g_scale.state = SCALE_STATE_TARE;
    
    UI_ShowTare();
    
    g_scale.zero_offset = s_last_filtered_adc;
    g_scale.current_weight = 0.0f;
    
    Storage_SaveConfig(g_scale.zero_offset, g_scale.scale_factor);
    
    g_scale.state = SCALE_STATE_NORMAL;
}

void App_Scale_Calibrate(float reference_weight_g) {
    int32_t delta_adc;
    
    if (reference_weight_g <= 0.0f) return;
    
    g_scale.state = SCALE_STATE_CALIB;
    
    
    UI_ShowCalib();
    
    delta_adc = s_last_filtered_adc - g_scale.zero_offset;
    if (delta_adc != 0) {
        g_scale.scale_factor = (float)delta_adc / reference_weight_g;
    }
    
    Storage_SaveConfig(g_scale.zero_offset, g_scale.scale_factor);
    OLED_Clear();
    g_scale.state = SCALE_STATE_NORMAL;
}

void App_Scale_Task(void) {
    int32_t raw_adc;

    if (Button_IsTarePressed()) {
        App_Scale_SetTare();
        UART1_SendString("\r\nEVENT: Button Tare Pressed (Saved to Flash)\r\n");
    }

    if (Button_IsCalibPressed()) {
        App_Scale_Calibrate(1000.0f);
        UART1_SendString("\r\nEVENT: Button Calib Pressed (1000g, Saved to Flash)\r\n");
    }

    if (HX711_IsReady()) {
        raw_adc = HX711_ReadRaw();
			//UART1_SendRawData(raw_adc);
        s_last_filtered_adc = Filter_Update(&adc_filter, raw_adc);

        if (g_scale.scale_factor != 0.0f) {
            g_scale.current_weight = (float)(s_last_filtered_adc - g_scale.zero_offset) / g_scale.scale_factor;
        }

        if (g_scale.current_weight < 0.0f) {
            g_scale.current_weight = 0.0f;
        }

        if (g_scale.current_weight > SCALE_MAX_WEIGHT_GRAM) {
            g_scale.state = SCALE_STATE_OVERLOAD;
        } else if (g_scale.state == SCALE_STATE_OVERLOAD) {
            g_scale.state = SCALE_STATE_NORMAL;
        }

        if (g_scale.state != SCALE_STATE_TARE && g_scale.state != SCALE_STATE_CALIB) {
            UI_UpdateWeight(g_scale.current_weight);
            
            if (g_scale.is_stable) {
                UI_ShowStable();
            } else {
                UI_ShowUnstable();
            }
        }
    }
}