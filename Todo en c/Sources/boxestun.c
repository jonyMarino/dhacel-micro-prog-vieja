#include "Mydefines.h"
#include "cnfbox.h"
#include "boxes.h"
#include "PE_Types.h"
#include "boxestun.h"
#include "Parametros.h"


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

/*  Reset canal 1 */
/******************/

const Numerico C1RES =	                          //nombre variable
      {NumHandler,						                    //funcion que procesa al box
      &Parametros[R_Reset],											  /* direccion en la E2Prom */
			"rES1",							                        //nombre display
			DECIMALES_CANAL1,											      //pos punto dec
      NULL,
      (const PunteroF*)&C1PER.DirProc,NULL				//enter rap,enter mant			};
      };


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
		
    

/*  Histeresis/AnchoBanda control1 */
/***********************************/

const Numerico C1AB =	                            //nombre variable
      {ABHandler,						                      /* funcion que procesa al box*/
      &Parametros[R_H1],											    /* direccion en la E2Prom */
			"Ab 1",							                        //nombre display
			DECIMALES_CANAL1,											      //pos punto dec
			NULL,							                          //parametro que modifica. 
			(const PunteroF*)&C1IN.DirProc,NULL					//enter rap,enter mant
			};

/*  Integral control1*/
/*********************/
const Numerico C1IN =	                              //nombre variable
      {NumHandler,						                      /* funcion que procesa al box*/
      &Parametros[R_IN1],											      /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"in 1",							                          //nombre display
			0,											                      //pos punto dec
			NULL,							                            //parametro que modifica. 
			(const PunteroF*)&C1DR.DirProc,NULL					  //enter rap,enter mant
			};


/*  derivada control1 */
/**********************/

const Numerico C1DR =	                                //nombre variable
      {NumHandler,						                        /* funcion que procesa al box*/
      &Parametros[R_DR1],											        /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"dr 1",							                            //nombre display
			0,											                        //pos punto dec
			NULL,							                              //parametro que modifica. 
			(PunteroF*)&C1ABA1.DirProc,NULL					        //enter rap,enter mant
			};



 /*  Auto sintonias */
/********************/
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


const char * const Selftune[2]={
      "oFF ",									
			"Ston",
};


const Textual C1STN=
      {TxtHandler,						                //funcion que procesa al box
			&Parametros[R_Stn],							        // direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor
			"St 1",									                //nombre display
			Selftune,                               // Array donde estan los textos
			NULL,						                        //parametro que modifica.

   #if CANTIDAD_CANALES > 1
      #ifdef SP_EXTERNO
  			(PunteroF*)&Principal1.DirProc,NULL		 //enter rap,enter mant			
      #else
  			(PunteroF*)&C2RES.DirProc,NULL				 //enter rap,enter mant			
      #endif
	 #else
			(PunteroF*)&Principal1.DirProc,NULL			//enter rap,enter mant			
	 #endif
 			};



/*  Histeresis/AnchoBanda Alarma1 */
/**********************************/

const Numerico C1ABA1 =	                    //nombre variable
      {ABAHandler,						              /* funcion que procesa al box*/
      &Parametros[R_HA1],									  /* direccion en la E2Prom */
			"AbA1",							                  //nombre display
			DECIMALES_CANAL1,										  //pos punto dec
      #if ALARMAS_CH1 > 1
 			NULL,						                      //parametro que modifica.
			(PunteroF*)&C1ABA2.DirProc,NULL				//enter rap,enter mant			
      #else
			NULL,							                    //parametro que modifica. 
			(PunteroF*)&C1STN.DirProc,NULL				//enter rap,enter mant			
      #endif
			};




/*  Histeresis/AnchoBanda Alarma2 */
/**********************************/
const Numerico C1ABA2 =	                      //nombre variable
      {ABAHandler,						                /* funcion que procesa al box*/
      &Parametros[R_HA1+1],										/* direccion en la E2Prom */
			"AbA2",							                    //nombre display
			DECIMALES_CANAL1,										    //pos punto dec
      #if ALARMAS_CH1 > 2
 			NULL,						                        //parametro que modifica.
			(PunteroF*)&C1ABA3.DirProc,NULL				  //enter rap,enter mant			
      #else
			NULL,							                      //parametro que modifica. 
			(PunteroF*)&C1STN.DirProc,NULL				  //enter rap,enter mant			
      #endif
			};


/*  Histeresis/AnchoBanda Alarma3 */
/**********************************/
const Numerico C1ABA3 =	                   //nombre variable
      {ABAHandler,						             /* funcion que procesa al box*/
      &Parametros[R_HA1+2],								 /* direccion en la E2Prom */
			"AbA3",							                 //nombre display
			DECIMALES_CANAL1,										 //pos punto dec
			NULL,							                   //parametro que modifica. 

    #ifdef EQA1
			(PunteroF*)&Principal1.DirProc,NULL		//enter rap,enter mant			
    #else
			(PunteroF*)&C1STN.DirProc,NULL				//enter rap,enter mant			

    #endif

			};

/* Periodo Alarma1*/
/******************/

TDato dPeriodoA1={
  	 &PRom[R_Pra],		                        /* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					                    /* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_0, &Lim_10				                  //limites
};


const Textual C1PRA1=
      {TxtHandler,						                 /* funcion que procesa al box*/
			&Parametros[R_Pra],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"PrA1",									                 //nombre display
			Periodos,                                //Array donde estan los textos
			NULL,						                         //parametro que modifica.
			(PunteroF*)&C1ABA1.DirProc,NULL			     //Proximos estados			
			};


