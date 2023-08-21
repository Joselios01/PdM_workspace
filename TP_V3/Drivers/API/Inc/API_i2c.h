/*
 * API_i2c.h
 *
 *  Created on: Aug 10, 2023
 *      Author: MSI-Joseluis
 */



/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "stdint.h"


/* Private typedef -----------------------------------------------------------*/
/* Constants defines----------------------------------------------------------*/
#define Len_buf       10  // longitud max trama de i2c
#define TimeOut_I2C   100

/* Private defines -----------------------------------------------------------*/

/* Private functions prototypes ----------------------------------------------*/
/*

*/
void i2c_Init(void);
bool i2c_Read  (uint8_t Addr_Device, char * ptr_txbuf, uint8_t long_trama);
bool i2c_Write (uint8_t Addr_Device, char * ptr_txbuf, uint8_t long_trama);


