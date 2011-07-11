#ifndef _BASE_TIMERS_H
#define _BASE_TIMERS_H

#include "BaseTimers_1ms_40ms.h"
#include "LinkedList.h"
#include "ConfBaseTimer.h"

unsigned long BaseTimers_getCuenta(unsigned char NroTime); 

bool BaseTimers_moveOut(void * timer,unsigned int NroList);

void BaseTimers_actualizarTimers(unsigned int NroList,unsigned char NroTime);

void BaseTimers_incrementar(unsigned int ms,unsigned char NroTime);

unsigned char getUltimoNroTimer();

void setNroTimer(unsigned char val);    


#endif
