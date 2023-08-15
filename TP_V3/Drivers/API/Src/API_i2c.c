/*
 * API_i2c.c
 *
 *  Created on: Aug 10, 2023
 *      Author: MSI-Joseluis
 */



/* Includes ------------------------------------------------------------------*/
#include "API_i2c.h"
#include "stm32f429ZI.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;

/* Constants defines----------------------------------------------------------*/
#define		len_TxOneByte		1    //
#define		len_TxTwoByte		2    //


/* Private defines -----------------------------------------------------------*/
/* Private functions prototypes ----------------------------------------------*/
void i2c_Handler_Error(void);


/* Name & Code of Function ---------------------------------------------------*/
//
//



void i2c_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;//  GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 1000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	  i2c_Handler_Error();
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	  i2c_Handler_Error();
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	  i2c_Handler_Error();

}


void i2c_Handler_Error(void)
{
	BSP_LED_Init(LED1);
	while(1)
	{
		BSP_LED_Toggle(LED1);
		HAL_Delay(1000);

	}

}


bool i2c_Write_RegDataByte(uint8_t Addr_Device, uint8_t Reg_Device, char DataLoad)
{
    bool resp= API_OK;
	char dat[2];
	dat[0]=Reg_Device;
	dat[1]=DataLoad;

	if (HAL_OK != HAL_I2C_Master_Transmit(&hi2c1, Addr_Device,(uint8_t *) dat , len_TxTwoByte, TimeOut_I2C))
		resp = API_ERROR;

	return resp;
}


bool i2c_RegRead(uint8_t Addr_Device, uint8_t Reg_Device, char * ptr_txbuf, uint8_t long_trama)
{
	bool resp = API_OK;
	char data[3];
	data[0]=Reg_Device;
	if (HAL_OK != HAL_I2C_Master_Transmit(&hi2c1, Addr_Device,(uint8_t *) data , 1, TimeOut_I2C))
		resp = API_ERROR;

	if (HAL_OK != HAL_I2C_Master_Receive (&hi2c1, Addr_Device, (uint8_t *) ptr_txbuf , long_trama , TimeOut_I2C))
		resp = API_ERROR;

	return resp;

}


bool i2c_WriteTrama(uint8_t Addr_Device, char * ptr_txbuf, uint8_t long_trama)
{
    bool resp = API_OK;

	char dat[long_trama];

	for (int i=0;i<long_trama;i++)
	  {
	  dat[i]= ptr_txbuf[i];
	  }

	if (HAL_OK != HAL_I2C_Master_Transmit(&hi2c1, Addr_Device,(uint8_t *) dat , long_trama, TimeOut_I2C))
		resp = API_ERROR;

	return resp;
}


/*
bool i2c_SetRegDev(uint8_t Addr_Device, uint8_t Reg_Device)
{
	bool resp = API_OK;
//	char data[3];
//	data[0]=Reg_Device;
	if(HAL_OK != HAL_I2C_Master_Transmit(&hi2c1, Addr_Device,(uint8_t *) Reg_Device , 1, TimeOut_I2C))
		resp = API_ERROR;

	return resp;

}




bool i2c_Write_OneByte(uint8_t Addr_Device, char DataLoad)
{
    bool resp= API_OK;

	if (HAL_OK != HAL_I2C_Master_Transmit(&hi2c1, Addr_Device,DataLoad , len_TxOneByte, TimeOut_I2C))
		resp = API_ERROR;

	return resp;
}

*/

