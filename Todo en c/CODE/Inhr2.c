/** ###################################################################
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : Inhr2.C
**     Project   : Project_2
**     Processor : MC9S12GC32CFU16
**     Beantype  : BitIO
**     Version   : Bean 02.068, Driver 03.09, CPU db: 2.87.392
**     Compiler  : CodeWarrior HC12 C Compiler
**     Date/Time : 5/4/2009, 5:32 PM
**     Abstract  :
**         This bean "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Note: This bean is not set in the "Safe mode". See help.
**         Methods of this bean are mostly implemented as a macros
**         (if supported by target language and compiler).
**     Settings  :
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       3             |  PP1_PWM1_KWP1(antes)
**                       52            |  PAD01_AN01
**             ----------------------------------------------------
**
**         Port name                   : P
**
**         Bit number (in port)        : 1
**         Bit mask of the port        : 2
**
**         Initial direction           : Input (direction can be changed)
**         Safe mode                   : no
**         Initial output value        : 1
**         Initial pull option         : up
**
**         Port data register          : PTP       [600]
**         Port control register       : DDRP      [602]
**
**         Optimization for            : speed
**     Contents  :
**         SetDir - void Inhr2_SetDir(bool Dir);
**         GetVal - bool Inhr2_GetVal(void);
**         PutVal - void Inhr2_PutVal(bool Val);
**         ClrVal - void Inhr2_ClrVal(void);
**         SetVal - void Inhr2_SetVal(void);
**         NegVal - void Inhr2_NegVal(void);
**
**     (c) Copyright UNIS, spol. s r.o. 1997-2007
**     UNIS, spol. s r.o.
**     Jundrovska 33
**     624 00 Brno
**     Czech Republic
**     http      : www.processorexpert.com
**     mail      : info@processorexpert.com
** ###################################################################*/

/* MODULE Inhr2. */

#include "Inhr2.h"
  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"

#pragma DATA_SEG Inhr2_DATA
#pragma CODE_SEG Inhr2_CODE
#pragma CONST_SEG Inhr2_CONST          /* Constant section for this module */
/*
** ===================================================================
**     Method      :  Inhr2_GetVal (bean BitIO)
**
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pin and returns it
**           b) direction = Output : returns the last written value
**     Parameters  : None
**     Returns     :
**         ---             - Input value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)

** ===================================================================
*/
/*
bool Inhr2_GetVal(void)

**  This method is implemented as a macro. See Inhr2.h file.  **
*/

/*
** ===================================================================
**     Method      :  Inhr2_PutVal (bean BitIO)
**
**     Description :
**         This method writes the new output value.
**         Note: If direction is set to input the method code may not
**               work properly.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Val             - Output value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)
**     Returns     : Nothing
** ===================================================================
*/
#ifdef opcion_1
void Inhr2_PutVal(bool Val)
{
  if (Val) {
    setReg8Bits(PTP, 2);               /* PTP1=1 */
 } else { /* !Val */
    clrReg8Bits(PTP, 2);               /* PTP1=0 */
  } /* !Val */
}

#else

  void Inhr2_PutVal(bool Val)
{
  if (Val) {
    setReg8Bits(PTAD, 2);              /* PTAD1=1 */
  } else { /* !Val */
    clrReg8Bits(PTAD, 2);              /* PTAD1=0 */
  } /* !Val */
}

#endif
/*
** ===================================================================
**     Method      :  Inhr2_ClrVal (bean BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**         Note: If direction is set to input the method code may not
**               work properly.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Inhr2_ClrVal(void)

**  This method is implemented as a macro. See Inhr2.h file.  **
*/

/*
** ===================================================================
**     Method      :  Inhr2_SetVal (bean BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**         Note: If direction is set to input the method code may not
**               work properly.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Inhr2_SetVal(void)

**  This method is implemented as a macro. See Inhr2.h file.  **
*/

/*
** ===================================================================
**     Method      :  Inhr2_NegVal (bean BitIO)
**
**     Description :
**         This method negates (inverts) the output value.
**         Note: If direction is set to input the method code may not
**               work properly.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void Inhr2_NegVal(void)

**  This method is implemented as a macro. See Inhr2.h file.  **
*/

/*
** ===================================================================
**     Method      :  Inhr2_SetDir (bean BitIO)
**
**     Description :
**         This method sets direction of the bean.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Dir        - Direction to set (FALSE or TRUE)
**                      FALSE = Input, TRUE = Output
**     Returns     : Nothing
** ===================================================================
*/

#ifdef opcion_1
void Inhr2_SetDir(bool Dir)
{
  if (Dir) {
    setReg8Bits(DDRP, 2);              /* DDRP1=1 */
  } else { /* !Dir */
    clrReg8Bits(DDRP, 2);              /* DDRP1=0 */
  } /* !Dir */
}

#else

  void Inhr2_SetDir(bool Dir)
{
  if (Dir) {
    setReg8Bits(DDRAD, 2);             /* DDRAD1=1 */
  } else { /* !Dir */
    clrReg8Bits(DDRAD, 2);             /* DDRAD1=0 */
  } /* !Dir */
}

#endif

/* END Inhr2. */
/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 2.98 [03.98]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/
