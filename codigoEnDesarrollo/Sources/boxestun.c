#include "Mydefines.h"
#include "cnfbox.h"
#include "boxes.h"
#include "PE_Types.h"
#include "boxestun.h"

extern volatile const int PRom[];

#pragma CONST_SEG DEFAULT 
#pragma STRING_SEG DEFAULT


extern const Numerico C1RES;
extern const Numerico C1H;
extern const Numerico C1AB;
extern const Textual C1PER;
extern const Textual C1PRA1;
extern const Textual C1PRA2;
extern const Textual C1PRA3;
extern const Numerico C1IN;
extern const Numerico C1DR;
extern const Numerico C1HA1;
extern const Numerico C1HA2;
extern const Numerico C1HA3;
extern const Numerico C1ABA1;
extern const Numerico C1ABA2;
extern const Numerico C1ABA3;
extern const Textual C1STN;


#if  CANTIDAD_CANALES>1
extern const Numerico C2RES;
extern const Numerico C2H;
extern const Numerico C2AB;
extern const Textual C2PER;
extern const Textual C2PRA;
extern const Numerico C2IN;
extern const Numerico C2DR;
extern const Numerico C2HA;
extern const Numerico C2ABA;
extern const Textual C2STN;
#endif

/* PARAMETROS de sintonia  (tun)  */
/**********************************/		

//dan1001
#ifdef _APARATO_VIEJO
/*  Reset canal 1 para aparato viejo*/
/************************************/

const Numerico C1RES =	                          //nombre variable
      {NumHandler,						                    //funcion que procesa al box
      &Parametros[R_Reset],											  /* direccion en la E2Prom */
			"rES1",							                        //nombre display
			DECIMALES_CANAL1,											      //pos punto dec
      NULL,
      (const PunteroF*)&C1PER.DirProc,NULL				//enter rap,enter mant			};
      };
#else
/*  Reset canal 1 para aparato nuevo*/
/************************************/

const PunteroF R_C1RES[2]={ 
    &C1H.DirProc,
    &C1PER.DirProc
    }; 

const Numerico C1RES =	                          //nombre variable
      {NumHandler,						                    /* funcion que procesa al box*/
      &Parametros[R_Reset],											        /* direccion en la E2Prom */
			"rES1",							                        //nombre display
			DECIMALES_CANAL1,											      //pos punto dec
			(int *)&PRom[R_Tcontrol1],							    //parametro que modifica. 
			R_C1RES,NULL					                      //enter rap,enter mant
			};
#endif
//dan1001


/*  Periodo control 1*/
/*********************/
const char * const Periodos[10]={
      ".1  ",									
			".2  ",
			".5  ",
			"1   ",
			"2   ",
			"5   ",
			"10  ",
			"20  ",
			"50  ",
			"AnL ",
};

const Textual C1PER=
      {TxtHandler,						                 /* funcion que procesa al box*/
			&Parametros[R_Per],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Pr 1",									                 //nombre display
			&Periodos[0],                            //Array donde estan los textos
			NULL,						                         //parametro que modifica.
			(PunteroF*)&C1AB.DirProc,NULL					   //Proximos estados
			};
		
    

//dan1001
#ifdef _APARATO_VIEJO
/*  Histeresis/AnchoBanda control1 para aparato viejo*/
/*****************************************************/
const Numerico C1AB =	 //nombre variable
      {ABHandler,						 /* funcion que procesa al box*/
      &Parametros[R_H1],											 /* direccion en la E2Prom */
			"Ab 1",							 //nombre display
			DECIMALES_CANAL1,											 //pos punto dec
			NULL,							 //parametro que modifica. 
			(const PunteroF*)&C1IN.DirProc,NULL					 //enter rap,enter mant
			};
#else
/*  Histeresis/AnchoBanda control1 para aparato nuevo*/
/*****************************************************/
 const PunteroF R_C1H[8]={
&C1HA1.DirProc,
&C1HA1.DirProc,
&C1PRA1.DirProc,
&C1PRA1.DirProc,
&C1HA1.DirProc,
&C1HA1.DirProc,
&C1HA1.DirProc,
&Principal1.DirProc

};

const Numerico C1H =	                          //nombre variable
      {NumHandler,						                  /* funcion que procesa al box*/
      &Parametros[R_H1],											        /* direccion en la E2Prom */
			"H  1",							                      //nombre display
			DECIMALES_CANAL1,											    //pos punto dec
			(int *)&PRom[R_Talar1],						    //parametro que modifica.
			R_C1H,NULL					                      //enter rap,enter mant
			};
		
