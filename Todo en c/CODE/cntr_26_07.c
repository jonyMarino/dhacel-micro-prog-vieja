/** ###################################################################
**     Filename  : _12_10.C
**     Project   : _12_10
**     Processor : MC9S12A256BCPV
**     Version   : Driver 01.10
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 13/07/2005, 16:06
**     Abstract  :
**         Main module. 
**         Here is to be placed user's code.
**     Settings  :
**     Contents  :
**         No public methods
**
**     (c) Copyright UNIS, spol. s r.o. 1997-2004
**     UNIS, spol. s r.o.
**     Jundrovska 33
**     624 00 Brno
**     Czech Republic
**     http      : www.processorexpert.com
**     mail      : info@processorexpert.com
** ###################################################################*/
/* MODULE _12_10 */

#include <stdlib.h>
#include <termio.h>
#include "stdio.h"
/* Including used modules for compiling procedure */ 
#include "Cpu.h"
#include "Events.h"
#include "Display1.h"
#include "bits5ULN.h"
#include "bits2ULN.h"
#include "trx.h"
#include "PTSL.h"
#include "PWSN.h"
#include "PUL.h"
#include "WDog1.h"
#include "AS1.h"
#include "ADC1.h"
#include "FLASH1.h"
#include "PWM.h"
#include "PWM.h"
#include "TI1.h"
#include "O7.h"
#include "In1.h"
#include "In2.h"
#include "PWM4.h"
#include "PWM5.h"
#include "PWM6.h"
#include "O2.h"
#include "O4.h"
#include "O6.h"
#include "ADC.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "limits.h"
#include "TimerOld.h"

//////////////////mis includes//////////////////////////////
#include "Mydefines.h"
#include "boxes.h"
#include "Comunicacion.h"
#include "procesamiento.h"
#include "IFsh10.h"
#include "Programador.h"
#include "piddefines.h"
#include "paramdefines.h"
#include "Control.h"
#include "SelfTune.h"
#include "Sensores.h"
#include "cnfbox.h"
#include "teclas.h"
#include "boxescolcal.h"
#include "System.h"
#include "SensorRPM.h"
#include "CapturadorPT2.h"
#include "RlxMTimer.h"
#include "BaseTimers_1ms_40ms.h"
#include "FuncionVF.h"
#include "vfboxes.h" 
#include "EPM203Manejador.h"
#include "presentarValor.h"
#include "ManejadorImpresionPersistente.h"
#include "MethodContainer.h"
#include "Integrador.h"
#include "Humedad.h"
#include "adq.h"
#include "Parametros.h"
#include "Medicion.h"
#include "valorControl.h"
#include "PID.h"
#include "Alarmas.h"
#include "LoopDeControlYAlarmas.h"
#include "TimeOut.h"

 #pragma CONST_SEG DEFAULT
////////////////////////////////////////////////////////////////////////////
		              
#pragma DATA_SEG DEFAULT
////////////////////////////////////////////////////////////////////////////////


#ifdef _PRINTER

 struct ArgPresentaValor val1={
     &SetPoint[0],
     " setpoint "
 };
 
 struct EPM203Conf _confPrint_;
    
 struct MIPConf mipConf;
     

#endif


#ifdef RPM

struct CapturadorPT2 * cap;

struct SensorRpm sensorRPM;

SensorRpmConf confRpm;

#endif

//salidas de control
struct ArgControl _controles[CANTIDAD_SAL_CONTROL]={
  0,0,outc1
};

//salidas de Alarma
struct ArgAlarma _alarmas[ALARMAS_CH1]={
  0,0,outa1
};

struct ArgLoop argLoop={
   _controles,_alarmas,CANTIDAD_CANALES,ALARMAS_CH1
};



#if CANT_VALORES == 2  && CANTIDAD_CANALES == 2
datoAdq valores[MAX_CANTIDAD_VALORES]={ 
    {(int*)&ValoresCalculados[0],0},
    {(int*)&ValoresCalculados[1],1},
  };
#elif CANT_VALORES == 2
datoAdq valores[MAX_CANTIDAD_VALORES]={ 
    {(int*)&ValoresCalculados[0],0},
    {(int*)&SetPoint[0],0},
  };
#else
datoAdq valores[MAX_CANTIDAD_VALORES]={ 
    {(int*)&ValoresCalculados[0],0},
};
#endif  

 

void main(void)
{ 
  
  PE_low_level_init();
 
 #ifdef _PRINTER
  TERMIO_Init();
 #endif 
 #ifndef RPM
  setReg8Bits(DDRT, 126);
 #else
  setReg8Bits(DDRT, 122);
  PTSL_PutBit(0,TRUE);    //pongo en 1 la referencia del Schmit trigger
 #endif 
  
  System_init();
  
  intLoopControlYAlarmas(&argLoop);
  
 #ifdef _PRINTER
   
  _confPrint_.direccion = PRom[R_PrnDireccion];
  _confPrint_.fuente = PRom[R_PrnFuente];
  mipConf.intervalo = PRom[R_PrnIntervalo];
  mipConf.habilitado = PRom[R_PrnHabilitado];
 
  
  EPM203Manejador_init(&_confPrint_);
  ManejadorImpresionPersistenteInit(&mipConf);
  addMethodAImprimir(NULL,_Sensor); 
  addMethodAImprimir(&val1,_Valor);
 #else
  
   #ifndef RPM
    AS1_Init();
   #endif
  
 #endif
  
  
 #ifdef RPM
  
  confRpm.iDecimales=PRom[R_Decimales];
  confRpm.iFiltro=PRom[R_Filtro1];
  confRpm.pulsosPorVuelta=PRom[R_Rpm];
  confRpm.AjusteGan=PRom[R_AJU];
  CapturadorPT2_Init();
  cap=CapturadorPT2_getInstancia();
  SenRpm_Init(&sensorRPM,1000,cap,&confRpm);
  
 #endif

  setEstateAutoTune();

 #ifdef INTEGRADOR
  intIntegrador()
  getValueFlash();
 #endif

 #ifdef programador
  Prog_Init();                          // Inicializacion del programador  
 #endif
 		  
 #ifdef RPM   
 	setLimitesRPM ();
 #else	 
  setBufferSensor();
 #endif
 
  resetPWM();

  Boxes_Init(); 

  ResetScroll(); 


/////////////////// LOOP CONTINUO /////////////////////////
  for(;;)
  {    
  
  Tecla=get_key();
  
  executeBoxes();
  
  executeMethods(ListaPrincipal);
  
  #ifdef programador
  procesaTeclasProg();
  #endif
  
  isTimeReturnPrincipal();
  
  }

  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
}/*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/


/* END _12_10 */

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 2.95 [03.62]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/

