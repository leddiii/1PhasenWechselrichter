#include "Wechselrichter.h"
#include "math.h"
#include "main.h"

float tiefpass(float Uk, float Ykminus1)
{
    float T = 1;                // T = Zeitkonstante
    float Ta = 0.1;             // Ta= Abtastzeit
    float b = Ta / (T + Ta);    // b wird errechnet
    float a = 1 - b;            // a wird errechnet
    YK = b * Uk + a * Ykminus1; // berechnung des neuen Yk wertes
    return YK;
}

int *getPWM_Array(double Sollwert)
{
    Omega = 2.0 * M_PI * Sollwert;

    for (int Wert = 0; Wert <= size; Wert++)
    {
    	Winkel = (180.0/254.0)*Wert;
    	Winkelzeit = ((1/Sollwert)/360.0) * Winkel;

        Puls = sin(Omega * Winkelzeit) * 255;
        Array[Wert] = (int)Puls;

        printf(" Wert=  %3i  Winkel=  %1.10f   Winkelzeit=  %1.10f   Puls=   %4.4f  \t \n",Wert, Winkel, Winkelzeit, Puls);
    }

    return Array;
}


/* #include "Wechselrichter.h"

float YK = 0;
float tiefpass(float Uk, float Ykminus1)
{
    YK = b * Uk + a * Ykminus1; // berechnung des neuen Yk wertes
    return YK;
}


int *getPWM_Array(double Sollwert)
{
	double Winkel = 0.0;
    Omega = 2.0 * M_PI * Sollwert;

    for (int Wert = 0; Wert <= size; Wert++)
    {
        Winkel = (180.0/254.0)*Wert;
        Winkelzeit = ((1/Sollwert)/360.0) * Winkel;
        Puls = sin(Omega * Winkelzeit) * 7058.8235;
        Array[Wert] = (int)Puls;
    }
    return Array;
}


HAL_StatusTypeDef StartPWM_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length){
  /* Parameter Überprüfen */
  /*assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  if((htim->State == HAL_TIM_STATE_BUSY)){
     return HAL_BUSY;
  } else if((htim->State == HAL_TIM_STATE_READY)) {
    if(((uint32_t)pData == 0U ) && (Length > 0U)){
      return HAL_ERROR;
    } else {
      htim->State = HAL_TIM_STATE_BUSY;
    }
  }
  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* DMA Error meldung Rückgabe */
    /*  htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;

      /* Aktivieren DMA Kanal */
    /*  HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)pData, (uint32_t)&htim->Instance->CCR1, Length);

      /* Aktivieren von Timer Erfassen/vergleichen 1 DMA */
   /*   __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);
    }
   /* break;

    case TIM_CHANNEL_2:
    {
      /* DMA Error meldung Rückgabe */
    /*  htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;

      /* Aktivieren DMA Kanal */
    /*  HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)pData, (uint32_t)&htim->Instance->CCR2, Length);

      /* Aktivieren von Timer Erfassen/vergleichen 2 DMA */
   /*   __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);
    }
    break;

    default:
    break;
  }
  /* Aktivieren den Captur Kanals */
  /*TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

  if(IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET)
  {
    /* Aktivieren Main Ausgang */
 /*   __HAL_TIM_MOE_ENABLE(htim);
  }

  /* Aktivieren der Peripherie */
 /* __HAL_TIM_ENABLE(htim);

  /* rückgabe Funktionsausgabe */
    /* return HAL_OK;
}


void FU_Test_Init(void){
    HAL_Delay(500);
    for (int i = 0; i < 5; i++) {
      HAL_Delay(500);
      GPIOA->BSRR = 0b0000000000100000;
      HAL_Delay(1000);
      GPIOA->BSRR = 0b0000000000100000 << 16;
    }
}


/*void UART_Greeting(void){
    char *greet1="Nucleo F446RE 1 Phasen FU V0.1\n\r";
    char *greet2="------------------------\n\r\n\r";
    HAL_UART_Transmit(&huart2,(uint8_t *)greet1, strlen(greet1), 0xFFFF);
    HAL_UART_Transmit(&huart2,(uint8_t *)greet2, strlen(greet2), 0xFFFF);
}*/


    /*void Error_Mes(void){
    GPIOA->BSRR = 0b0000000000100000;
    while (1);
}
*/