/* Periodo Alarma2*/
/******************/

TDato dPeriodoA2={
  	 &PRom[R_Pra+1],		                        /* Direccion donde se encuentra el dato*/			 
  	 NO_FUNCTION,					                      /* Funcion a llamar luego de la escritura del dato*/
  	 &Lim_0, &Lim_10				                    //limites
};

const Textual C1PRA2=
      {TxtHandler,						                   /* funcion que procesa al box*/
			&Parametros[R_Pra+1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"PrA2",									                   //nombre display
			Periodos,                                  //Array donde estan los textos
			NULL,						                           //parametro que modifica.
			(PunteroF*)&C1ABA2.DirProc,NULL			       //Proximos estados			
			};

/* Periodo Alarma3*/
/******************/

const Textual C1PRA3=
      {TxtHandler,						                    /* funcion que procesa al box*/
			&Parametros[R_Pra+2],									      /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"PrA3",									                    //nombre display
			Periodos,                                   //Array donde estan los textos
			NULL,						                            //parametro que modifica.
			(PunteroF*)&C1ABA3.DirProc,NULL			        //Proximos estados			
			};



#if CANTIDAD_CANALES>1
/* Reset Canal2*/
/***************/

 const Numerico C2RES =	                          //nombre variable
      {NumHandler,						                    //funcion que procesa al box
      &Parametros[R_Reset+1],											/* direccion en la E2Prom */
			"rES2",							                        //nombre display
			DECIMALES_CANAL2,											      //pos punto dec
      NULL,
      (const PunteroF*)&C2PER.DirProc,NULL				//enter rap,enter mant			};
      };


/* Periodo Control2*/
/*******************/

const Textual C2PER=
      {TxtHandler,						                  /* funcion que procesa al box*/
			&Parametros[R_Per+1],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Pr 2",									                  //nombre display
			Periodos,
			NULL,						                          //parametro que modifica.
			(PunteroF*)&C2AB.DirProc,NULL					    //Proximos estados
			};
			

/* Periodo Canal2 Alarma1*/
/*************************/

const Textual C2PRA=
      {TxtHandler,						                  /* funcion que procesa al box*/
			&Parametros[R_Pra+ALARMAS_CH1],						/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"PrA2",									                  //nombre display
			Periodos,
		  NULL,						                          //parametro que modifica.
			(PunteroF*)&C2ABA.DirProc,NULL					  //Proximos estados
			};


/* Histeresis/AnchoBanda Control2 */
/**********************************/

const Numerico C2AB =	                            //nombre variable
      {ABHandler,						                      /* funcion que procesa al box*/
      &Parametros[R_H1+1],											  /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Ab 2",							                        //nombre display
			DECIMALES_CANAL2,											      //pos punto dec
			NULL,							                          //parametro que modifica. 
			(const PunteroF*)&C2IN.DirProc,NULL					//enter rap,enter mant
			};

/* Integral Control2 */
/**********************/

const Numerico C2IN =	                             //nombre variable
      {NumHandler,						                     /* funcion que procesa al box*/
      &Parametros[R_IN1+1],											   /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"in 2",							                         //nombre display
			0,											                     //pos punto dec
			NULL,							                           //parametro que modifica. 
			(PunteroF*)&C2DR.DirProc,NULL					       //enter rap,enter mant
			};

/* Derivada Control2 */
/*********************/

const Numerico C2DR =	                              //nombre variable
      {NumHandler,						                      /* funcion que procesa al box*/
      &Parametros[R_DR1+1],											    /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"dr 2",							                          //nombre display
			0,											                      //pos punto dec
			NULL,							                            //parametro que modifica. 
			(PunteroF*)&C2ABA.DirProc,NULL					      //enter rap,enter mant
			};

/* Histeresis/AnchoBanda Alarma2 */
/**********************************/

const Numerico C2HA =	                               //nombre variable
      {ABAHandler,						                       /* funcion que procesa al box*/
      &Parametros[R_HA1+1],											     /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AbA2",							                           //nombre display
			DECIMALES_CANAL2,											         //pos punto dec
			NULL,							                             //parametro que modifica. 
			(PunteroF*)&C2STN.DirProc,NULL					       //enter rap,enter mant
			};

const Numerico C2ABA =	                              //nombre variable
      {ABAHandler,						                        /* funcion que procesa al box*/
      &Parametros[R_HA1+1],											      /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AbA2",							                            //nombre display
			#if CANTIDAD_CANALES>1
			DECIMALES_CANAL2,											          //pos punto dec
			#else
			DECIMALES_CANAL1,											          //pos punto dec
			#endif
			NULL,							                              //parametro que modifica. 
			(PunteroF*)&C2STN.DirProc,NULL					        //enter rap,enter mant
			};

/* AutoSintonia Control2 */
/*************************/

const Textual C2STN=
      {TxtHandler,						                          /* funcion que procesa al box*/
			&Parametros[R_Stn+1],											        /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"St 2",									                          //nombre display
			Selftune,                                         // Array donde estan los textos
			NULL,						                                  //parametro que modifica.
			(PunteroF*)&Principal1.DirProc,NULL	              //enter rap,enter mant
			};

#endif