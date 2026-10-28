#ifndef __APP_SCALE_H
#define __APP_SCALE_H

#include "stm32f10x.h"
#include "filter.h"
#include <stdint.h>

#define SCALE_MAX_WEIGHT_GRAM   20000.0f

typedef enum {
    SCALE_STATE_INIT = 0,
    SCALE_STATE_NORMAL,
    SCALE_STATE_TARE,
    SCALE_STATE_CALIB,
    SCALE_STATE_OVERLOAD
} ScaleState_t;

typedef struct {
    ScaleState_t state;
    int32_t      zero_offset;
    float        scale_factor;
    float        current_weight;
    uint8_t      is_stable;
} ScaleSystem_t;

extern ScaleSystem_t g_scale;

void App_Scale_Init(void);
void App_Scale_SetTare(void);
void App_Scale_Calibrate(float reference_weight_g);
void App_Scale_Task(void);

#endif /* __APP_SCALE_H */
