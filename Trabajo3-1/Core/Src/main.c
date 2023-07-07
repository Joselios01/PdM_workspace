///* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "stm32f4xx_hal.h"


typedef uint32_t 	tick_t;  //AL_GetTick() devuelve un uint32_t, pero se define un tipo tick_t que solo manejara valores relacionados a tiempo.
typedef bool 		bool_t;


typedef struct {
	tick_t		Duration;   //el tipo tick_t es un uint32_t
	tick_t  	StartTime;  // usar un typedef para estas variables facilita la definicion de variables en las funciones
    bool_t      Running;    // el mismo caso para la variable bool.
}delay_t;



void SysDelayInit( delay_t * Time_Var, tick_t Duration); //inicializa el contador pero no arranca
bool_t SysDelayRead ( delay_t * Time_Var);
void SysDelayWrite( delay_t * Time_Var, tick_t Duration);   // funcion para activar/desativar AutoStart;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define		Delay_Led1				600 //frecuencia base para leds
#define		Delay_Led2				200 //frecuencia base para leds
#define		Delay_Led3				500 //frecuencia base para leds
#define		MaxDuration				60000 //limite de conteo en 1 hora

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t State;

typedef uint32_t tick_t;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  BSP_LED_Init(LED1);  // inicializa los puestos fisicos cableados de la board
  BSP_LED_Init(LED2);  // BSP -> Board Support Package
  BSP_LED_Init(LED3);
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO); //inicializa boton BSP en modo GPIO

  delay_t Timmer1;   						// variable tipo estructura delay_t
  delay_t Timmer2;   						// variable tipo estructura delay_t
  delay_t Timmer3;   						// variable tipo estructura delay_t
  SysDelayInit( & Timmer1 , Delay_Led1 );	// Configura Timmer1
  SysDelayInit( & Timmer2 , Delay_Led2 );	// Configura Timmer1
  SysDelayInit( & Timmer3 , Delay_Led3 );	// Configura Timmer1

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */
      if (SysDelayRead (&Timmer1))
	  {
	  BSP_LED_Toggle(LED1);
	  }
      if (SysDelayRead (&Timmer2))
	  {
	  BSP_LED_Toggle(LED2);
	  }
      if (SysDelayRead (&Timmer3))
	  {
	  BSP_LED_Toggle(LED3);
	  }
  }
  /* USER CODE END 3 */
}




//inicializa el temporizador.
void SysDelayInit( delay_t * Time_Var, tick_t Duration)
{
    if (Time_Var == NULL)
    	Error_Handler();
    else
    {
    	if(0 <= Duration && MaxDuration >= Duration ) // asegura que el timer no supere una hora.
        {
    		Time_Var->Duration = Duration;
    		Time_Var->StartTime= HAL_GetTick();		// obtiene el valor para
    		Time_Var->Running  = true;		        // inicializa apagado...
        }
        else
        	Error_Handler();
    }
	return;
}

//Funcion que pregunta si completo el tiempo
// si completa el tiempo y se pregunta por su estado se recarga y vuelve a iniciar.
// true -> si completa el tiempo   false-> mientras espera
bool_t SysDelayRead ( delay_t * Time_Var)
{
bool Respuesta=false;
	if (Time_Var != NULL)  // asegura que la variable Time_Var no este vacia.
	{
		if (Time_Var->Running)  // entra mientras este contando tiempos.
		{
			if(HAL_GetTick() - Time_Var->StartTime >= Time_Var->Duration)
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
			Time_Var->StartTime = HAL_GetTick();
			Time_Var->Running = true;
		}
	}
	return (Respuesta);
}


//Funcion para cambiar el temporizador
void SysDelayWrite( delay_t * Time_Var, tick_t Duration)
{
	if (Time_Var != NULL)  // asegura que la variable Time_Var no este vacia.
	{
		if(0 <= Duration && MaxDuration >= Duration ) // asegura que el timer no supere una hora.
			Time_Var->Duration = Duration;
	}
	return;

}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
