#include "Mydefines.h"
#include "cnfbox.h"
#include "boxes.h"
#include "Programador.h"
#include "PE_Types.h"
#include "boxesprog.h"
#include "Parametros.h"

#pragma CONST_SEG DEFAULT 
#pragma STRING_SEG DEFAULT

#ifndef prog_viejo
#ifdef programador
extern const Numerico Program;
extern const Numerico Segmento;
extern const Textual Tipo_temp;
extern const Textual Tipo_Tolerancia_Gral;
extern const Numerico Tolerancia_Gral;
extern const Textual Cond_Emergencia_Gral;
extern const Textual Tipo_Segmento;
extern const Numerico Velocidad;
extern const Numerico JMP_Programa;
extern const Numerico Ciclos_Programa;
extern const Textual Cond_Emergencia_JMP;
extern const Textual Tipo_Tolerancia;
#endif
#else
extern const Numerico Tiempo;
extern const Numerico Tolerancia;
extern const Numerico Temp_f;
extern const Numerico Temp_i;
extern const Textual Cond_Emergencia;
#endif

int	 programa_ingresado, segmento_ingresado, temperatura_inicial,  tipo_segmento, temperatura_f,
	   tiempo, tolerancia,tolerancia_gral, condicion_emer,condicion_emer_gral,tipo_tolerancia,tipo_tolerancia_gral,tipo_temperatura_inicial;
int next_tiempo,next_tol,next_prog;
int Lim_Segmento1;	 /* Para el prog nuevo, pantalla de ingreso de programa*/
			
/************************************/
/************************************/
/* PARAMETROS de Programador (nuevo)*/
/************************************/
/************************************/

/*Programa*/
/**********/
#ifndef prog_viejo

#ifdef programador		
const TDato dPrograma={
  	 &programa_ingresado,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_0, &Lim_9,	 /*Limites inferior y superior del dato*/
		 0
};
//Jon
const Numerico Program =	         //nombre variable
      {
      ProgramaHandler,						     /* funcion que procesa al box*/
      &dPrograma,									     /* direccion en la E2Prom */
			"PrG ",							         //nombre display
			0,									    		 //pos punto dec
			NULL,					               //parametro que modifica. 
			(PunteroF*)&Segmento.DirProc,NULL			 //enter rap,enter mant
			};
			


const TDato dSegmento={
  	 &segmento_ingresado,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_0, &Lim_Segmento1,	 /*Limites inferior y superior del dato*/
		 0
};

const PunteroF R_Seg[2]={ 
    &Tipo_Segmento.DirProc,
    &Tipo_temp.DirProc
    }; 

const Numerico Segmento =	             //nombre variable
      {
       SegmentoHandler,						     /* funcion que procesa al box*/
      &dSegmento,									     /* direccion en la E2Prom */
			"SEG ",							         //nombre display
			0,									    		 //pos punto dec
			&Lim_0,					 //parametro que modifica. 
			R_Seg,NULL,					 //enter rap,enter mant
			};
			

/*****************************/
/*****************************/
/* Parte General (Segmento 0)*/
/*****************************/
/*****************************/

/*Tipo de Temperatura inicial*/
/*****************************/

const TDato dTTempInicial={
  	 &tipo_temperatura_inicial,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_0, &Lim_2,	 /*Limites inferior y superior del dato*/
		 0
};
static const char * TIPO_TEMP[2]={
      "SEt ",			 
      "AMb "									
};
const PunteroF R_Tipo_temp[2]={ 
    &Temp_i.DirProc,
    &Tipo_Tolerancia_Gral.DirProc
    };
     
const Textual Tipo_temp=
      {
      TxtHandler,						      /* funcion que procesa al box*/
			&dTTempInicial,											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"tti ",									            //nombre display
			&TIPO_TEMP[0],                   // Array donde estan los textos
			&tipo_temperatura_inicial,					//parametro que modifica.
			R_Tipo_temp,NULL					 //Proximos estados
			};


/*Temperatura inicial*/
/*********************/

