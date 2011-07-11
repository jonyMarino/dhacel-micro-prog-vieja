/** ###################################################################
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : CapPT2.C
**     Project   : cntr_26_07
**     Processor : MC9S12GC32CFU16
**     Beantype  : Capture
**     Version   : Bean 02.083, Driver 01.08, CPU db: 2.87.320
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 14/04/2008, 01:06 p.m.
**     Abstract  :
**         This bean "Capture" simply implements the capture function
**         of timer. The counter counts the same way as in free run mode. On
**         the selected edge of the input signal (on the input pin), the current
**         content of the counter register is written into the capture
**         register and the OnCapture event is called.
**     Settings  :
**             Timer capture encapsulation : Capture
**
**         Timer
**             Timer                   : TIM
**             Counter shared          : Yes
**
**         High speed mode
**             Prescaler               : divide-by-32
**           Maximal time for capture register
**             Xtal ticks              : 1048576
**             microseconds            : 131072
**             milliseconds            : 131
**             seconds (real)          : 0.1310720
**             Hz                      : 8
**           One tick of timer is
**             microseconds            : 2
**
**         Initialization:
**              Timer                  : Disabled
**              Events                 : Enabled
**
**         Timer registers
**              Capture                : TC2       [84]
**              Counter                : TCNT      [68]
**              Mode                   : TIOS      [64]
**              Run                    : TSCR1     [70]
**              Prescaler              : TSCR2     [77]
**
**         Used input pin              : 
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       7             |  PT2_PWM2_IOC2
**             ----------------------------------------------------
**
**         Port name                   : T
**         Bit number (in port)        : 2
**         Bit mask of the port        : 4
**
**         Signal edge/level           : rising
**         Pull option                 : off
**
**     Contents  :
**         Enable          - byte CapPT2_Enable(void);
**         Disable         - byte CapPT2_Disable(void);
**         Reset           - byte CapPT2_Reset(void);
**         GetCaptureValue - byte CapPT2_GetCaptureValue(word *Value);
**
**     (c) Copyright UNIS, spol. s r.o. 1997-2004
**     UNIS, spol. s r.o.
**     Jundrovska 33
**     624 00 Brno
**     Czech Republic
**     http      : www.processorexpert.com
**     mail      : info@processorexpert.com
** ###################################################################*/


/* MODULE CapPT2. */

#include "CapPT2.h"
#include "stdtypes.h"

#pragma DATA_SEG CapPT2_DATA                                            
#pragma CODE_SEG CapPT2_CODE                     

static bool EnUser;                    /* Enable/Disable device by user */
volatile word CapPT2_CntrState;        /* Content of counter */
dword CapPT2InCount=0;
word CapPT2OfCountWait=0; /* Over flow actualizado a cada momento, aun cuando no haya llegado un pulso*/
word CapPT2OfCount;  /*(Capture over flow count)Contador del numero de over flows del contador de tiempo*/
static bool init;

void CapPT2_Reset(void){
  CapPT2_CntrState = TCNT;
  CapPT2OfCount=0;
  CapPT2InCount=0; 
  CapPT2OfCountWait=0;
}

