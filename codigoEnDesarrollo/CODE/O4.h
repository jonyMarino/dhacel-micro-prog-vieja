/** ###################################################################
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : O4.H
**     Project   : cntr_26_07
**     Processor : MC9S12GC32CFU16
**     Beantype  : BitIO
**     Version   : Bean 02.068, Driver 03.09, CPU db: 2.87.392
**     Compiler  : CodeWarrior HC12 C Compiler
**     Date/Time : 20/08/2009, 10:06
**     Abstract  :
**         This bean "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Note: This bean is set to work in Output direction only.
**         Methods of this bean are mostly implemented as a macros
**         (if supported by target language and compiler).
**     Settings  :
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       1             |  PP3_PWM3_KWP3
**             ----------------------------------------------------
**
**         Port name                   : P
**
**         Bit number (in port)        : 3
**         Bit mask of the port        : 8
**
**         Initial direction           : Output (direction cannot be changed)
**         Initial output value        : 0
**         Initial pull option         : off
**
**         Port data register          : PTP       [600]
**         Port control register       : DDRP      [602]
**
**         Optimization for            : speed
**     Contents  :
**         PutVal - void O4_PutVal(bool Val);
**
**     (c) Copyright UNIS, spol. s r.o. 1997-2007
**     UNIS, spol. s r.o.
**     Jundrovska 33
**     624 00 Brno
**     Czech Republic
**     http      : www.processorexpert.com
**     mail      : info@processorexpert.com
** ###################################################################*/

#ifndef O4_H_
#define O4_H_

/* MODULE O4. */

  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"

#pragma CODE_SEG O4_CODE
/*
** ===================================================================
**     Method      :  O4_PutVal (bean BitIO)
**
**     Description :
**         This method writes the new output value.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Val             - Output value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)
**     Returns     : Nothing
** ===================================================================
*/
void O4_PutVal(bool Val);

#pragma CODE_SEG DEFAULT

/* END O4. */
#endif /* #ifndef __O4_H_ */
/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 2.98 [03.98]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/
