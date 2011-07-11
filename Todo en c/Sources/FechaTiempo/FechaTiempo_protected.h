#ifndef _FECHA_TIEMPO_PROTECTED_H
#define _FECHA_TIEMPO_PROTECTED_H

#include "FechaTiempo.h"
#include "PE_Types.h"

#pragma CODE_SEG FechaTiempo_CODE

byte super_setTiempoValidado(const void*_class,void * self,byte hour,byte min,byte secs);
byte super_setFechaValidada(const void*_class,void * self,word year,byte month,byte day);
void super_getTime(const void*_class,void * self,TIMEREC *time);
void super_getDate(const void*_class,void * self,DATEREC *date);

#pragma CODE_SEG DEFAULT

#endif