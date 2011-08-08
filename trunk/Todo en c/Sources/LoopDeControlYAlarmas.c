#include "LoopDeControlYAlarmas.h"
#include "Medicion.h"
#include "Parametros.h"
#include "MethodContainer.h"
#include "Method.h"
#include "teclas.h"
#include "boxes.h"

#ifdef MEN_DINAMIC 
struct Method* Loopmethod;
 
void intLoopControlYAlarmas(struct ArgLoop* arg){
  Loopmethod=initMethod(loopDeControlYAlarmas,arg);
  MethodContainer_add(Loopmethod,ListaPrincipal);
}
#endif

void loopDeControlYAlarmas(void* arg){
  byte i;
  struct ArgLoop* argTmp=(struct ArgLoop*)arg;
 
 for(i=0;i<argTmp->CantCanales;i++){
  
  changeSensor(i);
  
  setSetPoint(i);
 }
 
 if(isADCready()){
 
  for(i=0;i<argTmp->CantCanales;i++) {
    
    calculaValorFinal(i);
  
    //Salida de control 1        
    calculosDeControl(&(argTmp->_control[i]));
   }
 
  for(i=0;i<argTmp->CantAlarmas;i++) 
   //Salida de Alarma 1   
   calculosDeAlarma(&(argTmp->_alarmas[i]));
  
  FlagCleaner = 0;              //lo limpio despues de procesar todos los flags

  setFlagADCready(FALSE);
  
  Mostrar_Proc=TRUE;  //Mostrar los valores en la pantalla principal (se procesa en Main Handler)
  
 }

}