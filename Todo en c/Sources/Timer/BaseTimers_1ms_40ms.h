#ifndef _Base_Timers_1ms_40ms_h
#define _Base_Timers_1ms_40ms_h

#include "BaseTimers.h"
#include "Method.h"

typedef struct{
  unsigned int NroList;
  unsigned char NroTime;
}argIncs;
      
void InitBaseTimers_1ms_40ms();

void BaseTimers_1ms_40ms_lockInc();

void BaseTimers_1ms_40ms_unlockInc();
   
void BaseTimers_1ms_40ms_inc1(argIncs* argInc1ms); 

void BaseTimers_1ms_40ms_inc40(argIncs* argImc40ms);
   
void BaseTimers_1ms_40ms_setArguMethod1ms(argIncs* argInc1ms);

void BaseTimers_1ms_40ms_setArguMethod40ms(argIncs* argInc40ms);

#endif