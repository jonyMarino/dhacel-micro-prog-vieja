<<<<<<< .mine
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

 #pragma CONST_SEG DEFAULT
////////////////////////////////////////////////////////////////////////////
// VARIABLES GLOBALES
////////////////////////////////////////////////////////////////////////////
///////VAriables que se ven desde la comunicacion//////
#pragma DATA_SEG MYDATA

						              
#pragma DATA_SEG DEFAULT
////////////////////////////////////////////////////////////////////////////////


bool Flag1;								              // contador (10 seg) de RTIs usado para la navegacion

//U8  CtrlMode;								          // normal / manual
PunteroF PtrTmp;			                  /* para el manipuleo */


int bufferSensor[CANTIDAD_CANALES];



#if !defined (programador) || defined (ADQ)
extern bool flag_1seg;
extern char flagFinMeseta;
#endif

#ifdef ADQ
int stateAdq=ERR_OK;
#endif
////////////// variables de los displays ///////////////////////////////
byte DotNum[CANTIDAD_DISPLAYS];			    /* punto decimal a encender */
/////////////////////////////////////////////////////////////

/////////////LEDS//////////////////////
bool led[NUM_SALIDAS];
///////////////////////////////////


////////////MEMORIZACION/////////////////////////

bool Mostrar_Proc;


////////////////////////////////////////////////

/////////////////FUNCION PRINCIPAL//////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////				

 


#ifdef jony_15_08
void BorrarPagApagado(void){
  (void) EraseSectorInternal(FLASH_APAGADO_START);  
}
#endif


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
struct ArgControl control1={
  0,0,outc1
};

//salidas de Alarma
struct ArgAlarma Alarma1={
  0,0,outa1
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
  
  byte i;
  long ValProv,tmpint;
  int ValLin,ValRet;
  int vCompTamb;
  bool flagBorrado=FALSE;
  extern bool AD_proc;
  
  #ifdef ADQ
  unsigned int contPeriodoAdq=0;
  #endif
  
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

  Boxes_Init(); 


  #ifdef INTEGRADOR
  Acumulador[0] = (*(long*)FLASH_APAGADO_START);
  #endif


  if( PRom[R_Stn]!=St_OFF)
    EscribirWord((word)&PRom[R_Stn],St_OFF);			                // Siempre al empezar Stn en cero


  #ifndef jony_05_07
  #ifdef programador
  Prog_Init();                          // Inicializacion del programador  
 	#endif
 	#endif
 	
 	#if defined( jony_15_08) && defined(programador)
 	Timer_Run(2000,&flagBorrado,UNICO_SET);
 	#endif
   
 #ifndef RPM   
 	for(i=0;i<CANTIDAD_CANALES;i++){
 		A_Sensor(PRom[R_Sensor+i],i);			  //Limites para el sensor y dec y el PTSL
	  bufferSensor[i]=PRom[R_Sensor+i];
 	}
 #else	
 	setLimitesRPM ();
 #endif
 

  PWM_SetRatio16(0,outc1);
  PWM_SetRatio16(0,outa1);
  PWM_SetRatio16(0,outa2);
  PWM_SetRatio16(0,outa3);		 

  setPWM_period(PWM_Anl,0);	
  setPWM_period(PWM_Anl,1);	
  setPWM_period(PWM_Anl,2);	
  setPWM_period(PWM_Anl,3);	



  PtrTmp=&Principal1.DirProc;		      // Empezar en Principal

 
 #ifdef programador
 
 for(i=0;i<2;i++){
  ADC_Start(0);
  
  while(!AD_proc)
    WDog1_Clear();
  
  AD_proc=FALSE;
 }
 
 

    #ifdef jony_05_07
       Prog_Init();                          // Inicializacion del programador  
      #ifdef jony_08_07
      if(PRom[R_Programa]!=NO_PROGRAMA){
        t_prog_state state=get_ProgState(0);  
    
      if(state==PROG_STOPED)
        set_MainText("StoP");

      }
  
      #endif
 	
 	  #else
 	
 	  for(i=0;i<CANTIDAD_SAL_CONTROL;i++)	  
 		 if (PRom[R_Programa+i]!=NO_PROGRAMA) 		  
 		  ReestablecerPrograma(i);
 	
 	  #endif
 	
 while (save_parametros);                // Por si se cambio algun parametro en ReestablecerPrograma()		 
 
