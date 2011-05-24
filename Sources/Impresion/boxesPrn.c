#include <stddef.h>
#include "boxesPrn.h"
#include "MIPConf.h"
#include "BTFPConf.h"
#include "boxes.h"
#include "cnfbox.h"
#include "EPM203Conf.h"

const Textual boxFuente;
const Textual boxDireccion;

const Numerico boxAnio =	             //nombre variable
      {NumHandler,						     /* funcion que procesa al box*/
      &Parametros[R_PrnAnio],									     /* direccion en la E2Prom */
			"A;o ",							         //nombre display
			0,									    		 //pos punto dec
			NULL,					 //parametro que modifica. 
			(PunteroF*)&boxMes.DirProc,NULL					 //enter rap,enter mant
			};
			

		
const Numerico boxMes =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
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
			NULL,							 //parametro que modifica. 
			(PunteroF*)&boxIntervalo.DirProc,NULL					 //enter rap,enter mant
			};
			
			
const Numerico boxIntervalo =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &Parametros[R_PrnIntervalo],											 /* direccion en la E2Prom */
			"int ",							 //nombre display
			0,											 //pos punto dec
			NULL,							 //parametro que modifica. 
			(PunteroF*)&boxFuente.DirProc,NULL					 //enter rap,enter mant
			};

const char  * const strFuentes[3]={
      "8.16",
      "1220",
      "7.16"
};

extern volatile const struct EPM203Conf epm203Conf;
extern struct EPM203Manejador os;

byte setFuente(int val,byte a){
  return EPM203Manejador_setFuente(&os,val);
}

const TDato datoFuente={
  (char*)&epm203Conf+offsetof(struct EPM203Conf,fuente),							      /* Direccion del parámetro a Escribir*/
  setFuente,		/* Funcion a ejecutar Luego de la escritura del parámetro*/
  &Lim_0,&Lim_3,			        /* Limites inferior y superior */
  0										// canal
};
const Textual boxFuente=
      {
      TxtHandler,						            /* funcion que procesa al box*/
			&datoFuente,					  /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"FuEntE    ",									          //nombre display
			strFuentes,                       // Array donde estan los textos
			NULL,						                  //parametro que modifica.
			(PunteroF*)&boxDireccion.DirProc,NULL	//Proximos estados
			};			
			
static const char * strDireccion[]={
      "dir ",
      "inv "
};

byte setDireccion(int val,byte a){
  return EPM203Manejador_setDireccion(&os,val);
}

const TDato datoDireccion={
  (char*)&epm203Conf+offsetof(struct EPM203Conf,direccion),							      /* Direccion del parámetro a Escribir*/
  setDireccion,		/* Funcion a ejecutar Luego de la escritura del parámetro*/
  &Lim_0,&Lim_2,			        /* Limites inferior y superior */
  0										// canal
};
const Textual boxDireccion=
      {
      TxtHandler,						            /* funcion que procesa al box*/
			&datoDireccion,					  /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"DirEccion    ",									          //nombre display
			strDireccion,                       // Array donde estan los textos
			NULL,						                  //parametro que modifica.
			(PunteroF*)&Principal1.DirProc,NULL	//Proximos estados
			};