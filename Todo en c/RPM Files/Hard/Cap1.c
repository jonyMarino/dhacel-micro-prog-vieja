/** ###################################################################
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : Cap1.C
**     Project   : TEST19_InputCapture
**     Processor : MC9S12GC32CFU16
**     Beantype  : Capture
**     Version   : Bean 02.083, Driver 01.08, CPU db: 2.87.320
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 11/04/2008, 01:33 p.m.
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
**             Prescaler               : divide-by-16
**           Maximal time for capture register
**             Xtal ticks              : 2097152
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
**              Capture                : TC7       [94]
**              Counter                : TCNT      [68]
**              Mode                   : TIOS      [64]
**              Run                    : TSCR1     [70]
**              Prescaler              : TSCR2     [77]
**
**         Used input pin              : 
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       14            |  PT7_IOC7
**             ----------------------------------------------------
**
**         Port name                   : T
**         Bit number (in port)        : 7
**         Bit mask of the port        : 128
**
**         Signal edge/level           : rising
**         Priority                    : 1
**         Pull option                 : off
**
**     Contents  :
**         Enable          - byte Cap1_Enable(void);
**         Disable         - byte Cap1_Disable(void);
**         Reset           - byte Cap1_ResetAll(void);
**         GetCaptureValue - byte Cap1_GetCaptureValue(word *Value);
**
**     (c) Copyright UNIS, spol. s r.o. 1997-2004
**     UNIS, spol. s r.o.
**     Jundrovska 33
**     624 00 Brno
**     Czech Republic
**     http      : www.processorexpert.com
**     mail      : info@processorexpert.com
** ###################################################################*/


/* MODULE Cap1. */

#include "Events.h"
#include "Cap1.h"

#pragma DATA_SEG Cap1_DATA                                            
#pragma CODE_SEG Cap1_CODE                     

//#define DEBUG

#ifdef  DEBUG
#include "display.h"
#include "Timer.h"

struct Timer time;
#endif


static bool EnUser;                    /* Enable/Disable device by user */
volatile word Cap1_CntrState;          /* Content of counter */
dword CapInCount=0;
word CapOfCountWait=0; /* Over flow actualizado a cada momento, aun cuando no haya llegado un pulso*/
word CapOfCount;  /*(Capture over flow count)Contador del numero de over flows del contador de tiempo*/
bool init;

void Cap1_Reset(void){
  Cap1_CntrState = TCNT;
  CapOfCount=0;
  CapInCount=0; 
  CapOfCountWait=0;
}

/*
** ===================================================================
**     Method      :  Cap1_GetCaptureValue (bean Capture)
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
#define Cap1_GetCaptureValue(Value) \
 (*(Value) = TC7 , *(Value) -= Cap1_CntrState , ERR_OK)



/*
** ===================================================================
**     Method      :  Cap1_Enable (bean Capture)
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
byte Cap1_Enable(void)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    EnUser = TRUE;                     /* If yes then set the flag "device enabled" */
  /* TFLG1: C7F=1,C6F=0,C5F=0,C4F=0,C3F=0,C2F=0,C1F=0,C0F=0 */
    TFLG1 = 128;                       /* Reset interrupt request flag */
    TIE_C7I = 1;                       /* Enable interrupt */
    TFLG2_TOF = 1;                     /* Reset overflow interrupt request flag */
    TSCR2_TOI = 1;                     /* Enable overflow interrupt */
  /* TCTL3: EDG7A=1 */
    TCTL3 |= 64;                       /* Enable capture function */ 
   // Cap1_Reset();
    init=TRUE;
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  Cap1_Disable (bean Capture)
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
byte Cap1_Disable(void)
{
  if (EnUser) {                        /* Is the device enabled by user? */
    EnUser = FALSE;                    /* If yes then set the flag "device disabled" */
    TIE_C7I = 0;                       /* Disable interrupt */
    TSCR2_TOI = 0;                     /* Disable overflow interrupt */
  /* TCTL3: EDG7B=0,EDG7A=0 */
    TCTL3 &= ~192;                     /* Disable capture function */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  Cap1_ResetAll (bean Capture)
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
byte Cap1_ResetAll(word *Value)

**  This method is implemented as a macro. See Cap1.h file.  **
*/

/*
** ===================================================================
**     Method      :  Cap1_GetCaptureValue (bean Capture)
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
byte Cap1_GetCaptureValue(word *Value)

**  This method is implemented as a macro. See Cap1.h file.  **
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
void Cap1_GetCapResult(TCapture * result){
  word time;
  word ofs;
  
  #ifndef DEBUG
  Cpu_DisableInt();  
  if(CapInCount==0){
    result->pulsos=0;
    result->microsegundos=0; 
    return;  
  }
  result->pulsos= CapInCount;
  Cap1_GetCaptureValue(&time);
  ofs=CapOfCount;
  if(Cap1_CntrState>TC7)			//el valor de la resta es negativo?
    ofs-=1;  
  
 // Cap1_Reset();
  //init=TRUE;
  //Actualizo las varables al ultimo pulso
  Cap1_CntrState=TC7;
  CapOfCountWait-= CapOfCount;
  CapInCount=0;
  CapOfCount=0;
  //fin de actualizacion
  
  Cpu_EnableInt();
  
//  if(!ofs)
//    result.microsegundos = (ulong)timer * 2;
//  else
    result->microsegundos =  (ulong)ofs * 131070 + (ulong)time * 2;
  #endif
}

/*
** ===================================================================
**     Method      :  Cap1_Init (bean Capture)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
void Cap1_Init(void)
{
  /* TCTL3: EDG7B=0,EDG7A=0 */
  clrReg8Bits(TCTL3, 192); 
  
  Cap1_CntrState = 0;                  /* Initial state of variable */
  EnUser = FALSE;                      /* Disable device */
  TSCR2_PR = 4;                        /* Set prescaler register according to the selected high speed CPU mode */
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
void Cap1_OnCapture(void)
{
  /* Write your code here ... */
  #ifdef DEBUG
  word timer;
  #endif
  
  Cpu_DisableInt();
  #ifndef DEBUG 
  
  if(init){        
    init=FALSE;
    Cap1_Reset();  
  }else {
    CapInCount++;
    CapOfCount=CapOfCountWait;   
  }
  #endif
  
  #ifdef DEBUG  
    
    Cap1_GetCaptureValue(&timer);    
    Pasar_Numero(timer/100,0,0);
    Pasar_Numero(CapOfCountWait,1,0);
    Cap1_Reset();
  #endif 
  
  Cpu_EnableInt();
}

void Cap1_OnOverflow(void)
{
  /* Write your code here ... */
  Cpu_DisableInt();
  CapOfCountWait++;
  Cpu_EnableInt();

}

#pragma CODE_SEG __NEAR_SEG NON_BANKED     
ISR(Cap1_Interrupt)
{
  /* TFLG1: C7F=1,C6F=0,C5F=0,C4F=0,C3F=0,C2F=0,C1F=0,C0F=0 */
  TFLG1 = 128;                         /* Reset interrupt request flag */
  Cap1_OnCapture();                    /* Invoke user event */
}

#pragma CODE_SEG Cap1_CODE                     
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
ISR(Cap1_IntOverflow)
{
  TFLG2_TOF = 1;                       /* Reset overflow interrupt request flag */
  Cap1_OnOverflow();                   /* Invoke user event */
}

#pragma CODE_SEG Cap1_CODE                     

/* END Cap1. */

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 2.95.01 [03.62]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/
