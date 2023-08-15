/*
 * LCD04x16.c
 *
 *  Created on: Aug 13, 2023
 *      Author: MSI-Joseluis
 */



/* Includes ------------------------------------------------------------------*/

#include "LCD04x16.h"
#include "string.h"
#include "main.h"
#include "stm32f429ZI.h"  // para uso de los leds, que avisas Error

/* Private typedef -----------------------------------------------------------*/
#define DevAddress  0b01001110  // direccion del device, ya tiene corrimiento


//                      D7,D6,D5,D4,BL,E,WR,RS

#define Clear_port              0b00000000   // and &
#define msk_Clear               0b11110000   // and &
#define msk_RS                  0b00000001   // or  |
#define msk_WR                  0b00000010   // or  |
#define msk_Enabled             0b00000100   // or  |
#define msk_LigthOff            0b00000000   // or  |
#define msk_LigthOn             0b00001000   // or  |


/* Constants defines----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/
bool flag_luz=true;
/* Private functions prototypes ----------------------------------------------*/


/* Name & Code of Function ---------------------------------------------------*/

void LCD04x16_init (void)
{
	i2c_Init();

	// 4 bit initialisation
	HAL_Delay(10);  // wait for >40ms
	LCD04x16_TxCmd (0x30);
	HAL_Delay(10);  // wait for >4.1ms
	LCD04x16_TxCmd (0x30);
	HAL_Delay(10);  // wait for >100us
	LCD04x16_TxCmd (0x30);
	HAL_Delay(10);
	LCD04x16_TxCmd (0x20);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	LCD04x16_TxCmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(10);
	LCD04x16_TxCmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(10);
	LCD04x16_TxCmd (0x01);  // clear display
	HAL_Delay(10);
	LCD04x16_TxCmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(10);
	LCD04x16_TxCmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
	HAL_Delay(10);
	LCD04x16_TxCmd(0x01);




}

void lcd_send_cmd4bits(void)
{
	uint8_t data_t[4];
	data_t[0] = 0x38 |0x08;  //BL = 1 en=0, wr= 0 rs=0
	data_t[1] = 0x38 |0x0C;  //BL = 1 en=1, wr= 0 rs=0
	data_t[2] = 0x38 |0x0C;  //BL = 1 en=1, wr= 0 rs=0
	data_t[3] = 0x38 |0x08;  //BL = 1 en=0, wr= 0 rs=0
	i2c_WriteTrama(DevAddress, (uint8_t *)data_t, 4);
	HAL_Delay(10);

}



//void LCD04x16_TxCmd(char LCD_Cmd)
void LCD04x16_TxCmd(char LCD_Cmd)
{
	  char data_u, data_l;
		uint8_t data_t[4];
		data_u = (LCD_Cmd&0xf0);
		data_l = ((LCD_Cmd<<4)&0xf0);
		data_t[0] = data_u|0x0C;  //en=1, rs=0
		data_t[1] = data_u|0x08;  //en=0, rs=0
		data_t[2] = data_l|0x0C;  //en=1, rs=0
		data_t[3] = data_l|0x08;  //en=0, rs=0
		i2c_WriteTrama(DevAddress, (uint8_t *)data_t, 4);

/*
	char data_u, data_l,msk;
	uint8_t data_t[4];
	data_u = (LCD_Cmd&0xf0);
	data_l = ((LCD_Cmd<<4)&0xf0);
	msk = msk_LigthOn; //msk_ligthOff
	data_t[0] = data_u|msk_Enabled | msk; // 0x0C;  //en=1, rs=0
	data_t[1] = data_u|Clear_port  | msk;  //0x08;  //en=0, rs=0

	i2c_WriteTrama(DevAddress, (uint8_t *)data_t, 2);

	data_t[0] = data_l|msk_Enabled | msk; // 0x0C;  //en=1, rs=0
	data_t[1] = data_l|Clear_port  | msk;  //0x08;  //en=0, rs=0

	i2c_WriteTrama(DevAddress, (uint8_t *)data_t, 2);
*/
}


//void LCD04x16_TxChar(char LCD_Dat)
void LCD04x16_TxChar(char LCD_Dat)
{

	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (LCD_Dat&0xf0);
	data_l = ((LCD_Dat<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=1
	data_t[1] = data_u|0x09;  //en=0, rs=1
	data_t[2] = data_l|0x0D;  //en=1, rs=1
	data_t[3] = data_l|0x09;  //en=0, rs=1

	i2c_WriteTrama(DevAddress, (uint8_t *)data_t, 4);

	/*
	char data_u, data_l,msk;
	uint8_t data_t[4];
	data_u = (LCD_Dat&0xf0);
	data_l = ((LCD_Dat<<4)&0xf0);
	msk = msk_LigthOn;
	data_t[0] = (data_u & msk_Clear)| msk_RS | msk | msk_Enabled ; //  0x0D ;  //en=1, rs=1
	data_t[1] = (data_u & msk_Clear)| msk_RS | msk ; //0x09;  //en=0, rs=1
	data_t[2] = (data_l & msk_Clear)| msk_RS | msk | msk_Enabled ; //0x0D;  //en=1, rs=1
	data_t[3] = (data_l & msk_Clear)| msk_RS | msk ; // 0x09;  //en=0, rs=1
//	HAL_I2C_Master_Transmit (&hi2c1, DevAddress,(uint8_t *) data_t, 4, 100);
	i2c_WriteTrama(DevAddress, (uint8_t *)data_t, 4);
*/

}

void lcd_BackLigth (bool state)
{
	flag_luz= state;
}



