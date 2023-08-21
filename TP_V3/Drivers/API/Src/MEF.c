/*
 * MEF.c
 *
 *  Created on: Aug 15, 2023
 *      Author: Joseluis Rodriguez
 */

/* Includes ------------------------------------------------------------------*/
#include "MEF.h"
#include "API_delay.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum{
    st_getDT,
	st_sendDT,
    st_printDT,
} StatesMEF_t;

delay_t Timmer_MEF;         // Crea una variable tipo temporizador para la MEF
StatesMEF_t actual_state;   // definicion de una variable con los estados en los que se mueve.

/* Constants defines----------------------------------------------------------*/
#define     Time_salto_MEF		300  // tiempo para saltar entre estados

/* Private defines -----------------------------------------------------------*/
bool  Flag_getDT, Flag_sendDT, Flag_printDT;// Banderas para mirar el estado de los eventos.


/* StatesMEF_init(); ---------------------------------------------------------*/
//inicializa la maquina de estados.
//le asigna un estado inicial
//configura el temporizador para iniciar la secuencia.
void StatesMEF_init()
{
	actual_state = st_getDT;
	SysDelayInit( & Timmer_MEF, Time_salto_MEF);
	Flag_getDT= true;    // inicia tomando una lectura
	Flag_sendDT= false;
	Flag_printDT = false;
}


/* StatesMEF_Refresh(); ------------------------------------------------------*/
// refresca maquina de estados y habilita los flag al salir de cada uno.
// reinicia el temporizador y vuelve a comenzar
// la bandera solo se habilita al cumplir tiempo y salir del estado
// y solo se resetea con los llamados externos desde main.
void StatesMEF_Refresh()
{
	switch (actual_state)
	{
		case st_getDT :
		{
			if (SysDelayRead(& Timmer_MEF))   //identifica que el boton se presiono, activa delay
			{
				Flag_getDT= true;
				actual_state = st_sendDT;
				SysDelayInit( & Timmer_MEF, Time_salto_MEF );
			}
			break;
		}
		case st_sendDT :
		{
   		    if (SysDelayRead(& Timmer_MEF))
		    {
   		    	Flag_sendDT= true;
			    actual_state = st_printDT;
			    SysDelayInit( & Timmer_MEF, Time_salto_MEF );
		    }
		    break;

		}
		case st_printDT :
		{
   		    if (SysDelayRead(& Timmer_MEF))
		    {
   		    	Flag_printDT= true;
			    actual_state = st_getDT;
			    SysDelayInit( & Timmer_MEF, Time_salto_MEF );
		    }
		    break;
		}

		default :
			actual_state = st_getDT;   // asegura que va a caer en el primer estado que inicia MEF
		break;
	}

}


/* Read_Fecha(); -------------------------------------------------------------*/
// no recibe parametros
// lee el valor del flag de obtener fecha y hora del DS1307
// lo resetea despues de responder positivo
// asi segura una sola entrada
bool Read_Fecha()   //habilita leer del DS1307 una sola vez
{
   bool resp= false;

   if(Flag_getDT)
   {
	   Flag_getDT = false;
	   resp = true;
   }
   return resp;

}


/* Send_Fecha(); -------------------------------------------------------------*/
// no recibe parametros
// lee el valor del flag de enviar por uart
// lo resetea despues de responder positivo
// asi segura una sola entrada
bool Send_Fecha()  // habilita enviar por uart una sola vez
{
   bool resp= false;

   if(Flag_sendDT)
   {
	   Flag_sendDT = false;
	   resp = true;
   }
   return resp;
}


/* Print_Fecha(); ------------------------------------------------------------*/
// no recibe parametros
// lee el valor del flag de escribir en LCD
// lo resetea despues de responder positivo
// asi segura una sola entrada
bool Print_Fecha()  // habilita escribir en LCD una sola vez
{
   bool resp= false;

   if(Flag_printDT)
   {
	   Flag_printDT = false;
	   resp = true;
   }
   return resp;

}

