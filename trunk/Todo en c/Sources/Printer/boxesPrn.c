#include <stddef.h>
#include "boxesPrn.h"
#include "boxes.h"
#include "cnfbox.h"

extern const Textual boxFuente;
extern const Textual boxDireccion;

#ifdef _PRINTER

const Numerico boxAnio =	             //nombre variable
      {YearHandler,						     /* funcion que procesa al box*/
      &Parametros[R_PrnAnio],									     /* direccion en la E2Prom */
			"Anio",							         //nombre display
			0,									    		 //pos punto dec
			NULL,					 //parametro que modifica. 
			(PunteroF*)&boxMes.DirProc,NULL					 //enter rap,enter mant
			};
			

		
const Numerico boxMes =	 //nombre variable
      {MonthHandler,						 /* funcion que procesa al box*/
      &Parametros[R_PrnMes],											 /* direccion en la E2Prom */
			"mES ",							 //nombre display
			0,											 //pos punto dec
			NULL,							 //parametro que modifica. 
			(PunteroF*)&boxDia.DirProc,NULL					 //enter rap,enter mant
			};
			
			
		
const Numerico boxDia =	 //nombre variable
      {
      DiaHandler,						 /* funcion que procesa al box*/
      &Parametros[R_PrnDia],											 /* direccion en la E2Prom */
			"diA ",							 //nombre display
			0,											 //pos punto dec
			NULL,							 //parametro que modifica. 
			(PunteroF*)&boxHora.DirProc,NULL					 //enter rap,enter mant
			};
			
const Numerico boxHora =	 //nombre variable
      {
      HoraHandler,						 /* funcion que procesa al box*/
      &Parametros[R_PrnHora],											 /* direccion en la E2Prom */
			"HorA",							 //nombre display
			2,											 //pos punto dec
			NULL,
			#ifdef ADQ
			(PunteroF*)&Principal1.DirProc,NULL
			#else							 
			(PunteroF*)&boxIntervalo.DirProc,NULL					 //enter rap,enter mant
			#endif
			};
			
			
		
const Numerico boxIntervalo =	          //nombre variable
      {NumHandler,						                  /* funcion que procesa al box*/
      &Parametros[R_PrnIntervalo],											 /* direccion en la E2Prom */
			"int ",							                        //nombre display
			0,											               //pos punto dec
			NULL,							                        //parametro que modifica. 
			(PunteroF*)&boxFuente.DirProc,NULL					 //enter rap,enter mant
			};

const char  * const strFuentes[3]={
      "8.16",
      "1220",
      "7.16"
};


const Textual boxFuente=
      {
      TxtHandler,						            /* funcion que procesa al box*/
			&Parametros[R_PrnFuente],					          /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"FuEntE    ",									          //nombre display
			strFuentes,                       // Array donde estan los textos
			NULL,						                  //parametro que modifica.
			(PunteroF*)&boxDireccion.DirProc,NULL	//Proximos estados
			};			
	
static const char * strDireccion[]={
      "dir ",
      "inv "
};


const Textual boxDireccion=
      {
      TxtHandler,						            /* funcion que procesa al box*/
			&Parametros[R_PrnDireccion],					           /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
	  	"dirEccion    ",									          //nombre display
			strDireccion,                            // Array donde estan los textos
			NULL,						                        //parametro que modifica.
			(PunteroF*)&Principal1.DirProc,NULL	    //Proximos estados
			};
			   
#endif