const TDato dTempInicial={
  	 &temperatura_inicial,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
     #if CANTIDAD_CANALES == 1
		 &Lim_Inf_ch[0], &Lim_Sup_ch[0],								 //limites
     #else
		 &Lim_m1999, &Lim_9999,				  				 //limites
     #endif
     0
};
const Numerico Temp_i =	             //nombre variable
      {/*Jon*/NumHandler,						     /* funcion que procesa al box*/
      &dTempInicial,									     /* direccion en la E2Prom */
			"t  i",							         //nombre display
#if CANTIDAD_CANALES == 1
      DECIMALES_CANAL1,											 //pos punto dec
#else
			0,									    		 //pos punto dec
#endif
			NULL,					 //parametro que modifica. 
			(PunteroF*)&Tipo_Tolerancia_Gral.DirProc,NULL,					 //enter rap,enter mant
			};	

/*Tipo de Tolerancia General*/
/****************************/
const TDato dTToleranciaGral={
  	 &tipo_tolerancia_gral,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_0, &Lim_5,	 /*Limites inferior y superior del dato*/
		 0
};

const PunteroF R_Tipo_Tolerancia_Gral[5]={ 
    &Cond_Emergencia_Gral.DirProc,
    &Tolerancia_Gral.DirProc,
    &Tolerancia_Gral.DirProc,
    &Tolerancia_Gral.DirProc,
    &Cond_Emergencia_Gral.DirProc,
    }; 

const char * TEXTOS_TOLERANCIAS_GRAL[5]={
      "OFF ",
      "Lo  ",
      "Hi  ",
      "bAnd",
      "no  "									
};

const Textual Tipo_Tolerancia_Gral=
      {
      /*Jon*/TxtHandler,						/* funcion que procesa al box*/
			&dTToleranciaGral,											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"ttoL",									//nombre display
			TEXTOS_TOLERANCIAS_GRAL,             // Array donde estan los textos
			&tipo_tolerancia_gral,						 //parametro que modifica.
			R_Tipo_Tolerancia_Gral,NULL					 //Proximos estados
			};			

/*Tolerancia General*/
/********************/
const TDato dToleranciaGral={
  	 &tolerancia_gral,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
     #if CANTIDAD_CANALES == 1
		 &Lim_Inf_ch[0], &Lim_Sup_ch[0],								 //limites
     #else
		 &Lim_m1999, &Lim_9999,				  				 //limites
     #endif
     0
};

const Numerico Tolerancia_Gral =	             //nombre variable
      {
      /*Jon*/NumHandler,						     /* funcion que procesa al box*/
      &dToleranciaGral,									     /* direccion en la E2Prom */
			"toL ",							         //nombre display
#if CANTIDAD_CANALES == 1
      DECIMALES_CANAL1,											 //pos punto dec
#else
			0,									    		 //pos punto dec
#endif
			NULL,					 //parametro que modifica. 
			(const PunteroF*)&Cond_Emergencia_Gral.DirProc,NULL					 //enter rap,enter mant
			};
			

/*Condicion de emergencia General*/
/*********************************/

const TDato dCondicionEmergenciaGral={
  	 &condicion_emer_gral,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_0, &Lim_5,	 /*Limites inferior y superior del dato*/
		 0
};

const char * const TEXTOS_EMERGENCIA_GRAL[5]={		// Hay 3 condiciones de emergencia que utilizan 
			"cc  ",																// estos strings incluyendo este
			"rS  ",
			"rP  ",
			"EndP",
			"no  "
};


const Textual Cond_Emergencia_Gral=
      {
      CondEGralHandler,						/* funcion que procesa al box*/
			&dCondicionEmergenciaGral,											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Emer",									//nombre display
			TEXTOS_EMERGENCIA_GRAL,             // Array donde estan los textos
			NULL,						 //parametro que modifica.
			(PunteroF*)&Tipo_Segmento.DirProc,NULL					 //Proximos estados
			};			


/*****************************/
/*****************************/
/* Parte de segmentos        */
/*****************************/
/*****************************/



