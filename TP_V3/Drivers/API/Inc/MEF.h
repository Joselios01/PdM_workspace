/*
 * MEF.h
 *
 *  Created on: Aug 15, 2023
 *      Author: Joseluis Rodriguez
 */

#ifndef API_INC_MEF_H_
#define API_INC_MEF_H_

/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"

/* public typedef ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void StatesMEF_init();
void StatesMEF_Refresh();
bool Send_Fecha();
bool Read_Fecha();
bool Print_Fecha();

/* Public defines ------------------------------------------------------------*/

#endif /* API_INC_MEF_H_ */
