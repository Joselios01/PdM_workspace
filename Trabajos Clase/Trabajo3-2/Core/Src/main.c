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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

// definicion de tipo de datos usado para la MEF
typedef enum{
    BUTTON_DOWN,
    BUTTON_FALLING,
    BUTTON_UP,
    BUTTON_RAISING
} buttonMEF_t;



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define		Delay_1				500 //frecuencia base para leds
#define		Delay_2				500 //frecuencia base para leds
#define		Delay_3				500//frecuencia base para leds
#define		Delay_Rebote		100//frecuencia base para leds
#define     Num_Leds		    3  // secuencia de 3 LEDS
#define     Num_Secuencias		5  // en el case :3 Cambiar secuencias.

#define     Time_Debounce_UsrBtn		40  // tiempo para eliminar ruido del UserButton

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
void ButtonMEF_init();		// inicializa
void ButtonMEF_update();	// debe leer las entradas, resolver la lógica de
					// transición de estados y actualizar las salidas
void UsrBtn_Pressed();			// debe invertir el estado del LED1
void UsrBtn_Released();		// debe invertir el estado del LED3

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

delay_t Timmer_UsrBtn;   	// variable publica para ser vista desde las funciones

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  Led_TypeDef Orden_Led[Num_Leds]={LED3,LED2,LED1};// secuencia de inicializacion
  tick_t      Orden_Time[Num_Leds]={Delay_1,Delay_2,Delay_3}; // inicializacion de tiempo

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  BSP_LED_Init(LED1);  // inicializa los puestos fisicos cableados de la board
  BSP_LED_Init(LED2);  // BSP -> Board Support Package
  BSP_LED_Init(LED3);
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO); //inicializa boton BSP en modo GPIO

  delay_t Timmer[Num_Leds];   	// variable vectores de tipo estructura delay_t

  buttonMEF_t EstadoActual;

  for (uint8_t  i = 0 ; Num_Leds > i ; i++)  //inicializacion de Num_Led_Secuencia (3)
     SysDelayInit( & Timmer[i] , Orden_Time[i] );	// Setea con la variable Secuencia_Time

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */
  uint8_t i=0;
  uint8_t Secuencias=0;
  State =0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */


	  	  //esta rutina hace el cambio de frecuencia y led de forma no bloqueante.

	  BSP_LED_On(Orden_Led[i]);   //enciende led que espera a apagar
	  if (SysDelayRead (& Timmer[i] ))  // confirma tiempo led activo
		  {
		  BSP_LED_Off(Orden_Led[i]);	//al completar tiempo apaga para apuntar al siguiente
		  if (Num_Leds == i+1)
			  i=0;			// si sobre pasa apuntador lo resetea para apuntar a 0 nuevamente
		  else
			  i++;
		  SysDelayInit( & Timmer[i] , Orden_Time[i] );	// Captura valor actual de Systick para iniciar tiempo
		  }


	  switch (State )
	  {
		  case 1 : // espera un tiempo y pregunta boton


		  case 2: // se mueve dentro de 5 posibles secuencias.

			  if (Secuencias== 5)
				  Secuencias=0;   //resetea y obliga a realizar secuencia 0

			  if (Secuencias== 4)
			  {
				  Secuencias++;
			  	  Orden_Led[0]=LED2;
			  	  Orden_Led[1]=LED2;
			  	  Orden_Led[2]=LED3;
			  }
			  if (Secuencias== 3)
			  {
				  Secuencias++;
			  	  Orden_Led[0]=LED1;
			  	  Orden_Led[1]=LED3;
			  	  Orden_Led[2]=LED3;
			  }
			  if (Secuencias== 2)
			  {
				  Secuencias++;
			  	  Orden_Led[0]=LED1;
			  	  Orden_Led[1]=LED2;
			  	  Orden_Led[2]=LED2;
			  }
			  if (Secuencias== 1)
			  {
				  Secuencias++;
			  	  Orden_Led[0]=LED3;
			  	  Orden_Led[1]=LED2;
			  	  Orden_Led[2]=LED1;
			  }
			  if (Secuencias== 0)
			  {
				  Secuencias++;
			  	  Orden_Led[0]=LED1;
			  	  Orden_Led[1]=LED2;
			  	  Orden_Led[2]=LED3;
			  }
			  State++;
			  break;

		  case 3:

			  if (!BSP_PB_GetState(BUTTON_USER))
			  {
				  State=0;
			  }
			  break;

		  default :

			  break;

	  }





  }
  /* USER CODE END 3 */
}



void ButtonMEF_init()
{
	EstadoActual = BUTTON_UP;
}

void ButtonMEF_update()
{
	switch (EstadoActual)
	{
		case BUTTON_UP :
		{

			if (BSP_PB_GetState(BUTTON_USER))
			{
				EstadoActual = BUTTON_FALLING;
				SysDelayInit( & Timmer_UsrBtn, Time_Debounce_UsrBtn );
			}
			break;
		}
		case BUTTON_FALLING :
		{
			  if (SysDelayRead(& Timmer_))
			  {
				  if(BSP_PB_GetState(BUTTON_USER))
					  State++;  // si se mantiene entra a cambiar secuncia
				  else
					  State--;	// ruido detectado, resetea a esperar pulsacion
			  }
			  break;

		}
		case BUTTON_DOWN :
		{

		}
		case BUTTON_RAISING :
		{

		}
		default :


	}

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
