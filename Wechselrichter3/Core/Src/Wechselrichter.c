#include "Wechselrichter.h"
#include "math.h"
#include "main.h"

float tiefpass(float Uk, float Ykminus1)
{
    float T = 1;                // T = Zeitkonstante
    float Ta = 0.01;             // Ta= Abtastzeit
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
    	Winkel = (180.0/256.0)*Wert;
    	Winkelzeit = ((1/Sollwert)/360.0) * Winkel;

        Puls = (sin(((Omega * Winkelzeit + Winkel + M_PI / 2.0) /180.0) * M_PI)) * 7058.8235;
        Array[Wert] = (int)Puls;

        printf(" Wert=  %3i  Winkel=  %1.10f   Winkelzeit=  %1.10f   Puls=   %4.4f  \t \n",Wert, Winkel, Winkelzeit, Puls);
    }

    return Array;
}
