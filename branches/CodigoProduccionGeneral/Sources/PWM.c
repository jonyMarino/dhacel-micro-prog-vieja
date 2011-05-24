/** ###################################################################
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : TO1.C
**     Project   : _10_03
**     Processor : MC9S12GC32CFU16
**     Beantype  : TimerOut
**     Version   : Bean 02.095, Driver 01.08, CPU db: 2.87.320
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 19/07/2006, 10:03 a.m.
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
**                       6             |  PT1_PWM1_IOC1
**             ----------------------------------------------------
**
**         Timer name                  : TIM (16-bit)
**         Compare name                : TC1
**           Counter shared            : No
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
**              Compare                : TC1       [82]
**
**         Flip-flop registers
**              Mode                   : TCTL2     [73]
**     Contents  :
**         Enable      - byte TO1_Enable(void);
**         Disable     - byte TO1_Disable(void);
**         SetPeriodUS - byte TO1_SetPeriodUS(word Time);
**         SetValue    - byte TO1_SetValue(void);
**         ClrValue    - byte TO1_ClrValue(void);
**
**     (c) Copyright UNIS, spol. s r.o. 1997-2004
**     UNIS, spol. s r.o.
**     Jundrovska 33
**     624 00 Brno
**     Czech Republic
**     http      : www.processorexpert.com
**     mail      : info@processorexpert.com
** ###################################################################*/


/* MODULE PWM. */

#include "Events.h"
#include "PE_Error.h"
#include "Mydefines.h"
#include "PWM.h"


#pragma DATA_SEG PWM_DATA                                            
#pragma CODE_SEG PWM_CODE                     

const dword PeriodsTicks[PWM_Last_Period+1]={ 
    50000,	 /* 100 ms*/
   100000,	 /* 200 ms*/
   250000,	 /* 500 ms*/
   500000,	 /*   1  s*/
  1000000,	 /*   2  s*/
  2500000,	 /*   5  s*/
  5000000,	 /*  10  s*/
 10000000,	 /*  20  s*/
 25000000,	 /*  50  s*/
     2500    /* Anl(5ms) */
};

static dword Period[NUM_SALIDAS],Duty[NUM_SALIDAS];

#define DutyFree(num) Period[num] - Duty[num]

/*
** ===================================================================
**     Method      :  TO1_SetPeriod (bean TimerOut)
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
byte setPWM_period(int Mode,byte salida)
{
  if (Mode>PWM_Last_Period)
    return ERR_RANGE;
  Period[salida] = PeriodsTicks[Mode];

  return ERR_OK;                       /* OK */ 
}

byte PWM_SetRatio16(word Ratio,byte salida)		      /*Ratio es un valor de 0 a 1000*/
{
  
  if (Ratio >1000 || salida>NUM_SALIDAS)            /* Is the given value out of range? */
    return ERR_RANGE;                               /* If yes then error */
  
  if (Ratio==1000)
    Duty[salida] = Period[salida];
  else if(Ratio==0)
    Duty[salida] = 0;
  else if (Period[salida]<4000000)		              /*Verifico los valores para decidir que operacion hacer debido a los limites de un long*/
    Duty[salida] = (Period[salida]*Ratio)/1000;
  else
    Duty[salida] = (Period[salida]/1000)*Ratio;
  
  if(Duty[salida]<15){//15 es el minimo tiempo para asegurar la corrida de la interrupcion  
    if(Duty[salida]<8) 
      Duty[salida]=0;
    else 
      Duty[salida]=15;  
  } 
  else{
    long dutyfree=DutyFree(salida);
    
    if (dutyfree<15){
      if(dutyfree<8) 
        Duty[salida]=Period[salida];
      else 
        Duty[salida]=Period[salida]-15;      
    }
  }
  return ERR_OK;                       /* OK */
}


/*
** ===================================================================
**     Method      :  TO1_Init (bean TimerOut)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
void PWM_Init(void)
{					
byte i;
  
  for(i=0;i<NUM_SALIDAS;i++){
    setPWM_period(PWM_50sec,i);
    PWM_SetRatio16(0,i);
  }
  
  TCTL2_OM1=1;
  TCTL1_OM4=1;
  TCTL1_OM5=1;
  TCTL1_OM6=1;  /* Poner en cero en la proxima interrupcion */
  TFLG1 |= 254;                       /* Reset request flag */
  TIE |=  2|16|32|64;								 /* Enable interrupt */
}

