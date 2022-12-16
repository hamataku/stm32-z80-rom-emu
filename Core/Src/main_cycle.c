#include "main_cycle.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "rom.h"
#include "gpio.h"
#include "stm32f4xx_it.h"

#define PERIOD 40
#define GPIO_NUMBER 16U

extern uint32_t SystemCoreClock;
static uint32_t systick_max_value;

GPIO_InitTypeDef GPIO_InitStruct_highImpedence = {0};
GPIO_InitTypeDef GPIO_InitStruct_output = {0};

void main_init(void)
{
    GPIO_InitStruct_highImpedence.Pin = D0_Pin | D1_Pin | D2_Pin | D3_Pin | D4_Pin | D5_Pin | D6_Pin | D7_Pin;
    GPIO_InitStruct_highImpedence.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct_highImpedence.Pull = GPIO_NOPULL;

    GPIO_InitStruct_output.Pin = D0_Pin | D1_Pin | D2_Pin | D3_Pin | D4_Pin | D5_Pin | D6_Pin | D7_Pin;
    GPIO_InitStruct_output.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct_output.Pull = GPIO_NOPULL;
    GPIO_InitStruct_output.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    systick_max_value = SystemCoreClock / 1000;

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
        // bool cs = false;

        // printf("clk up\n");
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

        if ((GPIOC->IDR & 0x03) == 0x00) {
            address = (GPIOB->IDR & 0x77f7) | (GPIOC->IDR & 0x0808);
            // set data pin to output
            GPIOA->MODER = (GPIOA->MODER | 0b01010101010101010000000000);

            GPIOA->ODR = ((uint32_t)rom_data[address]) << 5;
            // cs = true;
        } else {
            // set data pin to input(high impedence)
            GPIOA->MODER = (GPIOA->MODER & (~0b11111111111111110000000000));
            // cs = false;
        }

        // if (cs) {
        //     printf("%x\n", address);
        // }

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

        if ((GPIOC->IDR & 0x03) == 0x00) {
            address = (GPIOB->IDR & 0x77f7) | (GPIOC->IDR & 0x0808);
            // set data pin to output
            GPIOA->MODER = (GPIOA->MODER | 0b01010101010101010000000000);

            GPIOA->ODR = ((uint32_t)rom_data[address]) << 5;
        } else {
            // set data pin to input(high impedence)
            GPIOA->MODER = (GPIOA->MODER & (~0b11111111111111110000000000));
        }
    }
}