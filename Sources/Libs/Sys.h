#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "BTFechaPersistente.h"

extern struct BTFechaPersistente baseTiempo;

#define Sys_getBaseTiempo() &baseTiempo

void System_init(void);
void Sys_getDate(DATEREC * date);
void Sys_getTime(TIMEREC * time);


#endif