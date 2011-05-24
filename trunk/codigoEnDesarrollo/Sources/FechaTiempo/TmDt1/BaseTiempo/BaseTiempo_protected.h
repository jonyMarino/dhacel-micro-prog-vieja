#ifndef _BASE_TIMEPO_PROTECTED_H
#define _BASE_TIMEPO_PROTECTED_H

#include "TmDt1_protected.h"

void BaseTiempo_defConstructor(void * self,va_list * args);
void BaseTiempo_constructor(void * _self,word Year,byte Month,byte Day,byte Hour,byte Min,byte secs,bool start);

void BaseTiempo_incUnSegundo(void * _self);
byte BaseTiempo_setTime(void * _self,byte horas,byte min,byte segs);

#endif