/*Tipo de Segmento   */
/*********************/
const TDato dTSegmento={
  	 &tipo_segmento,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_0, &Lim_6,	 /*Limites inferior y superior del dato*/
		 0
};
static const char * RMP_MST[6]={
      "End ",
      "rMPt",
      "rMPV",									
			"MSt ",
			"SteP",
			"JMP "
			
};

const PunteroF R_Tipo_Segmento[6]={ 
    &Program.DirProc,
    &Temp_f.DirProc,
    &Temp_f.DirProc,
    &Tiempo.DirProc,
    &Temp_f.DirProc,
    &JMP_Programa.DirProc    
    }; 
const Textual Tipo_Segmento=
      {
      #ifdef jony_17_08
      TipoSegHandler,						/* funcion que procesa al box*/
			#endif
			&dTSegmento,											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"tiPo SEG ",									//nombre display
			RMP_MST,             // Array donde estan los textos
			&tipo_segmento,						 //parametro que modifica.
			R_Tipo_Segmento,NULL					 //Proximos estados
			};


/*Temperatura final  */
/*********************/
const TDato dTempFinal={
  	 &temperatura_f,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
     #if CANTIDAD_CANALES == 1
		 &Lim_Inf_ch[0], &Lim_Sup_ch[0],								 //limites
     #else
		 &Lim_m1999, &Lim_9999,				  				 //limites
     #endif
     0
};
const PunteroF R_Temp_f[5]={ 
    NULL,
    &Tiempo.DirProc,
    &Velocidad.DirProc,
    &Tiempo.DirProc,
    &Tiempo.DirProc,
    }; 
    
const Numerico Temp_f =	             //nombre variable
      {/*Jon*/NumHandler,						     /* funcion que procesa al box*/
      &dTempFinal,									     /* direccion en la E2Prom */
			"t  F",							         //nombre display
#if CANTIDAD_CANALES == 1
      DECIMALES_CANAL1,											 //pos punto dec
#else
			0,									    		 //pos punto dec
#endif
			&tipo_segmento,					 //parametro que modifica. 
			R_Temp_f,NULL					 //enter rap,enter mant
			};


/*Tiempo*/
/********/
const TDato dTiempo={
  	 &tiempo,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_1, &Lim_9999,	 /*Limites inferior y superior del dato*/
		 0
};
const PunteroF R_Tiempo_vel[3]={
&Tolerancia.DirProc,
&Cond_Emergencia.DirProc,
&Segmento.DirProc,
};

const Numerico Tiempo =	             //nombre variable
      {/*Jon*/NumHandler,						     /* funcion que procesa al box*/
      &dTiempo,									     /* direccion en la E2Prom */
			"tEMP",							         //nombre display
			0,									    		 //pos punto dec
			&next_tiempo,					 //parametro que modifica. 
			R_Tiempo_vel,NULL					 //enter rap,enter mant
			};


/*Velocidad*/
/***********/


const Numerico Velocidad =	             //nombre variable
      {/*Jon*/NumHandler,						     /* funcion que procesa al box*/
      &dTiempo,									     /* direccion en la E2Prom */
			"VEL ",							         //nombre display
#if CANTIDAD_CANALES == 1
      DECIMALES_CANAL1,											 //pos punto dec
#else
			0,
#endif   									    		 //pos punto dec
			&next_tiempo,					 //parametro que modifica. 
			R_Tiempo_vel,NULL					 //enter rap,enter mant
			};

/*Salto a  sub-Programa*/
/***********************/
const TDato dJMPPrograma={
  	 &sub_programa,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_0, &Lim_9,	 /*Limites inferior y superior del dato*/
		 0
};
const Numerico JMP_Programa =	             //nombre variable
      {/*Jon*/NumHandler,						     /* funcion que procesa al box*/
      &dJMPPrograma,									     /* direccion en la E2Prom */
			"PrGn",							         //nombre display
			0,									    		 //pos punto dec
			NULL,					 //parametro que modifica. 
			(PunteroF*)&Ciclos_Programa.DirProc,NULL					 //enter rap,enter mant
			};
			
