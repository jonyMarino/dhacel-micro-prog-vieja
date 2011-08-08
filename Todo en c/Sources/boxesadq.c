#include "boxesPrn.h"
#include "boxesAdq.h"
#include "boxes.h"
#include "cnfbox.h"

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
