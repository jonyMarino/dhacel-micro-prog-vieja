#ifndef _NODO_DOBLE_H
#define _NODO_DOBLE_H

#include "nodo.h"

#pragma DATA_SEG NodoDoble_DATA                                            
#pragma CODE_SEG NodoDoble_CODE 

struct NodoDbl{
  struct Nodo _base;
  struct NodoDbl * prev;
};

extern const struct Class NodoDbl;

 
/*
** ===================================================================
**     Method     : NodoDbl_Constructor
**    Description : Constructor del Objeto
** ===================================================================
*/
//new(&NodoDbl,void * dato,struct NodoDbl *next,
//       struct NodoDbl *prev);
//newAlloced(void*self,NodoDbl,void * dato,struct NodoDbl *next,
//            struct NodoDbl *prev);
/*
** ===================================================================
**     Method     : Nodo_getPrev 
**    Description : Devuelve el puntero al nodo anterior
** ===================================================================
*/
struct NodoDbl * Nodo_getPrev(void*_self);

#pragma DATA_SEG DEFAULT                                            
#pragma CODE_SEG DEFAULT 


#endif