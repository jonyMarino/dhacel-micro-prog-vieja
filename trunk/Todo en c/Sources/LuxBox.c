#include "LuxBox.h"
#include "cnfbox.h"
#include "boxes.h"

#ifdef LUX
 
 extern const Textual RESET;
 
 const char * const reset[2]={
      "no  ",									
			"Si  ",
};

const Textual RESET=
      {TxtHandler,						         /* funcion que procesa al box*/
			&Parametros[R_RSTL],							 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"rESEt    ",									         //nombre display
			reset,                        // Array donde estan los textos
			NULL,						                 //parametro que modifica.
      (PunteroF*)&Principal1.DirProc,
       NULL				//enter rap,enter mant			
      
 			};
 			
#endif