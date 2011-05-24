#include "PE_Types.h"
#include "cnfbox.h"
#include "Control.h"
#include "mydefines.h"
#include "vprop.h"


 #ifdef VPROP

/**************************************************************************************************/

/*Controla la posicion de la valvula propo sin realimentacion.Tambien da la posicion 
inicial de la valvula .Recibe el duty,topen en segundos, dband en porcentaje, 
tiempo de smpl en ms, y lleva un contador con la pos de la valvula.
Por problemas de precision, el contador debe ir de 0 a CMAX para 0 a 100%
Si en topn la valvula recorre de 0 a CMAX, en smpl recorre CMAX * smpl /topn */

  int outvprop(int dutyc,								  //duty de 0 a 1000
             int topen,											//topen en segundos
             int dbnd,											//banda muerta en %
             int smpl,											//tiempo de actualizacion en ms
             long *cont_pos){								//pos de valvula de 0 a 1000000 por presicion
  

#define CMAX 1000000
#define OPEN 1
#define CLOSE 2
#define REST 0


int sal;
long aper = 0;
static int inivp = FALSE;                                 //para la init
																													

if(dbnd < 2)
  dbnd = 2;                                               //limito la dbnd

aper = *cont_pos;   											                //tomo pos de valvula
																													//para la init este cont comienza en cero
if(inivp == FALSE){																				//si estoy en init
  aper = aper + smpl;																			//cuento
  sal = CLOSE;																						//cierro
  if(aper > (long)topen * 1500){                          //y veo si llegue a topen * 1,5 milisegundos
    inivp = TRUE;																					//si llegue saco flag y ya no entro mas aqui
    aper = 0;																							//y aviso que estoy en cero apertura
  }
}

else {
  if(aper > CMAX)
    aper = CMAX;
  if(aper < 0)
    aper = 0;																              //limito
  
  if ((aper * 1000)/CMAX <= dutyc-dbnd){		              //si aper menos que duty, abro
    aper = aper + CMAX *(long)smpl/((long)topen*1000);
    sal = OPEN;
  }else if ((aper * 1000)/CMAX >= dutyc+dbnd){			      //si aper mas que duty, cierro
    aper = aper - CMAX *(long)smpl/((long)topen*1000);    //paso topen a ms 
		sal = CLOSE;
  }else sal = REST;																	      //si nada de lo ant, no muevo

  
  if(dutyc >= 1000-dbnd)																	//corrijo los extremos
    sal = OPEN;
  if(dutyc <= 0+dbnd)
    sal = CLOSE;
}

  *cont_pos = aper;																				//recupero
  return sal;
             }
  
           
 #endif

  



































