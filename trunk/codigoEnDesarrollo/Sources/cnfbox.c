#ifndef _cnfbox_h
#define _cnfbox_h

#include "Mydefines.h"
#include "PE_Types.h"
#include "boxes.h"
#include "boxesprog.h"
#include "boxesadq.h"
#include "boxestun.h"
#include "boxescal.h"
#include "boxesset.h"
#include "cnfbox.h"
#include "Programador.h"
#include "boxescolcal.h"
#include "vfboxes.h"
#include "Sensores.h"
#include "boxesRPM.h"

#ifdef _PRINTER
  #include "ManejadorImpresionPersistente.hpp"
  #include "MIPConf.h"
  #include "BTFPConf.h"
  #include "boxesPrn.h"
#endif

#ifdef ADQ
 #include "BTFPConf.h"
 #include "boxesPrn.h"
#endif
#include "IntegradorBoxes.h"
#include "adq.h"

#pragma CONST_SEG DEFAULT 
#pragma STRING_SEG DEFAULT

extern const Textual Ccalstn;

extern const Textual C1STN;

#ifndef programador
extern const Textual Estado;
#endif
extern Titulo Colcal;

/* Definicion de las estructuras de los boxes de navegacion */
/************************************************************/
/*		void(*DirTask)(void);		funcion que ejecuta tareas especiales de c/box*/

///VARIABLES EXTERNAS QUE UTILIZAN LOS BOXES //////////////
extern int Estado_Adquisicion;															
extern volatile const int PRom[R_ATA+1];


////Las de Programacion/////////
int Lim_Segmento2[CANTIDAD_SAL_CONTROL];  /* Limite para el segmento que se puede cambiar mientras corre*/
int Lim_TieSeg[CANTIDAD_SAL_CONTROL]; 
/////////////////////////////////
int Lim_Inf_ch[CANTIDAD_CANALES],Lim_Sup_ch[CANTIDAD_CANALES];
int Lim_Dec[CANTIDAD_CANALES];
const int Lim_m9999=-9999;
const int Lim_m1999=-1999;
const int Lim_m1=-1;
const int Lim_0=0;
const int Lim_1=1;
const int Lim_2=2;
const int Lim_3=3;
const int Lim_4=4;
const int Lim_5=5;
const int Lim_6=6;
const int Lim_7=7;
const int Lim_8=8;
const int Lim_9=9;
const int Lim_10=10;
const int Lim_24=24;
const int Lim_80=80;
const int Lim_255=1000;

const int Lim_600=600;
const int Lim_1000=1000;
const int Lim_1300=1300;
const int Lim_1500=1500;
const int Lim_9999=9999;
const int Lim_999=999;
const int Lim_3600=3600;
#ifdef jony_28_06
const int Lim_32767=32767;
#endif

#ifdef prog_viejo
const int LimInf_Seg=1;
#else
const int LimInf_Seg=0;
#endif


const int Limite_sensor_Sup=NUM_SENSORES;
#if defined (_PRINTER) || defined(ADQ)
const int Lim_12 = 12;
const int Lim_31 = 31;
const int Lim_2006 = 2006;
const int Lim_2008 = 2008;
const int Lim_2099 = 2099;
const int Lim_2359 = 2359;
#endif
extern const Numerico C1SP102;

/************************************/
/* Definicion y armado de los boxes */
/************************************/
#ifdef VF_PROG
extern const NumericoRO Principal1;
#else
extern const Numerico Principal1;
#endif

extern const Titulo alarmas;

extern const Numerico C1SP;
extern const Numerico C1POT;
extern const Numerico AL1;
extern const Numerico AL2;
extern const Numerico AL3;
extern const Numerico P1;
extern const Numerico Segmento1;
extern const Numerico SegundosSegmento1;
#ifdef LOBOV
extern const NumericoRO Indicacion;
#endif

#ifdef VF_PROG
 extern const Numerico Principal1_VF_PROG;
#endif

#if  CANTIDAD_CANALES>1
extern const Numerico C2SP;
extern const Numerico C2POT;
extern const Numerico C2AL1;
extern const Numerico P2;
#elif defined(DOBLE_SP)
 extern const Numerico DSP;
#endif

extern const Numerico Codigo1;

#ifdef _PRINTER
extern const Titulo titImpresion;
extern const Textual boxPrnHabilitado;
#endif

#ifdef ADQ
extern const Titulo titAdq;
extern const Textual boxAdqHabilitado;
extern const Textual boxAdqBorrado;
extern const Textual boxAdqTranfer;
extern const Numerico boxAdqPeriodo;
#endif

extern const Titulo Sintonia;
extern const Titulo Calibracion;
extern const Titulo Setting;
extern const Titulo Limites;
extern const Numerico LimiteInferior1;
extern const Numerico LimiteSuperior1;
extern const Numerico PotenciaInferior1;
extern const Numerico PotenciaSuperior1;


#if  CANTIDAD_CANALES>1
extern const Numerico LimiteInferior2;
extern const Numerico LimiteSuperior2;
extern const Numerico PotenciaInferior2;
extern const Numerico PotenciaSuperior2;
#endif

#ifdef programador
extern const Titulo Programas;
#endif


#if defined(_PRINTER) || defined(ADQ)

int anioIngresado=0;
int mesIngresado=0;	

#ifdef _PRINTER
byte setIntervaloMIVista(int valor,byte chan){
  //return setIntervaloMI(&mi,valor);
   mi.setIntervalo(valor); 
   return 0; 
}

byte  setHabilitadoMIVista(int valor,byte chan){
  //return setHabilitadoMI(&mi,valor);
  mi.setHabilitado(valor);
  return 0;
}	

#endif

byte setAnioDT(int valor,byte chan){
  anioIngresado = valor;
  return setFecha(&baseTiempo,valor,1,1);  
}

byte setMesDT(int valor,byte chan){
  mesIngresado = valor;
  if(anioIngresado!=0)
    return setFecha(&baseTiempo,anioIngresado,valor,1); 

  return setFecha(&baseTiempo,bTConf.anio,valor,1);   
}

byte  setDiaDT(int valor,byte chan){
  int mes = (mesIngresado)?mesIngresado:bTConf.mes;
  int anio = (anioIngresado)?anioIngresado:bTConf.anio; 
  
  return setFecha(&baseTiempo,bTConf.anio,mes,valor); 
}

byte setHoraDT(int valor,byte chan){
  return setTiempo(&baseTiempo,valor/100,valor%100,0);
}


#endif					

