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

#define	WLCD_Start	0x30
#define	WLCD_4bit	0x20  // 4bit mode
#define	WLCD_Char	0x28 // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
#define	WLCD_On		0x08 //Display on/off control --> D=0,C=0, B=0  ---> display off
#define WLCD_CLS    0x01  // clear display
#define	WLCD_Cursor	0x06 //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
#define	WLCD_Blink	0x0C //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)

#define	Bytes_pulso_enabled	4 //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)


/* Constants defines----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/
bool flag_luz=true;
/* Private functions prototypes ----------------------------------------------*/


/* Name & Code of Function ---------------------------------------------------*/

void LCD04x16_init (void)
{
	i2c_Init();

	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	LCD04x16_TxCmd (WLCD_Start);   //LCD start
	HAL_Delay(5);  // wait for >4.1ms
	LCD04x16_TxCmd (WLCD_Start);   //LCD start
	HAL_Delay(1);  // wait for >100us
	LCD04x16_TxCmd (WLCD_Start);   //LCD start
	HAL_Delay(1);
	LCD04x16_TxCmd (WLCD_4bit);    // 4bit mode
	HAL_Delay(1);
	LCD04x16_TxCmd (WLCD_Char);    // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	LCD04x16_TxCmd (WLCD_On);      //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	LCD04x16_TxCmd (WLCD_CLS);     // clear display
	HAL_Delay(1);
	LCD04x16_TxCmd (WLCD_Cursor);  //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	LCD04x16_TxCmd (WLCD_Blink);   //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
	HAL_Delay(1);
	LCD04x16_TxCmd (WLCD_CLS);     // clear display
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
		i2c_Write(DevAddress,(char *)data_t, Bytes_pulso_enabled);


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

	i2c_Write(DevAddress, (char *)data_t, Bytes_pulso_enabled);



}

// habilita mascara y mantiene encendida/apagada la pantalla
// implementacion incompleta.
void lcd_BackLigth (bool state)
{
	flag_luz= state;
}