/*Ciclos de Programa*/
/********************/

const TDato dCiclos={
  	 &sub_programa,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_0, &Lim_255,	 /*Limites inferior y superior del dato*/
		 0
};
const PunteroF R_Ciclos_Programa[2]={
&Cond_Emergencia_JMP.DirProc,
&Segmento.DirProc,
};

const Numerico Ciclos_Programa =	             //nombre variable
      {/*Jon*/NumHandler,						     /* funcion que procesa al box*/
      &dCiclos,									     /* direccion en la E2Prom */
			"CicL",							         //nombre display
			0,									    		 //pos punto dec
			&next_prog,					 //parametro que modifica. 
			R_Ciclos_Programa,NULL					 //enter rap,enter mant
			};

/*Condicion de Emergencia JMP*/
/*****************************/

const TDato dCondEmerJMP={
  	 &condicion_emer,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_0, &Lim_5,	 /*Limites inferior y superior del dato*/
		 0
};
const Textual Cond_Emergencia_JMP=
      {/*Jon*/TxtHandler,						/* funcion que procesa al box*/
			&dCondEmerJMP,											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"EMEr",									//nombre display
			TEXTOS_EMERGENCIA_GRAL,             // Array donde estan los textos
			NULL,						 //parametro que modifica.
			(PunteroF*)&Segmento.DirProc,NULL					 //Proximos estados
			};

/*Tolerancia*/
/************/
const TDato dTolerancia={
  	 &tolerancia,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
     #if CANTIDAD_CANALES == 1
		 &PRom[R_Lim_Inf], &PRom[R_Lim_Sup],								 //limites
     #else
		 &Lim_m1999, &Lim_9999,				  				 //limites
     #endif
     0
};
const Numerico Tolerancia =	             //nombre variable
      {
      /*Jon*/NumHandler,						     /* funcion que procesa al box*/
      &dTolerancia,									     /* direccion en la E2Prom */
			"toL ",							         //nombre display
#if CANTIDAD_CANALES == 1
      DECIMALES_CANAL1,											 //pos punto dec
#else
			0,									    		 //pos punto dec
#endif
			NULL,					 //parametro que modifica. 
			(PunteroF*)&Tipo_Tolerancia.DirProc,NULL					 //enter rap,enter mant
			};
			
/*Tipo de Tolerancia */
/*********************/
const TDato dTTolerancia={
  	 &tipo_tolerancia,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_0, &Lim_4,	 /*Limites inferior y superior del dato*/
		 0
};
const PunteroF R_Tipo_Tolerancia[2]={
&Cond_Emergencia.DirProc,
&Segmento.DirProc
};

const Textual Tipo_Tolerancia=
      {
      /*Jon*/TxtHandler,						/* funcion que procesa al box*/
			&dTTolerancia,											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"ttoL",									//nombre display
			TEXTOS_TOLERANCIAS_GRAL,             // Array donde estan los textos
			&next_tol,						 //parametro que modifica
			R_Tipo_Tolerancia,NULL					 //Proximos estados
			};			

/*Condicion de Emergencia*/
/*************************/
const TDato dCondEmergencia={
  	 &condicion_emer,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_0, &Lim_4,	 /*Limites inferior y superior del dato*/
		 0
};
const Textual Cond_Emergencia=
      {/*Jon*/TxtHandler,						/* funcion que procesa al box*/
			&dCondEmergencia,											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"EMEr",									//nombre display
			TEXTOS_EMERGENCIA_GRAL,             // Array donde estan los textos
			NULL,						 //parametro que modifica.
			(PunteroF*)&Segmento.DirProc,NULL					 //Proximos estados
			};

#endif

/************************************/
/************************************/
/* PARAMETROS de Programador (viejo)*/
/************************************/
/************************************/
#else
/*Temperatura inicial*/
/*********************/
														
