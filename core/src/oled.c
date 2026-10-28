#include "oled.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"

#define OLED_I2C            I2C1
#define OLED_ADDRESS        0x78
#define OLED_WIDTH          128
#define OLED_PAGE_COUNT     8

/* Font 5x7 */
static const unsigned char Font_Digit[10][5] = {
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, {0x00, 0x42, 0x7F, 0x40, 0x00},
    {0x42, 0x61, 0x51, 0x49, 0x46}, {0x21, 0x41, 0x45, 0x4B, 0x31},
    {0x18, 0x14, 0x12, 0x7F, 0x10}, {0x27, 0x45, 0x45, 0x45, 0x39},
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, {0x01, 0x71, 0x09, 0x05, 0x03},
    {0x36, 0x49, 0x49, 0x49, 0x36}, {0x06, 0x49, 0x49, 0x29, 0x1E}
};

static const unsigned char Font_Upper[26][5] = {
    {0x7E,0x11,0x11,0x11,0x7E}, {0x7F,0x49,0x49,0x49,0x36}, {0x3E,0x41,0x41,0x41,0x22},
    {0x7F,0x41,0x41,0x22,0x1C}, {0x7F,0x49,0x49,0x49,0x41}, {0x7F,0x09,0x09,0x09,0x01},
    {0x3E,0x41,0x49,0x49,0x7A}, {0x7F,0x08,0x08,0x08,0x7F}, {0x00,0x41,0x7F,0x41,0x00},
    {0x20,0x40,0x41,0x3F,0x01}, {0x7F,0x08,0x14,0x22,0x41}, {0x7F,0x40,0x40,0x40,0x40},
    {0x7F,0x02,0x0C,0x02,0x7F}, {0x7F,0x04,0x08,0x10,0x7F}, {0x3E,0x41,0x41,0x41,0x3E},
    {0x7F,0x09,0x09,0x09,0x06}, {0x3E,0x41,0x51,0x21,0x5E}, {0x7F,0x09,0x19,0x29,0x46},
    {0x46,0x49,0x49,0x49,0x31}, {0x01,0x01,0x7F,0x01,0x01}, {0x3F,0x40,0x40,0x40,0x3F},
    {0x1F,0x20,0x40,0x20,0x1F}, {0x7F,0x20,0x18,0x20,0x7F}, {0x63,0x14,0x08,0x14,0x63},
    {0x07,0x08,0x70,0x08,0x07}, {0x61,0x51,0x49,0x45,0x43}
};

static void OLED_WriteCommand(unsigned char command) {
    while (I2C_GetFlagStatus(OLED_I2C, I2C_FLAG_BUSY));
    I2C_GenerateSTART(OLED_I2C, ENABLE);
    while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(OLED_I2C, OLED_ADDRESS, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(OLED_I2C, 0x00);
    while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
    I2C_SendData(OLED_I2C, command);
    while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(OLED_I2C, ENABLE);
}

static void OLED_I2C_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000;
    I2C_Init(OLED_I2C, &I2C_InitStructure);
    I2C_Cmd(OLED_I2C, ENABLE);
}

static void OLED_SH1106_Init(void) {
    OLED_WriteCommand(0xAE);
    OLED_WriteCommand(0xD5); OLED_WriteCommand(0x80);
    OLED_WriteCommand(0xA8); OLED_WriteCommand(0x3F);
    OLED_WriteCommand(0xD3); OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x40);
    OLED_WriteCommand(0xAD); OLED_WriteCommand(0x8B);
    OLED_WriteCommand(0xA1);
    OLED_WriteCommand(0xC8);
    OLED_WriteCommand(0xDA); OLED_WriteCommand(0x12);
    OLED_WriteCommand(0x81); OLED_WriteCommand(0x80);
    OLED_WriteCommand(0xD9); OLED_WriteCommand(0x22);
    OLED_WriteCommand(0xDB); OLED_WriteCommand(0x35);
    OLED_WriteCommand(0xA4);
    OLED_WriteCommand(0xA6);
    OLED_WriteCommand(0xAF);
}

