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

/* Private functions prototypes ----------------------------------------------*/
void i2c_Handler_Error(void);


/* Name & Code of Function ---------------------------------------------------*/

/* i2c_Init()------------- ---------------------------------------------------*/
// incluye la conguracin tanto del periferico como de los pines fisicos
// la funcion bloquea en caso de error de incializacion
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

/* i2c_Handler_Error()--------------------------------------------------------*/
// Funcion de error, hace blinking sobre el led rojo
// la funcion bloquea mostrando el error
void i2c_Handler_Error(void)
{
	BSP_LED_Init(LED1);
	while(1)
	{
		BSP_LED_Toggle(LED1);
		HAL_Delay(1000);

	}

}



/* i2c_Read(DevAddress,(uint8_t * ) dato, longitud)---------------------------*/
// Funcion de lectura de puerto I2C, configurando en la primera posicion de dato,
// un valor a enviar para la direccion a leer.
// la funcion realiza lectura entregando dato en el puntero ptr_txbuf
// Devuelve API_ERROR en caso de falla ya sea en escritura o en lectura.
// quien llama la funcion debe procesar la falla
bool i2c_Read(uint8_t Addr_Device, char * ptr_txbuf, uint8_t long_trama)
{
	bool resp = API_OK;
	if (HAL_OK != HAL_I2C_Master_Transmit(&hi2c1, Addr_Device,(uint8_t *) ptr_txbuf , 1, TimeOut_I2C))
		resp = API_ERROR;

	if (HAL_OK != HAL_I2C_Master_Receive (&hi2c1, Addr_Device,(uint8_t *) ptr_txbuf , long_trama , TimeOut_I2C))
		resp = API_ERROR;

	return resp;
}


/* i2c_wirte(DevAddress,(uint8_t * ) dato, longitud)---------------------------*/
// Funcion de ecritura por puerto I2C, configurando en la primera posicion de ptr_txbuf,
// un valor a enviar para la direccion a escribir.
// la funcion realiza escritura del puntero en el I2C controlado por la longitud
// de datoa enviar.
// Devuelve API_ERROR en caso de falla en escritura.
// quien llama la funcion debe procesar la falla
bool i2c_Write(uint8_t Addr_Device, char * ptr_txbuf, uint8_t long_trama)
{
	assert(ptr_txbuf != NULL);
	bool resp = API_OK;

	if (HAL_OK != HAL_I2C_Master_Transmit(&hi2c1, Addr_Device,(uint8_t *) ptr_txbuf , long_trama, TimeOut_I2C))
		resp = API_ERROR;

	return resp;
}






