#ifndef _NODO_H
#define _NODO_H

#include "Object.h"

#pragma CODE_SEG  NODO_CODE
#pragma DATA_SEG  NODO_DATA
#pragma CONST_SEG DEFAULT

struct Nodo {
	struct Object super;
	void * dato;
	struct Nodo *next;
};

//extern const void * const Nodo;
extern const struct Class Nodo;

/*
** ===================================================================
**     Method     : Nodo_Constructor
**    Description : Constructor del Objeto
** ===================================================================
*/
//new(&Nodo,void * dato,struct Nodo *next);
//newAlloced(void*self,Nodo,void * dato,struct Nodo *next);
/*
** ===================================================================
**     Method     : Nodo_getDato 
**    Description : Evento al llegar al tiempo fijado
** ===================================================================
*/
void* Nodo_getDato(void*);

/*
** ===================================================================
**     Method     : Nodo_setDato 
**    Description : setea el dato
** ===================================================================
*/
void  Nodo_setDato(void*_self,void* dato);

/*
** ===================================================================
**     Method     : Nodo_getNext 
**    Description : Devuelve el puntero al siguiente nodo
** ===================================================================
*/
struct Nodo* Nodo_getNext(void*);

/*
** ===================================================================
**     Method     : Nodo_setNext 
**    Description : Setea el puntero al siguiente nodo
** ===================================================================
*/
void Nodo_setNext(void*_self,struct Nodo * nodo);
/*
** ===================================================================
**     Method     : Nodo_differ 
**    Description : Compara los datos de los 2 nodos
** ===================================================================
*/
Byte Nodo_differ(void*_self,struct Nodo* node);

#pragma CODE_SEG  DEFAULT
#pragma DATA_SEG  DEFAULT
#pragma CONST_SEG DEFAULT

#endif