const Numerico C1AB =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &Parametros[R_H1],											 /* direccion en la E2Prom */
			"Ab 1",							 //nombre display
			DECIMALES_CANAL1,											 //pos punto dec
			NULL,							 //parametro que modifica. 
			(const PunteroF*)&C1IN.DirProc,NULL					 //enter rap,enter mant
			};

#endif
//dan1001

/*  Integral control1*/
/*********************/
const Numerico C1IN =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &Parametros[R_IN1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"in 1",							 //nombre display
			0,											 //pos punto dec
			NULL,							 //parametro que modifica. 
			(const PunteroF*)&C1DR.DirProc,NULL					 //enter rap,enter mant
			};

//dan1001
#ifdef _APARATO_VIEJO
/*  derivada control1 para aparato viejo*/
/****************************************/
const Numerico C1DR =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &Parametros[R_DR1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"dr 1",							 //nombre display
			0,											 //pos punto dec
			NULL,							 //parametro que modifica. 
			(PunteroF*)&C1ABA1.DirProc,NULL					 //enter rap,enter mant
			};

#else
/*  derivada control1 para aparato nuevo*/
/****************************************/
const Numerico C1DR =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &Parametros[R_DR1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"dr 1",							 //nombre display
			0,											 //pos punto dec
			NULL,							 //parametro que modifica. 
			(PunteroF*)&C1STN.DirProc,NULL					 //enter rap,enter mant
			};
#endif
//dan1001

//dan1001
#ifdef _APARATO_VIEJO
 /*  Auto sintonias aparato viejo*/
/*********************************/
const PunteroF R_C1STN1[8]={
&C1HA1.DirProc,
&C1HA1.DirProc,
&C1PRA1.DirProc,
&C1PRA1.DirProc,
&C1HA1.DirProc,
&C1HA1.DirProc,
&C1HA1.DirProc,
&Principal1.DirProc
};

const PunteroF R_C1STN2[8]={
&C1HA2.DirProc,
&C1HA2.DirProc,
&C1PRA2.DirProc,
&C1PRA2.DirProc,
&C1HA2.DirProc,
&C1HA2.DirProc,
&C1HA2.DirProc,
&Principal1.DirProc
};

const PunteroF R_C1STN3[8]={
&C1HA3.DirProc,
&C1HA3.DirProc,
&C1PRA3.DirProc,
&C1PRA3.DirProc,
&C1HA3.DirProc,
&C1HA3.DirProc,
&C1HA3.DirProc,
&Principal1.DirProc
};

//andy
/*
const char * Selftune[6]={
      "oFF ",									
			"Ston",
			"StoK",
			"SErr",
			"StoK",
			"SErr"
};
*/
const char * const Selftune[2]={
      "oFF ",									
			"Ston",
};
//andy

const Textual C1STN=
      {TxtHandler,						         /* funcion que procesa al box*/
			&Parametros[R_Stn],							 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"St 1",									         //nombre display
			Selftune,                        // Array donde estan los textos
			NULL,						                 //parametro que modifica.

   #if CANTIDAD_CANALES > 1
      #ifdef SP_EXTERNO
  			(PunteroF*)&Principal1.DirProc,NULL				//enter rap,enter mant			
      #else
  			(PunteroF*)&C2RES.DirProc,NULL				//enter rap,enter mant			
      #endif
	 #else
	    #ifdef DH102
	    (PunteroF*)&Principal1.DirProc,&Calibracion.DirProc	
	    #else
			(PunteroF*)&Principal1.DirProc,NULL				//enter rap,enter mant			
	     #endif
	 #endif
 			};
#else
/*  Auto sintonias aparato nuevo*/
/********************************/
const PunteroF R_C1STN1[8]={
&C1HA1.DirProc,
&C1HA1.DirProc,
&C1PRA1.DirProc,
&C1PRA1.DirProc,
&C1HA1.DirProc,
&C1HA1.DirProc,
&C1HA1.DirProc,
&Principal1.DirProc
};

const PunteroF R_C1STN2[8]={
&C1HA2.DirProc,
&C1HA2.DirProc,
&C1PRA2.DirProc,
&C1PRA2.DirProc,
&C1HA2.DirProc,
&C1HA2.DirProc,
&C1HA2.DirProc,
&Principal1.DirProc
};

const PunteroF R_C1STN3[8]={
&C1HA3.DirProc,
&C1HA3.DirProc,
&C1PRA3.DirProc,
&C1PRA3.DirProc,
&C1HA3.DirProc,
&C1HA3.DirProc,
&C1HA3.DirProc,
&Principal1.DirProc
};

