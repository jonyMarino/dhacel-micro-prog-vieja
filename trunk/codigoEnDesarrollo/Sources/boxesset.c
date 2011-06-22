/* PARAMETROS de "set" */
/***********************/
#include "Mydefines.h"
#include "cnfbox.h"
#include "boxes.h"
#include "PE_Types.h"
#include "Sensores.h"
#include "boxesset.h"
#include "display.h"
#include "In1.h"

#pragma CONST_SEG DEFAULT 
#pragma STRING_SEG DEFAULT

extern const Textual C1CN;
extern const Textual C1TSC;
extern const Textual C1AL1;
extern const Textual C1AL2;
extern const Textual C1AL3;
extern const Textual C1TAL1;
extern const Textual C1TAL2;
extern const Textual C1TAL3;
extern const Numerico Decimales1;
extern const Numerico Decimales2;

extern const Numerico AL1;

#if  CANTIDAD_CANALES>1
extern const Textual C2CN;
extern const Textual C2TSC;
extern const Textual C2AL;
extern const Textual C2TAL;
#endif

extern const Textual Version1;
extern const Numerico Id;
extern const Numerico SetC;

extern const Numerico Vrmp1;

#ifndef programador
 extern const Textual Estado;
#endif
extern const Numerico Topn;
extern const Numerico Dbnd;

#ifdef jony_28_06
extern const Numerico ModMain;
#endif


/*Modo Control 1*/		
const char * const T_Control[2]={
      "On-O",									
			"Pid ",
			};

const Textual C1CN=
      {
      TxtHandler,						                  /* funcion que procesa al box*/
			&Parametros[R_Tcontrol1],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"C  1",									                //nombre display
			T_Control,                       // Array donde estan los textos
			NULL,							                      //parametro que modifica.
			(PunteroF*)&C1TSC.DirProc,NULL					//Proximos estados
			};

/*Modo Salida 1*/		

#ifdef RPM
const char * const T_Salcont[3]={
      "drct",									
			"InV ",
			"MAn "
			};

#define _drct 0
#define _inv 1
#define _man 2



#else
const char * const T_Salcont[3]={
      "cAL ",									
			"rEF ",
			"MAn "
			};

#define _cal 0
#define _ref 1
#define _man 2

#endif

const Textual C1TSC=
      {
      TxtHandler,						                  /* funcion que procesa al box*/
			&Parametros[R_Tsalcont],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"C  1",									                //nombre display
			T_Salcont,                       // Array donde estan los textos
			NULL,							                      //parametro que modifica.
			#ifdef COFACO   
			(PunteroF*)&C1TAL1.DirProc,NULL					//Proximos estados
			#else
			(PunteroF*)&C1AL1.DirProc,NULL						//Proximos estados
			#endif
			};

/*Modo Alarma 1*/		

//dan1001
#ifdef _APARATO_VIEJO
/*Modos Alarma aparato viejo*/		
const char  * const T_Alar[7]={
      "E   ",									
			"d   ",
			"ErEt",
			"drEt",
			"dblk",
			"rEt ",
			"no  ",
			};
#define _e 0
#define _d 1
#define _eret 2
#define _dret 3
#define _dbloq 4
#define _RET 5
#define _no 6
#else
/*Modo Alarma aparato nuevo*/		
const char * const T_Alar[9]={
      "E   ",									
			"d   ",
			"ErEt",
			"drEt",
			"dbLK",
      "EPrP",
      "dPrP",
      "rEt ",
      "no  "
			};
#define _e 0
#define _d 1
#define _eret 2
#define _dret 3
#define _dbloq 4
#define _eprop 5
#define _dprop 6
#define _RET 7
#define _no 8
#endif

//dan1001


const Textual C1AL1=
      {
      TxtHandler,						                //funcion que procesa al box*/
			&Parametros[R_Talar1],											//direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			#ifdef COFACO
			"AL 1",
			#else
			"A  1",
			#endif									              //nombre display
			T_Alar,                        //Array donde estan los textos
			NULL,							                    //parametro que modifica.

 #ifndef COFACO 
  #if ALARMAS_CH1 > 1
			(PunteroF*)&C1AL2.DirProc,NULL				//Proximos estados
  #else
			(PunteroF*)&C1TAL1.DirProc,NULL				//Proximos estados
  #endif
 #else
   (PunteroF*)&Version1.DirProc,NULL			//Proximos estados
 #endif 
			};

