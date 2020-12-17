#include "Wechselrichter.h"
#include "math.h"
#include "main.h"
#include "tim.h"


/*
In der Funktion getPWM_Array1 wird der übergebene Wert (frequenz) in 256  Werte aufegteilt und in ein Array geschrieben, welches zurück gegeben wird.
*/



uint16_t *getPWM_Array1(double Sollwert)
{
    Omega = 2.0 * M_PI * Sollwert;											// Omega wird errechnet durch: 2*Pi*f

    for (int Wert = 0; Wert <= 255; Wert++)
    {
    	Winkel = (180.0/255.0)*Wert;										// Der Winkel des Sinus wird errechnet
    	Winkelzeit = ((1/Sollwert)/360.0) * Winkel;							// Die Zeit, die bei dem bestimmten Winkel abgelaufen ist wird berechnet

        Puls = (sin(Omega * Winkelzeit))*(9.98*Sollwert);					// der Puls, welcher nachher in den DMA geschrieben wird, wird berechnet
        Array1[Wert] = (uint16_t)Puls;										// der ersten Hälfte des Array werden die Werte hinzugefügt

    }
    for (int i=255;i<=size;i++)												// Die zweite Hälfte des Arrays wird mit 0 beschrieben
    {
    	Array1[i]=0;
    }

    return Array1;															// das Array wird zurück gegeben
}

/*
In der Funktion getPWM_Array2 wird der übergebene Wert (frequenz) in 256  Werte aufegteilt und in ein Array geschrieben, welches zurück gegeben wird.
*/


uint16_t *getPWM_Array2(double Sollwert)
{
    Omega = 2.0 * M_PI * Sollwert;											// Omega wird errechnet durch: 2*Pi*f

    for (int Wert = 0; Wert <= 255; Wert++)
    {
    	Winkel = (180.0/255.0)*Wert;										// Der Winkel des Sinus wird errechnet
    	Winkelzeit = ((1/Sollwert)/360.0) * Winkel;							// Die Zeit, die bei dem bestimmten Winkel abgelaufen ist wird berechnet

        Puls = (sin(Omega * Winkelzeit))*(9.98*Sollwert);					// der Puls, welcher nachher in den DMA geschrieben wird, wird berechnet
        Array2[Wert+256] = (uint16_t)Puls;									// der zweiten Hälfte des Array werden die Werte hinzugefügt

    }
    for (int i=0;i<=255;i++)												// Die erste Hälfte des Arrays wird mit 0 beschrieben
    {
    	Array2[i]=0;
    }

    return Array2;															// das Array wird zurück gegeben
}


/*
In der Funktion NEW_Pres wird der übergebene Wert als Prescaler der Timer generiret
*/

void NEW_Pres(double Sollwert)
{	uint8_t NEW_PS=0;
	NEW_PS= 63.0-Sollwert;													// NEW_PS wird Errechnet
	__HAL_TIM_SET_PRESCALER(&htim1, NEW_PS);								// dem Timer 1 wird der neue Prescaler zugewiesen
	__HAL_TIM_SET_PRESCALER(&htim8, NEW_PS);								// dem Timer 8 wird der neue Prescaler zugewiesen

}

