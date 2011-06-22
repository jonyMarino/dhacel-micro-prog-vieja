#ifndef _BoxesStruct
#define _BoxesStruct

#include "PE_Types.h"
#include "stddef.h"
#include "Mydefines.h"
#include "SensorRPM.h"

typedef void (**PunteroF)(void);
#define NO_FUNCTION NULL
/* definicion de la estructura de dato de los Boxes*/         
/*******************************************************/
typedef const struct{
  int * Fdir;							      /* Direccion del parámetro a Escribir*/
  byte (*OnWrite)(int,byte);		/* Funcion a ejecutar Luego de la escritura del parámetro*/
  int * Inf,* Sup;			        /* Limites inferior y superior */
  byte chan;										// canal
}TDato;
			
/* definicion de boxes con variables de texto no en Rom*/
/*******************************************************/
typedef struct{
		void(*DirProc)(void);		/* funcion que procesa al box*/
		TDato * sDato;             /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
		char * TxtDpy;				  /* texto del titulo */
    const char  **TxtDpy0;
 		int * VdeNavegacion;	/*Variable que determina los proximos estados. if FALSE, depende del primero*/		
		const PunteroF * Teclar; //Proximo estado toque rapido
		PunteroF   Teclaf; //Proximo estado toque mantenido

}Textual;

/* definicion de boxes con variables numericas no en Rom*/
/********************************************************/
typedef struct{
		void(*DirProc)(void);		    /* funcion que procesa al box*/
		TDato * sDato;                 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/	
		char * TxtDpy;				    /* texto del titulo */
		byte Dot;					          /* punto decimal a enc */
		int * VdeNavegacion;			/*Variable que determina los proximos estados. if FALSE, depende del primero*/		
		PunteroF * Teclar;			//Proximo estado toque rapido
		PunteroF   Teclaf;			//Proximo estado toque mantenido		 	
}Numerico;


/* definicion de boxes con variables numericas no en Rom*/
/********************************************************/
typedef struct{
		void(*DirProc)(void);		    /* funcion que procesa al box*/
		int * Dato;                 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/	
		char * TxtDpy;				    /* texto del titulo */
		byte Dot;					          /* punto decimal a enc */
		int * VdeNavegacion;			/*Variable que determina los proximos estados. if FALSE, depende del primero*/		
		const PunteroF * Teclar;			//Proximo estado toque rapido
		PunteroF   Teclaf;			//Proximo estado toque mantenido		 	
}NumericoRO;

/* definicion de boxes de titulo */
/*********************************/

typedef struct{
		void(*DirProc)(void);		    /* funcion que procesa el box. DirProc es un puntero a una funcion que no tiene parametros y que regresa void */
		char * TxtDpy;				      /* texto del titulo */
		char * TxtDpySup;			    /* texto del titulo */
		int * VdeNavegacion;			/*Variable que determina los proximos estados. if FALSE, depende del primero*/
		PunteroF * Teclar;			/* Toque Rapido*/
		PunteroF   Teclaf;			/* Toque Prolongado*/
}Titulo;

/* Puntero a los Boxes*/
typedef union{
	Numerico * Num;
	NumericoRO * NumRO;
	Textual	* Txt;
	Titulo * Tit;	
}TPtrBox;
///VALORES DE LOS TEXTOS////////////
//////////AUTO SINTONIA/////////////
#define St_OFF 0
#define Stc 1
#define St_Ok 4
#define St_Err 5

//////////TIPO SEGMENTO/////////////
#define			End		0
#define     rMPt  1
#define     rMPV	2								
#define			MSt		3
#define			SteP  4
#define			JMP		5

//////////TIPO TEMPERATURA INICIAL///
#define SEt 0
#define AMb 1	

//////////TIPO DE CONTROL/////////////
#define _onoff 0
#define _pid 1								

///////////TOLERANCIAS//////////////
      
#define	OFF  0
#define	Lo   1
#define	Hi   2
#define	bAnd 3			
#define	no   4

//////////CONDICION DE EMERGENCIA///
#define cc   0
#define rS   1
#define rP   2
#define EndP 3
#define no   4

//////////ALARMAS//////////////////
#ifdef _APARATO_VIEJO
#define _e 0
#define _d 1
#define _eret 2
#define _dret 3
#define _dbloq 4
#define _RET 5
#else
#define _e 0
#define _d 1
#define _eret 2
#define _dret 3
#define _dbloq 4
#define _eprop 5
#define _dprop 6
#define _RET 7
#endif
///////////////////////////////////

//////////////////////////////////////////////////////////////////

