/**
  ******************************************************************************
  * @file    stm32f4xx_hal_msp_template.c
  * @author  MCD Application Team
  * @brief   This file contains the HAL System and Peripheral (PPP) MSP initialization
  *          and de-initialization functions.
  *          It should be copied to the application folder and renamed into 'stm32f4xx_hal_msp.c'.           
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_msp.h"

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @defgroup HAL_MSP HAL MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions HAL MSP Private Functions
  * @{
  */

/**
  * @brief  Initializes the Global MSP.
  * @note   This function is called from HAL_Init() function to perform system
  *         level initialization (GPIOs, clock, DMA, interrupt).
  * @retval None
  */
void HAL_MspInit(void)
{
}

/**
  * @brief  DeInitializes the Global MSP.
  * @note   This functiona is called from HAL_DeInit() function to perform system
  *         level de-initialization (GPIOs, clock, DMA, interrupt).
  * @retval None
  */
void HAL_MspDeInit(void)
{

}

/**
  * @brief  Initializes the PPP MSP.
  * @note   This functiona is called from HAL_PPP_Init() function to perform 
  *         peripheral(PPP) system level initialization (GPIOs, clock, DMA, interrupt)
  * @retval None
  */
void HAL_PPP_MspInit(void)
{

}

/**
  * @brief  DeInitializes the PPP MSP.
  * @note   This functiona is called from HAL_PPP_DeInit() function to perform 
  *         peripheral(PPP) system level de-initialization (GPIOs, clock, DMA, interrupt)
  * @retval None
  */
void HAL_PPP_MspDeInit(void)
{

}

/**
  * @brief System clock setup
  * @note HSI at 100MHz
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    // Configurar HSI como oscilador principal
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 200;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2; // Salida a 100 MHz
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    // Configurar prescaladores
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2; // Máximo 50 MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
        Error_Handler();
    }
}

void GPIO_Config(void){
    // Habilitar el reloj de GPIOA y GPIOB
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Configurar PA8 (TIM1_CH1)
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // Pines como función alternativa
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // Velocidad alta
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1; // Función alternativa para TIM1_CH1
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configurar PB0 (TIM3_CH3)
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3; // Función alternativa para TIM3_CH3
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void enable_tim1_ch1(TIM_HandleTypeDef* htim1){
    // Habilitar el reloj de TIM1
    __HAL_RCC_TIM1_CLK_ENABLE();

    htim1->Instance = TIM1;
    htim1->Init.Prescaler = 99; // Prescaler = 99 para reducir frecuencia a 1 MHz (100 MHz / (99 + 1))
    htim1->Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1->Init.Period = 999; // Periodo = 999 para una frecuencia de 1 kHz (1 MHz / 1000)
    htim1->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1->Init.RepetitionCounter = 0; // No se usa en este caso
    HAL_TIM_PWM_Init(htim1);

    // Configurar el canal TIM1_CH1
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1; // Modo PWM
    sConfigOC.Pulse = 500; // Ciclo de trabajo 50% (Pulse = 500 de 1000)
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH; // Polaridad alta
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE; // Modo rápido deshabilitado
    HAL_TIM_PWM_ConfigChannel(htim1, &sConfigOC, TIM_CHANNEL_1);
}

void enable_tim3_ch3(TIM_HandleTypeDef* htim3){
    // Habilitar el reloj de TIM3
    __HAL_RCC_TIM3_CLK_ENABLE();

    htim3->Instance = TIM3;
    htim3->Init.Prescaler = 99; // Prescaler = 99 para reducir frecuencia a 1 MHz (100 MHz / (99 + 1))
    htim3->Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3->Init.Period = 999; // Periodo = 999 para una frecuencia de 1 kHz (1 MHz / 1000)
    htim3->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3->Init.RepetitionCounter = 0; // No se usa en este caso
    HAL_TIM_PWM_Init(htim3);

    // Configurar el canal TIM1_CH1
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1; // Modo PWM
    sConfigOC.Pulse = 500; // Ciclo de trabajo 50% (Pulse = 500 de 1000)
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH; // Polaridad alta
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE; // Modo rápido deshabilitado
    HAL_TIM_PWM_ConfigChannel(htim3, &sConfigOC, TIM_CHANNEL_3);
}
/**
  * @}
  */
static void Error_Handler(void){
    /* Implement here */
}


