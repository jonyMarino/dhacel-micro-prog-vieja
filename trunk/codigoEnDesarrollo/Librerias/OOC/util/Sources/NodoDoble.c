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
#include "nodo_protected.h"
#include "NodoDoble.h"


#pragma DATA_SEG NodoDoble_DATA                                            
#pragma CODE_SEG NodoDoble_CODE 
#pragma CONST_SEG DEFAULT

void NodoDbl_DefConstructor(void * _self,va_list *args);

const struct Class NodoDbl={
      CLASS_INITIALIZATION(Class,
                       NodoDbl,
                       Object,
                       NodoDbl_DefConstructor,
                       NULL,
                       NULL,
                       NULL)
};

//const void * const NodoDbl= &TableNodoDbl;

/*
** ===================================================================
**     Method     : NodoDbl_Constructor
**    Description : Constructor del Objeto
** ===================================================================
*/
void NodoDbl_Constructor(void * _self,
                            void*dato,
                            struct NodoDbl* next,
                            struct NodoDbl* prev)
{
  struct NodoDbl* _n = _self;
  
  Nodo_Constructor(_self,dato,(struct Nodo*)next);
  _n->prev=prev;  
}
/*
** ===================================================================
**     Method     : NodoDbl_DefConstructor
**    Description : Constructor por defecto del Objeto
** ===================================================================
*/
void NodoDbl_DefConstructor(void * _self,va_list *args){
  NodoDbl_Constructor(_self,va_arg(*args,void*),va_arg(*args,struct NodoDbl*),
                    va_arg(*args,struct NodoDbl*));  
}
/*
** ===================================================================
**     Method     : Nodo_getPrev 
**    Description : Devuelve el puntero al nodo anterior
** ===================================================================
*/
struct NodoDbl * Nodo_getPrev(void*_self){
  struct NodoDbl* _n = _self;
  return _n->prev;
}

#pragma CODE_SEG NodoDoble_CODE


