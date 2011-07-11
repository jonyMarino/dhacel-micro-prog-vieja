#ifndef ALARMAS_H
#define ALARMAS_H

#include "Mydefines.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"

struct ArgAlarma{
  byte NroControl;
  byte NroAlarma;
  byte salida;
};

extern bool flagAlarmaConet; 
extern int duty_alar_ch[CANTIDAD_CANALES];

#define isAlarmaConet  flagAlarmaConet

void calculosDeAlarma(struct ArgAlarma* arg);

#endif