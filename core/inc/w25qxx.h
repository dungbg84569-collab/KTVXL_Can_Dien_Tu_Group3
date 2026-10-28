#ifndef __W25QXX_H
#define __W25QXX_H

#include "stm32f10x.h"
#include <stdint.h>

#define W25QXX_CS_GPIO_PORT     GPIOA
#define W25QXX_CS_GPIO_CLK      RCC_APB2Periph_GPIOA
#define W25QXX_CS_PIN           GPIO_Pin_4

#define W25X_WriteEnable        0x06
#define W25X_WriteDisable       0x04
#define W25X_ReadStatusReg      0x05
#define W25X_WriteStatusReg     0x01
#define W25X_ReadData           0x03
#define W25X_FastReadData       0x0B
#define W25X_PageProgram        0x02
#define W25X_SectorErase        0x20
#define W25X_BlockErase         0xD8
#define W25X_ChipErase          0xC7
#define W25X_DeviceID           0x90

void W25QXX_Init(void);
uint16_t W25QXX_ReadID(void);
void W25QXX_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void W25QXX_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void W25QXX_SectorErase(uint32_t DstAddr);
void W25QXX_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);

#endif /* __W25QXX_H */