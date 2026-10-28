#include "storage.h"
#include "w25qxx.h"

#define STORAGE_FLASH_ADDR      0x000000
#define STORAGE_MAGIC_NUMBER    0x43414E33

void Storage_Init(void) {
    W25QXX_Init();
}

void Storage_SaveConfig(int32_t zero_offset, float scale_factor) {
    ScaleConfig_t config;

    config.magic_number = STORAGE_MAGIC_NUMBER;
    config.zero_offset = zero_offset;
    config.scale_factor = scale_factor;

    W25QXX_Write((uint8_t*)&config, STORAGE_FLASH_ADDR, sizeof(ScaleConfig_t));
}

void Storage_LoadConfig(int32_t* zero_offset, float* scale_factor) {
    ScaleConfig_t config;

    W25QXX_Read((uint8_t*)&config, STORAGE_FLASH_ADDR, sizeof(ScaleConfig_t));

    if (config.magic_number == STORAGE_MAGIC_NUMBER) {
        *zero_offset = config.zero_offset;
        *scale_factor = config.scale_factor;
    } else {
        *zero_offset = 0;
        *scale_factor = 420.0f;
    }
}