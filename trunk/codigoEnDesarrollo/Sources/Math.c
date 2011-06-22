#include "Math.h"

static const word div_dec[]={1,10,100,1000,10000};// divisores del decimal/////////////////////////

word pow10 (byte expo){

//if (expo>4) return 0;
  
return div_dec[expo];
}



///////////////////////////////////////////////////////////////////////
// Rutina de codificacion de numeros float a integer 
//
// IN:  Numero.
// OUT: Numero en ASCII.
///////////////////////////////////////////////////////////////////////
/*int FloatToInt(float valor,byte decimales)
{ int valor_int;
  byte i;		  
 
 for (i=0;i<decimales;i++)
  valor*=10;
 
 valor+=0.5;
 valor_int= valor;
 return valor_int;
}
*/