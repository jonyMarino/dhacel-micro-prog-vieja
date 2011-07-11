#ifndef _DOBLE_LIST_H
#define _DOBLE_LIST_H

#include "NodoDoble.h"
#include "LinkedList.h"

//void * differ (struct DobleList * _l);
    
unsigned char DobleList_deleteNodo(struct NodoDoble * _n);
    
struct NodoDoble * DobleList_insertFirst(void * dato);

void * DobleList_getDato(unsigned int nNodo);

unsigned int DobleList_cantidad(void);

void* DobleList_getDato();

#endif