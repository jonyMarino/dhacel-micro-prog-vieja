#ifndef _Comunicacion
#define _Comunicacion

#include "AS1.h"

/* MODULE Comunicacion*/

extern bool AS1_Tx;
extern byte ASespera;
extern byte Step;


void ModBus_Recive(AS1_TComData dat);

void ModBus_Send(void);

#endif