/*
 * API_uart.c
 *
 *  Created on: Aug 1, 2023
 *      Author: joseluis
 */

#include <API_uart.h>
#include "string.h"
#include "stdio.h"


#define		UartBaudRate		115200
#define 	Time_Tx_Uart		1000 //salta si en un seg no a terminado de enviar
#define 	Time_Rx_Uart		100 //salta si X ms no a recibido nada
//#define		BSP_Uart			USART3


UART_HandleTypeDef BSP_huart;

void Uart_Error(void); //funcion provada solo aplica para este Driver.


bool_t uartInit(void)
{
  /* USER CODE BEGIN USART3_Init 0 */
  bool_t resp=false;
  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  /* Peripheral clock enable */
  __HAL_RCC_USART3_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOD_CLK_ENABLE();
  /**USART3 GPIO Configuration
  PD8     ------> USART3_TX
  PD9     ------> USART3_RX
  */
  GPIO_InitStruct.Pin = STLK_RX_Pin|STLK_TX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


  BSP_huart.Instance = USART3;

  BSP_huart.Init.BaudRate = UartBaudRate;
  BSP_huart.Init.WordLength = UART_WORDLENGTH_8B;
  BSP_huart.Init.StopBits = UART_STOPBITS_1;
  BSP_huart.Init.Parity = UART_PARITY_NONE;
  BSP_huart.Init.Mode = UART_MODE_TX_RX;
  BSP_huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  BSP_huart.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&BSP_huart) != HAL_OK)
	  return resp;
  HAL_Delay(2000);
  uartSendString((uint8_t *)"\r\n");
  uartSendString((uint8_t *)"inicializando Periferico UART                inicializando GPIO de UART\r\n");
  uartSendString((uint8_t *)"**********************************           ***********************************\r\n");
  uartSendString((uint8_t *)"*Periferico  : UART3             *           *RX Pin/Port: PIN_8 / GPIOD       *\r\n");
  uartSendString((uint8_t *)"*BaudRate    : 115200            *           *TX Pin/Port: PIN_9 / GPIOD       *\r\n");
  uartSendString((uint8_t *)"*Word Length : 8Bytes            *           *Mode       : MODE_AF_PP          *\r\n");
  uartSendString((uint8_t *)"*Parity      : None              *           *Pull       : NOPULL              *\r\n");
  uartSendString((uint8_t *)"*Flow Control: None              *           *Speed      : SPEED_FREQ_VERY_HIGH*\r\n");
  uartSendString((uint8_t *)"*Muestreo    : over sampling 16  *           *Alternate  : GPIO_AF7_USART3     *\r\n");
  uartSendString((uint8_t *)"**********************************           ***********************************\r\n");
  HAL_Delay(1000);
  uartTX_Enter();
  uartSendString((uint8_t *)"*Serial port READY to Use*\r\n");
  uartSendString((uint8_t *)"--------------------------\r\n");
  uartTX_Enter();
  uartTX_Enter();
  resp = true;


  return resp;
  /* USER CODE END USART3_Init 2 */

}

void uartTX_Enter (void)
{
  uint8_t CRLFbuff[] = "\r\n";
  HAL_UART_Transmit(&BSP_huart, CRLFbuff, 2, 0xFF);    /* USER CODE BEGIN 3 */
}


void uartSendString(uint8_t * pstring)
{
	assert(pstring != NULL);
	uint16_t size = strlen (pstring);
	HAL_UART_Transmit(&BSP_huart, (uint8_t *)pstring, size, Time_Tx_Uart);
}


void uartSendStringSize(uint8_t * pstring, uint16_t size)
{

assert(pstring != NULL);
assert(size!=0);


HAL_UART_Transmit(&BSP_huart, pstring, size, Time_Tx_Uart);


}


void uartReceiveStringSize(uint8_t * pstring, uint16_t size)
{
	assert(pstring != NULL);
	assert(size!=0);

	HAL_UART_Receive(&BSP_huart, pstring, size, Time_Rx_Uart);

}


void Uart_Error()
{
	  /* USER CODE BEGIN Error_Handler_Debug */
	  /* User can add his own implementation to report the HAL error return state */
	  __disable_irq();
	  while (1)
	  {
	  }
	  /* USER CODE END Error_Handler_Debug */

}




