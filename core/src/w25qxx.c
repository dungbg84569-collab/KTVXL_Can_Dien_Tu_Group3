#include "w25qxx.h"

static uint8_t SPI1_ReadWriteByte(uint8_t data) {
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, data);

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI1);
}

static void W25QXX_Select(void) {
    GPIO_ResetBits(W25QXX_CS_GPIO_PORT, W25QXX_CS_PIN);
}

static void W25QXX_Deselect(void) {
    GPIO_SetBits(W25QXX_CS_GPIO_PORT, W25QXX_CS_PIN);
}

void W25QXX_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = W25QXX_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(W25QXX_CS_GPIO_PORT, &GPIO_InitStructure);
    W25QXX_Deselect();

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
}

uint16_t W25QXX_ReadID(void) {
    uint16_t temp = 0;
    W25QXX_Select();
    SPI1_ReadWriteByte(W25X_DeviceID);
    SPI1_ReadWriteByte(0x00);
    SPI1_ReadWriteByte(0x00);
    SPI1_ReadWriteByte(0x00);
    temp |= SPI1_ReadWriteByte(0xFF) << 8;
    temp |= SPI1_ReadWriteByte(0xFF);
    W25QXX_Deselect();
    return temp;
}

static void W25QXX_WriteEnable(void) {
    W25QXX_Select();
    SPI1_ReadWriteByte(W25X_WriteEnable);
    W25QXX_Deselect();
}

static void W25QXX_WaitBusy(void) {
    W25QXX_Select();
    SPI1_ReadWriteByte(W25X_ReadStatusReg);
    while ((SPI1_ReadWriteByte(0xFF) & 0x01) == 0x01);
    W25QXX_Deselect();
}

void W25QXX_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead) {
    W25QXX_Select();
    SPI1_ReadWriteByte(W25X_ReadData);
    SPI1_ReadWriteByte((ReadAddr >> 16) & 0xFF);
    SPI1_ReadWriteByte((ReadAddr >> 8) & 0xFF);
    SPI1_ReadWriteByte(ReadAddr & 0xFF);
    while (NumByteToRead--) {
        *pBuffer = SPI1_ReadWriteByte(0xFF);
        pBuffer++;
    }
    W25QXX_Deselect();
}

void W25QXX_SectorErase(uint32_t DstAddr) {
    W25QXX_WriteEnable();
    W25QXX_WaitBusy();
    W25QXX_Select();
    SPI1_ReadWriteByte(W25X_SectorErase);
    SPI1_ReadWriteByte((DstAddr >> 16) & 0xFF);
    SPI1_ReadWriteByte((DstAddr >> 8) & 0xFF);
    SPI1_ReadWriteByte(DstAddr & 0xFF);
    W25QXX_Deselect();
    W25QXX_WaitBusy();
}

void W25QXX_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) {
    W25QXX_WriteEnable();
    W25QXX_Select();
    SPI1_ReadWriteByte(W25X_PageProgram);
    SPI1_ReadWriteByte((WriteAddr >> 16) & 0xFF);
    SPI1_ReadWriteByte((WriteAddr >> 8) & 0xFF);
    SPI1_ReadWriteByte(WriteAddr & 0xFF);
    while (NumByteToWrite--) {
        SPI1_ReadWriteByte(*pBuffer);
        pBuffer++;
    }
    W25QXX_Deselect();
    W25QXX_WaitBusy();
}

void W25QXX_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) {
    W25QXX_SectorErase(WriteAddr);
    W25QXX_WritePage(pBuffer, WriteAddr, NumByteToWrite);
}