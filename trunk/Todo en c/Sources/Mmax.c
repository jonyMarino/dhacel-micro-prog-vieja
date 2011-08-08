#include "Mydefines.h"
#include "boxes.h"
#include "In1.h"
#include "display.h"
#include "Mmax.h"

#ifdef MMAX

int bufferMmax;
bool flagMmax;
	
void calculoMmax (){
   if(!In1_GetVal()){
     bufferMmax=getValCalculado(0);
     flagMmax=FALSE;
     
  } else {
      flagMmax=TRUE;
      if(getValCalculado(0)>bufferMmax)
        bufferMmax=getValCalculado(0);
      
  }
}






void mantenerMaxIndicacion (void){
  
  if(flagMmax==FALSE){
     PresentarValor(getValCalculado(0),0);
     //Pasar_Numero(getValCalculado(0),0,DotNum[0]);  
  } else {
      PresentarValor(bufferMmax,0);
      //Pasar_Numero(bufferMmax,0,DotNum[0]);   
  }
}


int getBufferMmax() {

  return bufferMmax;

}

bool getFlagMmax(){
  
  return flagMmax;
}

#endif