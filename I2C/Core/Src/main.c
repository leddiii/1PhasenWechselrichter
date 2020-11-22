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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"

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

uint8_t sendbuf [64];
uint8_t loeschen[8];
uint8_t Invert[6];
uint8_t Helli[7];
uint8_t ack[2];
uint8_t  len;
uint8_t bcc;
uint8_t ret;
uint8_t ausfuehrung=0;
volatile uint8_t status;




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
uint8_t edip240_SchalterStellung (uint8_t BaseAdress, uint8_t I2CAdress)
{
	uint8_t bcc=0;
	uint8_t ack[1];
	uint8_t Sendeanfrage[4];
	uint8_t Rueckgabewert[7];
	uint8_t Pruefsumme=0;

	Sendeanfrage[0]=I2CAdress+1;
	Sendeanfrage[1]=0x01;
	Sendeanfrage[2]=0x53;
	for (int i=0;i<=2;i++)
	{
		bcc += Sendeanfrage[i];
	}
	Sendeanfrage[3]=bcc;

	HAL_I2C_Master_Transmit(&hi2c1,BaseAdress,Sendeanfrage,4,0xff);
	HAL_I2C_Master_Receive(&hi2c1,BaseAdress,ack,1,0xff);
	HAL_I2C_Master_Receive(&hi2c1,BaseAdress,Rueckgabewert,7,0xff);


for(int i=0;i<=6;i++)
{
	printf("Rueckgabewert %2i= %2x \n",i,Rueckgabewert[i]);
}


	for(int p=0;p<=5;p++)
	{
		Pruefsumme += Rueckgabewert[p];
	}
	printf("Pruefsumme= %2x \n",Pruefsumme);
	if (Pruefsumme== Rueckgabewert[6])
	{
		if (Rueckgabewert[0]==I2CAdress)
		{
			if(Rueckgabewert[2]==0x1B)
			{
				if(Rueckgabewert[3]==0x41)
				{
					return Rueckgabewert[5];
				}
			}
		}
	}

}