const TDato dTempInicial={
  	 &temperatura_f,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
     #if CANTIDAD_CANALES == 1
		 &Lim_Inf_ch[0], &Lim_Sup_ch[0],								 //limites
     #else
		 &Lim_m1999, &Lim_9999,				  				 //limites
     #endif
     0
};
const Numerico Temp_i =	             //nombre variable
      {Temperatura1Handler,						     /* funcion que procesa al box*/
      &dTempInicial,									     /* direccion en la E2Prom */
			"1S 1",							         //nombre display
#if CANTIDAD_CANALES == 1
      DECIMALES_CANAL1,											 //pos punto dec
#else
			0,									    		 //pos punto dec
#endif
			NULL,					 //parametro que modifica. 
			(PunteroF*)&Cond_Emergencia.DirProc,NULL,					 //enter rap,enter mant
			};	
			
/*Temperatura */
/**************/
const TDato dTempFinal={
  	 &temperatura_f,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
     #if CANTIDAD_CANALES == 1
		 &Lim_Inf_ch[0], &Lim_Sup_ch[0],								 //limites
     #else
		 &Lim_m1999, &Lim_9999,				  				 //limites
     #endif
     0
};

    						 
const Numerico Temp_f =	             //nombre variable
      {Temperatura2Handler,						     /* funcion que procesa al box*/
      &dTempFinal,									     /* direccion en la E2Prom */
			"1S 1",							         //nombre display
#if CANTIDAD_CANALES == 1
      DECIMALES_CANAL1,											 //pos punto dec
#else
			0,									    		 //pos punto dec
#endif
			NULL,					 //parametro que modifica. 
			(PunteroF*)&Cond_Emergencia.DirProc,NULL					 //enter rap,enter mant
			};			

/*Condicion de Emergencia*/
/*************************/
const char * const TEXTOS_EMERGENCIA_GRAL[5]={		// Hay 3 condiciones de emergencia que utilizan 
			"cc  ",																// estos strings incluyendo este
			"rS  ",
			"rP  ",
			"EndP",
			"End "
};


const PunteroF R_Emergencia[2]={
&Tiempo.DirProc,
&Temp_i.DirProc
};

const TDato dCondEmergencia={
  	 &condicion_emer,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_0, &Lim_5,	 /*Limites inferior y superior del dato*/
		 0
};
const Textual Cond_Emergencia=
      {CondEHandler,						/* funcion que procesa al box*/
			&dCondEmergencia,											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"1E 1",									//nombre display
			TEXTOS_EMERGENCIA_GRAL,             // Array donde estan los textos
			&Lim_0,						 //parametro que modifica.
			R_Emergencia,NULL					 //Proximos estados
			};
			
/*Tiempo*/
/********/
const TDato dTiempo={
  	 &tiempo,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_1, &Lim_9999,	 /*Limites inferior y superior del dato*/
		 0
};


const Numerico Tiempo =	             //nombre variable
      {TiempoHandler,						     /* funcion que procesa al box*/
      &dTiempo,									     /* direccion en la E2Prom */
			"1t 1",							         //nombre display
			0,									    		 //pos punto dec
			NULL,					 //parametro que modifica. 
			(PunteroF*)&Tolerancia.DirProc,NULL					 //enter rap,enter mant
			};

/*Tolerancia*/
/************/
const TDato dTolerancia={
  	 &tolerancia,		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
     #ifdef jony_05_07
		 &Lim_m1999, &Lim_9999,				  				 //limites
     #endif
     0
};

const Numerico Tolerancia =	             //nombre variable
      {
      ToleranciaHandler,						     /* funcion que procesa al box*/
      &dTolerancia,									     /* direccion en la E2Prom */
			"1d 1",							         //nombre display
#if CANTIDAD_CANALES == 1
      DECIMALES_CANAL1,											 //pos punto dec
#else
			0,									    		 //pos punto dec
#endif
			NULL,					 //parametro que modifica. 
			(PunteroF*)&Temp_f.DirProc,NULL					 //enter rap,enter mant
			};

#endif