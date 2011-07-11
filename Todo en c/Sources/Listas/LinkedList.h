#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include "PE_Types.h"
#include "stddef.h"
#include "nodo.h"
#include "ConfLista.h"

#pragma CODE_SEG  LinkedList_CODE
#pragma DATA_SEG  LinkedList_DATA

#define CANT_MAX_LISTAS   10

typedef struct {
  struct Nodo * _nodo;
}LinkedList;


void LinkedListInit(unsigned char NroList);

void deleteLinkedList(unsigned char NroList);

unsigned int LinkedList_size(unsigned char NroList);

bool LinkedList_contains(void * dato,unsigned char NroList);

void * LinkedList_eliminarPrimero(unsigned char NroList);

void * LinkedList_get(unsigned int index,unsigned char NroList);
  
bool LinkedList_moveOut(void * dato,unsigned char NroList);
  
bool LinkedList_isEmpty(unsigned char NroList);

void LinkedList_clear(unsigned char NroList);

bool LinkedList_add(void *dato,unsigned char NroList);

bool LinkedList_aniadirFinal(void *dato,unsigned char NroList);

bool hasNext(struct Nodo * _nodo); 

void *getDataObj(struct Nodo * _nodo); 

struct Nodo * LinkedList_getPrimerNodo(unsigned char NroList); 
  
void LinkedList_setPrimerNodo(struct Nodo *,unsigned char NroList); 

void LinkedList_deleteNodo(struct Nodo*tmpNodo,struct Nodo*tmpNodoAnterior,unsigned char NroList);
 

#pragma CODE_SEG  DEFAULT
#pragma DATA_SEG  DEFAULT

#endif