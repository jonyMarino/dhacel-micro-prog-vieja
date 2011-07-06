
/* MODULE RTI. */

#include "Events.h"
#include "RTI.h"
#include "PWSN.h"
#include "Mydefines.h"
#include "ADC.h"
#include "BoxesStruct.h"
#include "Programador.h"
#pragma DATA_SEG RTI_DATA                                            
#pragma CODE_SEG RTI_CODE                     
#pragma CONST_SEG DEFAULT


/*
** ===================================================================
**     Method      :  Inicialización del timer
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED     

void RTI_Init(void){
  /* TSCR1: TEN=0,TSWAI=0,TSFRZ=0,TFFCA=0 */ 
  clrReg8Bits(TSCR1, 240); 
  /* OC7M: OC7M0=0 */
  clrReg8Bits(OC7M, 1);                 
  /* TIOS: IOS0=1 */
  setReg8Bits(TIOS, 1);                 
  /* TCTL2: OM1=0,OL1=0,OM0=0,OL0=0 */
  clrReg8Bits(TCTL2, 3);               
  /* TTOV: TOV0=0 */
  clrReg8Bits(TTOV, 1);                 
  /* TSCR2: TCRE=0,PR2=1,PR1=0,PR0=1 */
  clrSetReg8Bits(TSCR2, 10, 5);         
  /* TFLG1: C7F=1,C6F=1,C5F=1,C4F=1,C3F=1,C2F=1,C1F=1,C0F=1 */
  setReg8(TFLG1, 255);                  
  /* TIE: C0I=1 */
  setReg8Bits(TIE, 1);                  
              
  TC0 = 782;
   /* TSCR1: TEN=1 */
  setReg8Bits(TSCR1, 128);     
}

/*
** ===================================================================
**     Method      :  RTI_Interrupt (bean TimerInt)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/


ISR(RTI_Interrupt)
{


TC0 += TimerVal;                    /* Add value corresponding with periode */
TFLG1 = 1;                           /* Reset interrupt request flag */




}



#pragma CODE_SEG RTI_CODE                     



/* END RTI. */

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 2.95 [03.62]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/