const char * const Selftune[6]={
      "oFF ",									
			"Ston",
			"StoK",
			"SErr",
			"StoK",
			"SErr"
};


const Textual C1STN=
      {TxtHandler,						              /* funcion que procesa al box*/
			&Parametros[R_Stn],											    /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"St 1",									              //nombre display
			Selftune,                      // Array donde estan los textos
			(int *)&PRom[R_Talar1],						    //parametro que modifica.
			R_C1STN1,NULL		                      //enter rap,enter mant
 			};
#endif
//dan1001


//dan1001
#ifdef _APARATO_VIEJO
/*  Histeresis/AnchoBanda Alarma1 para aparato viejo*/
/****************************************************/
const Numerico C1ABA1 =	                   //nombre variable
      {ABAHandler,						             /* funcion que procesa al box*/
      &Parametros[R_HA1],											   /* direccion en la E2Prom */
			"AbA1",							                 //nombre display
			DECIMALES_CANAL1,										 //pos punto dec
      #if ALARMAS_CH1 > 1
 			NULL,						    //parametro que modifica.
			(PunteroF*)&C1ABA2.DirProc,NULL				//enter rap,enter mant			
      #else
			NULL,							                        //parametro que modifica. 
			(PunteroF*)&C1STN.DirProc,NULL				//enter rap,enter mant			
      #endif
			};
#else
/*  Histeresis/AnchoBanda Alarma1 para aparato nuevo*/
/****************************************************/
const Numerico C1HA1 =	                    //nombre variable
      {NumHandler,						              /* funcion que procesa al box*/
      &Parametros[R_HA1],											    /* direccion en la E2Prom */
			"HA 1",							                  //nombre display
			DECIMALES_CANAL1,											//pos punto dec
      #if ALARMAS_CH1 > 1
 			(int *)&PRom[R_Talar1+1],						  //parametro que modifica.
			R_C1STN2,NULL		                      //enter rap,enter mant
      #else
			NULL,							                    //parametro que modifica. 
			(const PunteroF*)&Principal1.DirProc,NULL		//enter rap,enter mant
      #endif
			};
			
const Numerico C1ABA1 =	                   //nombre variable
      {NumHandler,						             /* funcion que procesa al box*/
      &Parametros[R_HA1],											   /* direccion en la E2Prom */
			"AbA1",							                 //nombre display
			DECIMALES_CANAL1,										 //pos punto dec
      #if ALARMAS_CH1 > 1
 			(int *)&PRom[R_Talar1+1],						    //parametro que modifica.
			R_C1STN2,NULL		                      //enter rap,enter mant
      #else
			NULL,							                        //parametro que modifica. 
			(PunteroF*)&Principal1.DirProc,NULL				//enter rap,enter mant			
      #endif
			};

#endif
//dan1001

//dan1001
#ifdef _APARATO_VIEJO
/*  Histeresis/AnchoBanda Alarma2 para aparato viejo*/
/****************************************************/
const Numerico C1ABA2 =	                   //nombre variable
      {ABAHandler,						             /* funcion que procesa al box*/
      &Parametros[R_HA1+1],											   /* direccion en la E2Prom */
			"AbA2",							                 //nombre display
			DECIMALES_CANAL1,										 //pos punto dec
      #if ALARMAS_CH1 > 2
 			NULL,						    //parametro que modifica.
			(PunteroF*)&C1ABA3.DirProc,NULL				//enter rap,enter mant			
      #else
			NULL,							                        //parametro que modifica. 
			(PunteroF*)&C1STN.DirProc,NULL				//enter rap,enter mant			
      #endif
			};
#else
/*  Histeresis/AnchoBanda Alarma2 para aparato nuevo*/
/****************************************************/
const Numerico C1HA2 =	                    //nombre variable
      {ABAHandler,						              /* funcion que procesa al box*/
      &Parametros[R_HA1+1],											    /* direccion en la E2Prom */
			"AbA2",							                  //nombre display
			DECIMALES_CANAL1,											//pos punto dec
      #if ALARMAS_CH1 > 2
 			(int *)&PRom[R_Talar1+2],						  //parametro que modifica.
			R_C1STN2,NULL		                      //enter rap,enter mant
      #else
			NULL,							                    //parametro que modifica. 
			(const PunteroF*)&Principal1.DirProc,NULL		//enter rap,enter mant
      #endif
			};
			
