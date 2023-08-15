/*
 * DS1307.c
 *
 *  Created on: Aug 10, 2023
 *      Author: MSI-Joseluis
 */



/* Includes ------------------------------------------------------------------*/
#include "DS1307.h"
#include "string.h"
#include "main.h"
#include "stm32f429ZI.h"  // para uso de los leds, que avisas Error


/* Private typedef -----------------------------------------------------------*/
char data[10];

/* Constants defines----------------------------------------------------------*/
#define DevAddress  0b11010000  // direccion del device, ya tiene corrimiento

#define Reg_Sec     0  // Reg_Device segundos
#define Reg_Min     1  // Reg_Device minutos
#define Reg_Hour    2  // Reg_Device Hora (24Hrs)
#define Reg_Week    3  // Reg_Device dia semana 1-8
#define Reg_Day     4  // Reg_Device dia
#define Reg_Month   5  // Reg_Device Mes
#define Reg_Year    6  // Reg_Device Año

#define DS1307_Error_Delay	100   // frecuencia de led en caso de error en Driver DS1307

/* Private defines -----------------------------------------------------------*/
/* Private functions prototypes ----------------------------------------------*/



/* DS1307_();  ------------------------------------------------------------------------------------*/
/* recibe   : no recibe datos
 * devuelve : Bloquea en caso de inicializar dispositivo erroneamente
 * Es una funcion Bloqueante, en caso de error
 * Bloquea desde el Driver i2c
 */
void DS1307_Init(void)
{
	i2c_Init();
}


/* DS1307_SetTime(puntero a char de 6 posiciones fijas);  ---------------------------------------------------*/
/* recibe   : puntero a char de 6 posiciones
 * devuelve : respuesta de Error en la escritura.
 *
 * No es una funcion Bloqueante
 */
bool DS1307_SetTime(char * ptr_dtime) //(Hour,Min,Sec)
{
	bool resp= API_OK;

	  char tx_dat;
	  tx_dat= ((ptr_dtime[0] & 0x0f) << 4) | (ptr_dtime[1] & 0x0f); // ASCII to BCD
	  if( API_ERROR == i2c_Write_RegDataByte(DevAddress, Reg_Hour, tx_dat))
		  resp = API_ERROR;
	  tx_dat= ((ptr_dtime[2] & 0x0f) << 4) | (ptr_dtime[3] & 0x0f); // ASCII to BCD
	  if( API_ERROR == i2c_Write_RegDataByte(DevAddress, Reg_Min, tx_dat))
		  resp = API_ERROR;
	  tx_dat= ((ptr_dtime[4] & 0x0f) << 4) | (ptr_dtime[5] & 0x0f); // ASCII to BCD
	  if( API_ERROR == i2c_Write_RegDataByte(DevAddress, Reg_Sec, tx_dat))
		  resp = API_ERROR;

   return resp;

}

/* DS1307_SetDate(puntero a char de 6 posiciones fijas);  ---------------------------------------------------*/
/* recibe   : puntero a char de 6 posiciones
 * devuelve : respuesta de Error en la escritura.
 *
 * No es una funcion Bloqueante
 */
bool DS1307_SetDate(char * ptr_ddate)   //Day,Month,Year)
{
	bool resp= API_OK;
	 char tx_dat;
	  tx_dat= ((ptr_ddate[0] & 0x0f) << 4) | (ptr_ddate[1] & 0x0f); // ASCII to BCD
	  if( API_ERROR == i2c_Write_RegDataByte(DevAddress, Reg_Day, tx_dat))
		  resp = API_ERROR;

	  tx_dat= ((ptr_ddate[2] & 0x0f) << 4) | (ptr_ddate[3] & 0x0f); // ASCII to BCD
	  if( API_ERROR == i2c_Write_RegDataByte(DevAddress, Reg_Month, tx_dat))
		  resp = API_ERROR;

	  tx_dat= ((ptr_ddate[4] & 0x0f) << 4) | (ptr_ddate[5] & 0x0f); // ASCII to BCD
	  if( API_ERROR == i2c_Write_RegDataByte(DevAddress, Reg_Year, tx_dat))
		  resp = API_ERROR;

	  return resp;

}

/* DS1307_GetDate(char[6]);  ---------------------------------------------------*/
/* recibe   :char de 6 posiciones
 * devuelve :lee i2c y entrega en formato ddmmyy
 * funcionBloqueante solo en caso de error
 * En caso de Error genera una en led_rojo una frecuencia espesifica
 */
void DS1307_GetDate(char hora[len_Wdate])
{
	if (HAL_OK != i2c_RegRead(DevAddress,Reg_Day,(uint8_t * ) hora, len_Bdate))
		DS1307_Error();
    					 // 6 char, formato "ddmmyy" [0] decena de d


    // convierte BCD del DS1307 en ASCII para entregarlo.
	hora[5]= (hora[2] & 0x0f) | ascii_mask ;        //unidad año
	hora[4]= (hora[2] & 0xf0) >> 4 | ascii_mask;    //decena año
	hora[3]= (hora[1] & 0x0f) | ascii_mask ;        //unidad mes
	hora[2]= (hora[1] & 0xf0) >> 4 | ascii_mask;    //decena mes
	hora[1]= (hora[0] & 0x0f) | ascii_mask ;        //unidad dia
	hora[0]= (hora[0] & 0xf0) >> 4 | ascii_mask;    //decena dia

}




/* DS1307_GetTime(char[6]);  ---------------------------------------------------*/
/* recibe   :char de 6 posiciones
 * devuelve :lee i2c y entrega en formato hhmmss
 * funcionBloqueante solo en caso de error
 * En caso de Error genera una en led_rojo una frecuencia espesifica
 */
void DS1307_GetTime(char fecha[len_Wtime])
{
	if (HAL_OK != i2c_RegRead(DevAddress,Reg_Sec,(uint8_t *) fecha, len_Btime))
		DS1307_Error();  // error de lectura
    					 // 6 char, formato "hhmmss" [0] decena de h

    // convierte BCD del DS1307 en ASCII para entregarlo.
    fecha[5]= (fecha[0] & 0x0f) | ascii_mask ;     //unidad Seg
	fecha[4]= (fecha[0] & 0xf0) >> 4 | ascii_mask; //decena seg
	fecha[0] = fecha[2];  // salva antes de sobre-escribirlo
	fecha[3]= (fecha[1] & 0x0f) | ascii_mask ;     //unidad min
	fecha[2]= (fecha[1] & 0xf0) >> 4 | ascii_mask; //decena min
	fecha[1]= (fecha[0] & 0x0f) | ascii_mask ;     //unidad Hr
	fecha[0]= (fecha[0] & 0xf0) >> 4 | ascii_mask; //decena Hr

}


/* DS1307_Error();  ---------------------------------------------------*/
/* no recibe datos
 * no devuelve datos
 * funcionBloqueante
 * En caso de Error genera una en led_rojo una frecuencia espesifica
 */
void DS1307_Error ( void)
{
	BSP_LED_Init(LED_RED);
	while(1)
	{
		BSP_LED_Toggle(LED_RED);
		HAL_Delay(DS1307_Error_Delay);

	}

}
