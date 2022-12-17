#include "main_cycle.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "rom.h"
#include "gpio.h"
#include "stm32f4xx_it.h"

#define PRINT

uint32_t moder_output = 0;
uint32_t moder_analog = 0;

#define WAIT()           \
    asm volatile("nop"); \
    asm volatile("nop"); \
    asm volatile("nop"); \
    asm volatile("nop"); \
    asm volatile("nop"); \
    asm volatile("nop"); \
    asm volatile("nop"); \
    asm volatile("nop"); \
    asm volatile("nop"); \
    asm volatile("nop");

void main_init(void)
{
    // https://rappy.hatenablog.com/entry/2019/11/17/221651
    // https://controllerstech.com/stm32-gpio-input-configuration/
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = D0_Pin | D1_Pin | D2_Pin | D3_Pin | D4_Pin | D5_Pin | D6_Pin | D7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    moder_output = GPIOA->MODER;

    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    moder_analog = GPIOA->MODER;

    // CPU RESET
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);
}

void main_cycle(void)
{
    uint16_t address = 0;
    uint16_t address_prev = 0;
    while (1) {
        CLK_OUT_GPIO_Port->ODR ^= CLK_OUT_Pin;
        WAIT();

        if ((GPIOC->IDR & 3U) == 0x00) {
            // set data pin to output
            GPIOA->MODER = moder_output;

            address = (GPIOB->IDR & 0x77f7) | (GPIOC->IDR & 0x0808);
            GPIOA->ODR = ((uint32_t)rom_data[address]) << 5;
#ifdef PRINT
            HAL_Delay(10);
            if (address != address_prev) {
                printf("\n%x,%x", address | ((uint16_t)(GPIOC->IDR & 0x01) << 15), rom_data[address]);
                address_prev = address;
            }
#endif
        } else {
            // set data pin to analog(high impedence)
            GPIOA->MODER = moder_analog;

#ifdef PRINT
            HAL_Delay(10);
            address = (GPIOB->IDR & 0x77f7) | (GPIOC->IDR & 0x0808);
            if ((GPIOC->IDR & 0x01) && (address != address_prev)) {
                printf("\n%x", address | ((uint16_t)(GPIOC->IDR & 0x01) << 15));
                address_prev = address;
            }
#endif
        }
    }
}