/*Modo Alarma 2*/		
const Textual C1AL2=
      {
      TxtHandler,						                //funcion que procesa al box*/
			&Parametros[R_Talar1+1],											//direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
		  #ifdef COFACO
		  "AL 2",
		  #else
			"A  2",									              //nombre display
			#endif
			T_Alar,                        //Array donde estan los textos
			NULL,							                    //parametro que modifica.
#if ALARMAS_CH1 > 2
			(PunteroF*)&C1AL3.DirProc,NULL				//Proximos estados
#else
		#ifdef COFACO
		  (PunteroF*)&Decimales1.DirProc,NULL				//Proximos estados
		#else
			(PunteroF*)&C1TAL1.DirProc,NULL				//Proximos estados
    #endif
#endif                           
			};

/*Modo Alarma 3*/		
const Textual C1AL3=
      {
      TxtHandler,						                //funcion que procesa al box*/
			&Parametros[R_Talar1+2],											//direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"A  3",									              //nombre display
			T_Alar,                        //Array donde estan los textos
			NULL,							                    //parametro que modifica.
			(PunteroF*)&C1TAL1.DirProc,NULL				//Proximos estados
			};


/* Tipo de alarma 1*/
/*Tipo Alarma 1*/		
const char * const Alarmas[6]={
	    "rEL ",									
			"AbS ",
			"rEFr",
			"bAn ",
			"SG  ",
			"EoC ",
};

#define _rel 0
#define _abs 1
#define _refr 2
#define _ban 3
#define _seg 4
#define _eoc 5


const Textual C1TAL1=
      {TxtHandler,						            /* funcion que procesa al box*/
			&Parametros[R_T_AL],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
		  #ifdef COFACO
			"A  1",
			#else
			"AL 1",									            //nombre display
			#endif
			Alarmas,
			NULL,						                    //parametro que modifica.



  #if ALARMAS_CH1 > 1
  			(PunteroF*)&C1TAL2.DirProc,NULL				//Proximos estados
  #else


//  			(PunteroF*)&Decimales1.DirProc,NULL				//Proximos estados
     #ifndef COFACO
			#if CANTIDAD_CANALES >1
		  (PunteroF*)&C2TSC.DirProc,NULL			//Proximos estados	
			#else
			(PunteroF*)&Version1.DirProc,NULL			//Proximos estados
      #endif
     #else
       (PunteroF*)&C1AL1.DirProc,NULL				//Proximos estados
     #endif


  #endif


			};

/*Tipo Alarma 2*/		
const Textual C1TAL2=
      {TxtHandler,						            /* funcion que procesa al box*/
			&Parametros[R_T_AL+1],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			#ifdef COFACO
			"A  2",
			#else
			"AL 2",
			#endif									            //nombre display
			Alarmas,
			NULL,						                    //parametro que modifica.
#if ALARMAS_CH1 > 2
			(PunteroF*)&C1TAL3.DirProc,NULL				//Proximos estados
#else                     
		#ifdef COFACO
		  (PunteroF*)&C1AL1.DirProc,NULL				//Proximos estados
		#else	
			(PunteroF*)&Decimales1.DirProc,NULL				//Proximos estados
    #endif
#endif
			};

/*Tipo Alarma 3*/		
const Textual C1TAL3=
      {TxtHandler,						            /* funcion que procesa al box*/
			&Parametros[R_T_AL+2],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AL 3",									            //nombre display
			Alarmas,
			NULL,						                    //parametro que modifica.
			//(PunteroF*)&Decimales1.DirProc,NULL				//Proximos estados
			(PunteroF*)&Version1.DirProc,NULL			//Proximos estados
			};


//dan
/*			#if CANTIDAD_CANALES>1
			(PunteroF*)&C2CN.DirProc,NULL				//Proximos estados
			#else
			(PunteroF*)&Version1.DirProc,NULL		//enter rap,enter mant
			#endif
*/
//dan
//			};

#if CANTIDAD_CANALES >1
/*Modo Control 2*/		
const Textual C2CN=
      {
      TxtHandler,						                  /* funcion que procesa al box*/
			&Parametros[R_Tcontrol1+1],										/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"C  2",									                //nombre display
			T_Control,                       // Array donde estan los textos
			NULL,							                      //parametro que modifica.
			(PunteroF*)&C2TSC.DirProc,NULL					//Proximos estados
			};
			
