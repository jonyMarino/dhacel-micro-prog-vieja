#ifndef _METHOD_TIMER_H
#define _METHOD_TIMER_H

#include "stdtypes.h"
#include "Timer.h"
#include "Object.h"
#include "BaseTimers_1ms_40ms.h"

#pragma DATA_SEG MethodTimer_DATA                                            
#pragma CODE_SEG MethodTimer_CODE 
#pragma CONST_SEG DEFAULT

/*  Definicion del Timer*/

struct MethodTimer{
  struct Timer _base;
  argIncs argBase;
};

struct MethodTimer* MTimer_Init(ulong tiempo,void (*pf)(void*),void * Obj);

void executeMethodsTimer(unsigned char NroList);

struct MethodTimer* getUltimoMTimer();

void MTimer_setTime(struct MethodTimer*t, ulong _tiempo);

ulong MTimer_getTime(struct MethodTimer* t);

void MTimer_Stop(struct MethodTimer*t);

uchar MTimer_isfinish(struct MethodTimer*t);

void MTimer_Restart(struct MethodTimer*t);

bool isStartMTimer(struct MethodTimer*t);

#define _MethodTimer_getFunction(_timer) ((struct MethodTimer*)_timer)->pf

#define _MethodTimer_getObj(_timer) ((struct MethodTimer*)_timer)->Obj


#pragma DATA_SEG DEFAULT                                            
#pragma CODE_SEG DEFAULT 

#endif