#ifdef RPM
 byte  setPulsosPorVuelta(int valor,byte chan){
   //sensorRpm.conf.pulsosPorVuelta = valor;
     SenRpm_setPulsosPorVuelta(&sensorRpm,valor);
  return 0;
}	

byte  setDecimalesRpm(int valor,byte chan){
  // sensorRpm.conf.iDecimales = (fbyte)valor;
   EscribirWord((word)&PRom[R_Decimales],valor);
   Lim_Inf_ch[0]= 9999;
   Lim_Sup_ch[0]= 9999;
   SenRpm_setDecimales(&sensorRpm,valor);
  return 0;
}	

byte  setFiltro(int valor,byte chan){
   //sensorRpm.conf.iFiltro = (fbyte)valor;
   SenRpm_setFiltro(&sensorRpm,valor);
  return 0;
}	
#endif

const TDato Parametros[R_ATA+1]={
/*SP1(0)*/{&PRom[R_SetPoint], NO_FUNCTION, &PRom[R_Lim_Inf+0], &PRom[R_Lim_Sup+0], CHAN_1},	 //0
/*SP2*/{&PRom[R_SetPoint+1], NO_FUNCTION, &PRom[R_Lim_Inf+1], &PRom[R_Lim_Sup+1], CHAN_2},

#ifdef RPM
/*AL1(2) */{&PRom[R_AL], NO_FUNCTION,&Lim_0, &Lim_Sup_ch[0], CHAN_1},	
#else
/*AL1(2) */{&PRom[R_AL], NO_FUNCTION,&Lim_Inf_ch[0], &Lim_Sup_ch[0], CHAN_1},								 //4
#endif

/*AL2 */{&PRom[R_AL+1], NO_FUNCTION, &Lim_Inf_ch[0], &Lim_Sup_ch[0], CHAN_1},
/*AL3 */{&PRom[R_AL+2], NO_FUNCTION, &Lim_Inf_ch[0], &Lim_Sup_ch[0], CHAN_1},

/*H1(5)*/{&PRom[R_H1], NO_FUNCTION, &Lim_m1999, &Lim_9999, CHAN_1},									 
/*H2*/{&PRom[R_H1+1], NO_FUNCTION, &Lim_m1999, &Lim_9999, CHAN_2},

/*HA1(7)*/{&PRom[R_HA1], NO_FUNCTION, &Lim_m1999, &Lim_9999, CHAN_1},								 
/*HA2*/{&PRom[R_HA1+1], NO_FUNCTION, &Lim_m1999, &Lim_9999, CHAN_2},
/*HA3*/{&PRom[R_HA1+2], NO_FUNCTION, &Lim_m1999, &Lim_9999, CHAN_3},

/*PER1(10)*/{&PRom[R_Per],NO_FUNCTION,&Lim_0,&Lim_10,CHAN_1},															 //20
/*PER2*/{&PRom[R_Per+1],NO_FUNCTION,&Lim_0,&Lim_10,CHAN_2},

/*Sensor1(12)*/{&PRom[R_Sensor],A_Sensor,&Lim_0,&Limite_sensor_Sup,CHAN_1},										 //24
/*Sensor2*/{&PRom[R_Sensor+1],A_Sensor,&Lim_0,&Limite_sensor_Sup,CHAN_2},

#ifdef RPM
 /*DecimalesRPM(14)*/{&config.iDecimales,setDecimalesRpm,&Lim_0,&Lim_2,CHAN_1},
#else
/*Decimales1(14)*/{&PRom[R_Decimales],A_Decimales,&Lim_0, &Lim_Dec[0],CHAN_1},								 //28
#endif
/*Decimales2*/{&PRom[R_Decimales+1],A_Decimales,&Lim_0, &Lim_Dec[0],CHAN_2},

/*Offset1(16)*/{&PRom[R_Offset],NO_FUNCTION,&Lim_m9999,&Lim_9999,CHAN_1},											 //32
/*Offset2*/{&PRom[R_Offset+1],NO_FUNCTION,&Lim_m9999, &Lim_9999,CHAN_2},

/*Gan1(18)*/{&PRom[R_Gan],NO_FUNCTION,&Lim_m1999,&Lim_9999,CHAN_1},														 //36
/*Gan2*/{&PRom[R_Gan+1],NO_FUNCTION,&Lim_m1999,&Lim_9999,CHAN_2},	

/*ACT1(20) */{&PRom[R_ACT],NO_FUNCTION,&Lim_m1999,&Lim_9999,CHAN_1},													 //40
/*ACT2 */{&PRom[R_ACT+1],NO_FUNCTION,&Lim_m1999,&Lim_9999,CHAN_2},

/*AGT1(22)*/{&PRom[R_AGT],NO_FUNCTION,&Lim_m1999,&Lim_9999,CHAN_1},														 //44
/*AGT2*/{&PRom[R_AGT+1],NO_FUNCTION,&Lim_m1999,&Lim_9999,CHAN_2},

/*ACP1(24) */{&PRom[R_ACP],NO_FUNCTION,&Lim_m1999,&Lim_9999,CHAN_1},													 //48
/*ACP2 */{&PRom[R_ACP+1],NO_FUNCTION,&Lim_m1999,&Lim_9999,CHAN_2},

/*R_AGP(26)*/{&PRom[R_AGP],NO_FUNCTION,&Lim_m1999,&Lim_9999,CHAN_1},													 //52
/*R_AGP*/{&PRom[R_AGP+1],NO_FUNCTION,&Lim_m1999,&Lim_9999,CHAN_2},

/*AAnlLow1(28)*/{&PRom[R_AAnlLow],NO_FUNCTION,&Lim_m1999,&Lim_9999,CHAN_1},										 //56
/*AAnlLow2*/{&PRom[R_AAnlLow+1],NO_FUNCTION,&Lim_m1999,&Lim_9999,CHAN_2},

/*AAnlHi1(30)*/{&PRom[R_AAnlHi],NO_FUNCTION,&Lim_m1999,&Lim_9999,CHAN_1},											 //60
/*AAnlHi2*/{&PRom[R_AAnlHi+1],NO_FUNCTION,&Lim_m1999,&Lim_9999,CHAN_2},

/*LimInferior1(32)*/{&PRom[R_Lim_Inf+0],NO_FUNCTION,&Lim_Inf_ch[0],&PRom[R_Lim_Sup+0],CHAN_1},	 //64
/*LimInferior2*/{&PRom[R_Lim_Inf+1],NO_FUNCTION,&Lim_Inf_ch[0],&PRom[R_Lim_Sup+1],CHAN_2},

/*LimSuperior1(34)*/{&PRom[R_Lim_Sup],NO_FUNCTION,&PRom[R_Lim_Inf+0],&Lim_Sup_ch[0],CHAN_1},	 //68
/*LimSuperior2*/{&PRom[R_Lim_Sup+1],NO_FUNCTION,&PRom[R_Lim_Inf+1],&Lim_Sup_ch[0],CHAN_2},

/*Tcontrol1(36)*/{&PRom[R_Tcontrol1],NO_FUNCTION,&Lim_0,&Lim_2,CHAN_1},												 //76
/*Tcontrol2*/{&PRom[R_Tcontrol1+1],NO_FUNCTION,&Lim_0,&Lim_2,CHAN_2},

/*Reset1(38)*/{&PRom[R_Reset],NO_FUNCTION,&PRom[R_Lim_Inf+0], &PRom[R_Lim_Sup+0],/*&Lim_Inf_ch[0],&Lim_Sup_ch[0]*/CHAN_1},							 //80
/*Reset2*/{&PRom[R_Reset+1],NO_FUNCTION,&Lim_Inf_ch[0],&Lim_Sup_ch[0],CHAN_2},

/*Integral1(40)*/{&PRom[R_IN1],NO_FUNCTION,&Lim_0,&Lim_1000,CHAN_1},													 //88
/*Integral2*/{&PRom[R_IN1+1],NO_FUNCTION,&Lim_0,&Lim_1000,CHAN_2},

/*Derivada1(42) */{&PRom[R_DR1],NO_FUNCTION,&Lim_0,&Lim_600,CHAN_1},													 //92
/*Derivada2 */{&PRom[R_DR1+1],NO_FUNCTION,&Lim_0,&Lim_600,CHAN_2},

/*PeriodoA1(44)*/{&PRom[R_Pra],NO_FUNCTION,&Lim_0,&Lim_10,AL_1},
/*PeriodoA2*/{&PRom[R_Pra+1],NO_FUNCTION,&Lim_0,&Lim_10,AL_2},
/*PeriodoA3*/{&PRom[R_Pra+2],NO_FUNCTION,&Lim_0,&Lim_10,AL_3},	

/*LimPotInf1(47)*/{&PRom[R_Pot_Inf],NO_FUNCTION,&Lim_0,&Lim_1000,CHAN_1},											//104
/*LimPotInf2*/{&PRom[R_Pot_Inf+1],NO_FUNCTION,&Lim_0,&Lim_1000,CHAN_2},

/*LimPotSup1(49) */{&PRom[R_Pot_Sup],NO_FUNCTION,&Lim_0,&Lim_1000,CHAN_1},										//108
/*LimPotSup2 */{&PRom[R_Pot_Sup+1],NO_FUNCTION,&Lim_0,&Lim_1000,CHAN_2},

/*Tsalcont1(51)*/{&PRom[R_Tsalcont],NO_FUNCTION,&Lim_0,&Lim_3,CHAN_1},												//112
/*Tsalcont2*/{&PRom[R_Tsalcont+1],NO_FUNCTION,&Lim_0,&Lim_3,CHAN_2},

/*Potencia1(53) */{&PRom[R_Potman],NO_FUNCTION,&Lim_0,&Lim_1000,CHAN_1},											//116
/*Potencia2 */{&PRom[R_Potman+1],NO_FUNCTION,&Lim_0,&Lim_1000,CHAN_2},

/*AutoSintonia1(55)*/{&PRom[R_Stn],A_Selftun,&Lim_0,&Lim_2,CHAN_1},														//120
/*AutoSintonia2*/{&PRom[R_Stn+1],A_Selftun,&Lim_0,&Lim_2,CHAN_2},

/*TipoA1(57)*/{&PRom[R_T_AL],NO_FUNCTION,&Lim_0,&Lim_6,AL_1},																	//128
/*TipoA2*/{&PRom[R_T_AL+1],NO_FUNCTION,&Lim_0,&Lim_6,AL_2},
/*TipoA3*/{&PRom[R_T_AL+2],NO_FUNCTION,&Lim_0,&Lim_6,AL_3},

/*Talar1(60)*/{&PRom[R_Talar1],NO_FUNCTION,&Lim_0,&Lim_7,AL_1},
/*Talar2*/{&PRom[R_Talar1+1],NO_FUNCTION,&Lim_0,&Lim_7,AL_2},															//136
/*Talar3*/{&PRom[R_Talar1+2],NO_FUNCTION,&Lim_0,&Lim_7,AL_3},

/*Filtro1(63)*/{&PRom[R_Filtro1],NO_FUNCTION,&Lim_0,&Lim_255,CHAN_1},													//144
/*Filtro2*/{&PRom[R_Filtro1+1],NO_FUNCTION,&Lim_0,&Lim_255,CHAN_2},

#ifdef programador
/*RunPrograma1(57)*/{&PRom[R_Programa],CorrerPrograma,&Lim_0,&Lim_10,CHAN_1},									
/*RunPrograma2*/{&PRom[R_Programa+1],CorrerPrograma,&Lim_0,&Lim_10,CHAN_2},
/*Segmento1(62)*/{&SegmentoMostrado[0],CambiarSegmento,&LimInf_Seg,&Lim_Segmento2[CHAN_1],CHAN_1},		//132
/*Segmento2*/{&SegmentoMostrado[1],CambiarSegmento,&LimInf_Seg,&Lim_Segmento2[CHAN_2],CHAN_2},
/*TieSeg1(69)*/{&TiempoEnMinutos[0],CambiarTiempo,&Lim_0,&Lim_TieSeg[CHAN_1],CHAN_1},						//148
/*TieSeg2*/{&TiempoEnMinutos[1],CambiarTiempo,&Lim_0,&Lim_TieSeg[CHAN_2],CHAN_2},
#endif

#ifdef CCAL
// si es  programador entonces posicion nro. 70 sino 68
/*Pss1(71 o 69) */{&PRom[R_Pss],NO_FUNCTION,&Lim_0,&Lim_1000,CHAN_1},											          //152
/*Tss1 */{&PRom[R_Tss],NO_FUNCTION,&Lim_0,&Lim_1000,CHAN_1},											          //153
/*SPSS1*/{&PRom[R_SPlim], NO_FUNCTION,&PRom[R_Lim_Inf],&PRom[R_Lim_Sup],CHAN_1},	 
/*SPST1*/{&PRom[R_SPst], NO_FUNCTION,&PRom[R_Lim_Inf], &PRom[R_Lim_Sup], CHAN_1},	 

/*SPSB1(75 o 73)*/{&PRom[R_SPsb], NO_FUNCTION,&PRom[R_Lim_Inf], &PRom[R_Lim_Sup], CHAN_1},	          //156 
/*Indf */{&PRom[R_Indf], NO_FUNCTION, &Lim_0,&Lim_9999,CHAN_1},	 
/*Indf */{&PRom[R_Drdf], NO_FUNCTION, &Lim_0,&Lim_1000,CHAN_1},	 
/*Indf */{&PRom[R_Abdf], NO_FUNCTION, &Lim_0,&Lim_9999,CHAN_1},	 

/*SPlimst(79 o 77)*/{&PRom[R_SPlimst], NO_FUNCTION,&Lim_0,&Lim_1000,CHAN_1},        //160	 
#endif

#ifdef _PRINTER
/*None53(115 o 105)*/{/*&PRom[R_PrnIntervalo]*/&(confMI.intervalo),setIntervaloMIVista,&Lim_1,&Lim_9999,CHAN_1},
/*None54*/{&bTConf.anio,setAnioDT,&Lim_2008,&Lim_2099,CHAN_1},
/*None55*/{&bTConf.mes,setMesDT,&Lim_1,&Lim_12,CHAN_1},
/*None56*/{&bTConf.dia,setDiaDT,&Lim_1,&Lim_31,CHAN_1},
/*None57*/{&bTConf.dia,setHoraDT,&Lim_0,&Lim_2359,CHAN_1},
/*None58*/{&confMI.habilitado,setHabilitadoMIVista,&Lim_0,&Lim_2,CHAN_1},

#endif

#ifdef RPM
/*None59(120 o 111)*/{&config.pulsosPorVuelta,setPulsosPorVuelta,&Lim_1,&Lim_9999,CHAN_1},
/*None61*/{&config.iFiltro,setFiltro,&Lim_0,&Lim_255,CHAN_1},
/*AjusteRPM*/{&PRom[R_AJU],NO_FUNCTION,&Lim_m1999,&Lim_9999,CHAN_1},	
#endif

#ifdef VF
/*mostrar*/ {&PRom[R_MVF],NO_FUNCTION,&Lim_0,&Lim_2,CHAN_1},
/*LI meseta*/ {&PRom[R_LIMs],NO_FUNCTION,&Lim_0,&Lim_999,CHAN_1},
/*LI Vel*/ {&PRom[R_LIVel],NO_FUNCTION,&Lim_0,&Lim_999,CHAN_1},
#endif

#ifdef INTEGRADOR
/*R_reset*/{&PRom[R_RSTL],NO_FUNCTION,&Lim_0,&Lim_2,CHAN_1},
/*R_Unidades*/{&PRom[R_UND],NO_FUNCTION,&Lim_0,&Lim_3,CHAN_1},
/*R_Resolucion*/{&PRom[R_RSL],NO_FUNCTION,&Lim_0,&Lim_3,CHAN_1},
#endif

#ifdef ADQ
/*None54*/{&bTConf.anio,setAnioDT,&Lim_2008,&Lim_2099,CHAN_1},
/*None55*/{&bTConf.mes,setMesDT,&Lim_1,&Lim_12,CHAN_1},
/*None56*/{&bTConf.dia,setDiaDT,&Lim_1,&Lim_31,CHAN_1},
/*None57*/{&bTConf.dia,setHoraDT,&Lim_0,&Lim_2359,CHAN_1},
/*R_AdqHabilitado*/{&PRom[R_AdqHabilitado],setflagFechaHora,&Lim_0,&Lim_2,CHAN_1},
/*R_AdqBorrado*/{&PRom[R_AdqBorrado],BorrarTodoAdq,&Lim_0,&Lim_2,CHAN_1}, 
/*R_AdqTranfer*/{&PRom[R_AdqTranfer],NO_FUNCTION,&Lim_0,&Lim_2,CHAN_1},
/*R_AdqPeriodo*/{&PRom[R_AdqPeriodo],NO_FUNCTION,&Lim_2,&Lim_3600,CHAN_1},
#endif

/*topn(122 o 113)*/{&PRom[R_Topn],NO_FUNCTION,&Lim_0,&Lim_1000,0},
/*COM*/{&PRom[R_COM],NO_FUNCTION,&Lim_0,&Lim_2,0},
/*dbnd*/{&PRom[R_Dbnd],NO_FUNCTION,&Lim_0,&Lim_600,0},
/*SetC*/{&PRom[R_SetC],NO_FUNCTION,&Lim_0,&Lim_9999,0},
/*Version(125 o  116)*/{&PRom[R_Ver],NO_FUNCTION,&Lim_0,&Lim_5,0},
/*Id(126 o 117)*/{&PRom[R_Id],NO_FUNCTION,&Lim_0,&Lim_255,0},
/*ATA(127 o 118)*/{&PRom[R_ATA],NO_FUNCTION,&Lim_m1999,&Lim_9999,0},
		 						
};
								
