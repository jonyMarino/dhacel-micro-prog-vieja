#include "cnfbox.h"
#include "boxesRPM.h"
#include "boxescal.h"

#ifdef RPM 
const Titulo _RPM={ 
      TitleHandler,						/* funcion que procesa al box*/
			"rPM ",									//nombre display
			" -- ",									//nombre display
			NULL,							//parametro que modifica.
			(PunteroF*)&PulsosPorVuelta.DirProc,&Setting.DirProc  //Proximos estados
			};

const Numerico PulsosPorVuelta={
      NumHandler,					/* funcion que procesa al box*/
      &Parametros[R_Rpm],									/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"PPV",							//nombre display
			0,									/* punto decimal a enc */
			NULL,					//parametro que modifica.
		  (PunteroF*)&AjusteRpm.DirProc,NULL			//Proximos estados	
			
};

const Numerico DecimalesRpm={
      NumHandler,					/* funcion que procesa al box*/
      &Parametros[R_Decimales],									/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"dec",							//nombre display
			0,									/* punto decimal a enc */
			NULL,					//parametro que modifica.
			(PunteroF*)&Filtro.DirProc,NULL			//Proximos estados
      
};

const Numerico Filtro={
      NumHandler,					/* funcion que procesa al box*/
      &Parametros[R_Fil],									/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"FiL",							//nombre display
			1,									/* punto decimal a enc */
			NULL,					//parametro que modifica.
			(PunteroF*)&AAnlLow1.DirProc,NULL			//Proximos estados
      
};

const Numerico AjusteRpm={
      NumHandler,					/* funcion que procesa al box*/
      &Parametros[R_AJU],									/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AJt",							//nombre display
			3,									/* punto decimal a enc */
			NULL,					//parametro que modifica.
			(PunteroF*)&DecimalesRpm.DirProc,NULL			//Proximos estados
      
};
#endif