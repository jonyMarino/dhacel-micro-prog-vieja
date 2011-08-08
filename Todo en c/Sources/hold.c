#include "Mydefines.h"
#include "boxes.h"
#include "In1.h"
#include "display.h"
#include "hold.h"

#ifdef HOLD

int bufferH;
bool flagHold;
	
//estando definido HOLD si el pin In1 esta en nivel bajo la indicacion en el
//display superior no varia, de lo contrario, si esta en nivel alto funciona normalmente

void mantenerIndicacion (void){
  
  if(!In1_GetVal()){
     bufferH=getValCalculado(0);
     flagHold=FALSE;
     PresentarValor(getValCalculado(0),0);
     //Pasar_Numero(getValCalculado(0),0,DotNum[0]);  
  } else {
      flagHold=TRUE;
      PresentarValor(bufferH,0);
      //Pasar_Numero(bufferH,0,DotNum[0]);   
  }
}

int getBuffer() {

  return bufferH;

}

bool getFlagHold(){
  
  return flagHold;
}
#endif