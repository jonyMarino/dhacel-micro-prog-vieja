#include "Mydefines.h"
#include "boxes.h"
#include "In1.h"
#include "display.h"
#include "hold.h"

#ifdef HOLD

extern int ValFinal[4];
extern byte DotNum[CANTIDAD_DISPLAYS];
int bufferH;
bool flagHold;
	
//estando definido HOLD si el pin In1 esta en nivel bajo la indicacion en el
//display superior no varia, de lo contrario, si esta en nivel alto funciona normalmente

void mantenerIndicacion (void){
  
  if(!In1_GetVal()){
     bufferH=ValFinal[0];
     flagHold=FALSE;
     Pasar_Numero(ValFinal[0],0,DotNum[0]);  
  } else {
      flagHold=TRUE;
      Pasar_Numero(bufferH,0,DotNum[0]);   
  }
}

int getBuffer() {

  return bufferH;

}

bool getFlagHold(){
  
  return flagHold;
}
#endif