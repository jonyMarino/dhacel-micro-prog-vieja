/* PARAMETROS de calibracion */
/*****************************/

#include "Mydefines.h"
#include "cnfbox.h"
#include "boxes.h"
#include "Programador.h"
#include "PE_Types.h"
#include "Sensores.h"
#include "boxescal.h"

#pragma CONST_SEG DEFAULT 
#pragma STRING_SEG DEFAULT

extern const Textual TipoSensor1;
extern const Numerico Offset1;
extern const Numerico Ganancia1;
//extern constTextual Filtro1;
extern const Numerico Filtro1;
extern const Textual Comunicacion;

#if  CANTIDAD_CANALES>1
extern const Textual TipoSensor2;
extern const Numerico Offset2;
extern const Numerico Ganancia2;
extern const Numerico Filtro2;
extern const Numerico AAnlLow2;
extern const Numerico AAnlHi2;
#endif

extern const Numerico ACT1;
extern const Numerico AGT1;
extern const Numerico ATA1;
extern const Numerico ACP1;
extern const Numerico AGP1;
extern const Numerico AAnlLow1;
extern const Numerico AAnlHi1;

extern const NumericoRO Ta;

#ifdef VF 
extern const Textual mostrarDypInf;
extern const Numerico LimiteInfVelocidad;
extern const Numerico LimiteInfTempMeseta;
#endif

#if  CANTIDAD_CANALES>1
extern const Numerico ACT2;
extern const Numerico AGT2;
extern const Numerico ACP2;
extern const Numerico AGP2;
#endif
/* tipo de sensor canal 1 */
/**************************/
#ifdef _APARATO_VIEJO 
const char  * const T_Sensores[NUM_SENSORES]={
      "Jb  ",
      "JA  ",									
			"n   ",/* Pltinel*/
			"kb  ",
			"kA  ",
			"S   ",
			"r   ",
			"b   ",		/*K S r t Pt Lin1234*/
			"t   ",
			"Pt  ",
			#ifdef pirani
			"Pir ",
      "Pr1 ",
			#endif
			"Lin1",
			"Lin2",
			"Lin3",
			"Lin4",
			"MiLV",
			#ifdef SENS_HUMEDAD
			"HUm ",
			#endif
};
#else
const char  * const T_Sensores[NUM_SENSORES]={
      "J   ",									
			"n   ",
			"k   ",
			"S   ",
			"r   ",
			"t   ",
			"Pt  ",
			#ifdef pirani
			"Pir ",
			#endif
			"Lin ",
			"MiLV",
};
#endif
const Textual TipoSensor1=
      {
      TxtHandler,						            /* funcion que procesa al box*/
			&Parametros[R_Sensor],					  /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"SEn1",									          //nombre display
			T_Sensores,                       // Array donde estan los textos
			NULL,						                  //parametro que modifica.
			(PunteroF*)&Offset1.DirProc,NULL	//Proximos estados
			};
			

/* offset canal1*/
const Numerico Offset1={
      NumHandler,					              /* funcion que procesa al box*/
      &Parametros[R_Offset],						/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"oFS1",							              //nombre display
			DECIMALES_CANAL1,									/* punto decimal a enc */
			NULL,					                    //parametro que modifica.
 			(PunteroF*)&Ganancia1.DirProc,NULL//Proximos estados
			};
			
			
/* ganancia*/
const Numerico Ganancia1={
      NumHandler,						            /* funcion que procesa al box*/
      &Parametros[R_Gan],								/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"gAn1",								            //nombre display
			3,										            /* punto decimal a enc */
			NULL,						                  //parametro que modifica.
			(PunteroF*)&Filtro1.DirProc,&Comunicacion.DirProc	//Proximos estados
			};


const char  * const T_Com[2]={
      "dHt ",
      "dHS "									
		
};
		
const Textual Comunicacion=
  
      {
      TxtHandler,						            /* funcion que procesa al box*/
			&Parametros[R_COM],					  /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Com ",									          //nombre display
			T_Com,                       // Array donde estan los textos
			NULL,						                  //parametro que modifica.
			(PunteroF*)&ACT1.DirProc,NULL	//Proximos estados
			};
			




/*ajuste cero canal 1*/
const Numerico ACT1={
      NumHandler,									      /* funcion que procesa al box*/
      &Parametros[R_ACT],								/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AC 1",							              /* texto del titulo */
			DECIMALES_CANAL1,									/* punto decimal a enc */
			NULL,									            //Parametro que modifica
			(PunteroF*)&AGT1.DirProc,NULL			//Proximos estados
			};

/*ajuste gan canal 1*/
const Numerico AGT1={
      NumHandler,									      /* funcion que procesa al box*/
      &Parametros[R_AGT],								/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Ag 1",							              /* texto del titulo */
			1,													      /* punto decimal a enc */
			NULL,									            //Parametro que modifica
			(PunteroF*)&ACP1.DirProc,NULL			//Proximos estados
			};

