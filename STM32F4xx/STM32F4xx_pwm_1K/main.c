#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_msp.h"
#include <string.h>

void set_pwm_frequency(TIM_HandleTypeDef *htim, uint32_t freq)
{
  /*
  This methods sets the desired PWM frecuency, according to the
  following formula:
  TIM_frec/[(Prescaler+1)(Periodo+1)] = f 
  */
    uint32_t timer_clock = 100000000; // 100 MHz
    uint32_t prescaler = 999;
    uint32_t arr;

    arr = (timer_clock / ((prescaler + 1) * freq)) - 1;

    __HAL_TIM_SET_PRESCALER(htim, prescaler);
    __HAL_TIM_SET_AUTORELOAD(htim, arr);
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, arr / 2);
}

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

int main(void) {

  HAL_Init();

  /* Setup the system clock to 100 MHz */
  SystemClock_Config();

  /* Set tick interrupt priority */
  HAL_InitTick(TICK_INT_PRIORITY);
  
  /* Setup led pin */
  GPIO_Config();
  
  /* Enable timer 1 on channel 1 */
  enable_tim1_ch1(&htim1);
  
  /* Enable timer 3 on channel 3 */
  enable_tim3_ch3(&htim3);
  
  /* Setear frecuencia en htim1 */
  set_pwm_frequency(&htim1, 100);

  /* Iniciar PWM en TIM1_CH1 (PA8) */
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  /* Setear frecuencia en htim3 */
  set_pwm_frequency(&htim3, 100);

  /* Iniciar PWM en TIM3_CH3 (PB0) */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

  while (1){
    }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

