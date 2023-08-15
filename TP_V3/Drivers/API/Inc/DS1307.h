/*
 * DS1307.h
 *
 *  Created on: Aug 10, 2023
 *      Author: MSI-Joseluis
 */

#ifndef API_INC_DS1307_H_
#define API_INC_DS1307_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "API_i2c.h"
/* public typedef ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define len_Wdate	6 //length Word date
#define len_Wtime	6 //length Word time
#define len_Bdate	3 //length Bytes date
#define len_Btime	3 //length Bytes time
#define ascii_mask  0x30//length Bytes time

/* Exported functions prototypes ---------------------------------------------*/


/* Public defines ------------------------------------------------------------*/

void DS1307_Init(void);
bool DS1307_SetTime(char * ptr_dtime);
bool DS1307_SetDate(char * ptr_ddate);
void DS1307_GetTime(char fecha[6]);
void DS1307_GetDate(char hora[6]);
void DS1307_Error ( void);


#endif /* API_INC_DS1307_H_ */