 #endif 	


ResetScroll(); 


/////////////////// LOOP CONTINUO /////////////////////////
  for(;;)
  {    
  
  executeMethods(ListaPrincipal);
  
 
  #ifdef jony_25_06
  Tecla=get_key();
  #else
  Tecla=KeyEdge;
  KeyEdge=0;
  #endif
  
  (*PtrTmp)();                          // Funcion para el box correspondiente; llama a Num Handler, TextHandler, etc.	
  
 
  #ifdef jony_25_06
  /*  Verifico Funciones de programador*/
  #ifdef programador
  
  if(PRom[R_Programa]!=NO_PROGRAMA){
    /*if(Tecla=='s'){
        Prog_Stop();
        #ifdef jony_28_06
        set_MainText("StoP");
        #endif
    } */
      #ifdef LLAVES_EXT_P
      if(/*Tecla=='c' ||*/ !In2_GetVal()){
      #else
      if(Tecla=='c'){
      #endif
        Prog_Continue();
        #ifdef jony_28_06
        set_MainText("");
        #endif
    }
      #ifdef LLAVES_EXT_P
      if(/*Tecla=='p' ||*/ !In1_GetVal()){
      #else
      if(Tecla=='p'){
      #endif
        Prog_reset();
        #ifdef jony_28_06
        set_MainText("StoP");
        #endif
    }
  
  }
  #endif
  
  if(Tecla=='k')
    FlagCleaner=1;
  #endif
  
//normalizo para entrar en la tabla de linealizacion. Ajusta cero y offset del ad
//el ajuste de cero es absoluto y el de ganancia porcentual
  
  
//Nicolas 
/****************************************/
#ifdef ADQ

if(flag_1seg == TRUE && PRom[R_AdqHabilitado] && stateAdq!=ADQ_FULL){
   flag_1seg=FALSE;
   
     if(contPeriodoAdq>=(PRom[R_AdqPeriodo]-1) && PRom[R_AdqHabilitado]==ADQ_SI){
      contPeriodoAdq=0;
      stateAdq=adquirirValorAut(valores,CANT_VALORES);
     }else contPeriodoAdq++;
}

if(datoSerie == 't' || PRom[R_AdqTranfer]==1){
  if(PRom[R_AdqTranfer]==1)
    EscribirWord((word)&PRom[R_AdqTranfer],0);  //pongo en no
  datoSerie=0;
  bajarTodosDatosAdq(valores,CANT_VALORES);
}

#endif
#ifdef VF


ProcesoTeclasVF();


if( F_VF )
  if(flag_1seg == TRUE){
   flag_1seg=FALSE;
   calculoTempInstYtiempoMesetaVF(); 
  }

#endif

#ifdef INTEGRADOR
  if(flag_1seg == TRUE){
   flag_1seg=FALSE;
   _sumador();
  }
 #endif

/*****************************************/
#ifndef RPM

if(bufferSensor[0]!=PRom[R_Sensor]
  #if CANTIDAD_CANALES>1 
   || bufferSensor[1]!=PRom[R_Sensor+1]
  #endif
){
	for(i=0;i<CANTIDAD_CANALES;i++){
 		A_Sensor(PRom[R_Sensor+i],i);	
 	  bufferSensor[i]=PRom[R_Sensor+i];
	}
}
 #endif
 
 #ifndef RPM     
  if (AD_proc)									        // esta el flag de procesar los resultados del AD??
  {
 #endif
 
		
////////////////// ELECCION DE SET POINTS ////////////////

 setSetPoint();    

//////////////////////////////////////////////////////////
        
/////////////////// Calculos para los canales leidos////////// 

#ifdef RPM 
  setValCalculado(0,SenRpm_getValue(&sensorRPM)); 
#else
  calculaValorFinal();

#endif



/* Calculo de las salidas. Una a una, sin loop*/           

/**************************************************************************************************/
//Salida de control 1        

calculosDeControl(&control1);

/*************************************************************************************************
CALCULO DE LAS SALIDAS DE ALARMA

Para simplificar la notacion uso

#define  t_sp_alarma	 PRom[R_T_AL+NAL]
#define  t_sal_alarma  PRom[R_Talar1+NAL]
#define  vx 					 ValFinal[0+NCN]
#define  valcont 			 ValCont[0+NCN]
#define  dutyold 	 		 duty_alar_ch[NAL]
#define  spalar				 PRom[R_AL+NAL]
#define  res 					 PRom[R_Reset+NAL]
#define  hisalar			 PRom[R_HA1+NAL]
#define  abalar 			 PRom[R_HA1+NAL]
#define  dutyalar			 duty_alar_ch[NAL]

#define VAL_RET   ValFinal[0+NCN]

**************************************************************************************************/

/*CALCULO DE LA SALIDA DE ALARMA 1*/

calculosDeAlarma(&Alarma1);


/**********************************************************************************************************/
 /*CALCULO DE LA SALIDA DE ALARMA 2*/

#if ((ALARMAS_CH1 >= 2) || (ALARMAS_CH1 ==1 && CANTIDAD_CANALES == 2)) && !defined (RPM)

#undef NAL
#undef NCN

#if CANTIDAD_CANALES == 2
    #ifdef SP_EXTERNO
      #define NAL 1                                         //variables y parametros de canal1
      #define NCN 0
    #else
      #define NAL 1                                         //variables y parametros de canal1
      #define NCN 1
    #endif
#else
#define NAL 1                              //selecciono la alarma
#define NCN 0

#endif
#endif
/*******************************************************************************************************/
  /*CALCULO DE LA SALIDA DE ALARMA 3*/



#undef NAL
#undef NCN

#if (CANTIDAD_CANALES == 1 && ALARMAS_CH1 == 3)
#define NAL 2                                                                 //variables y parametros de canal1
#define NCN 0

#elif((defined (VPROP) && defined (DOS_ALARMAS) )) 
  #define NAL 1                                                                 //variables y parametros de canal1
  #define NCN 0
#endif

#ifdef NAL && NCN

#endif

  FlagCleaner = 0;              //lo limpio despues de procesar todos los flags

  AD_proc=FALSE;			// deshabilitar proceso de resultado de AD
  Mostrar_Proc=TRUE;  //Mostrar los valores en la pantalla principal (se procesa en Main Handler)

#ifndef RPM 
  }
#endif
/////////////Acciones de tiempo//////////////////
#ifdef jony_25_06
#ifdef programador
if(Prog_actualizar)
  Prog_ActualizarSetPoints();
#if defined( jony_15_08) && defined(programador)
if(flagBorrado){
  flagBorrado=FALSE;
  BorrarPagApagado();  
  HabilitarAccionPagApagado();
}
#endif
#endif
#endif
////////////////////////////////////////////////////////////////////////	

////////////////////////Comunicacion /////////////////////
/*    if (AS1_Tx==TRUE)						//Hay algo para enviar??
	  AS1_TxChar();						// Enviar
		
    if ( AS1_RecvChar(&msn[Step])==ERR_OK)  //Recibi algo????		
	  AS1_OnRxChar();											// Procesar
  */
  
///////// veo si vuelvo a la pantalla principal/////////////
	  
	  #ifdef VF_PROG    
	  if(Flag1 && PtrTmp!=&Principal1.DirProc && (VFstatus == ENDVF || PtrTmp!=&Principal1_VF_PROG.DirProc))		// PAsaron 10 segundos fuera de la pantalla principal y estando en FIN??
	  {
	   if(VFstatus != ENDVF){
	     PasarASCII("    ",1);   //borro la pantalla una ves 
	     ResetScroll();
	     PtrTmp=&Principal1_VF_PROG.DirProc;											//Volver a la pantalla principal
	     FstTime=TRUE;
	   }else{
	     PasarASCII("    ",1);   //borro la pantalla una ves 
	     ResetScroll();
	     PtrTmp=&Principal1.DirProc;											//Volver a la pantalla principal
	     FstTime=TRUE;
	   }
	  }
	  #else
	  if(Flag1 && PtrTmp!=&Principal1.DirProc)		// PAsaron 10 segundos fuera de la pantalla principal??
	  { 
	    PtrTmp=&Principal1.DirProc;											//Volver a la pantalla principal
	    FstTime=TRUE;
	  }
	  #endif     	 
  
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

=======
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

#include "tara.h"
#include "hold.h"

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
#include "Mmax.h"
////////////////////////////////////////////////////////////////////////////
// VARIABLES EN FLASH	 
////////////////////////////////////////////////////////////////////////////
#pragma CONST_SEG PARAMETERS_PAGE
volatile const int PRom[R_ATA+1] ={
10,    // R_SetPoint      //0
0,    // R_SetPoint+1

10,   // R_AL     
10,   // R_AL+1     
10,   // R_AL+2	         //4

-2,   // R_H1           //5
-2,   // R_H2      

-2,   // R_HA1           //7 
-2,   // R_HA2     
-2,   // R_HA3
    
0,    // R_Per1           //10   
0,    // R_Per2

SENSOR_JA,// R_Sensor1    //12
SENSOR_JA,// R_Sensor2

0,        // R_Decimales1 //14 
0,        // R_Decimales2          

0,        // R_Offset1     //16
0,        // R_Offset2  

1000,     // R_Gan1        //18
1000,     // R_Gan2  

0,        // R_ACT1        //20  
0,        // R_ACT2

0,        // R_AGT1       //22
0,        // R_AGT2

0,        // R_ACP1       //24
0,        // R_ACP2

0,        // R_AGP1       //26
0,        // R_AGP2

0,        // R_AAnlLow1    //28
0,        // R_AAnlLow2

100,      // R_AAnlHi1     //30
100,      // R_AAnlHi2

0,        // R_Lim_Inf1     //32
0,        // R_Lim_Inf2 

450,       
450,      // R_Lim_Sup 			 //35

0,          
0,        // R_Tcontrol1    //37  

0,          
0,        // R_Reset        //39
                            
0,          
0,        // R_IN1          //41

0,          
0,        // R_DR1          //43

0,         
0,
0,        // R_Pra          //46

0,          
0,       // R_Pot_Inf       //48

1000,       
1000,    // R_Pot_Sup      //50

0,          
0,       // R_Tsalcont      //52

0,          
0,        // R_Potman         //55

0,        
0,        // R_Stn          //56

_rel,         
_rel,       
_rel,     // R_T_AL          //59
                                
0,             
0,          
0,         //R_Talar          //62

1,          
2,          // R_Filtro1     //64


#ifdef programador
NO_PROGRAMA,
NO_PROGRAMA,// R_Programa 
0,          
0,
0,
0,
#endif         
#ifdef CCAL
200,         														// R_Pss 
120,         														// R_Tss
600,       															// R_SPlim
800,   																	// R_SPst
450,                                   // R_SPsb    
300,         														// R_Indf
30,        															// R_Drdf
200,   																	// R_Abdf
900,                                    // R_SPlimst
#endif         													 
#if defined(_PRINTER)
10,
0,
0,
0,
0,
0,
0,
0,
#endif
#ifdef RPM
1,
1000,
#endif
#ifdef VF
0,
35,
1,
#endif
#ifdef INTEGRADOR
0,
2,
0,
#endif
#ifdef ADQ
0,
0,
0,
0,
0,                                      // R_Adquirir
0,                                      // R_Borrar
0,                                      // R_Tranfer
5,                                      // R_periodoAdq 
#endif                                  
60,                                     // R_Topn 218
0,                                      // R_COM
0,                                      // R_Dbnd 219
0,                                      // R_SetC 220
0,                                      // R_Ver  221
1,                                      // R_Id   222 /* direccion del Id del aparato*/
0,                                      // R_ATA  229

};





#ifdef RPM
 // volatile const SensorRpmConf config=DEFAULT_SENSOR_RPM_CONF;
#endif
			

 #pragma CONST_SEG DEFAULT
////////////////////////////////////////////////////////////////////////////
// VARIABLES GLOBALES
////////////////////////////////////////////////////////////////////////////
//int PRam[PARAMETROS];	          //PARAMETROS en Flash

///////VAriables que se ven desde la comunicacion//////
#pragma DATA_SEG MYDATA
int ValFinal[4];
int SetPoint[4];                        // Set Point en Ram que se utiliza para el control y los calculos
int duty_cont_ch[4];
int duty_alar_ch[4];
word Result[4];						              //valor linealizado
#pragma DATA_SEG DEFAULT
////////////////////////////////////////////////////////////////////////////////
const int div_dec[]={1,10,100,1000,10000};// divisores del decimal/////////////////////////

byte  * ptr_ctes;							          // puntero a constantes en la flash
bool Flag1;								              // contador (10 seg) de RTIs usado para la navegacion

//U8  CtrlMode;								          // normal / manual
PunteroF PtrTmp;			                  /* para el manipuleo */
bool flagAlarmaEoC[3];

bool flagAlarma = FALSE;
long Vx[CANTIDAD_CANALES];              //Array de los valores que salen del ADC
int ValCont [CANTIDAD_CANALES];         //valor que controla el equipo con PID
long int TA;		                        //Temperatura ambiente
long buffer_deriv[CANTIDAD_CANALES];
long buffer_fil[CANTIDAD_CANALES];
long suminteg[CANTIDAD_CANALES];
int deriv[CANTIDAD_CANALES];
long auxiliar;
long auxiliar1;
int Dr;
long vxalar;
int dutytmp;
byte flagalar[10];
int bufferSensor[CANTIDAD_CANALES];
extern byte FlagCleaner;
extern int flagsst;

#if !defined (programador) || defined (ADQ)
extern bool flag_1seg;
extern char flagFinMeseta;
#endif

#ifdef VPROP
int duty_vprop;
long cnt_duty_vprop = 0;
#endif
#ifdef SENS_HUMEDAD
unsigned int Humedad =0;
#endif

#ifdef ADQ
int stateAdq=ERR_OK;
#endif
////////////// variables de los displays ///////////////////////////////
byte DotNum[CANTIDAD_DISPLAYS];			    /* punto decimal a encender */
/////////////////////////////////////////////////////////////

/////////////LEDS//////////////////////
bool led[NUM_SALIDAS];
///////////////////////////////////

///////////PWM/////////////
word RatioStore[4];
extern byte	KeyEdge;
int duty;
////////////MEMORIZACION/////////////////////////

bool Mostrar_Proc;

unsigned int contardorLoop=0;
////////////////////////////////////////////////

/////////////////FUNCION PRINCIPAL//////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////				

 
#define valnorm(i) (((Vx[i]+PRom[R_ACT+i])*(1000+PRom[R_AGT+i]))/1000)*MAXMV/MAXAD  

#pragma INLINE


#pragma INLINE


#ifdef jony_15_08
void BorrarPagApagado(void){
  (void) EraseSectorInternal(FLASH_APAGADO_START);  
}
#endif


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

#if CANT_VALORES == 2  && CANTIDAD_CANALES == 2
datoAdq valores[MAX_CANTIDAD_VALORES]={ 
    {(int*)&ValFinal[0],0},
    {(int*)&ValFinal[1],1},
  };
#elif CANT_VALORES == 2
datoAdq valores[MAX_CANTIDAD_VALORES]={ 
    {(int*)&ValFinal[0],0},
    {(int*)&SetPoint[0],0},
  };
#else
datoAdq valores[MAX_CANTIDAD_VALORES]={ 
    {(int*)&ValFinal[0],0},
};
#endif  

/**/
/* unsigned int j=0;

void resetJ(){
  j=0;
}

void setJ(){
   unsigned int i;
   for(i=0;i<20000;i++);
}

void probando(){
    unsigned int i;
    

    Pasar_Numero(j,1,0);
    for(i=0;i<2000;i++);
    
    j++; 
}


void tiemppp(){
   unsigned int i;
   for(i=0;i<20000;i++);
}

struct Method prueb={
 probando,NULL 

};

struct Method pruebtt={
 tiemppp,NULL 

};
*/

void main(void)
{ 
  #if CANTIDAD_CANALES>1
  char flagFisrtCalculo[CANTIDAD_CANALES]={0,0};	
  #else
  char flagFisrtCalculo[CANTIDAD_CANALES]={0};
  #endif
  byte i;
  long ValProv,tmpint;
  int ValLin,ValRet;
  int vCompTamb;
  bool flagBorrado=FALSE;
  extern bool AD_proc;
  
  #ifdef ADQ
  unsigned int contPeriodoAdq=0;
  #endif
  
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

  Boxes_Init(); 

  #ifdef SIMCIC
  auxiliar1 = *(word*)FLASH_APAGADO_START;
  EscribirWord((word)&PRom[R_SetPoint],auxiliar1);
  #endif

  #ifdef INTEGRADOR
  Acumulador[0] = (*(long*)FLASH_APAGADO_START);
  #endif


  if( PRom[R_Stn]!=St_OFF)
    EscribirWord((word)&PRom[R_Stn],St_OFF);			                // Siempre al empezar Stn en cero


  #ifndef jony_05_07
  #ifdef programador
  Prog_Init();                          // Inicializacion del programador  
 	#endif
 	#endif
 	
 	#if defined( jony_15_08) && defined(programador)
 	Timer_Run(2000,&flagBorrado,UNICO_SET);
 	#endif
   
 #ifndef RPM   
 	for(i=0;i<CANTIDAD_CANALES;i++){
 		A_Sensor(PRom[R_Sensor+i],i);			  //Limites para el sensor y dec y el PTSL
	  bufferSensor[i]=PRom[R_Sensor+i];
 	}
 #else	
 	setLimitesRPM ();
 #endif
 
 

#ifdef _APARATO_VIEJO

  PWM_SetRatio16(0,outc1);
  PWM_SetRatio16(0,outa1);
  PWM_SetRatio16(0,outa2);
  PWM_SetRatio16(0,outa3);		 

  setPWM_period(PWM_Anl,0);	
  setPWM_period(PWM_Anl,1);	
  setPWM_period(PWM_Anl,2);	
  setPWM_period(PWM_Anl,3);	


#else

 setPWM_period(PRom[R_Per],0);		    // Setear Periodo Canal1
 setPWM_period(PRom[R_Per+1],1);	    // Setear Periodo Canal1
 setPWM_period(PRom[R_Pra],2);       // Setear Periodo Alarma1
 setPWM_period(PRom[R_Pra+1],3);     // Setear Periodo Alarma1

#endif

  PtrTmp=&Principal1.DirProc;		      // Empezar en Principal

 
 #ifdef programador
 for(i=0;i<2;i++){
  ADC_Start(0);
  while(!AD_proc)
    WDog1_Clear();
  AD_proc=FALSE;
 }
 for(i=0;i<CANTIDAD_CANALES;i++){       // recorrer todos los canales
        #ifdef RUVAC
     			Linealizar(valnorm(i),SENSOR_MV,&ValRet);
     			if(PRom[R_Sensor+i]==SENSOR_PT)
          
          ValProv=ValRet;
          
           ValProv = ((ValProv+PRom[R_ACP+i])*(1000+PRom[R_AGP+i])/1000);  
  				
  				
  				if (PRom[R_Sensor+i]<SENSOR_PT)
              ValProv=ValProv - TA + PRom[R_ATA];   
          #ifdef _APARATO_VIEJO
          if ((PRom[R_Sensor+i]!=SENSOR_LIN1) &&(PRom[R_Sensor+i]!=SENSOR_LIN2) && (PRom[R_Sensor+i]!=SENSOR_LIN3) && (PRom[R_Sensor+i]!=SENSOR_LIN4))      
          #else
          if (PRom[R_Sensor+i]!=SENSOR_LIN)
          #endif
            ValProv/= div_dec[SENSOR_Decimales(PRom[R_Sensor+i])-PRom[R_Decimales+i]];  
          if (ValProv>9999) 
            Result[i]=ERR_OF;
  				else if (ValProv<-1999) 
  					Result[i]=ERR_UF;
  				ValProv=(ValProv+PRom[R_Offset+i])*PRom[R_Gan+i]/1000;         //preciso para pasar a long
  		//		ValProv=ValProv*PRom[R_Gan]/1000;					//no anda todo en una linea
  			 ValRet=(int)ValProv;
   			#endif
   		  
   		  vCompTamb =	CompTempAmb((t_sensor)PRom[R_Sensor+i]);          
         
        
				if (PRom[R_Sensor+i]<SENSOR_PT)
				/*como la termocupla me entrega los mv de la diferencia(jc-jf)para compensar debo !sumar los mv de jf*/
           Result[i]=Linealizar((long)(valnorm(i)+(long)vCompTamb),(t_sensor)PRom[R_Sensor+i],&ValLin);
        else
           Result[i]=Linealizar(valnorm(i),(t_sensor)PRom[R_Sensor+i],&ValLin);
        
        ValProv=ValLin; 
        
        #ifdef SENS_HUMEDAD
        if(PRom[R_Sensor+i]==SENSOR_PT || PRom[R_Sensor+i]==SENSOR_HUM)
        #else
        if(PRom[R_Sensor+i]==SENSOR_PT )
        #endif
         ValProv = ((ValProv+PRom[R_ACP+i])*(1000+PRom[R_AGP+i])/1000);  
				
           
        #ifdef _APARATO_VIEJO
        if ((PRom[R_Sensor+i]!=SENSOR_LIN1) &&(PRom[R_Sensor+i]!=SENSOR_LIN2) && (PRom[R_Sensor+i]!=SENSOR_LIN3) && (PRom[R_Sensor+i]!=SENSOR_LIN4))      
        #else
        if (PRom[R_Sensor+i]!=SENSOR_LIN)
        #endif
          ValProv/= div_dec[SENSOR_Decimales(PRom[R_Sensor+i])-PRom[R_Decimales+i]];  
        if (ValProv>9999) 
          Result[i]=ERR_OF;
				else if (ValProv<-1999) 
					Result[i]=ERR_UF;
				ValProv=(ValProv+PRom[R_Offset+i])*PRom[R_Gan+i]/1000;         //preciso para pasar a long
		//		ValProv=ValProv*PRom[R_Gan]/1000;					//no anda todo en una linea
		
				ValFinal[i]=(int) ValProv;
   }
 
//				if (PRom[R_Ver]==DIF)
//				ValFinal[0]=ValFinal[0]-ValFinal[1];




  #ifdef jony_05_07
  Prog_Init();                          // Inicializacion del programador  
  #ifdef jony_08_07
  if(PRom[R_Programa]!=NO_PROGRAMA){
    t_prog_state state=get_ProgState(0);  
    
    if(state==PROG_STOPED)
     set_MainText("StoP");

  }
  
  #endif
 	#else
 	for(i=0;i<CANTIDAD_SAL_CONTROL;i++)	  
 		 if (PRom[R_Programa+i]!=NO_PROGRAMA) 		  
 		  ReestablecerPrograma(i);
 	#endif
 	
 while (save_parametros);                // Por si se cambio algun parametro en ReestablecerPrograma()		 
 #endif 	


ResetScroll(); 



/*MethodContainer_add(&prueb,0);  
MethodContainer_add(&pruebtt,0);
RlxMTimer_Init(2000,resetJ,NULL);   
MTimer_Init(3000,setJ,NULL);
  */
/////////////////// LOOP CONTINUO /////////////////////////
  for(;;)
  {    
  
  executeMethods(ListaPrincipal);
  
  
  			#ifdef _TEST1
      Bit1_NegVal();

  		#endif
  #ifdef jony_25_06
  Tecla=get_key();
  #else
  Tecla=KeyEdge;
  KeyEdge=0;
  #endif
  (*PtrTmp)();                          // Funcion para el box correspondiente; llama a Num Handler, TextHandler, etc.	
  
  #ifdef _PRINTER
 // p.mainLoop();
  #endif
  
  #ifdef RPM
  //mainThread.executeMethods();
  #ifndef _PRINTER
  //threads.executeThreads();
  #endif
  #endif
  
 
  #ifdef jony_25_06
  /*  Verifico Funciones de programador*/
  #ifdef programador
  if(PRom[R_Programa]!=NO_PROGRAMA){
    /*if(Tecla=='s'){
        Prog_Stop();
        #ifdef jony_28_06
        set_MainText("StoP");
        #endif
    } */
      #ifdef LLAVES_EXT_P
      if(/*Tecla=='c' ||*/ !In2_GetVal()){
      #else
      if(Tecla=='c'){
      #endif
        Prog_Continue();
        #ifdef jony_28_06
        set_MainText("");
        #endif
    }
      #ifdef LLAVES_EXT_P
      if(/*Tecla=='p' ||*/ !In1_GetVal()){
      #else
      if(Tecla=='p'){
      #endif
        Prog_reset();
        #ifdef jony_28_06
        set_MainText("StoP");
        #endif
    }
  
  }
  #endif
  
  if(Tecla=='k')
    FlagCleaner=1;
  #endif
  
//normalizo para entrar en la tabla de linealizacion. Ajusta cero y offset del ad
//el ajuste de cero es absoluto y el de ganancia porcentual
  
  
//Nicolas 
/****************************************/
#ifdef ADQ

if(flag_1seg == TRUE && PRom[R_AdqHabilitado] && stateAdq!=ADQ_FULL){
   flag_1seg=FALSE;
   
     if(contPeriodoAdq>=(PRom[R_AdqPeriodo]-1) && PRom[R_AdqHabilitado]==ADQ_SI){
      contPeriodoAdq=0;
      stateAdq=adquirirValorAut(valores,CANT_VALORES);
     }else contPeriodoAdq++;
}

if(datoSerie == 't' || PRom[R_AdqTranfer]==1){
  if(PRom[R_AdqTranfer]==1)
    EscribirWord((word)&PRom[R_AdqTranfer],0);  //pongo en no
  datoSerie=0;
  bajarTodosDatosAdq(valores,CANT_VALORES);
}

#endif
#ifdef VF


ProcesoTeclasVF();


if( F_VF )
  if(flag_1seg == TRUE){
   flag_1seg=FALSE;
   calculoTempInstYtiempoMesetaVF(); 
  }

#endif

#ifdef INTEGRADOR
  if(flag_1seg == TRUE){
   flag_1seg=FALSE;
   _sumador();
  }
 #endif

/*****************************************/
#ifndef RPM

if(bufferSensor[0]!=PRom[R_Sensor]
  #if CANTIDAD_CANALES>1 
   || bufferSensor[1]!=PRom[R_Sensor+1]
  #endif
){
	for(i=0;i<CANTIDAD_CANALES;i++){
 		A_Sensor(PRom[R_Sensor+i],i);	
 	  bufferSensor[i]=PRom[R_Sensor+i];
	}
}
 #endif
 
 #ifndef RPM     
  if (AD_proc)									        // esta el flag de procesar los resultados del AD??
  {
 #endif
 
			#ifdef _TEST1
      Bit2_SetVal();

  		#endif
////////////////// ELECCION DE SET POINTS ////////////////
     for (i=0;i<CANTIDAD_SAL_CONTROL;i++){
        if (PRom[R_Stn+i]== Stc) 
          SetPoint[i] = PRom[R_SetPoint+i]; 
        #ifndef programador
        else 
          SetPoint[i] = PRom[R_SetPoint+i];
        #else
		    else if(PRom[R_Programa+i]!=NO_PROGRAMA) SetPoint[i] = get_SP_Programa(i) ;
		    else SetPoint[i] = PRom[R_SetPoint+i]; 
        #endif
        
     }
     #ifdef jony_13_07
     #ifdef SP_EXTERNO
     SetPoint[0]+=ValFinal[1];
     #endif
     #endif

     #ifndef SP_EXTERNO
		 #ifndef programador
		   SetPoint[0] = PRom[R_SetPoint+0];  
		 #endif
		 #endif
     
     #ifdef CCAL

			if(flagsst == _STRT)																			// primero veo inicio y pongo sp
        SetPoint[0] =Lim_Inf_ch[0];															// bien abajo para que no prenda nunca
			else if(flagsst == _SST)																	// primero veo sst y pongo sp
        SetPoint[0] = PRom[R_SPlim];
      else if(PRom[R_Stn]==Stc)																	// segundo veo autosint
        SetPoint[0] = (int)((long)PRom[R_SetPoint]*PRom[R_SPlimst]/1000);
      else if(!In1_GetVal())																		// tercero veo sp externo
        SetPoint[0] = PRom[R_SPsb]; 
      else		  																								// y si nada, el sp normal
				SetPoint[0] = PRom[R_SetPoint];

		 #endif
		 
		 
		 
		 #ifdef VF
		    SetPoint[0] = tempActVF;
		 #endif
		 
		 #ifdef DOBLE_SP
		  if(In1_GetVal()) 
        SetPoint[0] = PRom[R_SetPoint+0];
		  else
		    SetPoint[0] = PRom[R_SetPoint+2];
		 #endif
		 
		

//////////////////////////////////////////////////////////
        
/////////////////// Calculos para los canales leidos////////// 

#ifdef RPM 
  ValFinal[0] = SenRpm_getValue(&sensorRPM); 
#else
for(i=0;i<CANTIDAD_CANALES;i++){// recorrer todos los canales
   #ifdef RUVAC
    	Result[i]=Linealizar(valnorm(i),SENSOR_MV,&ValRet);             //linealizo mv y pongo en valret
      ValProv=ValRet;
   		ValProv=(ValProv+PRom[R_Offset+i])*PRom[R_Gan+i]/1000;          //aplico offset y gan
  		ValRet=(int)ValProv;                                            //y vuelvo a valret
   #endif
   vCompTamb =	CompTempAmb((t_sensor)PRom[R_Sensor+i]);          
   
   if (PRom[R_Sensor+i]<SENSOR_PT)
      	/*como la termocupla me entrega los mv de la diferencia(jc-jf)para compensar debo !sumar los mv de jf*/
      Result[i]=Linealizar((long)(valnorm(i)+(long)vCompTamb),(t_sensor)PRom[R_Sensor+i],&ValLin);
   else
      Result[i]=Linealizar(valnorm(i),(t_sensor)PRom[R_Sensor+i],&ValLin);
   
   if(flagFisrtCalculo[i]!=0)
    ValProv=ValLin;
   else{
     ValProv=0;
     flagFisrtCalculo[i]=1;
   }
   #ifdef SENS_HUMEDAD
   if((PRom[R_Sensor+i]==SENSOR_PT)||(PRom[R_Sensor+i]==SENSOR_HUM))
   #else
   if((PRom[R_Sensor+i]==SENSOR_PT))
   #endif
      ValProv = ((ValProv+PRom[R_ACP+i])*(1000+PRom[R_AGP+i])/1000);  
   
 #ifdef _APARATO_VIEJO
   if ((PRom[R_Sensor+i]!=SENSOR_LIN1) &&(PRom[R_Sensor+i]!=SENSOR_LIN2) && (PRom[R_Sensor+i]!=SENSOR_LIN3) && (PRom[R_Sensor+i]!=SENSOR_LIN4))      
 #else
   if (PRom[R_Sensor+i]!=SENSOR_LIN)
 #endif
     ValProv/= div_dec[SENSOR_Decimales(PRom[R_Sensor+i])-PRom[R_Decimales+i]];  
   if (ValProv>9999) 
     Result[i]=ERR_OF;
	 else if (ValProv<-1999) 
	   Result[i]=ERR_UF;
	 
 #ifdef TARA	 
	 puestaCero();
 #endif	 

	ValProv=(ValProv+PRom[R_Offset+i])*PRom[R_Gan+i]/1000;         //preciso para pasar a long
//				ValFinal[i]=(int) ValProv;


///¡¡¡¡



ValFinal[i] = filtro ((int)ValProv,
                       PRom[R_Filtro1+i],
                       DELTAT/100,
                       200,
                       &buffer_fil[i]
                       );
        }
 

if (PRom[R_Ver]==VER_DIF)
ValFinal[0]=ValFinal[0]-ValFinal[1];
				
#ifdef SENS_HUMEDAD
if(PRom[R_Sensor+1]==SENSOR_HUM){
  Humedad = interpolacionDoble();
  if(Humedad ==ERR_OF)
    Result[1]=ERR_OF;
 }
#endif


  #ifdef CCAL 
	 monit_colcal();
  #endif
  
#ifdef HOLD
  if(getFlagHold())
    ValFinal[0]=getBuffer();
#endif 

#ifdef MMAX
  calculoMmax();
  if(getFlagMmax())
    ValFinal[0]=getBufferMmax();
#endif
 

#endif



/* Calculo de las salidas. Una a una, sin loop*/           

/**************************************************************************************************/
//Salida de control 1        



#undef NCN
#undef NAL
#define NCN 0
#define NAL 0



if (!PRom[R_Stn+NCN]== Stc){                                            //primero veo stune

//calculo la integral

 if (t_sp_alarma == _refr)
      integral(abl,hisalar,spcont,spalar,kint,vxl,&suminteg[0+NCN],1);
 else
      integral(abl,hisalar,spcont,spalar,kint,vxl,&suminteg[0+NCN],1);

//la derivada

 Dr = derivada   (vxl,kdl,mdc,abl,&buffer_deriv[0+NCN],&deriv[0+NCN]);

//el valor final de control. Es long

 auxiliar =  cprop(abl,spcont,kint,vx,suminteg[0+NCN],Dr,res);      

//ajusto al rango de long a int
				
 if (auxiliar > 30000)
    valcont = 30000;
 else if (auxiliar < -30000)
    valcont = -30000;
 else
    valcont = (int)auxiliar;
 
//Calculo del duty

 if (abl >0 )

 #ifdef CCAL
	 if(flagsst == _SST)
    duty_cont_ch[0+NCN] = dtprop(valcont,PRom[R_Pot_Inf+NCN],PRom[R_Pss]);
	 else
    duty_cont_ch[0+NCN] = dtprop(valcont,PRom[R_Pot_Inf+NCN],PRom[R_Pot_Sup+NCN]);
 #else
    duty_cont_ch[0+NCN] = dtprop(valcont,PRom[R_Pot_Inf+NCN],PRom[R_Pot_Sup+NCN]);
 #endif

 else 
    duty_cont_ch[0+NCN] = dtonoff(valcont,duty_cont_ch[0+NCN]);
 
//Valor de la salida
#ifdef RPM
 if (PRom[R_Tsalcont+NCN] == _inv)
#else 
 if (PRom[R_Tsalcont+NCN] == _ref)
#endif 
    dutytmp = 1000-duty_cont_ch[0+NCN];
 else if (PRom[R_Tsalcont+NCN] == _man)
    dutytmp = PRom[R_Potman+NCN];
 else dutytmp = duty_cont_ch[0+NCN];
      
 #ifdef VPROP
 duty_vprop = dutytmp;
 
 #else
 #ifdef VF
 if(VFstatus == ENDVF)
   PWM_SetRatio16(0,outc1);       //si esta en "fin" desconecta la salida de control
 else
   PWM_SetRatio16(dutytmp,outc1); //de lo contrario funciona segun el sp
 #else
 PWM_SetRatio16(dutytmp,outc1);
 #endif

  if(abl<=0 && PRom[R_Tsalcont+NCN]!=_man)
    setPWM_period(PWM_Anl,outc1);	
 else setPWM_period(PRom[R_Per+NCN],outc1);
 
#endif 
 
     }else SelfTuneControl(0+NCN,spcont);
	

/**************************************************************************************************/
//Salida de control 2        

#if CANTIDAD_CANALES == 2

#undef NCN
#undef NAL
#define NCN 1
#define NAL 1



if (!PRom[R_Stn+NCN]== Stc){               //primero veo stune

//calculo la integral

 if (t_sp_alarma == _refr)
      integral(abl,hisalar,spcont,spalar,kint,vxl,&suminteg[0+NCN],1);
 else
      integral(abl,hisalar,spcont,spalar,kint,vxl,&suminteg[0+NCN],1);

//la derivada

 Dr = derivada   (vxl,kdl,mdc,abl,&buffer_deriv[0+NCN],&deriv[0+NCN]);

//el valor final de control. Es long

 auxiliar =  cprop(abl,spcont,kint,vx,suminteg[0+NCN],Dr,res);      

//ajusto al rango de long a int
				
 if (auxiliar > 30000)
    valcont = 30000;
 else if (auxiliar < -30000)
    valcont = -30000;
 else
    valcont = (int)auxiliar;
 
//Calculo del duty

 if (abl >0 )
    duty_cont_ch[0+NCN] = dtprop(valcont,PRom[R_Pot_Inf+NCN],PRom[R_Pot_Sup+NCN]);
 else 
    duty_cont_ch[0+NCN] = dtonoff(valcont,duty_cont_ch[0+NCN]);
 
//Valor de la salida

 if (PRom[R_Tsalcont+NCN] == _ref)
    dutytmp = 1000-duty_cont_ch[0+NCN];
 else if (PRom[R_Tsalcont+NCN] == _man)
    dutytmp = PRom[R_Potman+NCN];
 else dutytmp = duty_cont_ch[0+NCN];
      
 PWM_SetRatio16(dutytmp,outa3);

  if(abl<=0 && PRom[R_Tsalcont+NCN]!=_man)
    setPWM_period(PWM_Anl,outa3);	
 else setPWM_period(PRom[R_Per+NCN],outa3);
 
     }else SelfTuneControl(0+NCN,spcont);
#endif

/*************************************************************************************************
CALCULO DE LAS SALIDAS DE ALARMA

Para simplificar la notacion uso

#define  t_sp_alarma	 PRom[R_T_AL+NAL]
#define  t_sal_alarma  PRom[R_Talar1+NAL]
#define  vx 					 ValFinal[0+NCN]
#define  valcont 			 ValCont[0+NCN]
#define  dutyold 	 		 duty_alar_ch[NAL]
#define  spalar				 PRom[R_AL+NAL]
#define  res 					 PRom[R_Reset+NAL]
#define  hisalar			 PRom[R_HA1+NAL]
#define  abalar 			 PRom[R_HA1+NAL]
#define  dutyalar			 duty_alar_ch[NAL]

#define VAL_RET   ValFinal[0+NCN]

**************************************************************************************************/

/*CALCULO DE LA SALIDA DE ALARMA 1*/


#undef NCN
#undef NAL
#define NAL 0                                         //variables y parametros de canal1
#define NCN 0
    

/*Este modulo calcula la señal de alarma*/

switch (t_sp_alarma){				
    case _rel:														
		    vxalar = cprop(hisalar, spcont+spalar, 0, vx, 0, 0,res);                  //relativa, los dos sp      
    break;
 
    case _abs:														
		    vxalar = cprop(hisalar,spalar,0,vx,0,0,res);      												//absoluta solo el sp de al
		break;

    case _refr:																																//refrig, los dos sp con In y Dr
		    vxalar = cprop(hisalar,spcont+spalar,kint,vx,suminteg[0+NCN],(int)buffer_deriv[0+NCN],res);      
    break;
          
    case _ban:														
        if(vx > spcont)         
            vxalar =cprop(hisalar,spcont+spalar/2,0,vx,0,0,res);									//banda, arriba como rel
        else
            vxalar =1000 - cprop(hisalar,spcont-spalar/2,0,vx,0,0,res);						//abajo idem pero polarid inv
    break;



#ifdef programador
    case _seg:							                                                  //segmento							
        if(SegmentoActual[0+NCN]+OFFSET_SEG1==spalar && PRom[R_Programa]!=NO_PROGRAMA)
            vxalar = 30000;	                                                  //Siempre on
        else
            vxalar = -30000;                                                  //Siempre off
    break;
    
    case _eoc:
        if(isProgFinish(0+NCN))																								//fin de ciclo
            vxalar = 30000;	 
        else
            vxalar = -30000; 
    break;
#endif
    
#ifdef VF
    case _eoc:
        flagAlarmaEoC[0]=TRUE;
        flagAlarmaEoC[1]=FALSE;
        flagAlarmaEoC[2]=FALSE;
        if(VFstatus == ENDVF && tecla_d!=1){
            vxalar = 30000;
            flagAlarma=TRUE;
        }else
            vxalar = -30000;     
     break;
#endif     

    default:
		    vxalar = vx-spalar;
    break;
    };


/*Este modulo limita los valores a int*/

if (vxalar > 30000)
    vxalar = 30000;
if (vxalar < -30000)
    vxalar = -30000;



/*Este modulo calcula el valor del duty*/

 switch (t_sal_alarma){				
    case _e:														
        if (hisalar > 0)													                            //exceso, al reves que la señal de cal
            duty_alar_ch[NAL] = dtprop   (1000 - vxalar,0,1000);
        else 
            duty_alar_ch[NAL] = dtonoff(1000 - vxalar,duty_alar_ch[NAL]);
    break;    

    case _d:														
        if (hisalar > 0)																											//defecto
            duty_alar_ch[NAL] = dtprop   (vxalar,0,1000);                                     
        else 
            duty_alar_ch[NAL] = dtonoff(vxalar,duty_alar_ch[NAL]);
    break;

    case _eret:																																//exc con ret
	      if (FlagCleaner==1)																										//si fue pedido limpio flag
	          flagalar[NAL] = 0;	 

       if(flagalar[NAL]!=1)																										//si no hay flag de ret, calculo
            duty_alar_ch[NAL] = dtonoff(1000 - vxalar,duty_alar_ch[NAL]);      

       if(duty_alar_ch[NAL]==1000)																						//y si llegue a 1000, pongo flag
		        flagalar[NAL] = 1;
    break;

    case _dret:																																//def con ret
	      if (FlagCleaner==1)
	          flagalar[NAL] = 0;	
	      
       if(flagalar[NAL]!=1)
            duty_alar_ch[NAL] = dtonoff(vxalar,duty_alar_ch[NAL]);
       
       if(duty_alar_ch[NAL]==1000)
		        flagalar[NAL] = 1;
    break;

    case _dbloq:																															//bloqueada
            duty_alar_ch[NAL] = dtonoff(vxalar,duty_alar_ch[NAL]);						//calculo

       if(duty_alar_ch[NAL]==0)
    		    flagalar[NAL] = 1;
       
       if(flagalar[NAL] == 0)
            duty_alar_ch[NAL]=0;
    break;

    case _RET:
       #ifdef RUVAC
            duty_alar_ch[NAL]=((long)(ValRet - PRom[R_AAnlLow+NAL]))*1000/(PRom[R_AAnlHi+NAL]-PRom[R_AAnlLow+NAL]); 
       #else
            duty_alar_ch[NAL]=((long)(VAL_RET - PRom[R_AAnlLow+NAL]))*1000/(PRom[R_AAnlHi+NAL]-PRom[R_AAnlLow+NAL]); 
			 #endif
			 if(duty_alar_ch[NAL]>1000)
			      duty_alar_ch[NAL]=1000;
			 else if(duty_alar_ch[NAL]<0)
			      duty_alar_ch[NAL]=0;
			 break;
			 
    default:
			 duty_alar_ch[NAL] = 0;    //Alarma en "NO"
       break; 
 };

#ifdef VF
  if(flagAlarmaEoC[0]==TRUE){
//Calculo valor de la salida de alarma
  PWM_SetRatio16(duty_alar_ch[NAL],outa1);  

	if(PRom[R_Talar1+NAL]==_RET || hisalar<=0)
     setPWM_period(PWM_Anl,outa1);		
  else setPWM_period(PRom[R_Per+NCN],outa1);
 
  }else if(VFstatus == ENDVF)  //si estoy en "end" y no es EoC desconecto la alarma
    PWM_SetRatio16(0,outa1);
  else{
   PWM_SetRatio16(duty_alar_ch[NAL],outa1);  

	if(PRom[R_Talar1+NAL]==_RET || hisalar<=0)
     setPWM_period(PWM_Anl,outa1);		
  else setPWM_period(PRom[R_Per+NCN],outa1); 
  }

#else
   PWM_SetRatio16(duty_alar_ch[NAL],outa1);  

	if(PRom[R_Talar1+NAL]==_RET || hisalar<=0)
     setPWM_period(PWM_Anl,outa1);		
  else setPWM_period(PRom[R_Per+NCN],outa1);
#endif
/**********************************************************************************************************/
 /*CALCULO DE LA SALIDA DE ALARMA 2*/

#if ((ALARMAS_CH1 >= 2) || (ALARMAS_CH1 ==1 && CANTIDAD_CANALES == 2)) && !defined (RPM)

#undef NAL
#undef NCN

#if CANTIDAD_CANALES == 2
    #ifdef SP_EXTERNO
      #define NAL 1                                         //variables y parametros de canal1
      #define NCN 0
    #else
      #define NAL 1                                         //variables y parametros de canal1
      #define NCN 1
    #endif
#else
#define NAL 1                              //selecciono la alarma
#define NCN 0

#endif


/*Este modulo calcula la señal de alarma*/

switch (t_sp_alarma){				
    case _rel:														
		    vxalar = cprop(hisalar, spcont+spalar, 0, vx, 0, 0,res);                  //relativa, los dos sp      
    break;
 
    case _abs:														
		    vxalar = cprop(hisalar,spalar,0,vx,0,0,res);      												//absoluta solo el sp de al
		break;

    case _refr:																																//refrig, los dos sp con In y Dr
		    vxalar = cprop(hisalar,spcont+spalar,kint,vx,suminteg[0+NCN],(int)buffer_deriv[0+NCN],res);   		
    break;
          
    case _ban:														
        if(vx > spcont)         
            vxalar =cprop(hisalar,spcont+spalar/2,0,vx,0,0,res);									//banda, arriba como rel
        else
            vxalar =1000 - cprop(hisalar,spcont-spalar/2,0,vx,0,0,res);						//abajo idem pero polarid inv
    break;
    
 #ifdef VF    
    case _eoc:
        flagAlarmaEoC[0]=FALSE;
        flagAlarmaEoC[1]=TRUE;
        flagAlarmaEoC[2]=FALSE;
        if(VFstatus == ENDVF && tecla_d!=1){
            vxalar = 30000;
            flagAlarma=TRUE;
        }else
            vxalar = -30000;     
     break;
#endif     

#ifdef programador
    case _seg:							                                                  //segmento							
        if(SegmentoActual[0+NCN]+OFFSET_SEG1==spalar && PRom[R_Programa]!=NO_PROGRAMA)
            vxalar = 30000;	                                                  //Siempre on
        else
            vxalar = -30000;                                                  //Siempre off
    break;
    
    case _eoc:
        if(isProgFinish(0+NCN))																								//fin de ciclo
            vxalar = 30000;	 
        else
            vxalar = -30000; 
    break;
#endif

    default:
		    vxalar = vx-spalar;
    break;
    };


/*Este modulo limita los valores a int*/

if (vxalar > 30000)
    vxalar = 30000;
if (vxalar < -30000)
    vxalar = -30000;


/*Este modulo calcula el valor del duty*/

 switch (t_sal_alarma){				
    case _e:														
        if (hisalar > 0)													                            //exceso, al reves que la señal de cal
            duty_alar_ch[NAL] = dtprop   (1000 - vxalar,0,1000);
        else 
            duty_alar_ch[NAL] = dtonoff(1000 - vxalar,duty_alar_ch[NAL]);
    break;    

    case _d:														
        if (hisalar > 0)																											//defecto
            duty_alar_ch[NAL] = dtprop   (vxalar,0,1000);                                     
        else 
            duty_alar_ch[NAL] = dtonoff(vxalar,duty_alar_ch[NAL]);
    break;

    case _eret:																																//exc con ret
	      if (FlagCleaner==1)																										//si fue pedido limpio flag
	          flagalar[NAL] = 0;	 

       if(flagalar[NAL]!=1)																										//si no hay flag de ret, calculo
            duty_alar_ch[NAL] = dtonoff(1000 - vxalar,duty_alar_ch[NAL]);      

       if(duty_alar_ch[NAL]==1000)																						//y si llegue a 1000, pongo flag
		        flagalar[NAL] = 1;
    break;

    case _dret:																																//def con ret
	      if (FlagCleaner==1)
	          flagalar[NAL] = 0;	
	      
       if(flagalar[NAL]!=1)
            duty_alar_ch[NAL] = dtonoff(vxalar,duty_alar_ch[NAL]);
       
       if(duty_alar_ch[NAL]==1000)
		        flagalar[NAL] = 1;
    break;

    case _dbloq:																															//bloqueada
            duty_alar_ch[NAL] = dtonoff(vxalar,duty_alar_ch[NAL]);						//calculo

       if(duty_alar_ch[NAL]==0)
    		    flagalar[NAL] = 1;
       
       if(flagalar[NAL] == 0)
            duty_alar_ch[NAL]=0;
    break;

    case _RET:
       #ifdef RUVAC
            duty_alar_ch[NAL]=((long)(ValRet - PRom[R_AAnlLow+NAL]))*1000/(PRom[R_AAnlHi+NAL]-PRom[R_AAnlLow+NAL]); 
       #else
            #if CANTIDAD_CANALES>1
            duty_alar_ch[NAL]=((long)(VAL_RET - PRom[R_AAnlLow+NAL]))*1000/(PRom[R_AAnlHi+NAL]-PRom[R_AAnlLow+NAL]); 
            #else
            duty_alar_ch[NAL]=((long)(VAL_RET - PRom[R_AAnlLow]))*1000/(PRom[R_AAnlHi]-PRom[R_AAnlLow]);
            #endif 
			 #endif
			 if(duty_alar_ch[NAL]>1000)
			      duty_alar_ch[NAL]=1000;
			 else if(duty_alar_ch[NAL]<0)
			      duty_alar_ch[NAL]=0;
			 break;
			 
    default:
			 duty_alar_ch[NAL] = 0;    //Alarma en "NO"
       break; 
 };

#ifdef VF
  if(flagAlarmaEoC[1]==TRUE){ 
//Calculo valor de la salida de alarma
  PWM_SetRatio16(duty_alar_ch[NAL],outa2);  

	if(PRom[R_Talar1+NAL]==_RET || hisalar<=0)
     setPWM_period(PWM_Anl,outa2);		
  else setPWM_period(PRom[R_Per+NCN],outa2);
  }else if(VFstatus == ENDVF)
    PWM_SetRatio16(0,outa2);
  else{
    PWM_SetRatio16(duty_alar_ch[NAL],outa2);  

	if(PRom[R_Talar1+NAL]==_RET || hisalar<=0)
     setPWM_period(PWM_Anl,outa2);		
  else setPWM_period(PRom[R_Per+NCN],outa2);
  }
#else
   PWM_SetRatio16(duty_alar_ch[NAL],outa2);  

	if(PRom[R_Talar1+NAL]==_RET || hisalar<=0)
     setPWM_period(PWM_Anl,outa2);		
  else setPWM_period(PRom[R_Per+NCN],outa2);
#endif

#endif
/*******************************************************************************************************/
  /*CALCULO DE LA SALIDA DE ALARMA 3*/



#undef NAL
#undef NCN

#if (CANTIDAD_CANALES == 1 && ALARMAS_CH1 == 3)
#define NAL 2                                                                 //variables y parametros de canal1
#define NCN 0

#elif((defined (VPROP) && defined (DOS_ALARMAS) )) 
  #define NAL 1                                                                 //variables y parametros de canal1
  #define NCN 0
#endif

#ifdef NAL && NCN
//Este modulo calcula la señal de alarma

switch (t_sp_alarma){				
    case _rel:														
		    vxalar = cprop(hisalar, spcont+spalar, 0, vx, 0, 0,res);                  //relativa, los dos sp      
    break;
 
    case _abs:														
		    vxalar = cprop(hisalar,spalar,0,vx,0,0,res);      												//absoluta solo el sp de al
		break;

    case _refr:														
		    vxalar = cprop(hisalar,spcont+spalar,kint,vx,suminteg[0+NCN],0,res);      		//refrig, los dos sp con In y Dr
    break;
          
    case _ban:														
        if(vx > spcont)         
            vxalar =cprop(hisalar,spcont+spalar/2,0,vx,0,0,res);									//banda, arriba como rel
        else
            vxalar =1000 - cprop(hisalar,spcont-spalar/2,0,vx,0,0,res);						//abajo idem pero polarid inv
    break;

#ifdef VF    
    case _eoc:
        flagAlarmaEoC[0]=FALSE;
        flagAlarmaEoC[1]=FALSE;
        flagAlarmaEoC[2]=TRUE;
        if(VFstatus == ENDVF && tecla_d!=1){
            vxalar = 30000;
            flagAlarma=TRUE;
        }else
            vxalar = -30000;     
     break;
#endif     

#ifdef programador
    case _seg:							                                                  //segmento							
        if(SegmentoActual[0+NCN]+OFFSET_SEG1==spalar && PRom[R_Programa]!=NO_PROGRAMA)
            vxalar = 30000;	                                                  //Siempre on
        else
            vxalar = -30000;                                                  //Siempre off
    break;
    
    case _eoc:
        if(isProgFinish(0+NCN))																								//fin de ciclo
            vxalar = 30000;	 
        else
            vxalar = -30000; 
    break;
#endif

    default:
		    vxalar = vx-spalar;
    break;
    };


//Este modulo limita los valores a int

if (vxalar > 30000)
    vxalar = 30000;
if (vxalar < -30000)
    vxalar = -30000;


//Este modulo calcula el valor del duty

 switch (t_sal_alarma){				
    case _e:														
        if (hisalar > 0)													                            //exceso, al reves que la señal de cal
            duty_alar_ch[NAL] = dtprop   (1000 - vxalar,0,1000);
        else 
            duty_alar_ch[NAL] = dtonoff(1000 - vxalar,duty_alar_ch[NAL]);
    break;    

    case _d:														
        if (hisalar > 0)																											//defecto
            duty_alar_ch[NAL] = dtprop   (vxalar,0,1000);                                     
        else 
            duty_alar_ch[NAL] = dtonoff(vxalar,duty_alar_ch[NAL]);
    break;

    case _eret:																																//exc con ret
	      if (FlagCleaner==1)																										//si fue pedido limpio flag
	          flagalar[NAL] = 0;	 

       if(flagalar[NAL]!=1)																										//si no hay flag de ret, calculo
            duty_alar_ch[NAL] = dtonoff(1000 - vxalar,duty_alar_ch[NAL]);      

       if(duty_alar_ch[NAL]==1000)																						//y si llegue a 1000, pongo flag
		        flagalar[NAL] = 1;
    break;

    case _dret:																																//def con ret
	      if (FlagCleaner==1)
	          flagalar[NAL] = 0;	
	      
       if(flagalar[NAL]!=1)
            duty_alar_ch[NAL] = dtonoff(vxalar,duty_alar_ch[NAL]);
       
       if(duty_alar_ch[NAL]==1000)
		        flagalar[NAL] = 1;
    break;

    case _dbloq:																															//bloqueada
            duty_alar_ch[NAL] = dtonoff(vxalar,duty_alar_ch[NAL]);						//calculo

       if(duty_alar_ch[NAL]==0)
    		    flagalar[NAL] = 1;
       
       if(flagalar[NAL] == 0)
            duty_alar_ch[NAL]=0;
    break;

    case _RET:
       #ifdef RUVAC
            duty_alar_ch[NAL]=((long)(ValRet - PRom[R_AAnlLow+NAL]))*1000/(PRom[R_AAnlHi+NAL]-PRom[R_AAnlLow+NAL]); 
       #else
			      duty_alar_ch[NAL]=((long)(VAL_RET - PRom[R_AAnlLow]))*1000/(PRom[R_AAnlHi]-PRom[R_AAnlLow]); 
			 #endif
			 if(duty_alar_ch[NAL]>1000)
			      duty_alar_ch[NAL]=1000;
			 else if(duty_alar_ch[NAL]<0)
			      duty_alar_ch[NAL]=0;
			 break;
			 
    default:
			 duty_alar_ch[NAL] = 0;       //Alarma en "NO"
       break; 
 };

 #ifdef VF
  if(flagAlarmaEoC[2]==TRUE){
  //Calculo valor de la salida de alarma
    PWM_SetRatio16(duty_alar_ch[NAL],outa3);  

	  if(PRom[R_Talar1+NAL]==_RET || hisalar<=0)
      setPWM_period(PWM_Anl,outa3);		
    else setPWM_period(PRom[R_Per+NCN],outa3);
  }else if(VFstatus == ENDVF)
    PWM_SetRatio16(0,outa3);
   else{
    PWM_SetRatio16(duty_alar_ch[NAL],outa3);  

	  if(PRom[R_Talar1+NAL]==_RET || hisalar<=0)
      setPWM_period(PWM_Anl,outa3);		
    else setPWM_period(PRom[R_Per+NCN],outa3);
   }
  
#else
 PWM_SetRatio16(duty_alar_ch[NAL],outa3);  

	  if(PRom[R_Talar1+NAL]==_RET || hisalar<=0)
      setPWM_period(PWM_Anl,outa3);		
    else setPWM_period(PRom[R_Per+NCN],outa3);
#endif    

#endif

  FlagCleaner = 0;              //lo limpio despues de procesar todos los flags

			#ifdef _TEST1
      Bit2_ClrVal();
  		#endif
  		
  		
      AD_proc=FALSE;			// deshabilitar proceso de resultado de AD
      Mostrar_Proc=TRUE;  //Mostrar los valores en la pantalla principal (se procesa en Main Handler)

#ifndef RPM 
  }
#endif
/////////////Acciones de tiempo//////////////////
#ifdef jony_25_06
#ifdef programador
if(Prog_actualizar)
  Prog_ActualizarSetPoints();
#if defined( jony_15_08) && defined(programador)
if(flagBorrado){
  flagBorrado=FALSE;
  BorrarPagApagado();  
  HabilitarAccionPagApagado();
}
#endif
#endif
#endif
////////////////////////////////////////////////////////////////////////	

////////////////////////Comunicacion /////////////////////
/*    if (AS1_Tx==TRUE)						//Hay algo para enviar??
	  AS1_TxChar();						// Enviar
		
    if ( AS1_RecvChar(&msn[Step])==ERR_OK)  //Recibi algo????		
	  AS1_OnRxChar();											// Procesar
  */
  
///////// veo si vuelvo a la pantalla principal/////////////
	  
	  #ifdef VF_PROG    
	  if(Flag1 && PtrTmp!=&Principal1.DirProc && (VFstatus == ENDVF || PtrTmp!=&Principal1_VF_PROG.DirProc))		// PAsaron 10 segundos fuera de la pantalla principal y estando en FIN??
	  {
	   if(VFstatus != ENDVF){
	     PasarASCII("    ",1);   //borro la pantalla una ves 
	     ResetScroll();
	     PtrTmp=&Principal1_VF_PROG.DirProc;											//Volver a la pantalla principal
	     FstTime=TRUE;
	   }else{
	     PasarASCII("    ",1);   //borro la pantalla una ves 
	     ResetScroll();
	     PtrTmp=&Principal1.DirProc;											//Volver a la pantalla principal
	     FstTime=TRUE;
	   }
	  }
	  #else
	  if(Flag1 && PtrTmp!=&Principal1.DirProc)		// PAsaron 10 segundos fuera de la pantalla principal??
	  { 
	    PtrTmp=&Principal1.DirProc;											//Volver a la pantalla principal
	    FstTime=TRUE;
	  }
	  #endif     	 
  
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

>>>>>>> .r7
