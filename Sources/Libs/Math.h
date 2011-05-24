#ifndef _Math
#define _Math
#include "PE_Types.h"

#define max(a,b) (a>b)?a:b;

int pow10 (byte expo);

//int FloatToInt(float valor,byte decimales);

/*
** ===================================================================
**     Function      :  get_Decimales 
**     Description :  Devuelve la cantidad de decimales que tiene el
**                    valor pasado
** ===================================================================
*/
int get_Decimales(int val);
/*
** ===================================================================
**     Function      :  get_HexDig 
**     Description :  Devuelve un digito en Hexa
** ===================================================================
*/
char get_HexDig(word val);

#endif