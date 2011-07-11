#ifndef _RELAXED_M_TIMER_H
#define _RELAXED_M_TIMER_H

#include "MethodTimer.h"

#pragma DATA_SEG RlxMTimer_DATA                                            
#pragma CODE_SEG RlxMTimer_CODE 
#pragma CONST_SEG DEFAULT

struct RlxMTimer{
  struct MethodTimer _base;
  uchar execute;
};

extern const struct TimerClass RlxMTimer;


#pragma DATA_SEG DEFAULT                                            
#pragma CODE_SEG DEFAULT 

#endif