/* pantalla principal */
/*****************************/


#ifdef SIMCIC

const Numerico Principal1={
      MainHandler,								/* funcion que procesa al box*/
      &Parametros[R_SetPoint],		/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Pri ",											/* texto del titulo */
			1,													/* punto decimal a enc */
			NULL,
			(PunteroF*)&Vrmp1.DirProc,&Codigo1.DirProc,					      //Proximos estados
			};

#elif	 defined(CCAL)
const PunteroF R_Principal1[3]={

			&Ccalstn.DirProc,
      &Ccalstn.DirProc,		                                      //Proximos estados
      &C1POT.DirProc,		                                        //Proximos estados

};

const Numerico Principal1={
      MainHandler,								                              /* funcion que procesa al box*/
      &Parametros[R_SetPoint],					                        /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Pri ",											                              /* texto del titulo */
			1,													                              /* punto decimal a enc */
			&PRom[R_Tsalcont],								                        //Parametro que decide el proximo estado
			R_Principal1,
			&Codigo1.DirProc			
};


#elif (defined(DH102) && !defined(nico_1_10) && !defined(VF_PROG) && !defined(VF))
  
  const Numerico Principal1={
      MainHandler,								    /* funcion que procesa al box*/
      &Parametros[R_SetPoint],				/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Pri ",											    /* texto del titulo */
			1,													    /* punto decimal a enc */
			NULL,
			&C1SP.DirProc,
			&Codigo1.DirProc 
  };


