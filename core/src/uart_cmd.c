#include "uart_cmd.h"
#include "app_scale.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

UART_RxBuffer_t uart1_rx = {0};

void UART1_Init(uint32_t baudrate) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

void UART1_SendChar(char ch) {
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, (uint16_t)ch);
}

void UART1_SendString(const char *str) {
    while (*str) {
        UART1_SendChar(*str++);
    }
}

void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        char rx_char = (char)USART_ReceiveData(USART1);

        if (rx_char != '\n' && rx_char != '\r') {
            if (uart1_rx.rx_index < RX_BUFFER_SIZE - 1) {
                uart1_rx.rx_buffer[uart1_rx.rx_index++] = rx_char;
            }
        } else {
            if (uart1_rx.rx_index > 0) {
                uart1_rx.rx_buffer[uart1_rx.rx_index] = '\0';
                uart1_rx.flag_cmd_ready = 1;
            }
        }

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

void UART1_ProcessCommand(void) {
    if (!uart1_rx.flag_cmd_ready) {
        return;
    }

    if (strcmp(uart1_rx.rx_buffer, "TARE") == 0) {
        App_Scale_SetTare();
        UART1_SendString("\r\nOK: TARE DONE\r\n");
    } else if (strncmp(uart1_rx.rx_buffer, "CALIBRATE:", 10) == 0) {
        float calib_weight = 0.0f;
        if (sscanf(uart1_rx.rx_buffer + 10, "%f", &calib_weight) == 1) {
            App_Scale_Calibrate(calib_weight);
            UART1_SendString("\r\nOK: CALIBRATED\r\n");
        } else {
            UART1_SendString("\r\nERROR: Invalid Calib Weight\r\n");
        }
    } else if (strcmp(uart1_rx.rx_buffer, "CALIB") == 0) {
        App_Scale_Calibrate(1000.0f);
        UART1_SendString("\r\nOK: CALIB DONE (1000g)\r\n");
    } else if (strcmp(uart1_rx.rx_buffer, "GET_WEIGHT") == 0) {
        char resp_buf[64];
        sprintf(resp_buf, "\r\nWEIGHT: %.2f g\r\nSTATUS: STABLE\r\n", g_scale.current_weight);
        UART1_SendString(resp_buf);
    } else if (strcmp(uart1_rx.rx_buffer, "INFO_REQUEST") == 0) {
				UART1_SendString("\r\nGROUP_NAME: Group 3\r\n");
        UART1_SendString("MEMBERS: Nguyen Hoang Dung, Le Dinh Duy, Nguyen Duc Dung, Pham Tien Dung\r\n");
        UART1_SendString("PROJECT_NAME: Smart Electronic Scale\r\n");
        UART1_SendString("HARDWARE_USED: STM32F103C8T6, HX711, loadcell, W25Q128, OLED\r\n");
        UART1_SendString("SOFTWARE_USED: KeilC v5, STM32F10x STD, Git-Github\r\n");
        UART1_SendString("DATE: 2026-11-16\r\n");
    } else {
        UART1_SendString("\r\nERROR: UNKNOWN COMMAND\r\n");
    }

    uart1_rx.rx_index = 0;
    uart1_rx.flag_cmd_ready = 0;
    memset(uart1_rx.rx_buffer, 0, RX_BUFFER_SIZE);
}

//void UART1_SendRawData(int32_t raw_value) {
  //  char tx_buf[32];
    // Chỉ gửi định dạng số nguyên thuần túy kết thúc bằng \r\n
   // sprintf(tx_buf, "%ld\r\n", raw_value);
   // UART1_SendString(tx_buf)
//}