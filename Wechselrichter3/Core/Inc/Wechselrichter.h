#ifndef WECHSELRICHTER_H_INCLUDED
#define WECHSELRICHTER_H_INCLUDED

#include "main.h"

#define size 254
float YKminus1; // YKminus1 wird als float mit dem Wert0 deklariert
float YK;
double Puls;
double Omega;
uint8_t   Array[256];
double Winkel;
double Winkelzeit;

float tiefpass(float, float);
int *getPWM_Array(double);

#endif

/*#ifndef WECHSELRICHTER_H_INCLUDED
#define WECHSELRICHTER_H_INCLUDED

#include "stm32f4xx_hal.h"
#include "main.h"
#include "math.h"
#include "string.h"

#define size 254
#define T 1                	// T = Zeitkonstante
#define Ta 0.1          	// Ta= Abtastzeit
#define b Ta / (T + Ta)   // b wird errechnet
#define a 1 - b           // a wird errechnet

int Winkelzeit;
double Puls;
double Omega;
int Array[254];

float tiefpass(float Uk, float Ykminus1);
int *getPWM_Array(double Sollwert);
HAL_StatusTypeDef StartPWM_DMA(TIM_HandleTypeDef *htim,
                               uint32_t Channel, uint32_t *pData,
                               uint16_t Length);
//void UART_Greeting(void);
void FU_Test_Init(void);
void Error_Mes(void);

#endif
*/

