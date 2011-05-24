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
#include "NodoDoble.hpp"


#pragma DATA_SEG NodoDoble_DATA                                            
#pragma CODE_SEG NodoDoble_CODE 
#pragma CONST_SEG DEFAULT


/*
** ===================================================================
**     Method     : NodoDoble
**    Description : Constructor del Objeto
** ===================================================================
*/
NodoDoble::NodoDoble(void*dato,NodoDoble* next,NodoDoble* _prev):Nodo(dato,next),prev(_prev)
{}


void NodoDoble::setPrev(NodoDoble * nodo){
  prev = nodo;
}


#pragma CODE_SEG NodoDoble_CODE


