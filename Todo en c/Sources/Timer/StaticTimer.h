#ifndef STATIC_TIMER_H
#define STATIC_TIMER_H

#include "PE_Types.h"
#include "stddef.h"


#define CONT_INIT 0

#define CANTIDAD_CONTADORES 8 /*Aumentar en 1 cada vez que se quiere utilizar otro contador*/

#define Base1ms 1
#define Base40ms 40

/* tipos de contadores*/
typedef enum {
 CONTINUO_SET,
 CONTINUO_TOG,
 UNICO_SET,
 FIN 
}tipoContador;


typedef struct{
  word cuenta;
  word tiempo;
  byte tipo;
  bool * flag;
} T_CONTADOR;


byte Timer_Run(word tiempo,bool * flag, tipoContador tipo);

void HandlerStaticTimer(byte base);






#endif