#elif defined (DH102) && !defined(VF_PROG) && !defined(VF)
//aparato que solo muestra la tira de parametros de tun, codigo y la alarma, todo con toque rapido 

const Numerico Principal1={
      MainHandler,								    /* funcion que procesa al box*/
      &Parametros[R_SetPoint],				/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Pri ",											    /* texto del titulo */
			1,													    /* punto decimal a enc */
			NULL,
			&Codigo1.DirProc, 
		  NULL
	
};


#elif (defined(VF) && !defined(VF_BKR) && !defined (VF101) && !defined(VF_PROG))


const Numerico Principal1={
      MainHandler,								 /* funcion que procesa al box*/      
      &Parametros[R_SetPoint],		 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Fin ",											 /* texto del titulo */
			1,													 /* punto decimal a enc */
			NULL,         							 //Parametro que decide el proximo estado
			&etapas.DirProc,
			&alarmas.DirProc
			
};

#elif defined(VF_BKR) && !defined (VF101) && !defined(VF_PROG)

const Numerico Principal1={
      MainHandler,								/* funcion que procesa al box*/
      &Parametros[R_SetPoint],    /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Fin ",											/* texto del titulo */
			1,												  /* punto decimal a enc */
			NULL,
			#ifdef VF_BKR_C
			&Vrmp1.DirProc,
			#else
			&Tmpm1.DirProc,
			#endif
			&Codigo1.DirProc
		
};

