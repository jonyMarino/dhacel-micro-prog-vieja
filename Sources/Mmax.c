#include "Mydefines.h"
#include "boxes.h"
#include "In1.h"
#include "display.h"
#include "Mmax.h"

#ifdef MMAX

extern int ValFinal[4];
extern byte DotNum[CANTIDAD_DISPLAYS];
int bufferMmax;
bool flagMmax;
	
void calculoMmax (){
   if(!In1_GetVal()){
     bufferMmax=ValFinal[0];
     flagMmax=FALSE;
     
  } else {
      flagMmax=TRUE;
      if(ValFinal[0]>bufferMmax)
        bufferMmax=ValFinal[0];
      
  }
}






void mantenerMaxIndicacion (void){
  
  if(flagMmax==FALSE){
     
     Pasar_Numero(ValFinal[0],0,DotNum[0]);  
  } else {
      Pasar_Numero(bufferMmax,0,DotNum[0]);   
  }
}


int getBufferMmax() {

  return bufferMmax;

}

bool getFlagMmax(){
  
  return flagMmax;
}

#endif