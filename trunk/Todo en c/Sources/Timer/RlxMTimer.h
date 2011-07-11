#ifndef _RELAXED_M_TIMER_H
#define _RELAXED_M_TIMER_H

#include "MethodTimer.h"

#pragma DATA_SEG RlxMTimer_DATA                                            
#pragma CODE_SEG RlxMTimer_CODE 
#pragma CONST_SEG DEFAULT

struct RlxMTimer{
  struct MethodTimer * _base;
  struct Method* handlerTimerLoop;
  void (*onTime)(void*); 
  void * Obj;
  uchar execute;
};

struct RlxMTimer* RlxMTimer_Init(ulong tiempo,void (*pf)(void*),void * Obj );
void RlxMTimer_Delete(struct RlxMTimer * _self);
void RlxMTimer_OnTime(void * _self);
void RlxMTimer_Handler(void * a);
struct RlxMTimer* getUltimoRlxMTimer();
void RlxTimer_setTime(struct RlxMTimer*t, ulong _tiempo);
ulong RlxTimer_getTime(struct RlxMTimer* t);
void RlxTimer_Stop(struct RlxMTimer*t);
uchar RlxTimer_isfinish(struct RlxMTimer*t);
void RlxTimer_Restart(struct RlxMTimer*t);

#pragma DATA_SEG DEFAULT                                            
#pragma CODE_SEG DEFAULT 

#endif