#elif defined(VF_BKR) && defined (VF101)

const Numerico Principal1={
      MainHandler,								/* funcion que procesa al box*/
      &Parametros[R_SetPoint],    /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Fin ",											/* texto del titulo */
			1,												  /* punto decimal a enc */
			NULL,
		  #ifdef VF_BKR_C
			&Vrmp1.DirProc,
			#else
			&Tmpm1.DirProc,
			#endif
			&alarmas.DirProc
		
};

#elif defined (VF101) && !defined(VF_PROG)

const Numerico Principal1={
      MainHandler,								 /* funcion que procesa al box*/      
      &Parametros[R_SetPoint],		 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Fin ",											 /* texto del titulo */
			1,													 /* punto decimal a enc */
			NULL,         							 //Parametro que decide el proximo estado
			&etapas.DirProc,
			&alarmas.DirProc
			
};

#elif defined(VF_PROG)


const NumericoRO Principal1={
      
      NumHandlervfInicial,									                      /* funcion que procesa al box*/
			NULL,
			" PULSE ;E? PArA iniciAr  ",							                              /* texto del titulo */
			DECIMALES_CANAL1,						                      //pos punto dec
			NULL,									        //Parametro que modifica
			(const PunteroF*) &nroProg.DirProc,&Codigo1.DirProc						                            //Proximos estados
			};

const Numerico Principal1_VF_PROG={
      MainHandler,								 /* funcion que procesa al box*/      
      &Parametros[R_SetPoint],		 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Fin ",											 /* texto del titulo */
			1,													 /* punto decimal a enc */
			NULL,         							 //Parametro que decide el proximo estado
			&ProgramaEnAccion.DirProc,
			&Codigo1.DirProc
			
};


#elif defined (LOBOV)

const Numerico Principal1={
      MainHandler,								 /* funcion que procesa al box*/      
      &Parametros[R_SetPoint],		 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"SP 1",											 /* texto del titulo */
			1,													 /* punto decimal a enc */
			NULL,
			&Indicacion.DirProc,
	  	&Codigo1.DirProc
			
};

#else //equipo normal
   
   
    const PunteroF R_Principal1[5]={

      &C1SP.DirProc,	           //Proximos estados con bl1 
		  &Sintonia.DirProc,         //proximo estado bl2
		  NULL,                      //proximo estado bl3
			&C1SP.DirProc,             //proximo estado pot o dif(dos canales)                                               
		  &C1SP.DirProc	             //proximo estado nro de version
    };
   
    
const Numerico Principal1={
      MainHandler,								/* funcion que procesa al box*/
      &Parametros[R_SetPoint],		/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Pri ",											/* texto del titulo */
			1,													/* punto decimal a enc */
			&PRom[R_Ver],
			R_Principal1,
			&Codigo1.DirProc
			
};

#endif

#ifdef LOBOV
 const NumericoRO Indicacion={
      
      NumHandlerLOBOV,									                      /* funcion que procesa al box*/
      
      //&Parametros[R_IND],				                      /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			NULL,
			"mEdiCion    ",							                              /* texto del titulo */
			DECIMALES_CANAL1,						                      //pos punto dec
			NULL,									        //Parametro que modifica
			(const PunteroF*) &AL1.DirProc,NULL						                            //Proximos estados
			};
#endif

/*setpoints y alarmas*/			
/*****************************/

#ifdef CCAL
const PunteroF R_C1SP[3]={
      &AL1.DirProc,						                        //Proximos estados
			&AL1.DirProc,						                        //Proximos estados
			&AL1.DirProc,						                      //Proximos estados
};


#elif defined(DOBLE_SP)
const PunteroF R_C1SP[3]={
      &DSP.DirProc,						                        //Proximos estados
			&AL1.DirProc,						                        //Proximos estados
			&AL1.DirProc,						                      //Proximos estados
};
#else
const PunteroF R_C1SP[3]={
      &AL1.DirProc,						                        //Proximos estados
			&AL1.DirProc,						                        //Proximos estados
			&C1POT.DirProc,						                      //Proximos estados
};
#endif  

const PunteroF R_C1SP102[7]={
      &AL1.DirProc,						                        //Proximos estados
			&AL1.DirProc,
			&AL1.DirProc,						                        //Proximos estados
			&AL1.DirProc,
			&AL1.DirProc,						                        //Proximos estados
			&AL1.DirProc,						                        //Proximos estados
			&Principal1.DirProc,						                      //Proximos estados
};  
//#endif


const Numerico C1SP={
      #ifdef pirani
      NumExpoHandler,
      #elif defined(INTEGRADOR)
      NumHandlerLux,
      #else 
      NumHandler,									                      /* funcion que procesa al box*/
      #endif
      &Parametros[R_SetPoint+0],					                      /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"SP 1",							                              /* texto del titulo */
			DECIMALES_CANAL1,						                      //pos punto dec
			(int*)&PRom[R_Tsalcont],									        //Parametro que modifica
			R_C1SP,NULL						                            //Proximos estados
			};
			
const Numerico C1SP102={
      #ifdef pirani
      NumExpoHandler,
      #else
      NumHandler,									                      /* funcion que procesa al box*/
      #endif
      &Parametros[R_SetPoint+0],					                      /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"SP 1",							                              /* texto del titulo */
			DECIMALES_CANAL1,						                      //pos punto dec
			(int*)&PRom[R_Talar1],									        //Parametro que modifica
			R_C1SP102,NULL						                            //Proximos estados
			};
			

 const Numerico DSP={
      #ifdef pirani
      NumExpoHandler,
      #else
      NumHandler,									                      /* funcion que procesa al box*/
      #endif
      &Parametros[R_SetPoint+2],					                      /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"SP 2",							                              /* texto del titulo */
			DECIMALES_CANAL1,						                      //pos punto dec
			(int*)&PRom[R_Tsalcont],									        //Parametro que modifica
	  	 (R_C1SP+1),NULL						                            //Proximos estados
			};

