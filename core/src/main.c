#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>

#include "button.h"
#include "filter.h"
#include "uart_cmd.h"
#include "app_scale.h"

static __IO uint32_t TimingDelay;

void Delay_ms(uint32_t ms);

int main(void) {
    if (SysTick_Config(SystemCoreClock / 1000)) {
        while (1);
    }

    UART1_Init(115200);
    Button_Init();
    App_Scale_Init();

    UART1_SendString("\r\n========================================\r\n");
    UART1_SendString("   KTVXL - CAN DIEN TU SMART SCALE v1.0   \r\n");
    UART1_SendString("   Group 3 - STM32F103C8T6 Ready        \r\n");
    UART1_SendString("========================================\r\n");

    while (1) {
        UART1_ProcessCommand();
        App_Scale_Task();
        Delay_ms(10);
			
    }
}

void SysTick_Handler(void) {
    if (TimingDelay != 0x00) {
        TimingDelay--;
    }
}

void Delay_ms(uint32_t ms) {
    TimingDelay = ms;
    while (TimingDelay != 0);
}