/*ajuste cero Pt100 canal 1*/			
const Numerico ACP1={
      NumHandler,									      /* funcion que procesa al box*/
      &Parametros[R_ACP],								/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"ACP1",							              /* texto del titulo */
			DECIMALES_CANAL1,									/* punto decimal a enc */
			NULL,									            //Parametro que modifica
			(PunteroF*)&AGP1.DirProc,NULL			//Proximos estados
			};

/*ajuste gan Pt100 canal 1*/		
const Numerico AGP1={
      NumHandler,									      /* funcion que procesa al box*/
      &Parametros[R_AGP],								/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AGP1",							              /* texto del titulo */
			1,													      /* punto decimal a enc */
			NULL,									            //Parametro que modifica
//	(PunteroF*)&AAnlLow1.DirProc,NULL	//Proximos estados
  	//(PunteroF*)&ATA1.DirProc,NULL			//Proximos estados
    (PunteroF*)&Ta.DirProc,NULL	
			};			

/*ajuste minimo retr alarma */		
const Numerico AAnlLow1={
      NumHandler,									      /* funcion que procesa al box*/
      &Parametros[R_AAnlLow],						/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
		  #if CANTIDAD_CANALES>1
			"AnL1",							              /* texto del titulo */
      #else
      "AnL ",
      #endif
//  	1,													      /* punto decimal a enc */
      DECIMALES_CANAL1,
			NULL,									            //Parametro que modifica
			(PunteroF*)&AAnlHi1.DirProc,NULL	//Proximos estados
			};			

/*ajuste maximo retr alarma */		
const Numerico AAnlHi1={
      NumHandler,									      /* funcion que procesa al box*/
      &Parametros[R_AAnlHi],						/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
		  #if CANTIDAD_CANALES>1
			"AnH1",							              /* texto del titulo */
      #else
      "AnH ",
      #endif
//		1,													      /* punto decimal a enc */
      DECIMALES_CANAL1,
			NULL,									            //Parametro que modifica
      #if CANTIDAD_CANALES>1
      (PunteroF*)&AAnlLow2.DirProc,NULL
      #else
      (PunteroF*)&Principal1.DirProc,NULL
      #endif
			};			

/*ajuste temp ambiente*/		
const Numerico ATA1={
      NumHandler,									      /* funcion que procesa al box*/
      &Parametros[R_ATA],								/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AtA ",							              /* texto del titulo */
			1,													      /* punto decimal a enc */
			NULL,									            //Parametro que modifica
			#ifdef VF
			(PunteroF*)&mostrarDypInf.DirProc,NULL//Proximos estados
			#else
			(PunteroF*)&Ganancia1.DirProc,NULL//Proximos estados
			#endif
			};



const NumericoRO Ta={

      NumHandlerEsp,									      /* funcion que procesa al box*/
		  NULL,	
			"tA  ",							              /* texto del titulo */
			1,													      /* punto decimal a enc */
			NULL,	
      (PunteroF*)&ATA1.DirProc,NULL			//Proximos estados
};


#ifdef VF
#define _VF 0
#define _SP 1
const char  * const T_Mostrar[2]={
      "UF  ",									
			"SP  ",
			
};

const Textual mostrarDypInf=
      {
      TxtHandler,						            /* funcion que procesa al box*/
			&Parametros[R_MVF],					  /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"m   ",									          //nombre display
			T_Mostrar,                       // Array donde estan los textos
			NULL,						                  //parametro que modifica.
			(PunteroF*)&LimiteInfVelocidad.DirProc,NULL	//Proximos estados
			};
			

const Numerico LimiteInfTempMeseta={
      NumHandler,									      /* funcion que procesa al box*/
      &Parametros[R_LIMs],						/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			" Li mSt ",							              /* texto del titulo */
			0,													      /* punto decimal a enc */
			NULL,									            //Parametro que modifica
      (PunteroF*)&Ganancia1.DirProc,NULL	//Proximos estados
			};
			
			
const Numerico LimiteInfVelocidad={
      NumHandler,									      /* funcion que procesa al box*/
      &Parametros[R_LIVel],						/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			" Li VEL ",							              /* texto del titulo */
			0,													      /* punto decimal a enc */
			NULL,									            //Parametro que modifica
      (PunteroF*)&LimiteInfTempMeseta.DirProc,NULL	//Proximos estados
			};			