const Numerico C1POT={
      NumHandler,									                      /* funcion que procesa al box*/
      &Parametros[R_Potman],					                        /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Pot1",							                              /* texto del titulo */
			1,											                          //pos punto dec
			NULL,									                            //Parametro que modifica
			(const PunteroF*) &AL1.DirProc,NULL						  //Proximos estados
			};


const Titulo alarmas={ 
      TitleHandler,							/* funcion que procesa al box*/
			"ALArmAS   ",										//nombre display
			" -- ",										//nombre display
      NULL, 
		  (PunteroF*)&AL1.DirProc,
		  #ifdef VF_PROG
		  NULL
		  #else
		  (PunteroF)&Codigo1.DirProc
		  #endif
			};			

const Numerico AL1=	                                  //nombre variable
      {
      #ifdef pirani
      NumExpoHandler,
      #elif defined(INTEGRADOR)
      NumHandlerLux,
      #else 
      NumHandler,									                      /* funcion que procesa al box*/
      #endif									                          // funcion que procesa al box
      &Parametros[R_AL],										                  // direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor
			
			"A  1", //nombre display
			
			DECIMALES_CANAL1,								                  //pos punto dec
			NULL,							                                //parametro que modifica. 
      #if ALARMAS_CH1 > 1
			  (PunteroF*)&AL2.DirProc,NULL			          //enter rap,enter mant		
      #elif defined (INTEGRADOR)
			  (PunteroF*)&RESET.DirProc,NULL
      #else  
			  #ifdef programador
			  (PunteroF*)&P1.DirProc,NULL			 //enter rap,enter mant
			  #else
			    #if CANTIDAD_SAL_CONTROL>1  //&& !defined SENS_HUMEDAD
    			(PunteroF*)&C2SP.DirProc,NULL
					#else
					  #if defined(_PRINTER)
					  (PunteroF*)&boxPrnHabilitado.DirProc,NULL
					  #elif !defined (nico_1_10)   
  			    (PunteroF*)&Principal1.DirProc,NULL			          //enter rap,enter mant		
            #else
            &Sintonia.DirProc
            #endif
          #endif
			  #endif
      #endif
			};

const Numerico AL2 =	                                //nombre variable
      {
      #ifdef pirani
      NumExpoHandler,
      #elif defined(INTEGRADOR)
      NumHandlerLux,
      #else 
      NumHandler,									                      /* funcion que procesa al box*/
      #endif								                          // funcion que procesa al box
      &Parametros[R_AL+1],											              // direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor
			"A  2",							                              //nombre display
			DECIMALES_CANAL1,											            //pos punto dec
			NULL,							                                //parametro que modifica. 
      #if ALARMAS_CH1 > 2
			(PunteroF*)&AL3.DirProc,NULL			          //enter rap,enter mant		
      #elif defined (INTEGRADOR)
      (PunteroF*)&RESET.DirProc,NULL
      #else
			#ifdef programador
			(PunteroF*)&P1.DirProc,NULL			 //enter rap,enter mant
			#elif defined(_PRINTER)
			(PunteroF*)&boxPrnHabilitado.DirProc,NULL
			#else
			(PunteroF*)&Principal1.DirProc,NULL			          //enter rap,enter mant		
			#endif
      #endif
			};


const Numerico AL3 =	                                //nombre variable
      {
      #ifdef pirani
      NumExpoHandler,
      #elif defined(INTEGRADOR)
      NumHandlerLux,
      #else 
      NumHandler,									                      /* funcion que procesa al box*/
      #endif						                          // funcion que procesa al box
      &Parametros[R_AL+2],											              // direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor
			"A  3",							                              //nombre display
			DECIMALES_CANAL1,											            //pos punto dec
			NULL,							                                //parametro que modifica. 
#ifdef EQA1
			(PunteroF*)&C1STN.DirProc,NULL				//enter rap,enter mant			
#elif defined (INTEGRADOR)
      (PunteroF*)&RESET.DirProc,NULL
#else
			#ifdef programador
			(PunteroF*)&P1.DirProc,NULL			 //enter rap,enter mant
			#elif defined(_PRINTER)
			(PunteroF*)&boxPrnHabilitado.DirProc,NULL
			#else
			(PunteroF*)&Principal1.DirProc,NULL			          //enter rap,enter mant		
			#endif
#endif
			};
								 
#ifdef _PRINTER

static const char * SN[2]={
      "no  ",									
			"Si  "

};

const Textual boxPrnHabilitado=
      {TxtHandler,						/* funcion que procesa al box*/
			&Parametros[R_PrnHabilitado],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"ImPriMir    ",									//nombre display
			SN,             // Array donde estan los textos
			NULL,						 //parametro que modifica.
			(PunteroF*)&Principal1.DirProc,NULL					 //Proximos estados	
      };
      
#endif


#ifdef programador

const Numerico P1 =	                                    //nombre variable
      {NumHandler,						                          // funcion que procesa al box
      &Parametros[R_Programa],											          // direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor
			"P  1",							                              //nombre display
			0,											                          //pos punto dec
			NULL,							                                //parametro que modifica. 
			#if CANTIDAD_SAL_CONTROL>1
			(PunteroF*)&C2SP.DirProc,NULL
			#elif defined(_PRINTER)
			(PunteroF*)&boxPrnHabilitado.DirProc,NULL
			#else
			(PunteroF*)&Principal1.DirProc,NULL			 //enter rap,enter mant
			#endif
			};


/*#ifndef prog_viejo 

const NumericoRO Segmento1 =	                                    //nombre variable
      {NumHandler,						                          // funcion que procesa al box
      &SegmentoActual[0],											          // direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor
			"SEG1",							                              //nombre display
			0,											                          //pos punto dec	
			NULL,							                                //parametro que modifica. 
			(PunteroF*)&SegundosSegmento1.DirProc,NULL			 //enter rap,enter mant
			};

const NumericoRO SegundosSegmento1 =	                                    //nombre variable
      {Num_realtime_Handler,	                          // funcion que procesa al box
      ((int *)&SegundosSegmento[0])+1,										          // direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor
			"tIE1",							                              //nombre display
			0,											                          //pos punto dec
			NULL,							                                //parametro que modifica. 
			(PunteroF*)&AL1.DirProc,NULL			 //enter rap,enter mant
			};

#else		*/

