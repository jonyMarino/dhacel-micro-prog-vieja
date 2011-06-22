#include "Mydefines.h"
#include "cnfbox.h"
#include "boxes.h"
#include "Programador.h"
#include "PE_Types.h"
#include "boxesadq.h"

#pragma CONST_SEG DEFAULT 
#pragma STRING_SEG DEFAULT

#ifdef adquisidor

const Textual Adqui_State;
const Numerico ano;
const Numerico mes;
const Numerico dia;
const Numerico hora;
const Numerico intervalo;
const Textual Adquiriendo;

/*****************************/
/*****************************/
/* PARAMETROS de Adquisición */
/*****************************/
/*****************************/



const char * Adq_Estados[4]={
      "no  ",									
			"Si  ",
			"HLt ",
			"FULL"

};

const Textual Adqui_State=
      {EstadoHandler,						/* funcion que procesa al box*/
			&Estado_Adquisicion,											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"ESt ",									//nombre display
			4,
			Adq_Estados,                 // Array donde estan los textos
			NULL,					 //parametro que modifica.
			(PunteroF*)&ano.DirProc,NULL					 //Proximos estados
 			};

		
const Numerico ano =	             //nombre variable
      {NumHandlerRam,						     /* funcion que procesa al box*/
      &Parametros[R_Ano],									     /* direccion en la E2Prom */
			"A;o ",							         //nombre display
			0,									    		 //pos punto dec
			&Lim_2006, &Lim_2099,				  				 //limites
			NULL,					 //parametro que modifica. 
			(PunteroF*)&mes.DirProc,NULL					 //enter rap,enter mant
			};
			

		
const Numerico mes =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &Parametros[R_mes],											 /* direccion en la E2Prom */
			"mES ",							 //nombre display
			0,											 //pos punto dec
			&Lim_0, &Lim_12,								 //limites
			NULL,							 //parametro que modifica. 
			(PunteroF*)&dia.DirProc,NULL					 //enter rap,enter mant
			};
			
			
		
const Numerico dia =	 //nombre variable
      {
      DiaHandler,						 /* funcion que procesa al box*/
      &Parametros[R_dia],											 /* direccion en la E2Prom */
			"diA ",							 //nombre display
			0,											 //pos punto dec
			&Lim_1, &Lim_1,								 //limites
			NULL,							 //parametro que modifica. 
			(PunteroF*)&hora.DirProc,NULL					 //enter rap,enter mant
			};
			
					
const Numerico hora =	 //nombre variable
      {
      HoraHandler,						 /* funcion que procesa al box*/
      &Parametros[R_Hora],											 /* direccion en la E2Prom */
			"HorA",							 //nombre display
			2,											 //pos punto dec
			&Lim_0, &Lim_0,								 //limites
			NULL,							 //parametro que modifica. 
			(PunteroF*)&intervalo.DirProc,NULL					 //enter rap,enter mant
			};
			
		
const Numerico intervalo =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &Parametros[R_int],											 /* direccion en la E2Prom */
			"int ",							 //nombre display
			0,											 //pos punto dec
			&Lim_1, &Lim_9999,								 //limites
			NULL,							 //parametro que modifica. 
			(PunteroF*)&Adquiriendo.DirProc,NULL					 //enter rap,enter mant
			};



static const char * SN[2]={
      "no  ",									
			"Si  "

};

const Textual Adquiriendo=
      {TxtHandler,						/* funcion que procesa al box*/
			&Parametros[R_adq],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Adc ",									//nombre display
			2,											//Cantidad de textos validos
			&SN[0][0],             // Array donde estan los textos
			NULL,						 //parametro que modifica.
			(PunteroF*)&Adquisicion.DirProc,NULL					 //Proximos estados	
      };
#endif			
