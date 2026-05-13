#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_msp.h"
#include <string.h>


volatile uint32_t pulse_count_tim1 = 0;
volatile uint32_t target_pulses_tim1 = 1000;
volatile uint8_t pwm_running_tim1 = 0;

volatile uint32_t pulse_count_tim3 = 0;
volatile uint32_t target_pulses_tim3 = 0;
volatile uint8_t pwm_running_tim3 = 0;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /*
    Este método lleva el conteo de los pulsos ejecutados por los PWM.
    Cuando se cumplen, se detiene el PWM. 
    */
    if(htim->Instance == TIM1){
        if(pwm_running_tim1){
            pulse_count_tim1++;

            if(pulse_count_tim1 >= target_pulses_tim1){
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);

                HAL_TIM_Base_Stop_IT(&htim1);

                pwm_running_tim1 = 0;
            }
        }
    }

    if(htim->Instance == TIM3){
        if(pwm_running_tim3){
            pulse_count_tim3++;

            if(pulse_count_tim3 >= target_pulses_tim3){
                HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);

                HAL_TIM_Base_Stop_IT(&htim3);

                pwm_running_tim3 = 0;
            }
        }
    }
}

void start_pwm_pulses_tim1(uint32_t pulses){
    /*
    Este metodo inicia la operación del PWM en el timer 1 
    */
    target_pulses_tim1 = pulses;
    pulse_count_tim1 = 0;
    pwm_running_tim1 = 1;

    __HAL_TIM_SET_COUNTER(&htim1, 0);

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

    HAL_TIM_Base_Start_IT(&htim1);
}

void start_pwm_pulses_tim3(uint32_t pulses){
    /*
    Este metodo inicia la operación del PWM en el timer 3 
    */
    target_pulses_tim3 = pulses;
    pulse_count_tim3 = 0;
    pwm_running_tim3 = 1;

    __HAL_TIM_SET_COUNTER(&htim3, 0);

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

    HAL_TIM_Base_Start_IT(&htim3);
}

void TIM1_UP_TIM10_IRQHandler(void){
    HAL_TIM_IRQHandler(&htim1);
}

void TIM3_IRQHandler(void){
    HAL_TIM_IRQHandler(&htim3);
}

void set_pwm_frequency(TIM_HandleTypeDef *htim, uint32_t freq, uint32_t channel)
{
  /*
  Este metodo establece la frecuencia PWM, acorde a la siguiente formula:
  TIM_frec/[(Prescaler+1)(Periodo+1)] = f 
  */
    uint32_t timer_clock = 100000000; // 100 MHz
    uint32_t prescaler = 999;
    uint32_t arr;

    arr = (timer_clock / ((prescaler + 1) * freq)) - 1;

    __HAL_TIM_SET_PRESCALER(htim, prescaler);
    __HAL_TIM_SET_AUTORELOAD(htim, arr);
    __HAL_TIM_SET_COMPARE(htim, channel, arr / 2);
}

void desplazar_eje_x(float x, float vx, int dir){

    /*
    Este metodo emplea el pwm del temporizador 1
    para desplazar el eje x.

    Parametros:
    dir: direccion de desplazamiento [+ o -]
    vx: velocidad del eje x [m/s]
    x: posición del eje x [m]
    */

    if(dir){
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        HAL_Delay(1000);
    }else{
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }
    // pasos_por_segundo = v * (pasos_revolucion/paso_husillo)
    // Considerando full step del motor y un husillo de 8mm de 
    // avance 200/0.008 = 25000
    int pasos_por_segundo = vx * 25000; 
    /* Setear frecuencia en htim1 (PA8) */
    set_pwm_frequency(&htim1, pasos_por_segundo, 1);
  
    /* Setear el numero de pulsos deseado*/
    // numero_de_pasos = x * (pasos_por_revolucion / paso_husillo)
    // Considerando full step del motor y un husillo de 8mm de 
    // avance 200/0.008 = 25000
    int numero_pasos = x * 25000;
    start_pwm_pulses_tim1(numero_pasos);
}

void desplazar_eje_y(float y, float vy, int dir){
    /*
    Este metodo emplea el pwm del temporizador 3
    para desplazar el eje y.

    Parametros:
    dir: direccion de desplazamiento [+ o -]
    vy: velocidad del eje [m/s]
    y: posición del eje x [m]
    */

    if(dir){
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        HAL_Delay(1000);
    }else{
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }
    // pasos_por_segundo = v * (pasos_revolucion/paso_husillo)
    // Considerando full step del motor y un husillo de 8mm de 
    // avance 200/0.008 = 25000
    int pasos_por_segundo = vy * 25000; 
    /* Setear frecuencia en htim3 */
    set_pwm_frequency(&htim3, pasos_por_segundo, 3);

    /* Setear el numero de pulsos deseado*/
    // numero_de_pasos = y * (pasos_por_revolucion / paso_husillo)
    // Considerando full step del motor y un husillo de 8mm de 
    // avance 200/0.008 = 25000
    int numero_pasos = y * 25000;
    /* Iniciar PWM en TIM3_CH3 (PB0) */
    start_pwm_pulses_tim3(numero_pasos);
}

int main(void) {

  HAL_Init();
  
  /* Setup the system clock to 100 MHz */
  SystemClock_Config();

  /* Set tick interrupt priority */
  HAL_InitTick(TICK_INT_PRIORITY);
  
  /* Pin initializations*/
  GPIO_Config();
  
  /* Enable timer 1 on channel 1 */
  enable_tim1_ch1(&htim1);
  HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

  /* Enable timer 3 on channel 3 */
  enable_tim3_ch3(&htim3);
  HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
  
  desplazar_eje_x(0.1, 0.005, 1);
  //desplazar_eje_x(0.1, 0.005, 0);


  /*
  Algoritmo:
  1. Al activarse los finales de carrera, deben desabilitarse los pwm.
  2.    
  */

  // TODO: Es necesario encapsular el metodo y que este reciba como parametros
  // Posición deseada, y velocidad. Automaticamente debe ser capaz de calcular
  // el numero de pulsos del PWM y la dirección. 

  /****************** MAQUINA DE ESTADOS **********************/
  typedef enum{
    ESTADO_ESPERA,
    ESTADO_RETORNO_A_CASA,
    ESTADO_OPERACION,
  }EstadoRobot_t;

EstadoRobot_t estado_actual = ESTADO_ESPERA;

void maquina_estados_robot(void){
    switch(estado_actual){
        case ESTADO_ESPERA:
            /*
            Esperando comandos
            */
            break;

        case ESTADO_RETORNO_A_CASA:
            /*
            Ejecutando movimiento eje X
            */
            break;

        case ESTADO_OPERACION:
            /*
            Los movimientos se enviaran uno a uno
            desde la computadora. 
            */
            break;
    }
}

  while(1){
      //leer_entradas();
      maquina_estados_robot();
      //actualizar_salidas();
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