const Numerico Segmento1 =	                                    //nombre variable
      {NumHandler,						                          // funcion que procesa al box
      &Parametros[R_Segmento],											          // direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor
			"SEG1",							                              //nombre display
			0,											                          //pos punto dec	
			NULL,							                                //parametro que modifica. 
			(PunteroF*)&SegundosSegmento1.DirProc,NULL			 //enter rap,enter mant
			};

const Numerico SegundosSegmento1 =	                                    //nombre variable
      {TieSegHandler,	                          // funcion que procesa al box
      &Parametros[R_TieSeg],										          // direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor
			"tIE1",							                              //nombre display
			0,											                          //pos punto dec
			NULL,							                                //parametro que modifica. 
			(PunteroF*)&AL1.DirProc,NULL			 //enter rap,enter mant
			};			
/*#endif*/

 

#endif
			
#if CANTIDAD_CANALES>1
/**********SET POINT 2**************/
/***********************************/
const PunteroF R_C2SP[3]={
      &C2AL1.DirProc,						                        //Proximos estados
			&C2AL1.DirProc,						                        //Proximos estados
			&C2POT.DirProc,						                        //Proximos estados
};
const Numerico C2SP={
      #ifdef pirani
      NumExpoHandler,
      #else
      NumHandler,									                      /* funcion que procesa al box*/
      #endif									                      /* funcion que procesa al box*/
      &Parametros[R_SetPoint+1],        				                      /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"SP 2",							                              /* texto del titulo */
			DECIMALES_CANAL2,											            //pos punto dec
			&PRom[R_Tsalcont+1],									      //Parametro que modifica
			R_C2SP,NULL						                                  //Proximos estados
			};

/**********Potencia 2**************/
/***********************************/
const Numerico C2POT={
      NumHandler,									                      /* funcion que procesa al box*/
      &Parametros[R_Potman+1],				                        /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Pot2",							                              /* texto del titulo */
			1,											                          //pos punto dec
			NULL,									                            //Parametro que modifica
			(const PunteroF*)&C2AL1.DirProc,NULL						  //Proximos estados
			};

/**********Alarma ch2 al1***********/
/***********************************/
const Numerico C2AL1 =	                                //nombre variable
      {NumHandler,						                          // funcion que procesa al box
      &Parametros[R_AL+1],											              // direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor
			"A  2",							                              //nombre display
			DECIMALES_CANAL2,											            //pos punto dec
			NULL,							                                //parametro que modifica. 
//			(PunteroF*)&C2SP.DirProc,NULL			          //enter rap,enter mant		

			#ifdef programador
			(PunteroF*)&P2.DirProc,NULL			 //enter rap,enter mant
			#else
			(PunteroF*)&Principal1.DirProc,NULL			          //enter rap,enter mant		
			#endif
			};


#ifdef programador
const Numerico P2 =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &Parametros[R_Programa+1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"P  2",							 //nombre display
			0,											 //pos punto dec
			NULL,							 //parametro que modifica. 
			(const PunteroF*)&Principal1.DirProc,NULL			 //enter rap,enter mant
			};
#endif			
			
#endif

/* Codigo */
/**********/
const PunteroF R_Codigo2[7]={
   &AL1.DirProc,
   &AL1.DirProc,
   &AL1.DirProc,
   &AL1.DirProc,
   &AL1.DirProc,
   &AL1.DirProc,
   &Sintonia.DirProc
};


const TDato dCodigo={
  	 NULL,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_0, &Lim_9999,	 /*Limites inferior y superior del dato*/
		 CHAN_1
};

const Numerico Codigo1={
      CodHandler,				/* funcion que procesa al box*/
      &dCodigo,						/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"cod ",				    //nombre display
			0,								/* punto decimal a enc */
			#ifndef nico_1_10
			NULL,						  //parametro que modifica.
			#else
			&PRom[R_Talar1],
			#endif
			#ifdef _PRINTER 
			NULL,&titImpresion.DirProc			//Proximos estados en bl1, primero toque rápido luego toque prolongado
			#elif defined(programador)
			NULL,&Nprograma.DirProc
			#elif defined(ADQ)
			NULL,&titAdq.DirProc				
			#else
			 #ifndef nico_1_10
			 NULL,&Sintonia.DirProc			//Proximos estados en bl1, primero toque rápido luego toque prolongado
			 #else
			 R_Codigo2,&Principal1.DirProc 
			 #endif
			#endif
//			#endif
			};

/* Definicion de los titulos */
/*****************************/
#ifdef _PRINTER

const Titulo titImpresion={ 
      TitleHandler,										// funcion que procesa al box
			"Prn ",													//nombre display
			" -- ",													//nombre display
			NULL,											  //parametro que modifica.
			#ifdef programador
			(PunteroF*)&boxAnio.DirProc,&Programas.DirProc	//Proximos estados	
			#else
			(PunteroF*)&boxAnio.DirProc,&Sintonia.DirProc		//Proximos estados	
			#endif
			};
			
#endif			

#ifdef ADQ

static const char * SNA[2]={
      "no  ",									
			"Si  "

};

static const char * SNAH[3]={
      "no  ",									
			"Si  ",
			"FULL"

};

const Titulo titAdq={ 
      TitleHandler,										// funcion que procesa al box
			"Adq ",													//nombre display
			"  AdqUiSicion  ",							//nombre display
			NULL,											  //parametro que modifica.
			
			&boxAdqHabilitado.DirProc,&Sintonia.DirProc		//Proximos estados	
		
			};
			
const Textual boxAdqHabilitado=
      {TxtHandlerAdqHAbilitacion,						/* funcion que procesa al box*/
			&Parametros[R_AdqHabilitado],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AdqUirir    ",									//nombre display
			SNAH,             // Array donde estan los textos
			NULL,						 //parametro que modifica.
			(PunteroF*)&boxAdqBorrado.DirProc,NULL					 //Proximos estados	
      };
 
const Textual boxAdqBorrado=
      {TxtHandlerAdq,						/* funcion que procesa al box*/
			&Parametros[R_AdqBorrado],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"borrAr    ",									//nombre display
			SNA,             // Array donde estan los textos
			NULL,						 //parametro que modifica.
			(PunteroF*)&boxAdqTranfer.DirProc,NULL					 //Proximos estados	
      }; 
      