/*
** ===================================================================
**     Method      :  Conf_Toggle (bean TimerOut)
**
**     Description :
**         This method is internal. Permite Elegir cual ser� el proximo 
**  estado al interrumpir
** ===================================================================
*/
void PutVal (bool Value,byte salida){
  switch(salida){
    case 0:
      TCTL2_OL1=Value;
      break;
    case 1:
      TCTL1_OL4=Value; 
      break;
    case 2:
      TCTL1_OL5=Value;
      break;
    case 3:
      TCTL1_OL6=Value;
  }
}
/*
** ===================================================================
**     Method      :  Conf_Toggle (bean TimerOut)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
static dword Conf_Toggle(bool * next_Value,byte salida)
{
//static long CicloLow[NUM_SALIDAS]={65535,65535,65535,65535};

static long CicloLow[NUM_SALIDAS]={65535,65535};


  
if ((* next_Value &&  Duty[salida]!=Period[salida]) || Duty[salida]==0){
    PutVal (0,salida);  /* Poner la salida en bajo al interrumpir */                   
    * next_Value=0;
    return CicloLow[salida]; 
  }else{
    PutVal(1,salida);  /* Poner la salida en alto al interrumpir */                   
    * next_Value=1;
    CicloLow[salida] = DutyFree(salida);
    return Duty[salida];
  }
}

/*
** ===================================================================
**     Method      :  TO1_Interrupt (bean TimerOut)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED     
ISR(PWM_Interrupt)
{  
  static long Ciclo=65535;
  static bool Internal_Value=0,Previous_Value=1;
  
  if (Ciclo>65535){
    TC1 += 65500;
 		Ciclo-=65500;
  }else{
    TC1 += Ciclo;
    Ciclo=0;
  }
  
  TFLG1 = 2;          /* Reset interrupt request flag */
  
//  Cpu_EnableInt();
  
  if (Internal_Value!=Previous_Value){ /*�Hubo un cambio de estado en esta interrupcion???*/
 		  Previous_Value=Internal_Value;
 		  PWM_OnToggle(Internal_Value,0);   /* Is the pin output level high? */
  }
  
  if(Ciclo==0)
    Ciclo=Conf_Toggle(&Internal_Value,0);  
}


ISR(PWM4_Interrupt)
{  
  static long Ciclo=65535;
  static bool Internal_Value=0,Previous_Value=1;
  
  if (Ciclo>65535){
    TC4 += 65500;
 		Ciclo-=65500;
  }else{
    TC4 += Ciclo;
    Ciclo=0;
  }
  
  TFLG1 = 16;          /* Reset interrupt request flag */
  
//  Cpu_EnableInt();
  
  if (Internal_Value!=Previous_Value){  /* Hubo un cambio de estado en esta interrupcion???*/
 		  Previous_Value=Internal_Value;
 		  PWM_OnToggle(Internal_Value,1); 
  }
  
  if(Ciclo==0)
    Ciclo=Conf_Toggle(&Internal_Value,1);  
}

ISR(PWM5_Interrupt)
{  
  static long Ciclo=65535;
  static bool Internal_Value=0,Previous_Value=1;
  
  if (Ciclo>65535){
    TC5 += 65500;
 		Ciclo-=65500;
  }else{
    TC5 += Ciclo;
    Ciclo=0;
  }
  
  TFLG1 = 32;          /* Reset interrupt request flag */
  
//  Cpu_EnableInt();
  
  if (Internal_Value!=Previous_Value){  /* Hubo un cambio de estado en esta interrupcion???*/
 		  Previous_Value=Internal_Value;
 		  PWM_OnToggle(Internal_Value,2); 
  }
  
  if(Ciclo==0)
    Ciclo=Conf_Toggle(&Internal_Value,2);  
}

ISR(PWM6_Interrupt)
{  
  static long Ciclo=65535;
  static bool Internal_Value=0,Previous_Value=1;
  
  if (Ciclo>65535){
    TC6 += 65500;
 		Ciclo-=65500; 		
  }else{
    TC6 += Ciclo;
    Ciclo=0;
  }
  
  TFLG1 = 64;          /* Reset interrupt request flag */
  
//  Cpu_EnableInt();
  
  if (Internal_Value!=Previous_Value){  /* Hubo un cambio de estado en esta interrupcion???*/
 		  Previous_Value=Internal_Value;
 		  PWM_OnToggle(Internal_Value,3); 
  }
  
  if(Ciclo==0)
    Ciclo=Conf_Toggle(&Internal_Value,3);  
}
#pragma CODE_SEG PWM_CODE                     

/* END TO1. */

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 2.95 [03.62]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/
