/*
 * LCD04x16.h
 *
 *  Created on: Aug 13, 2023
 *      Author: MSI-Joseluis
 */

#ifndef API_INC_LCD04X16_H_
#define API_INC_LCD04X16_H_

/* Includes ------------------------------------------------------------------*/

#include "stdint.h"
#include "API_i2c.h"

/* public typedef ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
/* Public defines ------------------------------------------------------------*/

void LCD04x16_init (void);
void LCD04x16_TxCmd(char LCD_Cmd);
void lcd_send_cmd4bits(void);
//void lcd_send_cmd(char LCD_Cmd);
void LCD04x16_TxChar(char LCD_Dat);
void lcd_BackLigth (bool state);


#endif /* API_INC_LCD04X16_H_ */
