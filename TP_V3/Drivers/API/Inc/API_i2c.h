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
bool i2c_SetRegDev     (uint8_t Addr_Device, uint8_t Reg_Device);
bool i2c_Write_OneByte (uint8_t Addr_Device, char DataLoad);
bool i2c_Write_Str     (uint8_t Addr_Device, uint8_t * DataLoad);
*/
bool i2c_Write_RegDataByte(uint8_t Addr_Device, uint8_t Reg_Device, char DataLoad);
bool i2c_RegRead          (uint8_t Addr_Device, uint8_t Reg_Device, char * ptr_rxbuf, uint8_t long_trama);
bool i2c_WriteTrama       (uint8_t Addr_Device, char * ptr_txbuf, uint8_t long_trama);

void i2c_Init(void);

