#ifndef _TIMER_OLD_H
#define _TIMER_OLD_H

#include "Method.h"


#define OPEN 1
#define CLOSE 2
#define REST 0

extern bool DSave;
extern bool SaveNow;
extern word screen_cont;

/*																			 
** ===================================================================
**     Method      :  HabilitarAccionPagApagado 
** ===================================================================
*/
void HabilitarAccionPagApagado(void);

 
void TI40ms_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  TI40ms_OnInterrupt (module Events)
**
**     From bean   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the bean is enabled - "Enable" and the events are
**         enabled - "EnableEvent").
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI1ms_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  TI1ms_OnInterrupt (module Events)
**
**     From bean   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the bean is enabled - "Enable" and the events are
**         enabled - "EnableEvent").
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

#endif