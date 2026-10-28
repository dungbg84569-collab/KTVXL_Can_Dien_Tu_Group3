#ifndef __STORAGE_H
#define __STORAGE_H

#include "stm32f10x.h"
#include <stdint.h>

typedef struct {
    uint32_t magic_number;
    int32_t  zero_offset;
    float    scale_factor;
} ScaleConfig_t;

void Storage_Init(void);
void Storage_SaveConfig(int32_t zero_offset, float scale_factor);
void Storage_LoadConfig(int32_t* zero_offset, float* scale_factor);

#endif /* __STORAGE_H */