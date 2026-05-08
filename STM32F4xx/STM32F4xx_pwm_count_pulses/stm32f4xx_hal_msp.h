
#ifndef __STM32F3XX_HAL_MSP_H
#define __STM32F3XX_HAL_MSP_H

//#include "stm32f4xx_hal.h"

#ifdef __cplusplus
 extern "C" {
#endif

void HAL_MspInit(void);

void HAL_MspDeInit(void);

void HAL_PPP_MspInit(void);

void HAL_PPP_MspDeInit(void);

void SystemClock_Config(void);

void GPIO_Config(void);

void enable_tim1_ch1(TIM_HandleTypeDef* htim1);

void enable_tim3_ch3(TIM_HandleTypeDef* htim3);

static void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif 



