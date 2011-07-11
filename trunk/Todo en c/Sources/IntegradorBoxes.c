#include "IntegradorBoxes.h"
#include "cnfbox.h"
#include "boxes.h"

#ifdef INTEGRADOR
 
 extern const Textual Unidad;
 extern const Textual Resolucion;
 extern const Titulo Integrador;
 extern const Textual RESET;
 
 const Titulo Integrador={ 
      TitleHandler,							/* funcion que procesa al box*/
			"int ",										//nombre display
			"IntEGrAdor    ",										//nombre display
      NULL, 
			(PunteroF*)&Unidad.DirProc,
			&Setting.DirProc					
    
			};
 
 
 const char * const Unidades[3]={
      "HS  ",									
			"min ",
			"SEG "
};

const Textual Unidad=
      {TxtHandler,						         /* funcion que procesa al box*/
			&Parametros[R_UND],							 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"UnidAdES    ",									         //nombre display
			Unidades,                        // Array donde estan los textos
			NULL,						                 //parametro que modifica.
      (PunteroF*)&Resolucion.DirProc,
       NULL				//enter rap,enter mant			
      
 			};
 			
 
 const char * const resoluciones[3]={
      "U/1 ",									
			"U/10",
			"U/100    "
		
};

const Textual Resolucion=
      {TxtHandler,						         /* funcion que procesa al box*/
			&Parametros[R_RSL],							 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"rESoLUcion    ",									         //nombre display
			resoluciones,                        // Array donde estan los textos
			NULL,						                 //parametro que modifica.
      (PunteroF*)&Principal1.DirProc,
       NULL				//enter rap,enter mant			
      
 			};

 
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