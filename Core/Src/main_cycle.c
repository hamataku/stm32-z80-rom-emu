#include "main_cycle.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "rom.h"
#include "gpio.h"
#include "stm32f4xx_it.h"

#define PERIOD 200
#define PRINT

void main_init(void)
{
    // CPU RESET
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);
}

void main_cycle(void)
{
    uint16_t address = 0;
    while (1) {
        CLK_OUT_GPIO_Port->BSRR = CLK_OUT_Pin;

        // 10 cycle wait
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");

        if ((GPIOC->IDR & 3U) == 0x00) {
            address = (GPIOB->IDR & 0x77f7) | (GPIOC->IDR & 0x0808);

            // set data pin to output
            GPIOA->MODER = (GPIOA->MODER | 0b01010101010101010000000000);

            GPIOA->ODR = ((uint32_t)rom_data[address]) << 5;
#ifdef PRINT
            HAL_Delay(PERIOD);
            printf("%x,%x,EN1\n", address | ((uint16_t)(GPIOC->IDR & 0x01) << 15), rom_data[address]);
#endif

        } else {
            // set data pin to input(high impedence)
            GPIOA->MODER = (GPIOA->MODER & (~0b11111111111111110000000000));
#ifdef PRINT
            HAL_Delay(PERIOD);
            printf("%x,DIS1\n", address | ((uint16_t)(GPIOC->IDR & 0x01) << 15));
#endif
        }


        CLK_OUT_GPIO_Port->BSRR = (uint32_t)CLK_OUT_Pin << 16U;

        // 10 cycle wait
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");

        if ((GPIOC->IDR & 3U) == 0x00) {
            address = (GPIOB->IDR & 0x77f7) | (GPIOC->IDR & 0x0808);
            // set data pin to output
            GPIOA->MODER = (GPIOA->MODER | 0b01010101010101010000000000);

            GPIOA->ODR = ((uint32_t)rom_data[address]) << 5;
        } else {
            // set data pin to input(high impedence)
            GPIOA->MODER = (GPIOA->MODER & (~0b11111111111111110000000000));
        }
#ifdef PRINT
        HAL_Delay(PERIOD);
#endif
    }
}