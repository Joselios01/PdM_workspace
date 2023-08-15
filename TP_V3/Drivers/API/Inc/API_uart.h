/*
 * API_uart.h
 *
 *  Created on: Aug 1, 2023
 *      Author: joseluis
 */

#ifndef API_SRC_API_UART_H_
#define API_SRC_API_UART_H_

#include "stdbool.h"
#include "main.h"
#include "stdint.h"
#include "assert.h"
#include "stm32f4xx_hal.h"

#include "string.h"
#include "stdio.h"



#define UART_RX_Pin GPIO_PIN_8
#define UART_RX_GPIO_Port GPIOD
#define UART_TX_Pin GPIO_PIN_9
#define UART_TX_GPIO_Port GPIOD

// funciones publicas, son vistas desde MAIN
bool uartInit(void);
void uartSendString(uint8_t * pstring);
void uartSendStringSize(uint8_t * pstring, uint16_t size);
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);
void uartTX_CMD_Enter (void);
void uartTX_CMD_Home (void);


#endif /* API_SRC_API_UART_H_ */
