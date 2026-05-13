#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_msp.h"
#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include <ctype.h>


UART_HandleTypeDef huart2;

/* Buffer UART */
uint8_t rx_char;
char rx_buffer[64];
uint8_t rx_index = 0;

/* Variables por extraer */
float position = 0.0f;
float velocidad = 0.0f;
int n_value = -1;

/* Flag de mensaje completo */
volatile uint8_t message_ready = 0;


void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void MX_USART2_UART_Init(void)
{
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*
        PA2 -> USART2_TX
        PA3 -> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    huart2.Instance = USART2;

    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&huart2);

    /* Habilitar interrupción USART2 */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_IRQHandler(void){
    HAL_UART_IRQHandler(&huart2);
}

int main(void)
{ 
  
  HAL_Init();
  SystemClock_Config();
   //HAL_InitTick(TICK_INT_PRIORITY);
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  
  // Inicia la recepción por interrupción. 
  HAL_UART_Receive_IT(&huart2, &rx_char, 1);
  // HAL_UART_Transmit(&huart2, (uint8_t *)Message, strlen(Message), 100);
  
  while (1)
  {
        if(message_ready)
    {
        message_ready = 0;

        HAL_UART_Transmit(&huart2,
                          (uint8_t*)rx_buffer,
                          strlen(rx_buffer),
                          100);

        HAL_UART_Transmit(&huart2,
                          (uint8_t*)"\r\n",
                          2,
                          100);

        rx_index = 0;
    }
    
  }
 
}

/*
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    HAL_UART_Transmit(&huart2, UART2_rxBuffer, 1, 16);
    HAL_UART_Receive_IT(&huart2, UART2_rxBuffer, 16);
}
*/


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART2)
    {
        // Detectar fin de línea //
        if(rx_char == '\n')
        {
            rx_buffer[rx_index] = '\0';

            rx_index = 0;

            message_ready = 1;
        }
        else
        {
            // Guardar caracter //
            if(rx_index < sizeof(rx_buffer) - 1)
            {
                rx_buffer[rx_index++] = rx_char;
            }
        }

        // Reiniciar recepción 
        HAL_UART_Receive_IT(&huart2, &rx_char, 1);
    }
}