


#ifndef WECHSELRICHTER_H_INCLUDED
#define WECHSELRICHTER_H_INCLUDED

#include "main.h"

#define size 254
float YKminus1; // YKminus1 wird als float mit dem Wert0 deklariert
float YK;
double Puls;
double Omega;
uint16_t   Array[256];
double Winkel;
double Winkelzeit;

float tiefpass(float, float);
int *getPWM_Array(double);

#endif