//////////////////// INDICES DE LOS PARAMETROS DE LOS BOXES ///////////////////

// PARAMETROS QUE PUEDEN DEPENDER DE LA CANTIDAD DE CANALES(SUMAR 4 ENTRE ELLOS)
#define R_SetPoint    0
#define R_AL          R_SetPoint+2	//2 
#define R_H1          R_AL+3        //5
#define R_HA1         R_H1+2        //7
#define R_Per         R_HA1+3       //10
#define R_Sensor      R_Per+2       //12
#define R_Decimales   R_Sensor+2    //14
#define R_Offset      R_Decimales+2 //16
#define R_Gan         R_Offset+2    //18
#define R_ACT         R_Gan+2       //20
#define R_AGT         R_ACT+2       //22
#define R_ACP         R_AGT+2       //24
#define R_AGP         R_ACP+2       //26
#define R_AAnlLow     R_AGP+2       //28
#define R_AAnlHi      R_AAnlLow+2   //30
#define R_Lim_Inf     R_AAnlHi+2    //32
#define R_Lim_Sup     R_Lim_Inf+2		//34		
#define R_Tcontrol1   R_Lim_Sup+2   //36
#define R_Reset       R_Tcontrol1+2 //38
#define R_IN1         R_Reset+2     //40
#define R_DR1         R_IN1+2       //42
#define R_Pra         R_DR1+2       //44
#define R_Pot_Inf     R_Pra+3       //47
#define R_Pot_Sup     R_Pot_Inf+2   //49
#define R_Tsalcont    R_Pot_Sup+2   //51
#define R_Potman      R_Tsalcont+2  //53
#define R_Stn         R_Potman+2    //55
#define R_T_AL        R_Stn+2       //57
#define R_Talar1      R_T_AL+3      //60
#define R_Filtro1     R_Talar1+3    //63
#define Prox          R_Filtro1+2   //65

#ifdef programador

#define R_Programa    Prox
#define R_Segmento    R_Programa+2 
#define R_TieSeg      R_Segmento+2
#define Prox_P        R_TieSeg+2

#endif

#ifdef CCAL 

#define R_Pss         Prox
#define R_Tss         R_Pss+1
#define R_SPlim       R_Tss+1
#define R_SPst        R_SPlim+1
#define R_SPsb        R_SPst+1
#define R_Indf        R_SPsb+1
#define R_Drdf        R_Indf+1
#define R_Abdf        R_Drdf+1
#define R_SPlimst     R_Abdf+1
#define Prox_C     R_SPlimst+1

#endif

#ifdef _PRINTER

#if defined (CCAL)
#define R_PrnIntervalo  Prox_C
#elif defined (programador)
#define R_PrnIntervalo  Prox_P
#else 
#define R_PrnIntervalo  Prox
#endif

#define R_PrnAnio R_PrnIntervalo+1
#define R_PrnMes R_PrnAnio+1
#define R_PrnDia R_PrnMes+1
#define R_PrnHora R_PrnDia+1
#define R_PrnHabilitado R_PrnHora+1

#define Prox_Pr R_PrnHabilitado+1

#endif

#ifdef RPM

#ifdef _PRINTER
#define R_Rpm Prox_Pr
#else 
#define R_Rpm  Prox
#endif
 
#define R_Fil R_Rpm+1
#define R_AJU R_Fil+1
#define Prox_R R_AJU+1

#endif

#ifdef VF

#ifdef _PRINTER
  #define R_MVF  Prox_Pr
#else
  #define R_MVF  Prox
#endif

#define R_LIMs  R_MVF+1
#define R_LIVel  R_LIMs+1
#define Prox_VF R_LIVel+1

#endif


#ifdef INTEGRADOR

#define R_RSTL    Prox
#define R_UND     R_RSTL+1
#define R_RSL     R_UND+1
#define Prox_INT  R_RSL+1

#endif

#ifdef ADQ

   #ifdef programador
    #define R_PrnAnio Prox_P
   #elif defined (VF)
    #define R_PrnAnio Prox_VF
   #else
    #define R_PrnAnio Prox
   #endif
   
   #define R_PrnMes R_PrnAnio+1
   #define R_PrnDia R_PrnMes+1
   #define R_PrnHora R_PrnDia+1
   #define R_AdqHabilitado R_PrnHora+1
   #define R_AdqBorrado R_AdqHabilitado+1
   #define R_AdqTranfer R_AdqBorrado+1
   #define R_AdqPeriodo R_AdqTranfer+1
   #define ProxAdq  R_AdqPeriodo+1
   
#endif


