#ifndef WECHSELRICHTER_H_INCLUDED
#define WECHSELRICHTER_H_INCLUDED

#include "main.h"

#define size 511

double Puls;
double Omega;
uint16_t   Array1[511];
uint16_t   Array2[511];
double Winkel;
double Winkelzeit;


uint16_t *getPWM_Array1(double);
uint16_t *getPWM_Array2(double);
void NEW_Pres(double);

#endif

