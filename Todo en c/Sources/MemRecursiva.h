#ifndef _MEM_RECURSIVA_H
#define _MEM_RECURSIVA_H

#define Mmax 15  /* max. memory buffer elements */

/*
** ===================================================================
**     Method      :  MemRec_Save 
**
**     Description : Guarda un valor en una memoria indexada
**     Parameters  : valor
**     Returns     : indice	 (en el array donde se encuentra el valor)
** ===================================================================
*/
byte MemRec_Save(int valor);

int  MemRec_GetRelativeVal(byte index); 
#endif