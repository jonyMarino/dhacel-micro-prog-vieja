/* IMPLEMENTATION MODULE MemRecursiva; */
#include "PE_Types.h"
#include "MemRecursiva.h"


static int M[Mmax] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; /* memory puffer */
static byte P;                 /* memory index */
static bool OverFlow;                         /* input value */
     // Filt1_Y;                         /* output value (READ ONLY) */



byte MemRec_Save(int valor)
{
                                
  if (P > Mmax-1){
      P = 0;
      OverFlow = TRUE;
      }    
M[P] = valor;
P++ ;
    
return P-1;
}


int  MemRec_GetRelativeVal(byte index) 
{
  unsigned int actual_index;
  byte i;
  
 if (OverFlow){
  actual_index=P+1;
  
    for (i=0; i<index;i++){
    actual_index++;
      if (actual_index > Mmax-1)
      actual_index = 0;
    }
  return M[actual_index];

 }else return M[index];


}



/* END MemRecursiva. */
