/** ###################################################################
**     Filename  : Events.H
**     Project   : _7_12
**     Processor : MC9S12GC32CFU16
**     Beantype  : Events
**     Version   : Driver 01.03
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 14/12/2005, 16:27
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         Filt1_OnError - void Filt1_OnError(void);
**
**     (c) Copyright UNIS, spol. s r.o. 1997-2004
**     UNIS, spol. s r.o.
**     Jundrovska 33
**     624 00 Brno
**     Czech Republic
**     http      : www.processorexpert.com
**     mail      : info@processorexpert.com
** ###################################################################*/

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Display1.h"
#include "bits5ULN.h"
#include "bits2ULN.h"
#include "trx.h"
#include "PTSL.h"
#include "PWSN.h"
#include "PUL.h"
#include "WDog1.h"
#include "AS1.h"
#include "ADC1.h"
#include "FLASH1.h"
#include "PWM.h"
#include "TI1.h"
#include "O7.h"
#include "In1.h"
#include "In2.h"
#include "PWM4.h"
#include "PWM5.h"
#include "PWM6.h"
#include "O2.h"
#include "O4.h"
#include "O6.h"
#include "IFsh10.h"


void TI2_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     From bean   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the bean is enabled - "Enable" and the events are
**         enabled - "EnableEvent").
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void PWM_OnToggle(bool Value,byte salida);
/*
** ===================================================================
**     Event       :  PWM_OnToggle (module Events)
**
**     From bean   :  PWM [TimerOut]
**     Description :
**         This event is called at falling edge of the output signal.
**         (only when the bean is enabled - "Enable" and the events
**         are enabled - "EnableEvent").
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void TO2_OnFalling(void);
/*
** ===================================================================
**     Event       :  TO2_OnFalling (module Events)
**
**     From bean   :  TO2 [TimerOut]
**     Description :
**         This event is called at falling edge of the output signal.
**         (only when the bean is enabled - "Enable" and the events
**         are enabled - "EnableEvent").
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/



void PWM_OnRising(void);
/*
** ===================================================================
**     Event       :  PWM_OnRising (module Events)
**
**     From bean   :  PWM [TimerOut]
**     Description :
**         This event is called at rising edge of the output signal.
**         (only when the bean is enabled - "Enable" and the events
**         are enabled - "EnableEvent").
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void TI1_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
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

void AS1_OnRxChar(void);
/*
** ===================================================================
**     Event       :  AS1_OnRxChar (module Events)
**
**     From bean   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called after a correct character is
**         received. 
**         DMA mode:
**         If DMA controller is available on the selected CPU and
**         the receiver is configured to use DMA controller then
**         this event is disabled. Only OnFullRxBuf method can be
**         used in DMA mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AS1_OnTxComplete(void);
/*
** ===================================================================
**     Event       :  AS1_OnTxComplete (module Events)
**
**     From bean   :  AS1 [AsynchroSerial]
**     Description :
**         This event indicates that the transmitter is finished
**         transmitting all data, preamble, and break characters and
**         is idle. It can be used to determine when it is safe to
**         switch a line driver (e.g. in RS-485 applications). This
**         event is available only if property <Transmitter > is
**         enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END Events */
#endif /* __Events_H*/

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 2.95 [03.62]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/