#endif
/* filtro canal 1 */
const Numerico Filtro1={
      NumHandler,									      /* funcion que procesa al box*/
      &Parametros[R_Filtro1],						/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"FiL1",							              /* texto del titulo */
			1,													      /* punto decimal a enc */
			NULL,									            //Parametro que modifica
#if CANTIDAD_CANALES>1
			(PunteroF*)&TipoSensor2.DirProc,NULL//Proximos estados
#else

    	(PunteroF*)&AAnlLow1.DirProc,NULL	//Proximos estados
//  	(PunteroF*)&Principal1.DirProc,NULL //Proximos estados
#endif
			};
  
#if CANTIDAD_CANALES>1

/* tipo de sensor canal 2*/		
const Textual TipoSensor2=
      {TxtHandler,						          /* funcion que procesa al box*/
			&Parametros[R_Sensor+1],					/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"SEn2",									          //nombre display
			T_Sensores,                       // Array donde estan los textos
			NULL,							                //parametro que modifica.
			(PunteroF*)&Offset2.DirProc,NULL	//Proximos estados
			};
	
/* offset canal2*/
const Numerico Offset2={
      NumHandler,					              /* funcion que procesa al box*/
      &Parametros[R_Offset+1],					/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"oFS2",							              //nombre display
			DECIMALES_CANAL2,									/* punto decimal a enc */
			NULL,					                    //parametro que modifica.
			(PunteroF*)&Ganancia2.DirProc,NULL//Proximos estados
			};

/* ganancia canal 2*/
const Numerico Ganancia2={
      NumHandler,						            /* funcion que procesa al box*/
      &Parametros[R_Gan+1],							/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"gAn2",								            //nombre display
			3,										            /* punto decimal a enc */
			NULL,						                  //parametro que modifica.
			(PunteroF*)&Filtro2.DirProc,&ACT2.DirProc//Proximos estados
			};

/* filtro canal 2 */
const Numerico Filtro2={
      NumHandler,									      /* funcion que procesa al box*/
      &Parametros[R_Filtro1+1],					/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"FiL2",							              /* texto del titulo */
			1,													      /* punto decimal a enc */
			NULL,									            //Parametro que modifica
//	(PunteroF*)&Principal1.DirProc,NULL//Proximos estados
  	(PunteroF*)&AAnlLow1.DirProc,NULL	//Proximos estados

			};

/*ajuste cero canal 2*/
const Numerico ACT2={
      NumHandler,									      /* funcion que procesa al box*/
      &Parametros[R_ACT+1],							/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AC 2",							              /* texto del titulo */
			DECIMALES_CANAL2,									/* punto decimal a enc */													      /* punto decimal a enc */
			NULL,									            //Parametro que modifica
			(PunteroF*)&AGT2.DirProc,NULL			//Proximos estados
			};

/*ajuste gan canal 2*/			
const Numerico AGT2={
      NumHandler,									      /* funcion que procesa al box*/
      &Parametros[R_AGT+1],							/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Ag 2",       							      /* texto del titulo */
			1,													      /* punto decimal a enc */
			NULL,									            //Parametro que modifica
			(PunteroF*)&ACP2.DirProc,NULL			//Proximos estados
			};


/*ajuste cero Pt100 canal 2*/		
const Numerico ACP2={
      NumHandler,									      /* funcion que procesa al box*/
      &Parametros[R_ACP+1],							/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"ACP2",							              /* texto del titulo */
			DECIMALES_CANAL2,						      /* punto decimal a enc */
			NULL,									            //Parametro que modifica
			(PunteroF*)&AGP2.DirProc,NULL			//Proximos estados
			};


/*ajuste gan Pt100 canal 2*/		
const Numerico AGP2={
      NumHandler,									      /* funcion que procesa al box*/
      &Parametros[R_AGP+1],							/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AGP2",							              /* texto del titulo */
			1,													      /* punto decimal a enc */
			NULL,									            //Parametro que modifica
			(PunteroF*)&Ganancia2.DirProc,NULL//Proximos estados
			};
			
			
/*ajuste minimo retr alarma */		
const Numerico AAnlLow2={
      NumHandler,									      /* funcion que procesa al box*/
      &Parametros[R_AAnlLow+1],						/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AnL2",							              /* texto del titulo */
//  	1,													      /* punto decimal a enc */
      DECIMALES_CANAL2,
			NULL,									            //Parametro que modifica
			(PunteroF*)&AAnlHi2.DirProc,NULL	//Proximos estados
			};			

/*ajuste maximo retr alarma */		
const Numerico AAnlHi2={
      NumHandler,									      /* funcion que procesa al box*/
      &Parametros[R_AAnlHi+1],						/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AnH2",							              /* texto del titulo */
//		1,													      /* punto decimal a enc */
      DECIMALES_CANAL2,
			NULL,									            //Parametro que modifica
//	(PunteroF*)&ATA1.DirProc,NULL			//Proximos estados
    (PunteroF*)&Principal1.DirProc,NULL
			};			
			
#endif			