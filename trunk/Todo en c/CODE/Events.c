/** ###################################################################
**     Filename  : Events.C
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
/* MODULE Events */


#include "Cpu.h"
#include "Events.h"
#include "cnfbox.h"
#include "ADC.h"
#include "Programador.h"
#include "Comunicacion.h"
#include "display.h"

AS1_TComData datoSerie;

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
void TI2_OnInterrupt(void)
{
  /* Write your code here ... */
}


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
void PWM_OnToggle(bool Value,byte salida)
{
  /* Write your code here ... */
  switch(salida){
  case 0:
    O2_PutVal(Value);
    break;
  case 1:
    O4_PutVal(Value);
    break;
  case 2:
    O6_PutVal(Value);
    break;
  case 3:
    O7_PutVal(Value);    
  }
  led[salida]=Value;
}


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
void TO2_OnFalling(void)
{
  /* Write your code here ... */
}


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
void PWM_OnRising(void)
{
  /* Write your code here ... */
}


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
void TI1_OnInterrupt(void)
{
  /* Write your code here ... */
}


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
void AS1_OnRxChar(void)
{
  /* Write your code here ... */
  
  //if(AS1_RecvChar(&dat)==ERR_OK)
    AS1_RecvChar(&datoSerie);
    #if !defined (_PRINTER) && !defined(ADQ)
    ModBus_Recive(datoSerie);
    #endif
}

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
void AS1_OnTxComplete(void)
{
  /* Write your code here ... */
  #if !defined (_PRINTER) && !defined(ADQ)
  ModBus_Send();
  #endif
}

/* END Events */

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 2.95 [03.62]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/
