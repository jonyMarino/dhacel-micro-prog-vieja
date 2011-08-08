/** ###################################################################
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : TO2.H
**     Project   : _10_03
**     Processor : MC9S12GC32CFU16
**     Beantype  : TimerOut
**     Version   : Bean 02.095, Driver 01.08, CPU db: 2.87.320
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 14/07/2006, 14:11
**     Abstract  :
**         This bean implements a periodic signal generator 1:1 (Flip-Flop Output).
**         The signal alternates the "0" and "1" output values in exactly
**         regular intervals; that is why the time length of the "0" part
**         of one pulse is equal to the "1" part of one pulse.
**     Settings  :
**         Used output pin             : 
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       11            |  PT4_PWM4_IOC4
**             ----------------------------------------------------
**
**         Timer name                  : TIM (16-bit)
**         Compare name                : TC4
**           Counter shared            : Yes
**
**         High speed mode
**             Prescaler               : divide-by-4
**             Clock                   : 6250000 Hz
**           Pulse width
**             Xtal ticks              : 32
**             microseconds            : 4
**             seconds (real)          : 0.0000040
**             Hz                      : 250000
**             kHz                     : 250
**
**         Runtime setting             : pulse width interval (continual setting)
**             ticks                   : 32 to 80000 ticks
**             microseconds            : 4 to 10000 microseconds
**             milliseconds            : 1 to 10 milliseconds
**             seconds (real)          : 0.0000040 to 0.0100000 seconds
**             Hz                      : 100 to 250000 Hz
**             kHz                     : 1 to 250 kHz
**
**         Initialization:
**              Output level           : low
**              Timer                  : Enabled
**              Events                 : Enabled
**
**         Timer registers
**              Counter                : TCNT      [68]
**              Mode                   : TIOS      [64]
**              Run                    : TSCR1     [70]
**              Prescaler              : TSCR2     [77]
**
**         Compare registers
**              Compare                : TC4       [88]
**
**         Flip-flop registers
**              Mode                   : TCTL1     [72]
**     Contents  :
**         Enable      - byte TO2_Enable(void);
**         Disable     - byte TO2_Disable(void);
**         SetPeriodUS - byte TO2_SetPeriodUS(word Time);
**         SetValue    - byte TO2_SetValue(void);
**         ClrValue    - byte TO2_ClrValue(void);
**
**     (c) Copyright UNIS, spol. s r.o. 1997-2004
**     UNIS, spol. s r.o.
**     Jundrovska 33
**     624 00 Brno
**     Czech Republic
**     http      : www.processorexpert.com
**     mail      : info@processorexpert.com
** ###################################################################*/

#ifndef __TO2
#define __TO2

/* MODULE TO2. */

#include "Cpu.h"

#pragma CODE_SEG TO2_CODE                     
#pragma DATA_SEG TO2_DATA                                            

#define TO2_SPUSMin        4           /* Lower bound of interval for method SetPeriodUS */
#define TO2_SPUSMax        10000       /* Upper bound of interval for method SetPeriodUS */


byte TO2_Enable(void);
/*
** ===================================================================
**     Method      :  TO2_Enable (bean TimerOut)
**
**     Description :
**         Enables the bean - it starts the signal generation. Events
**         may be generated ("DisableEvent"/"EnableEvent").
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

byte TO2_Disable(void);
/*
** ===================================================================
**     Method      :  TO2_Disable (bean TimerOut)
**
**     Description :
**         Disables the timer - it stops signal generation and events
**         calling. When the timer is disabled, it is possible to
**         call methods "ClrValue" and "SetValue".
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

byte TO2_SetPeriodUS(word Time);
/*
** ===================================================================
**     Method      :  TO2_SetPeriodUS (bean TimerOut)
**
**     Description :
**         This method sets the new period of the output signal. The
**         period is expressed in microseconds as a 16-bit unsigned
**         integer number.
**         This method is available only if the runtime setting type
**         'from interval' is selected in the Timing dialog box in
**         the Runtime setting area.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Time            - Period to set [in microseconds]
**                      (4 to 10000 microseconds)
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_MATH - Overflow during evaluation
**                           ERR_RANGE - Parameter out of range
** ===================================================================
*/

byte TO2_SetValue(void);
/*
** ===================================================================
**     Method      :  TO2_SetValue (bean TimerOut)
**
**     Description :
**         This method sets (sets to "1" = "High") timer flip-flop
**         output signal level. It allow user to directly set the
**         output pin value (=flip-flop state), and can set the
**         signal polarity. This method works only when the timer is
**         disabled (Disable) otherwise it returns the error code.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_ENABLED - Bean is enabled. Bean must
**                           be disabled (see <a
**                           href="TimerOutMethods.html">Disable
**                           method</a> method)
** ===================================================================
*/

byte TO2_ClrValue(void);
/*
** ===================================================================
**     Method      :  TO2_ClrValue (bean TimerOut)
**
**     Description :
**         This method clears (sets to "0" = "Low") timer flip-flop
**         output signal level. It allow user to directly set the
**         output pin value (=flip-flop state), and can set the
**         signal polarity. This method works only when the timer is
**         disabled (Disable) otherwise it returns the error code.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_ENABLED - Bean is enabled. Bean must
**                           be disabled (see <a
**                           href="TimerOutMethods.html">Disable</a>
**                           method)
** ===================================================================
*/

#pragma CODE_SEG __NEAR_SEG NON_BANKED     
__interrupt void TO2_Interrupt(void);
#pragma CODE_SEG TO2_CODE                     
/*
** ===================================================================
**     Method      :  TO2_Interrupt (bean TimerOut)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/

void TO2_Init(void);
/*
** ===================================================================
**     Method      :  TO2_Init (bean TimerOut)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/

#pragma DATA_SEG DEFAULT                                  
#pragma CODE_SEG DEFAULT                                   

/* END TO2. */

#endif /* ifndef __TO2 */
/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 2.95 [03.62]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/