const Numerico C1ABA2 =	                   //nombre variable
      {ABAHandler,						             /* funcion que procesa al box*/
      &Parametros[R_HA1+1],											   /* direccion en la E2Prom */
			"AbA2",							                 //nombre display
			DECIMALES_CANAL1,										 //pos punto dec
      #if ALARMAS_CH1 > 2
 			(int *)&PRom[R_Talar1+2],						    //parametro que modifica.
			R_C1STN3,NULL		                      //enter rap,enter mant
      #else
			NULL,							                        //parametro que modifica. 
			(PunteroF*)&Principal1.DirProc,NULL				//enter rap,enter mant			
      #endif
			};

#endif
//dan1001



//dan1001
#ifdef _APARATO_VIEJO
/*  Histeresis/AnchoBanda Alarma3 para aparato viejo*/
/****************************************************/
const Numerico C1ABA3 =	                   //nombre variable
      {ABAHandler,						             /* funcion que procesa al box*/
      &Parametros[R_HA1+2],											   /* direccion en la E2Prom */
			"AbA3",							                 //nombre display
			DECIMALES_CANAL1,										 //pos punto dec
			NULL,							                        //parametro que modifica. 


#ifdef EQA1
			(PunteroF*)&Principal1.DirProc,NULL				//enter rap,enter mant			
#else
			(PunteroF*)&C1STN.DirProc,NULL				//enter rap,enter mant			

#endif


			};
#else
/*  Histeresis/AnchoBanda Alarma3 para aparato nuevo*/
/****************************************************/
const Numerico C1HA3 =	                    //nombre variable
      {ABAHandler,						              /* funcion que procesa al box*/
      &Parametros[R_HA1+2],											    /* direccion en la E2Prom */
			"HA 3",							                  //nombre display
			DECIMALES_CANAL1,											//pos punto dec
			NULL,							                    //parametro que modifica. 
			(const PunteroF*)&Principal1.DirProc,NULL		//enter rap,enter mant
			};
			
const Numerico C1ABA3 =	                   //nombre variable
      {ABAHandler,						             /* funcion que procesa al box*/
      &Parametros[R_HA1+2],											   /* direccion en la E2Prom */
			"AbA3",							                 //nombre display
			DECIMALES_CANAL1,										 //pos punto dec
			NULL,							                        //parametro que modifica. 
			(PunteroF*)&Principal1.DirProc,NULL				//enter rap,enter mant			
			};
#endif
//dan1001

/* Periodo Alarma1*/
/******************/
TDato dPeriodoA1={
  	 &PRom[R_Pra],		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_0, &Lim_10				//limites
};


const Textual C1PRA1=
      {TxtHandler,						             /* funcion que procesa al box*/
			&Parametros[R_Pra],											   /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"PrA1",									             //nombre display
			Periodos,                     //Array donde estan los textos
			NULL,						                     //parametro que modifica.
			(PunteroF*)&C1ABA1.DirProc,NULL			 //Proximos estados			
			};

