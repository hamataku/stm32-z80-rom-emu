/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PERIOD 40
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
GPIO_InitTypeDef GPIO_InitStruct_highImpedence = {0};
GPIO_InitTypeDef GPIO_InitStruct_output = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void data_pin_highImpedance()
{
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_highImpedence);
}

void data_pin_output()
{
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_output);
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* 0~255 */
uint8_t rom_data[500] = {};
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    /* USER CODE BEGIN 2 */
    GPIO_InitStruct_highImpedence.Pin = D0_Pin | D1_Pin | D2_Pin | D3_Pin | D4_Pin | D5_Pin | D6_Pin | D7_Pin;
    GPIO_InitStruct_highImpedence.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct_highImpedence.Pull = GPIO_NOPULL;

    GPIO_InitStruct_output.Pin = D0_Pin | D1_Pin | D2_Pin | D3_Pin | D4_Pin | D5_Pin | D6_Pin | D7_Pin;
    GPIO_InitStruct_output.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct_output.Pull = GPIO_NOPULL;
    GPIO_InitStruct_output.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    memset(rom_data, 0, sizeof(rom_data));
    rom_data[0x0] = 0x00;
    rom_data[0x1] = 0x00;
    rom_data[0x2] = 0x00;
    rom_data[0x80] = 0xC3;
    rom_data[0x81] = 0x00;
    rom_data[0x82] = 0x00;

    uint16_t address = 0;

    // CPU RESET
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        uint32_t t = HAL_GetTick();
        bool cs = false;

        // printf("clk up\n");
        CLK_OUT_GPIO_Port->BSRR = CLK_OUT_Pin;

        while (HAL_GetTick() - t < PERIOD / 2) {
            if (HAL_GPIO_ReadPin(CS_GPIO_Port, CS_Pin) == GPIO_PIN_RESET && HAL_GPIO_ReadPin(OE_GPIO_Port, OE_Pin) == GPIO_PIN_RESET) {
                address = (u_int16_t)HAL_GPIO_ReadPin(A0_GPIO_Port, A0_Pin)
                          | (u_int16_t)HAL_GPIO_ReadPin(A1_GPIO_Port, A1_Pin) << 1
                          | (u_int16_t)HAL_GPIO_ReadPin(A2_GPIO_Port, A2_Pin) << 2
                          | (u_int16_t)HAL_GPIO_ReadPin(A3_GPIO_Port, A3_Pin) << 3
                          | (u_int16_t)HAL_GPIO_ReadPin(A4_GPIO_Port, A4_Pin) << 4
                          | (u_int16_t)HAL_GPIO_ReadPin(A5_GPIO_Port, A5_Pin) << 5
                          | (u_int16_t)HAL_GPIO_ReadPin(A6_GPIO_Port, A6_Pin) << 6
                          | (u_int16_t)HAL_GPIO_ReadPin(A7_GPIO_Port, A7_Pin) << 7;
                data_pin_output();
                if (address >= sizeof(rom_data)) {
                    continue;
                }
                HAL_GPIO_WritePin(D0_GPIO_Port, D0_Pin, (rom_data[address] & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                HAL_GPIO_WritePin(D1_GPIO_Port, D1_Pin, (rom_data[address] & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                HAL_GPIO_WritePin(D2_GPIO_Port, D2_Pin, (rom_data[address] & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, (rom_data[address] & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, (rom_data[address] & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, (rom_data[address] & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, (rom_data[address] & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, (rom_data[address] & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                cs = true;
            } else {
                data_pin_highImpedance();
                cs = false;
            }
        }
        if (cs) {
            printf("0x0%x,0x0%x\n", address, rom_data[address]);
        }

        CLK_OUT_GPIO_Port->BSRR = (uint32_t)CLK_OUT_Pin << 16U;

        while (HAL_GetTick() - t < PERIOD) {
            if (HAL_GPIO_ReadPin(CS_GPIO_Port, CS_Pin) == GPIO_PIN_RESET && HAL_GPIO_ReadPin(OE_GPIO_Port, OE_Pin) == GPIO_PIN_RESET) {
                address = (u_int16_t)HAL_GPIO_ReadPin(A0_GPIO_Port, A0_Pin)
                          | (u_int16_t)HAL_GPIO_ReadPin(A1_GPIO_Port, A1_Pin) << 1
                          | (u_int16_t)HAL_GPIO_ReadPin(A2_GPIO_Port, A2_Pin) << 2
                          | (u_int16_t)HAL_GPIO_ReadPin(A3_GPIO_Port, A3_Pin) << 3
                          | (u_int16_t)HAL_GPIO_ReadPin(A4_GPIO_Port, A4_Pin) << 4
                          | (u_int16_t)HAL_GPIO_ReadPin(A5_GPIO_Port, A5_Pin) << 5
                          | (u_int16_t)HAL_GPIO_ReadPin(A6_GPIO_Port, A6_Pin) << 6
                          | (u_int16_t)HAL_GPIO_ReadPin(A7_GPIO_Port, A7_Pin) << 7;
                data_pin_output();
                if (address >= sizeof(rom_data)) {
                    continue;
                }
                HAL_GPIO_WritePin(D0_GPIO_Port, D0_Pin, (rom_data[address] & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                HAL_GPIO_WritePin(D1_GPIO_Port, D1_Pin, (rom_data[address] & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                HAL_GPIO_WritePin(D2_GPIO_Port, D2_Pin, (rom_data[address] & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, (rom_data[address] & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, (rom_data[address] & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, (rom_data[address] & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, (rom_data[address] & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, (rom_data[address] & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                cs = true;
            } else {
                data_pin_highImpedance();
                cs = false;
            }
        }
        // if (cs) {
        //     printf("0x0%x,0x0%x\n", address, rom_data[address]);
        // }
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 180;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Activate the Over-Drive mode
     */
    if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
