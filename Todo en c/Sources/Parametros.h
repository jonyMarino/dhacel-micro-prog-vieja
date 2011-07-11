#ifndef PARAMETROS_H
#define PARAMETROS_H

#include "Mydefines.h"
#include "cnfbox.h"
#include "Sensores.h"			
#include "paramdefines.h"	

extern volatile const int PRom[R_ATA+1];

#define getValParametro(n) PRom[n] 


#endif