const Textual boxAdqTranfer=
       {TxtHandler,						/* funcion que procesa al box*/
			&Parametros[R_AdqTranfer],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"trAnSFErir    ",									//nombre display
			SNA,             // Array donde estan los textos
			NULL,						 //parametro que modifica.
			(PunteroF*)&boxAdqPeriodo.DirProc,NULL					 //Proximos estados	
      };
      
const Numerico boxAdqPeriodo =	                                //nombre variable
      {NumHandler,						                          // funcion que procesa al box
      &Parametros[R_AdqPeriodo],											              // direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor
			"intErVAlo   ",							                              //nombre display
			DECIMALES_CANAL1,											            //pos punto dec
			NULL,							                                //parametro que modifica. 
			(PunteroF*)&boxAnio.DirProc,NULL			          //enter rap,enter mant		
			};                 

#endif
				
/* Definicion de los titulos */
/*****************************/

#ifdef programador

const Titulo Programas={ 
      #ifdef prog_viejo
      TitleProgHandler,
      #else
      TitleHandler,										/* funcion que procesa al box*/
			#endif
			"ProG",													//nombre display
			" -- ",													//nombre display
			NULL,											  //parametro que modifica.
		 #ifdef prog_viejo
		 (PunteroF*)&Temp_i.DirProc,
		 #else
		 (PunteroF*)&Program.DirProc,
		 #endif
		 #ifdef ADQ
		 &titAdq.DirProc	
		 #else 
		 NULL // &Sintonia.DirProc		//Proximos estados
		 #endif
			};
#endif   



const Titulo Sintonia={ 
      TitleHandler,							/* funcion que procesa al box*/
			"tun ",										//nombre display
			"SintoniA   ",										//nombre display
      NULL, 
		#ifdef CCAL
			(PunteroF*)&C1RES.DirProc,&Colcal.DirProc,					//Proximos estados
    #elif defined(DH102)
      (PunteroF*)&C1RES.DirProc,NULL,
    #elif defined RPM
      (PunteroF*)&C1RES.DirProc,&_RPM.DirProc,
    #else	
			(PunteroF*)&C1RES.DirProc,&Calibracion.DirProc,					//Proximos estados
		#endif
			};

const Titulo Calibracion={ 
      TitleHandler,						/* funcion que procesa al box*/
			"cAL ",									//nombre display
			"cALibrAcion   ",									//nombre display
			NULL,							//parametro que modifica.
		  #ifdef INTEGRADOR
		  (PunteroF*)&TipoSensor1.DirProc,&Integrador.DirProc  //Proximos estados
		  #else
			(PunteroF*)&TipoSensor1.DirProc,&Setting.DirProc  //Proximos estados
			#endif
			};

			

//dan1001
#ifdef _APARATO_VIEJO
/*Set aparato viejo*/
/*******************/
const Titulo Setting={ 
      TitleHandler,									/* funcion que procesa al box*/
			"SEt ",												//nombre display
			"conFiGurAcion   ",												//nombre display
			NULL,												//parametro que modifica.
			(PunteroF*)&C1TSC.DirProc,&Limites.DirProc				//Proximos estados
			};
#else
/*Set aparato nuevo*/
/*******************/
const Titulo Setting={ 
      TitleHandler,									/* funcion que procesa al box*/
			"SEt ",												//nombre display
			" -- ",												//nombre display
			NULL,												//parametro que modifica.
			(PunteroF*)&C1CN.DirProc,&Limites.DirProc				//Proximos estados
			};

#endif
//dan1001

const Titulo Limites={ 
      TitleHandler,										/* funcion que procesa al box*/
			"LiM ",													//nombre display
			"LiMitES   ",													//nombre display
			NULL,											//parametro que modifica.
			(PunteroF*)&LimiteInferior1.DirProc,&Principal1.DirProc		//Proximos estados
			};
			

/* PARAMETROS de "Limites" */
/***************************/

/* limite inferior del set-point de control principal*/ 
const Numerico LimiteInferior1={
      #if defined(INTEGRADOR)
      NumHandlerLux,
      #else 
      NumHandler,									                      /* funcion que procesa al box*/
      #endif
      &Parametros[R_Lim_Inf],
			"Li 1",
			DECIMALES_CANAL1,
			NULL,
			(PunteroF*)&LimiteSuperior1.DirProc,NULL
			};
			


/* limite superior del set-point de control principal*/  
const Numerico LimiteSuperior1={
      #if defined(INTEGRADOR)
      NumHandlerLux,
      #else 
      NumHandler,									                      /* funcion que procesa al box*/
      #endif
      &Parametros[R_Lim_Sup],
			"LS 1",
			DECIMALES_CANAL1,
			NULL,
			(PunteroF*)&PotenciaInferior1.DirProc,NULL
			};
						

/* limite inferior de potencia de control principal*/ 
const Numerico PotenciaInferior1={
      NumHandler,
      &Parametros[R_Pot_Inf],
			"Pi 1",
			1,
			NULL,
			(PunteroF*)&PotenciaSuperior1.DirProc,NULL
			};
			
			
/* limite superior de potencia de control principal*/  
const Numerico PotenciaSuperior1={
      NumHandler,
      &Parametros[R_Pot_Sup],
			"PS 1",
			1,
			NULL,	 
			#if CANTIDAD_SAL_CONTROL >1
					#ifdef SP_EXTERNO
    			  (PunteroF*)&Principal1.DirProc,NULL
					#else
    			  (PunteroF*)&LimiteInferior2.DirProc,NULL
					#endif
			#else
			(PunteroF*)&Principal1.DirProc,NULL
			#endif

			};
			
#if CANTIDAD_CANALES >1
const Numerico LimiteInferior2={
      NumHandler,
      &Parametros[R_Lim_Inf+1],
			"Li 2",
			DECIMALES_CANAL2,
			NULL,
			(PunteroF*)&LimiteSuperior2.DirProc,NULL
			};

const Numerico LimiteSuperior2={
      NumHandler,
      &Parametros[R_Lim_Sup+1],
			"LS 2",
			DECIMALES_CANAL2,
			NULL,
			(PunteroF*)&PotenciaInferior2.DirProc,NULL
			};

const Numerico PotenciaInferior2={
      NumHandler,
      &Parametros[R_Pot_Inf+1],
			"Pi 2",
			1,
			NULL,
			(PunteroF*)&PotenciaSuperior2.DirProc,NULL
			};

const Numerico PotenciaSuperior2={
      NumHandler,
      &Parametros[R_Pot_Sup+1],
			"PS 2",
			1,
			NULL,
	    (PunteroF*)&Principal1.DirProc,NULL
      };

#endif




#endif

 

/* fin  */


