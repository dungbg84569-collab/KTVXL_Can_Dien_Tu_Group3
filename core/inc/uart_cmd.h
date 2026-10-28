#ifndef __UART_CMD_H
#define __UART_CMD_H

#include "stm32f10x.h"
#include <stdint.h>

#define RX_BUFFER_SIZE 64

typedef struct {
    char rx_buffer[RX_BUFFER_SIZE];
    volatile uint8_t rx_index;
    volatile uint8_t flag_cmd_ready;
} UART_RxBuffer_t;

void UART1_Init(uint32_t baudrate);
void UART1_SendChar(char ch);
void UART1_SendString(const char *str);
void UART1_ProcessCommand(void);

//void UART1_SendRawData(int32_t raw_value);
#endif /* __UART_CMD_H */
