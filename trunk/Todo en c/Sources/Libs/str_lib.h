#ifndef _STR_LIB_H
#define _STR_LIB_H

#include <stdlib.h>


char * IntToStr(int integer, char * str, int max_size);

/*
** ===================================================================
**     Method      :  IntToStr (bean AsynchroSerial)
**
**     Description :
**         Convierte un integer en un string ingresandolo desde str
**     donde se ingresa el digito mas significativo, luego del digito
**     menos significativo se ingresa el caracter de fin de string
**     . La varible  max_size se utiliza como control.  
**     Parameters  :
**         integer         - entero a convertir
**       * str             - Puntero donde escribir el numero en ASCII
**       max_size :        - maximo tamaño que puede ocupar integer dentro de string   
**     Returns     :
**         ---             - NULL * El entero es mas grande de lo que admite str
**                         - Posicion siguiente al digito menos significativo
** ===================================================================
*/

char * FloatToStr(int integer, char * str, int max_size,int decimales);

#endif