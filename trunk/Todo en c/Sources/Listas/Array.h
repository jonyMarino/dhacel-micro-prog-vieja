#ifndef _ARRAY_H
#define _ARRAY_H

#include <limits.h>

#include "PE_Types.h"

#pragma DATA_SEG ARRAY_LIST_DATA                                            
#pragma CODE_SEG ARRAY_LIST_CODE 
#pragma CONST_SEG DEFAULT


typedef struct {
  void ** nodos;
  int cantidad;
  bool objetosSonPropios;
}Array;

void initArray(int cantidad);

void* Array_get(int index);

void Array_set(int index,void * dato);

void Array_ownsObjects(bool owns);

int Array_count();

bool Array_isEmpty();

void Array_delete();


#define INITIALIZATION_ARRAY(CLASS,ARRAY)\
  &CLASS,    \
  (void**)&ARRAY,		\
  sizeof(ARRAY)/sizeof(void*), \
  FALSE				
  
  
#define NEW_ARRAY(name,ARRAY) \
   Array name = { \
    INITIALIZATION_ARRAY(Array,ARRAY) \
  }
  
 
#pragma DATA_SEG DEFAULT                                            
#pragma CODE_SEG DEFAULT 



#endif