#if defined (programador) && !defined (ADQ)
#define R_Topn  Prox_P
#elif defined (CCAL)
#define R_Topn  Prox_C
#elif defined (_PRINTER) && !defined (VF) && !defined (RPM)
#define R_Topn  Prox_Pr
#elif defined (RPM)
#define R_Topn  Prox_R
#elif defined (VF)
#define R_Topn Prox_VF
#elif defined (INTEGRADOR)
#define R_Topn Prox_INT
#elif defined (ADQ)
#define R_Topn ProxAdq
#else 
#define R_Topn  Prox
#endif

#define R_COM  R_Topn+1
#define R_Dbnd R_COM+1
#define R_SetC R_Dbnd+1
#define R_Ver  R_SetC+1
#define R_Id   R_Ver+1
#define R_ATA  R_Id+1
  



///////////VERSION/BLOQUEOS////////
typedef enum{BL1,BL2,BL3,BL4,VER_DIF}TVersion;

////////////////////Boxes info //////////////////
#define DECIMALES_CANAL4   255
#define DECIMALES_CANAL3   254
#define DECIMALES_CANAL2   253
#define DECIMALES_CANAL1   252

////////////////////////////////////////////

extern volatile const int PRom[];

#ifdef VF_PROG
extern const NumericoRO Principal1;
#else
extern const Numerico Principal1;
#endif

extern const Numerico C1SP;

#ifdef VF_PROG
extern const Numerico Principal1_VF_PROG;
#endif

#ifdef RPM
 extern struct SensorRpm sensorRpm;
 extern const SensorRpmConf config;
#endif

#if defined(_PRINTER) || defined(ADQ)

#include "BTFechaPersistente.h"
 #ifdef _PRINTER 
 
  #include "ManejadorImpresionPersistente.hpp"
  
  #pragma CONST_SEG PARAMETERS_PAGE
  extern volatile const struct BTFPConf bTConf;
  extern  ManejadorImpresionPersistente mi;
  extern const Textual boxPrnHabilitado;
  extern volatile const struct MIPConf confMI;
  
  #pragma CONST_SEG DEFAULT 
  
 #endif
 
#ifdef ADQ   
#pragma CONST_SEG PARAMETERS_PAGE 
extern volatile const struct BTFPConf bTConf;
#pragma CONST_SEG DEFAULT 
#endif
 
#if defined(DS1307)
extern struct BaseTiempoDS1307 baseTiempo;
#endif

#endif

#ifdef DOBLE_SP
extern const Numerico DSP;
#endif
#ifdef programador
extern const Numerico Segmento1;
#endif
extern const TDato Parametros[R_ATA+1];
extern int Lim_Dec[CANTIDAD_CANALES];
extern int Lim_Inf_ch[CANTIDAD_CANALES];
extern int Lim_Sup_ch[CANTIDAD_CANALES];
extern int	 segmento_ingresado, temperatura_inicial,  tipo_segmento, temperatura_f,
	   tiempo, tolerancia,tolerancia_gral, condicion_emer,condicion_emer_gral,tipo_tolerancia,tipo_tolerancia_gral,tipo_temperatura_inicial;
extern int next_tiempo,next_tol,next_prog;
extern int Lim_Segmento1;	 /* Para el prog nuevo, pantalla de ingreso de programa*/
extern int Lim_Segmento2[CANTIDAD_SAL_CONTROL];  /* Limite para el segmento que se puede cambiar mientras corre*/
extern int Lim_TieSeg[CANTIDAD_SAL_CONTROL];
extern const PunteroF R_Emergencia[2];

extern int Lim_Inf_ch[CANTIDAD_CANALES],Lim_Sup_ch[CANTIDAD_CANALES];
extern int Lim_Dec[CANTIDAD_CANALES];
extern const int Lim_m9999;
extern const int Lim_m1999;
extern const int Lim_m1;
extern const int Lim_0;
extern const int Lim_1;
extern const int Lim_2;
extern const int Lim_3;
extern const int Lim_4;
extern const int Lim_5;
extern const int Lim_6;
extern const int Lim_7;
extern const int Lim_8;
extern const int Lim_9;
extern const int Lim_10;
extern const int Lim_24;
extern const int Lim_80;
extern const int Lim_255;
extern const int Lim_600;
extern const int Lim_1000;
extern const int Lim_9999;
extern const int Lim_1300;
extern const int Lim_1500;
extern const int Lim_999;
#ifdef jony_28_06
extern const int Lim_32767;
#endif

extern const TDato Parametros[R_ATA+1];
extern const Titulo Calibracion;
extern const Titulo Setting;
extern const Numerico Codigo1;
extern const Titulo alarmas;
#endif