/* Periodo Alarma2*/
/******************/
TDato dPeriodoA2={
  	 &PRom[R_Pra+1],		/* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					/* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_0, &Lim_10				//limites
};

const Textual C1PRA2=
      {TxtHandler,						             /* funcion que procesa al box*/
			&Parametros[R_Pra+1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"PrA2",									             //nombre display
			Periodos,                     //Array donde estan los textos
			NULL,						                     //parametro que modifica.
			(PunteroF*)&C1ABA2.DirProc,NULL			 //Proximos estados			
			};

/* Periodo Alarma3*/
/******************/
const Textual C1PRA3=
      {TxtHandler,						             /* funcion que procesa al box*/
			&Parametros[R_Pra+2],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"PrA3",									             //nombre display
			Periodos,                     //Array donde estan los textos
			NULL,						                     //parametro que modifica.
			(PunteroF*)&C1ABA3.DirProc,NULL			 //Proximos estados			
			};



#if CANTIDAD_CANALES>1
/* Reset Canal2*/
/***************/

#ifndef _APARATO_VIEJO

const PunteroF R_C2RES[2]={ 
    &C2H.DirProc,
    &C2PER.DirProc
    }; 
    
const Numerico C2RES =	                          //nombre variable
      {NumHandler,						                    /* funcion que procesa al box*/
      &Parametros[R_Reset+1],											      /* direccion en la E2Prom */
			"rES2",							                        //nombre display
			DECIMALES_CANAL2,											      //pos punto dec
			(int *)&PRom[R_Tcontrol1+1],							  //parametro que modifica. 
			R_C2RES,NULL					                      //enter rap,enter mant
			};

#else

  const Numerico C2RES =	                          //nombre variable
      {NumHandler,						                    //funcion que procesa al box
      &Parametros[R_Reset+1],											  /* direccion en la E2Prom */
			"rES2",							                        //nombre display
			DECIMALES_CANAL2,											      //pos punto dec
      NULL,
      (const PunteroF*)&C2PER.DirProc,NULL				//enter rap,enter mant			};
      };


 #endif
 


/* Periodo Control2*/
/*******************/
const Textual C2PER=
      {TxtHandler,						                  /* funcion que procesa al box*/
			&Parametros[R_Per+1],											      /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Pr 2",									                  //nombre display
			Periodos,
			NULL,						                          //parametro que modifica.
			(PunteroF*)&C2AB.DirProc,NULL					    //Proximos estados
			};

/* Periodo Canal2 Alarma1*/
/*************************/
const Textual C2PRA=
      {TxtHandler,						                  /* funcion que procesa al box*/
			&Parametros[R_Pra+ALARMAS_CH1],											      /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"PrA2",									                  //nombre display
			Periodos,
		  NULL,						                          //parametro que modifica.
			(PunteroF*)&C2ABA.DirProc,NULL					  //Proximos estados
			};

/* Histeresis/AnchoBanda Control2 */
/**********************************/
#ifndef _APARATO_VIEJO
const PunteroF R_C2H[7]={ 
    &C2HA.DirProc,
    &C2PRA.DirProc,
    &C2PRA.DirProc,
    &C2PRA.DirProc,
    &C2PRA.DirProc,
    &C2PRA.DirProc,
    &C2PRA.DirProc
    }; 
    
const Numerico C2H =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &Parametros[R_H1+1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"H  2",							 //nombre display
			DECIMALES_CANAL2,											 //pos punto dec
			(int *)&PRom[R_Talar1+1],							 //parametro que modifica. 
			R_C2H,NULL					 //enter rap,enter mant
			};

const Numerico C2AB =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &Parametros[R_H1+1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Ab 2",							 //nombre display
			DECIMALES_CANAL2,											 //pos punto dec
			NULL,							 //parametro que modifica. 
			(const PunteroF*)&C2IN.DirProc,NULL					 //enter rap,enter mant
			};
#else
const Numerico C2AB =	 //nombre variable
      {ABHandler,						 /* funcion que procesa al box*/
      &Parametros[R_H1+1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Ab 2",							 //nombre display
			DECIMALES_CANAL2,											 //pos punto dec
			NULL,							 //parametro que modifica. 
			(const PunteroF*)&C2IN.DirProc,NULL					 //enter rap,enter mant
			};
#endif
/* Integral Control2 */
/**********************/
const Numerico C2IN =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &Parametros[R_IN1+1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"in 2",							 //nombre display
			0,											 //pos punto dec
			NULL,							 //parametro que modifica. 
			(PunteroF*)&C2DR.DirProc,NULL					 //enter rap,enter mant
			};

/* Derivada Control2 */
/*********************/

const Numerico C2DR =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &Parametros[R_DR1+1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"dr 2",							 //nombre display
			0,											 //pos punto dec
			NULL,							 //parametro que modifica. 
			(PunteroF*)&C2ABA.DirProc,NULL					 //enter rap,enter mant
			};

/* Histeresis/AnchoBanda Alarma2 */
/**********************************/
const Numerico C2HA =	 //nombre variable
      {ABAHandler,						 /* funcion que procesa al box*/
      &Parametros[R_HA1+1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AbA2",							 //nombre display
			DECIMALES_CANAL2,											 //pos punto dec
			NULL,							 //parametro que modifica. 
			(PunteroF*)&C2STN.DirProc,NULL					 //enter rap,enter mant
			};

const Numerico C2ABA =	  //nombre variable
      {ABAHandler,						 /* funcion que procesa al box*/
      &Parametros[R_HA1+1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AbA2",							 //nombre display
			#if CANTIDAD_CANALES>1
			DECIMALES_CANAL2,											 //pos punto dec
			#else
			DECIMALES_CANAL1,											 //pos punto dec
			#endif
			NULL,							 //parametro que modifica. 
			(PunteroF*)&C2STN.DirProc,NULL					 //enter rap,enter mant
			};

/* AutoSintonia Control2 */
/*************************/
const Textual C2STN=
      {TxtHandler,						              /* funcion que procesa al box*/
			&Parametros[R_Stn+1],											  /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"St 2",									              //nombre display
			Selftune,                      // Array donde estan los textos
			NULL,						                      //parametro que modifica.
			(PunteroF*)&Principal1.DirProc,NULL	  //enter rap,enter mant
			};

#endif