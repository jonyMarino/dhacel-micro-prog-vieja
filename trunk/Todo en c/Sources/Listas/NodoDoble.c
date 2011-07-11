      /*MODULE  NodoDoble
**     Filename  : NodoDoble.C
**     Project   : Controlador
**     Processor : MC9S12GC32CFU
**     Version   : 0.0.1
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 23/01/2008, 15:11
**     Abstract  :
**							Nodo utilizable para Listas doblemente
**     enlazadas
*/

#include <stdarg.h>
#include "stddef.h"
#include "NodoDoble.h"


#pragma DATA_SEG NodoDoble_DATA                                            
#pragma CODE_SEG NodoDoble_CODE 
#pragma CONST_SEG DEFAULT

struct NodoDoble * prev;
/*
** ===================================================================
**     Method     : NodoDoble
**    Description : Constructor del Objeto
** ===================================================================
*/
void initNodoDoble(void*dato,struct NodoDoble* next,struct NodoDoble* _prev)
{
    initNodo(dato,next);
    prev=_prev;

}


void setPrev(struct NodoDoble * nodo){
  prev = nodo;
}


struct NodoDoble * getPrev(void)
{ 
  return prev; 

}

#pragma CODE_SEG NodoDoble_CODE

