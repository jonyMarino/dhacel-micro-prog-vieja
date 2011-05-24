/*MODULE  nodo
**     Filename  : nodo.C
**     Project   : Controlador
**     Processor : MC9S12GC32CFU
**     Version   : 0.0.1
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 23/01/2008, 15:11
**     Abstract  :
**							Nodo utilizable para distintas
**      estructuras de dato(Listas, arboles, etc.) 
*/

#include <stdarg.h>
#include "stddef.h"
#include "nodo.h"
#include "nodo_protected.h"

#pragma CODE_SEG  NODO_CODE
#pragma DATA_SEG  NODO_DATA
#pragma CONST_SEG DEFAULT

void Nodo_DefConstructor(void * _self,va_list *args);
Byte Nodo_differ(void*_self,struct Nodo* node);

const struct Class Nodo={
    CLASS_INITIALIZATION(Class,
                       Nodo,
                       Object,
                       Nodo_DefConstructor,
                       NULL,
                       NULL,
                       NULL)
};

/*
** ===================================================================
**     Method     : Nodo_Constructor
**    Description : Constructor del Objeto
** ===================================================================
*/
void Nodo_Constructor(void * _self,void*dato,struct Nodo* next){
  struct Nodo* _n = _self;
  
  _n->dato=dato;
  _n->next=next;  
}
/*
** ===================================================================
**     Method     : Nodo_DefConstructor
**    Description : Constructor por defecto del Objeto
** ===================================================================
*/
void Nodo_DefConstructor(void * _self,va_list *args){
  Nodo_Constructor(_self,va_arg(*args,void*),va_arg(*args,struct Nodo*));  
}
/*
** ===================================================================
**     Method     : Nodo_getDato 
**    Description : Evento al llegar al tiempo fijado
** ===================================================================
*/
void* Nodo_getDato(void*_self){
  struct Nodo* _n = _self;
  
  return _n->dato;

}

/*
** ===================================================================
**     Method     : Nodo_setDato 
**    Description : setea el dato
** ===================================================================
*/
void  Nodo_setDato(void*_self,void* dato){
  struct Nodo* _n = _self;
  
  _n->dato=dato;

}


/*
** ===================================================================
**     Method     : Nodo_getNext 
**    Description : Devuelve el puntero al siguiente nodo
** ===================================================================
*/
struct Nodo* Nodo_getNext(void*_self){
  struct Nodo* _n = _self;
  
  return _n->next;
}

/*
** ===================================================================
**     Method     : Nodo_setNext 
**    Description : Setea el puntero al siguiente nodo
** ===================================================================
*/
void Nodo_setNext(void*_self,struct Nodo * nodo){
  struct Nodo* _n = _self;
  
  _n->next=nodo;
}

/*
** ===================================================================
**     Method     : Nodo_differ 
**    Description : Compara los datos de los 2 nodos
** ===================================================================
*/
Byte Nodo_differ(void*_self,struct Nodo* node){
  struct Nodo* _n = _self;
  
  return _n->dato!=node->dato;
}