/*
** ===================================================================
**     Method      :  CapPT2_Enable (bean Capture)
**
**     Description :
**         Enables the bean - it starts the capture. Events may be
**         generated ("DisableEvent"/"EnableEvent").
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte CapPT2_Enable(void)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    EnUser = TRUE;                     /* If yes then set the flag "device enabled" */
  /* TFLG1: C7F=0,C6F=0,C5F=0,C4F=0,C3F=0,C2F=1,C1F=0,C0F=0 */
    TFLG1 |= 4;                         /* Reset interrupt request flag */
    TIE_C2I |= 1;                       /* Enable interrupt */
    TFLG2_TOF = 1;                     /* Reset overflow interrupt request flag */
    TSCR2_TOI = 1;                     /* Enable overflow interrupt */
  /* TCTL4: EDG2A=1 */
    TCTL4 |= 16;                       /* Enable capture function */
    init=TRUE;
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  CapPT2_Disable (bean Capture)
**
**     Description :
**         Disables the bean - it stops the capture. No events will
**         be generated.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte CapPT2_Disable(void)
{
  if (EnUser) {                        /* Is the device enabled by user? */
    EnUser = FALSE;                    /* If yes then set the flag "device disabled" */
    TIE_C2I = 0;                       /* Disable interrupt */
    TSCR2_TOI = 0;                     /* Disable overflow interrupt */
  /* TCTL4: EDG2B=0,EDG2A=0 */
    TCTL4 &= ~48;                      /* Disable capture function */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  CapPT2_Reset (bean Capture)
**
**     Description :
**         This method resets the counter register.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
/*
byte CapPT2_Reset(word *Value)

**  This method is implemented as a macro. See CapPT2.h file.  **
*/

/*
** ===================================================================
**     Method      :  CapPT2_GetCaptureValue (bean Capture)
**
**     Description :
**         This method gets the last value captured by enabled timer.
**         Note: one tick of timer is
**               2 us in high speed mode 
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Value           - A pointer to the content of the
**                           capture register
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
/*
byte CapPT2_GetCaptureValue(word *Value)

**  This method is implemented as a macro. See CapPT2.h file.  **
*/

/*
** ===================================================================
**     Method      :  Cap1_GetCapResult (bean Capture)
**
**     Description :
**         Este metodo obtiene el numero de pulsos que se obtuvieron a
**		 la entrada, y los microsegundos que transcurrieron entre el primero
**     y el ultimo pulso.
**     Parameters  :
**         NAME            - DESCRIPTION
**     Returns     :
**         ---             - Resultado del capturador
** ===================================================================
*/
void CapPT2_GetCapResult(TCapture * result){
  word time;
  word ofs;
  
  Cpu_DisableInt();  
  if(CapPT2InCount==0){
    Cpu_EnableInt();
    result->pulsos=0;
    result->microsegundos=0; 
    return;  
  }
  result->pulsos= CapPT2InCount;
  CapPT2_GetCaptureValue(&time);
  ofs=CapPT2OfCount;
  if(CapPT2_CntrState>TC2)			//el valor de la resta es negativo?
    ofs-=1;  
  
  //Actualizo las variables al ultimo pulso
  CapPT2_CntrState=TC2;
  CapPT2OfCountWait-= CapPT2OfCount;
  CapPT2InCount=0;
  CapPT2OfCount=0;
  //fin de actualizacion
  
  result->microsegundos =  (ulong)ofs * 131070 + (ulong)time * 2;
  
  Cpu_EnableInt();
  
  

}

/*
** ===================================================================
**     Method      :  CapPT2_Init (bean Capture)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
void CapPT2_Init(void)
{
  CapPT2_CntrState = 0;                /* Initial state of variable */
  /* TIOS: IOS2=0 */
  clrReg8Bits(TIOS, 4);
  /* PTT: PTT6=0,PTT5=0,PTT4=0,PTT1=0 */
  clrReg8Bits(PTT, 114);                
  /* DDRT: DDRT6=1,DDRT5=1,DDRT4=1,DDRT1=1 */
  setReg8Bits(DDRT, 114);               
   
  EnUser = FALSE;                      /* Disable device */
  
}

/*
** ===================================================================
**     Method      :  Interrupt (bean Capture)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
void CapPT2_OnCapture(void)
{
  /* Write your code here ... */
  
  Cpu_DisableInt();
  
  if(init){        
    init=FALSE;
    CapPT2_Reset();  
  }else {
    CapPT2InCount++;
    CapPT2OfCount=CapPT2OfCountWait;   
  }
  
  Cpu_EnableInt();
}

void CapPT2_OnOverflow(void)
{
  /* Write your code here ... */
  Cpu_DisableInt();
  CapPT2OfCountWait++;
  Cpu_EnableInt();

}

#pragma CODE_SEG __NEAR_SEG NON_BANKED     
ISR(CapPT2_Interrupt)
{
  /* TFLG1: C7F=0,C6F=0,C5F=0,C4F=0,C3F=0,C2F=1,C1F=0,C0F=0 */
  TFLG1 = 4;                           /* Reset interrupt request flag */
  CapPT2_OnCapture();                  /* Invoke user event */
}

#pragma CODE_SEG CapPT2_CODE                     
/*
** ===================================================================
**     Method      :  IntOverflow (bean Capture)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED     
ISR(CapPT2_IntOverflow)
{
  TFLG2_TOF = 1;                       /* Reset overflow interrupt request flag */
  CapPT2_OnOverflow();                 /* Invoke user event */
}

#pragma CODE_SEG CapPT2_CODE                     

/* END CapPT2. */

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 2.95.01 [03.62]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/
