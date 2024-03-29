/** ###################################################################
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : Display1.H
**     Project   : cntr_26_07
**     Processor : MC9S12GC32CFU16
**     Beantype  : ByteIO
**     Version   : Bean 02.053, Driver 03.08, CPU db: 2.87.392
**     Compiler  : CodeWarrior HC12 C Compiler
**     Date/Time : 20/08/2009, 10:06
**     Abstract  :
**         This bean "ByteIO" implements an one-byte input/output.
**         It uses one 8-bit port.
**         Note: This bean is set to work in Output direction only.
**         Methods of this bean are mostly implemented as a macros 
**         (if supported by target langauage and compiler).
**     Settings  :
**         Port name                   : A
**
**         Initial direction           : Output (direction cannot be changed)
**         Initial output value        : 0 = 000H
**         Initial pull option         : off
**
**         8-bit data register         : PORTA     [0]
**         8-bit control register      : DDRA      [2]
**
**             ----------------------------------------------------
**                   Bit     |   Pin   |   Name
**             ----------------------------------------------------
**                    0      |    41   |   PA0_ADDR8_DATA8
**                    1      |    42   |   PA1_ADDR9_DATA9
**                    2      |    43   |   PA2_ADDR10_DATA10
**                    3      |    44   |   PA3_ADDR11_DATA11
**                    4      |    45   |   PA4_ADDR12_DATA12
**                    5      |    46   |   PA5_ADDR13_DATA13
**                    6      |    47   |   PA6_ADDR14_DATA14
**                    7      |    48   |   PA7_ADDR15_DATA15
**             ----------------------------------------------------
**     Contents  :
**         PutVal - void Display1_PutVal(byte Val);
**
**     (c) Copyright UNIS, spol. s r.o. 1997-2007
**     UNIS, spol. s r.o.
**     Jundrovska 33
**     624 00 Brno
**     Czech Republic
**     http      : www.processorexpert.com
**     mail      : info@processorexpert.com
** ###################################################################*/

#ifndef Display1_H_
#define Display1_H_

/* MODULE Display1. */

  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"

#pragma CODE_SEG Display1_CODE
/*
** ===================================================================
**     Method      :  Display1_PutVal (bean ByteIO)
**
**     Description :
**         This method writes the new output value.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Val        - Output value (0 to 255)
**     Returns     : Nothing
** ===================================================================
*/
#define Display1_PutVal(Val) ( \
    (void)setReg8(PORTA, Val)          /* BIT0-BIT7=Val[bits 0-7] */ \
  )

#pragma CODE_SEG DEFAULT

/* END Display1. */
#endif /* #ifndef __Display1_H_ */
/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 2.98 [03.98]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/
