#include "BaseTimers_1ms_40ms.h"
#include "TimerOld.h"
#include "TI1.h"


// las funciones de estos metodos son siempre las mismas pero los argumentos pueden cambiar
struct Method m1ms;       
struct Method m40ms;

argIncs tmp40ms; //debo tener esta variable ya que utilizo el mismo timer para generar ambas interrupt(1ms y 40ms)

void InitBaseTimers_1ms_40ms(){
  static bool soloFirstTime=TRUE;
  
  
  if(soloFirstTime){
    soloFirstTime=FALSE;
    
    m1ms.pmethod= BaseTimers_1ms_40ms_inc1;
    
    m40ms.pmethod= BaseTimers_1ms_40ms_inc40;
    
    tmp40ms.NroList = ListaInt40ms;
    tmp40ms.NroTime = CuntaPrincipal;
    
    BaseTimers_1ms_40ms_setArguMethod40ms(&tmp40ms);
    
    MethodContainer_add(&m1ms,ListaInt1ms);
    
    MethodContainer_add(&m40ms,ListaInt40ms);
  }
  
}

void BaseTimers_1ms_40ms_lockInc(){
  TI1_Disable();
}

void BaseTimers_1ms_40ms_unlockInc(){
  TI1_Enable();
}

void BaseTimers_1ms_40ms_inc1(argIncs* argInc1ms){ 
  if(argInc1ms) {
    BaseTimers_incrementar(1,argInc1ms->NroTime);
    BaseTimers_actualizarTimers(argInc1ms->NroList,argInc1ms->NroTime);
  }
}

void BaseTimers_1ms_40ms_inc40(argIncs* argInc40ms){
  if(argInc40ms) {
    BaseTimers_incrementar(TIEMPO_GRABACION,argInc40ms->NroTime);
    BaseTimers_actualizarTimers(argInc40ms->NroList,argInc40ms->NroTime);
  }
}



void BaseTimers_1ms_40ms_setArguMethod1ms(argIncs* argInc1ms){
  m1ms.Obj=argInc1ms;
}

void BaseTimers_1ms_40ms_setArguMethod40ms(argIncs* argInc40ms){
  m40ms.Obj=argInc40ms;
}