/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "Wechselrichter.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t Puls_INT =0;
uint8_t Sollwert =0;
uint8_t Sollwert_ALT=0;


volatile uint8_t EIN_AUS =0;

double Sollwert_Double =0;

volatile static char DMA_CH2 [511];
volatile static char DMA_CH [511];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar (int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc (int ch, FILE *f)
#endif /*__GNUC__*/

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit (&huart2, (uint8_t*) &ch, 1, 0xFFFF);
	return ch;
}
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

	 HAL_ADC_Start(&hadc1);																				// Der ADC-Wandler 1 wird Aufgerufen und "eingeschaltet"
	 HAL_ADC_PollForConversion(&hadc1,100);																// Es wird gewartet, bis der ADC_Wandler fertig für die Kommunikation ist
	 Sollwert =HAL_ADC_GetValue(&hadc1);																// Der Variable Sollwert, wird der Wert des ADC_Wandlers übergeben
	 Sollwert_Double =Sollwert/35;

	 uint16_t *DMA_CH;																					// DMA_CH wird als uint16_t gecastet
	 DMA_CH = (uint16_t*) getPWM_Array1(Sollwert_Double) ;												// DMA_CH werden die Werte der Funktion "getPWM_Array1" übergeben

	 uint16_t *DMA_CH2;																					// DMA_CH2 wird als uint16_t gecastet
	 DMA_CH2 = (uint16_t*) getPWM_Array2(Sollwert_Double) ;												// DMA_CH2 werden die Werte der Funktion "getPWM_Array2" übergeben

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  MX_TIM8_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (EIN_AUS==1)
	  {																									// wenn EIN_AUS den Wert 1 hat, wird folgendes ausgeführt:
		  HAL_ADC_Start(&hadc1);																		// Der ADC-Wandler 1 wird Aufgerufen und "eingeschaltet"
		  HAL_ADC_PollForConversion(&hadc1,100);														// Es wird gewartet, bis der ADC_Wandler fertig für die Kommunikation ist

		  Sollwert =HAL_ADC_GetValue(&hadc1);															// Der Variable Sollwert, wird der Wert des ADC_Wandlers übergeben
		  Sollwert_Double =Sollwert/35;																	// Der Variable Sollwert_double wird der Wert (Sollwert/35) übergeben

		  HAL_GPIO_WritePin(SD_GPIO_Port,SD_Pin,SET);													// Der shutdown vom IR2110 wird deaktiviert (der IR wird freigegeben)

		  HAL_TIM_PWM_Start_DMA( &htim8, TIM_CHANNEL_4, (uint32_t*)DMA_CH, 511);						// Der Timer 8 starten den Channel 4 über DMA

		  HAL_TIM_PWM_Start_DMA( &htim1, TIM_CHANNEL_3, (uint32_t*)DMA_CH2, 511);						// Der Timer 1 starten den Channel 3 über DMA

		  if (Sollwert_ALT-3>Sollwert||Sollwert_ALT+3<=Sollwert)										// Wenn der Sollwert eine abweichung von mehr als 3 digits hat, wird folgendes ausgeführt:
		  {
			  uint16_t *DMA_CH;																			// DMA_CH wird als uint16_t gecastet
			  DMA_CH = (uint16_t*) getPWM_Array1(Sollwert_Double) ;										// DMA_CH werden die Werte der Funktion "getPWM_Array1" übergeben

			  uint16_t *DMA_CH2;																		// DMA_CH2 wird als uint16_t gecastet
			  DMA_CH2 = (uint16_t*) getPWM_Array2(Sollwert_Double) ;									// DMA_CH2 werden die Werte der Funktion "getPWM_Array2" übergeben

			  NEW_Pres(Sollwert_Double);																// Die funktion "NEW_Pres" wird aufgerufen um den Prescaler zu verändern

			  Sollwert_ALT=Sollwert;																	// Der Variable Sollwert_ALT wird der Wert der Variable Sollwert zugewiesen
		}
	  }

	  else
	  {
		  HAL_GPIO_WritePin(SD_GPIO_Port,SD_Pin,RESET);													// Der shutdown vom IR2110 wird aktiviert (der IR wird blockiert)
		  HAL_TIM_PWM_Stop_DMA(&htim8,TIM_CHANNEL_4);													// Der Timer 8 stoppt den Channel 4 über DMA
		  HAL_TIM_PWM_Stop_DMA(&htim1,TIM_CHANNEL_3);													// Der Timer 1 stoppt den Channel 3 über DMA
	  }
  }

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
	/*Error_Mes();*/
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
