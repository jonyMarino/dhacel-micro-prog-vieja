#ifndef _TMDT1_PROTECTED_H
#define _TMDT1_PROTECTED_H

#include "FechaTiempo_protected.h"
#include "TmDt1.h"

void TmDt1_GetTime(void * self,TIMEREC *Time);
byte TmDt1_setTiempoValidado(void * _self,byte Hour,byte Min,byte secs);
void TmDt1_GetDate(void * self,DATEREC *Date);
byte TmDt1_setFechaValidada(void * self,word Year,byte Month,byte Day);


#endif