void edip240_Gerade_zeichnen(uint8_t Baseadress, uint8_t I2CAdress, uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
{
	uint8_t sendbuf[10];
	uint8_t bcc=0;
	uint8_t ack[1];
	sendbuf[0]= I2CAdress;
	sendbuf[1]= 0x07;
	sendbuf[2]= 0x1B;
	sendbuf[3]= 0x47;
	sendbuf[4]= 0x44;
	sendbuf[5]= x1;
	sendbuf[6]= y1;
	sendbuf[7]= x2;
	sendbuf[8]= y2;

	for (int i=0;i<=8;i++)
		{
			bcc += sendbuf[i];
		}
		sendbuf[9]=bcc;

	HAL_I2C_Master_Transmit(&hi2c1,Baseadress,sendbuf,10,0xff);
	HAL_I2C_Master_Receive(&hi2c1,Baseadress,ack,1,0xff);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	uint8_t edip240_protokoll (uint16_t dev_adr, uint8_t max_byte, uint8_t timeout)
	{
	     uint8_t  k, ret;
	     uint8_t  edip_buf[6], summe, temp[2];



	    edip_buf[0] = 0x12;                // DC2
	    edip_buf[1] = 3;                // len
	    edip_buf[2] = 0x44;                // D
	    edip_buf[3] = max_byte;            // max_byte
	    edip_buf[4] = timeout;            // timeout



	    summe = 0;
	    for (k=0; k<5; k++)
	    {
	        summe = summe + edip_buf[k];
	    }
	    edip_buf[5] = summe;            // Prüfbyte bcc



	    ret = HAL_I2C_Master_Transmit(&hi2c1, dev_adr, edip_buf, 6, 10);
	    HAL_I2C_Master_Receive(&hi2c1, dev_adr, temp, 1, 1);
	    HAL_Delay(50);
	    return ret;
	}



	void edip240_Clear (uint8_t BaseAdress,uint8_t I2CAdress){
		uint8_t clear[6];
		uint8_t bcc;
		clear[0]= I2CAdress;
		clear[1]= 0x03;
		clear[2]= 0x1B;
		clear[3]= 0x44;
		clear[4]= 0x4C;
		for (int i=0;i<=4;i++){
				bcc += clear[i];
				}
		clear[5]= bcc;
	}
void Invertieren(uint8_t* INV){
		INV[0]=0x11;
		INV[1]=0x03;
		INV[2]=0x1B;
		INV[3]=0x44;
		INV[4]=0x49;
		INV[5]=0xBC;
}



void Helligkeit (uint8_t *Sendarray,uint8_t staerke)
{
	int bcc;
		Sendarray[0]=0x11;
		Sendarray[1]=0x04;
		Sendarray[2]=0x1B;
		Sendarray[3]=0x59;
		Sendarray[4]=0x48;
		Sendarray[5]=staerke;

		for (int i=0;i<=5;i++){
		bcc += Sendarray[i];
		}

		Sendarray[6]=bcc;
}




void edip240_touch_schalter(uint8_t BaseAdress,uint8_t I2CAdress, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t downcode, uint8_t upcode, char *string, uint8_t rahmen)
{
    uint8_t arr[64];
    uint8_t ack[2];
    uint8_t sum = 0;
    uint8_t len = 12;
    int str_len = 0;
    uint8_t arr_len = (uint8_t)strlen(string) + len;
    arr[0] = I2CAdress;
    arr[2] = 0x1B;
    arr[3] = 0x41;
    arr[4] = 0x4B;
    arr[5] = x1;
    arr[6] = y1;
    arr[7] = x2;
    arr[8] = y2;
    arr[9] = downcode;
    arr[10] = upcode;
    arr[11] = 0x43; // C für text begin
    for (int a = len; a < arr_len + 1; a++)
    {
        arr[a] = (uint8_t)string[str_len];
        if (str_len != strlen(string) - 1)
        {
            str_len++;
        }
        else
        {
            arr[a + 1] = 0x00;
            arr[a + 2] = 0x1B;
            arr[a + 3] = 0x41;
            arr[a + 4] = 0x45;
            arr[a + 5] = rahmen;
            arr_len = arr_len + 5;
            break;
        }
    }
    arr[1] = arr_len - 2;
    for (int b = 0; b < arr_len; b++)
    {
        sum = sum + arr[b];
    }
    arr[arr_len] = sum;
    HAL_I2C_Master_Transmit(&hi2c1,BaseAdress , arr, 21, 0xff);
    HAL_I2C_Master_Receive(&hi2c1, BaseAdress, ack, 1, 0xff);
}

/*
	void edip240_touch_schalter(uint8_t *arr, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t downcode, uint8_t upcode, char *string, uint8_t rahmen)
	{
	    uint8_t sum = 0;
	    int len = 12;
	    int str_len = 0;
	    int arr_len = (int)strlen(string) + len; // die Plus vier für die Rahmen einstellung
	    arr[0] = 0x11;
	    arr[2] = 0x1B;
	    arr[3] = 0x41;
	    arr[4] = 0x4B;
	    arr[5] = x1;
	    arr[6] = y1;
	    arr[7] = x2;
	    arr[8] = y2;
	    arr[9] = downcode;
	    arr[10] = upcode;
	    arr[11] = 0x43; // C für text begin
	    for (int a = len; a < arr_len + 1; a++)
	    {
	        arr[a] = (uint8_t)string[str_len];
	        if (str_len != strlen(string) - 1)
	        {
	            str_len++;
	        }
	        else
	        {
	        	arr[a + 1] = 0x00;
	            arr[a + 2] = 0x1B;
	            arr[a + 3] = 0x41;
	            arr[a + 4] = 0x45;
	            arr[a + 5] = rahmen;
	            arr_len = arr_len + 5;
	            break;
	        }
	    }
	    arr[1] = arr_len - 2;
	    for (int b = 0; b < arr_len; b++)
	    {
	        sum = sum + arr[b];
	    }
	    arr[arr_len] = sum;
	    arr_len++;
	    //printf(" Länge ist %i in Hex %x der bcc ist %x\n", arr_len, arr_len, sum);
	    //return arr_len;
	}


	void ediptft_touch_schalter(uint8_t *arr, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t downcode, uint8_t upcode, char *string, uint8_t rahmen)
	{
	    uint8_t sum = 0;
	    int len = 16;
	    int str_len = 0;
	    int arr_len = (int)strlen(string) + len; // die Plus vier für die Rahmen einstellung
	    arr[0] = 0x11;
	    arr[2] = 0x1B;
	    arr[3] = 0x41;
	    arr[4] = 0x4B;
	    arr[5] = x1 >> 8;
	    arr[6] = (uint8_t)x1;
	    arr[7] = y1 >> 8;
	    arr[8] = (uint8_t)y1;
	    arr[9] = x2 >> 8;
	    arr[10] = (uint8_t)x2;
	    arr[11] = y2 >> 8;
	    arr[12] = (uint8_t)y2;
	    arr[13] = downcode;
	    arr[14] = upcode;
	    arr[15] = 0x43; // C für text begin
	    for (int a = len; a < arr_len + 1; a++)
	    {
	        arr[a] = (uint8_t)string[str_len];
	        if (str_len != strlen(string) - 1)
	        {
	            str_len++;
	        }
	        else
	        {
	            arr[a + 1] = 0x1B;
	            arr[a + 2] = 0x41;
	            arr[a + 3] = 0x45;
	            arr[a + 4] = rahmen;
	            arr_len = arr_len + 4;
	            break;
	        }
	    }
	    arr[1] = arr_len - 2;
	    for (int b = 0; b < arr_len; b++)
	    {
	        sum = sum + arr[b];
	    }
	    arr[arr_len] = sum;
	    arr_len++;
	    printf(" Länge ist %i in Hex %x der bcc ist %x\n", arr_len, arr_len, sum);
	    //return arr_len;
	}



	int edip240_touch_schalter_lesen(uint8_t *sendbuffer)
{

    int len = 3;
    uint8_t sum = 0;
    if (sendbuffer[1] == 0x11)
    {
        sum = sum + 0x11;
        sum = sum + sendbuffer[2];
        for (int i = 1; i <= sendbuffer[2]; i++)
        {
            sum = sum + sendbuffer[i + 2];
            len++;
        }
        if (sum == sendbuffer[len])
        {
            return sendbuffer[len - 1];
        }
        else
        {
            return 0;
        }
    }
    return 0;
}


	*/



	/*
void getbuffer(uint8_t *get)
{
    get[0] = 0x12;
    get[1] = 0x01;
    get[2] = 0x53;
    get[3] = 0x66;
}

*/
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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(HAL_GPIO_ReadPin(B1_GPIO_Port,B1_Pin)==0)
	  	  {
	  	   while(HAL_GPIO_ReadPin(B1_GPIO_Port,B1_Pin)==0);
	  	   	   ret=HAL_I2C_IsDeviceReady(&hi2c1, 0xDE, 3, 0xffff);
	  		   if(ret==0)
	  		      {
	  			   edip240_protokoll(0xDE,64,2);
	  			   edip240_Clear(0xDE,0x11);
	  			   edip240_touch_schalter(0xDE,0x11,100,50,140,78,171,205,"Ein",9);
	  		      }
	  	  }

if(status==205)
	{ ausfuehrung=1;
	 HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,SET);
	 while(ausfuehrung==1)
	 {
		 for(int a = 0 ; a <= 128 ; a+3)
		 {
			 edip240_Gerade_zeichnen(0xDE,0x11,a,0,239,127);
			 HAL_Delay(10);
		 }
		 ausfuehrung=0;
	 }
	}

else if(status==171)
	{

	 HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,RESET);

	}

printf("Status: %3i\n",status);

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
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