/*Modo Salida 2*/		
const Textual C2TSC=
      {
      TxtHandler,						                  /* funcion que procesa al box*/
			&Parametros[R_Tsalcont+1],										/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"C  2",									                //nombre display
			T_Salcont,                       // Array donde estan los textos
			NULL,							                      //parametro que modifica.
			(PunteroF*)&C2AL.DirProc,NULL						//Proximos estados
			};

/*Modo Alarma 2*/		
const Textual C2AL=
      {
      TxtHandler,						                /* funcion que procesa al box*/
			&Parametros[R_Talar1+1],										/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"A  2",									              //nombre display
			T_Alar,                        // Array donde estan los textos
			NULL,							                    //parametro que modifica.
			(PunteroF*)&C2TAL.DirProc,NULL				//Proximos estados
			};

/*Tipo Alarma 2*/		
const Textual C2TAL=
      {TxtHandler,						            /* funcion que procesa al box*/
			&Parametros[R_T_AL+1],										/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AL 2",									            //nombre display
			Alarmas,
			NULL,						                    //parametro que modifica.
			(PunteroF*)&Decimales1.DirProc,NULL		//Proximos estados
			};
#endif

			
/* decimales1*///ATENCION!!! Siempre mostrarlos luego de elegir el sensor
const Numerico Decimales1={
      NumHandler,					/* funcion que procesa al box*/
      &Parametros[R_Decimales],									/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"dec1",							//nombre display
			0,									/* punto decimal a enc */
			NULL,					//parametro que modifica.
			#if CANTIDAD_CANALES >1
		  (PunteroF*)&Decimales2.DirProc,NULL			//Proximos estados	
			#else
			(PunteroF*)&Version1.DirProc,NULL			//Proximos estados
      #endif
};

/* decimales2*///ATENCION!!! Siempre mostrarlos luego de elegir el sensor
const Numerico Decimales2={
      NumHandler,					/* funcion que procesa al box*/
      &Parametros[R_Decimales+1],				/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"dec2",							//nombre display
			0,									/* punto decimal a enc */
			NULL,					//parametro que modifica.
			(PunteroF*)&Version1.DirProc,NULL			//Proximos estados
			};
/* version */
/*Tipo Alarma 2*/		


 
static const char * const Bloqueos[5]={
		  "bL1 ",								 
			"bL2 ",
			"bL3 ",
		  "Pot ",
		  numver,
};

//#define POT   2



const Textual Version1=
      {TxtHandler,					      /* funcion que procesa al box*/
			&Parametros[R_Ver],									/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"VEr1",								              //nombre display
			Bloqueos,
			NULL,						                    //parametro que modifica.
			(PunteroF*)&Id.DirProc,NULL					//Proximos estados
			};
			


//Id///
const Numerico Id={
      NumHandler,
      &Parametros[R_Id],
			"id  ",
			0,
			NULL,

			#ifdef VPROP
			(PunteroF*)&Topn.DirProc,NULL
			#else
			(PunteroF*)&SetC.DirProc,NULL
			#endif

			};
			
/* Set Contraseña*/
/*****************/
const Numerico SetC={
      NumHandler,
      &Parametros[R_SetC],
			"SetC",
			0,
			NULL,
		//	#ifdef VF
			//(PunteroF*)&AL1.DirProc,
		//	#else
			(PunteroF*)&Principal1.DirProc,
			//#endif
			#ifndef jony_28_06
			NULL
			#else
			(PunteroF)&ModMain.DirProc
			#endif
			};

/* Set topen*/
/*****************/
const Numerico Topn={
      NumHandler,
      &Parametros[R_Topn],
			"toPn",
			0,
			NULL,
			(PunteroF*)&Dbnd.DirProc,
			};


 /* Set dband*/
/*****************/
const Numerico Dbnd={
      NumHandler,
      &Parametros[R_Dbnd],
			"dbnd",
			1,
			NULL,
			(PunteroF*)&SetC.DirProc,
			};



/* Para Cambiar Var a mostrar en dpy inferior*/
/*********************************************/
#ifdef jony_28_06
extern int SetPoint[];
extern int dutytmp;
 
  int DirPar=(int)&SetPoint[R_SetPoint+0];
 
  

const TDato Mod={
  &DirPar,
  NO_FUNCTION,
  &Lim_0,&Lim_32767,  
};
const Numerico ModMain={
      NumHandler,
      &Mod,
			"AMod",
			0,
			NULL,
			(PunteroF*)&Principal1.DirProc,
			NULL
};
			
#endif