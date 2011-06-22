/** ###################################################################
**     MODULO DE DETERMINACIÓN DE CONSTANTES EN TIEMPO DE COMPILACION
**     PARA LLEGAR A ALGUN TIEMPO DE CONVERSION ESPECÍFICA
**     Filename  : AD_time.H
**     Project   : 26_07
**     Processor : MC9S12GC32CFU16
**     Version   : 0.0.1
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 23/07/2007, 12:24
**     Abstract  :
**          Este archivo esta pensado para ingresar las constantes que  
**          deben ser usadas para obtener dentro de cierta 
**          cantidad de canales(CANTIDAD_CANALES), un tiempo determinado
**          de conversión.
**     Comment   :
**         Usa:
**             CANTIDAD_CANALES (Mydefines.h)
**              AD_2s						(Mydefines.h)
**              AD_1s						(Mydefines.h)
**              AD_500ms				(Mydefines.h)
**              AD_250ms				(Mydefines.h)
**     Contents  :
**         KNMEDCH		- Cantidad de mediciones para un canal
**         KDIV_AD		- Constante por la que se divide cada canal
**         DIV_TA			- Constante por la que se divide la temperatura ambiente
**     Reference  :
**          AD_TIME(en segundos) = 0.000112*KNMEDCH*(CANTIDAD_CANALES+1) +0,04 (tiempo de grabacion)
**     mail      : marinon87@hotmail.com
** ###################################################################*/

#ifndef AD_TIME_H
#define AD_TIME_H




#include "Mydefines.h"




#if CANTIDAD_CANALES==2
  #ifdef AD_2s
    #define KNMEDCH     5714		 /*Numero de secuencias para obteber el resultado final*/

  #elif defined(AD_1s)
    #define KNMEDCH     2857		 /*Numero de secuencias para obteber el resultado final*/
  
  #elif defined(AD_500ms) 
    #define KNMEDCH     1428		 /*Numero de secuencias para obteber el resultado final*/
  
  #elif defined(AD_250ms)
    #define KNMEDCH     714		 /*Numero de secuencias para obteber el resultado final*/
  
  #endif
  
#elif CANTIDAD_CANALES==1
  #ifdef AD_2s
    #define KNMEDCH     8568		 /*Numero de secuencias para obteber el resultado final*/

  #elif defined(AD_1s)
  	#define KNMEDCH     4284		 /*Numero de secuencias para obteber el resultado final*/
  
  #elif defined(AD_500ms) 
    #define KNMEDCH     2142		 /*Numero de secuencias para obteber el resultado final*/
  
  #elif defined(AD_250ms)
    #define KNMEDCH      1071		 /*Numero de secuencias para obteber el resultado final*/
  
  #endif
  
#endif   



#endif  // FIN de archivo