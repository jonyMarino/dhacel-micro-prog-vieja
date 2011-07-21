#ifndef INTEGRADOR_H
#define INTEGRADOR_H

#include "Mydefines.h"
#include "PE_Types.h"

extern long Acumulador[CANTIDAD_CANALES];
extern long valorAmostrar[CANTIDAD_CANALES];


void intIntegrador();
void _sumador(void);
void integrador_reset(void);
void integrador_decimales(void);
void getValueFlash();

#endif