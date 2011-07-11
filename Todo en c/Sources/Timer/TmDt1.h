#ifndef __TmDt1
#define __TmDt1

/* MODULE TmDt1. */
#include <limits.h>
#include "FechaTiempo.h"
#include "PE_Types.h"

#pragma CODE_SEG TmDt1_CODE                     



struct TmDt1{  
  byte CntDay;                    /* Day counter */
  byte CntMonth;                  /* Month counter */
  word CntYear;                   /* Year Counter */
  dword TotalHthL;                
};


void TmDt1_Inc(void * self,dword);                     

void TmDt1_Init(void * self,word Year,byte Month,byte Day,byte Hour,byte Min,byte secs);

#pragma CODE_SEG DEFAULT                                   

/* END TmDt1. */

#endif /* ifndef __TmDt1 */
/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 2.95 [03.62]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/
