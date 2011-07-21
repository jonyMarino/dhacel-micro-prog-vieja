#ifndef MEDICION_H
#define MEDICION_H

#include "Mydefines.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"

extern int ValoresCalculados[CANTIDAD_CANALES];
extern byte EstatusResult[CANTIDAD_CANALES];

#ifdef RUVAC
extern int valLinealizadoMV[CANTIDAD_CANALES];
#endif

#ifdef SENS_HUMEDAD
extern unsigned int Humedad;
#endif

void calculaValorFinal(byte NroChanel);
int getValCalculado(char nroCanal);
byte getEstatusResult(char nroCanal);
void setValCalculado(int val,char nroCanal);
void setEstatusResult(byte val,char nroCanal);

#endif