static void OLED_SetPosition(unsigned char page, unsigned char column) {
    unsigned char real_column = column + 2;
    OLED_WriteCommand(0xB0 + page);
    OLED_WriteCommand(real_column & 0x0F);
    OLED_WriteCommand(0x10 | (real_column >> 4));
}

static void OLED_WriteData(unsigned char data) {
    while (I2C_GetFlagStatus(OLED_I2C, I2C_FLAG_BUSY));
    I2C_GenerateSTART(OLED_I2C, ENABLE);
    while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(OLED_I2C, OLED_ADDRESS, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(OLED_I2C, 0x40);
    while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
    I2C_SendData(OLED_I2C, data);
    while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(OLED_I2C, ENABLE);
}

void OLED_ClearPage(unsigned char page) {
    unsigned char column;
    OLED_SetPosition(page, 0);
    while (I2C_GetFlagStatus(OLED_I2C, I2C_FLAG_BUSY));
    I2C_GenerateSTART(OLED_I2C, ENABLE);
    while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(OLED_I2C, OLED_ADDRESS, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(OLED_I2C, 0x40);
    while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
    for (column = 0; column < OLED_WIDTH; column++) {
        I2C_SendData(OLED_I2C, 0x00);
        while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
    }
    I2C_GenerateSTOP(OLED_I2C, ENABLE);
}

void OLED_Init(void) {
    OLED_I2C_Init();
    OLED_SH1106_Init();
    OLED_Clear();
}

void OLED_Clear(void) {
    unsigned char page;
    for (page = 0; page < OLED_PAGE_COUNT; page++) {
        OLED_ClearPage(page);
    }
}

void OLED_Refresh(void) {
    // Hàm phụ trợ tương thích hệ thống
}

void OLED_ShowChar(unsigned char page, unsigned char column, char chr) {
    unsigned char i;
    OLED_SetPosition(page, column);
    if (chr >= '0' && chr <= '9') {
        for (i = 0; i < 5; i++) OLED_WriteData(Font_Digit[chr - '0'][i]);
        OLED_WriteData(0x00);
    } else if (chr >= 'A' && chr <= 'Z') {
        for (i = 0; i < 5; i++) OLED_WriteData(Font_Upper[chr - 'A'][i]);
        OLED_WriteData(0x00);
    } else if (chr == ' ') {
        for (i = 0; i < 6; i++) OLED_WriteData(0x00);
    } else if (chr == '.') {
        OLED_WriteData(0x00); OLED_WriteData(0x00); OLED_WriteData(0x00); OLED_WriteData(0x00); OLED_WriteData(0x60); OLED_WriteData(0x00);
    } else if (chr == ':') {
        OLED_WriteData(0x00); OLED_WriteData(0x36); OLED_WriteData(0x36); OLED_WriteData(0x00); OLED_WriteData(0x00); OLED_WriteData(0x00);
    } else if (chr == '-') {
        OLED_WriteData(0x08); OLED_WriteData(0x08); OLED_WriteData(0x08); OLED_WriteData(0x08); OLED_WriteData(0x08); OLED_WriteData(0x00);
    } else {
        for (i = 0; i < 6; i++) OLED_WriteData(0x00);
    }
}

void OLED_ShowString(unsigned char page, unsigned char column, const char *str) {
    while (*str != '\0') {
        OLED_ShowChar(page, column, *str);
        column += 6;
        if (column > 122) break;
        str++;
    }
}

void OLED_ShowNumber(unsigned char page, unsigned char column, unsigned int number) {
    char buffer[10];
    unsigned char i;
    if (number == 0) {
        OLED_ShowChar(page, column, '0');
        return;
    }
    i = 0;
    while (number > 0) {
        buffer[i] = (char)('0' + (number % 10));
        number /= 10;
        i++;
    }
    while (i > 0) {
        i--;
        OLED_ShowChar(page, column, buffer[i]);
        column += 6;
    }
}