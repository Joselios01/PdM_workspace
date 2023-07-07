/*
 * SysDelay.c
 *
 *  Created on: 26/06/2023
 *      Author: joseluis
 */

#include "SysDelay.h"

#define		MaxDuration		60000 //limite de conteo en 1 hora


void SysDelayInit( delay_t * Time_Var, tick_t Duration)
{
    if(0 <= Duration && MaxDuration >= Duration ) // asegura que el timer no supere una hora.
    {
		Time_Var->Duration = Duration;
		Time_Var->StartTime= HAL_GetTick();		// obtiene el valor para
		Time_Var->Running  = true;		        // inicializa apagado...
		Time_Var->AutoStart  = true;		    // inicializa em modo repetitivo
    }
    else
    {
    	while(1);
    }
	return;
}


bool_t SysDelayRead ( delay_t * Time_Var)
{
uint32_t Total;
bool Respuesta;
	if (Time_Var->Running)  // entra si el temporizador esta activo
	{
		Total = HAL_GetTick() - Time_Var->StartTime;
		if(Total >= Time_Var->Duration)
		{
			Time_Var->Running = false;  //apaga despues de cumplir el tiempo.
			Respuesta = true;
		}
		else
		{
			Respuesta = false;
		}
	}
	else   // resetea el contador y lo reinicia, una vez sea consultado.
	{
		Respuesta = false;
		if (Time_Var->AutoStart)
		{
			Time_Var->StartTime = HAL_GetTick();
			Time_Var->Running = true;
		}

	}
return (Respuesta);
}



void SysDelayAutoStart( delay_t * Time_Var, bool_t AutoStart)
{
	Time_Var->AutoStart  = AutoStart;		    // inicializa em modo repetitivo
	return;
}
