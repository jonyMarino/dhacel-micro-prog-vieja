#include "stdtypes.h"
#include "Cpu.h"
#include "Timer.h"
#include "BaseTimers_1ms_40ms.h"
#include "MethodTimer.h"

#pragma DATA_SEG Timer_DATA                                            
#pragma CODE_SEG Timer_CODE 
#pragma CONST_SEG DEFAULT

 
/*
** ===================================================================
**     Method      :  Timer_Construct 
**    Description : Metodo para setear los 
**                  valores de configuración del contador
** ===================================================================
*/
void Timer_Init(void* timer,unsigned int NroList,unsigned char NroTime){ 
  
  struct MethodTimer* mt=(struct MethodTimer*)timer;
  struct Timer* t=&(mt->_base);
  
  ulong cuenta = BaseTimers_getCuenta(NroTime);
 
  t->next_cuenta=cuenta+t->tiempo;
  
  if(cuenta >= t->next_cuenta)
    t->of=TRUE;  
  else
    t->of=FALSE;
  
  t->state=_timerStart;
    
  LinkedList_aniadirFinal(mt,NroList);

}


/*
** ===================================================================
**     Method      :  Timer_Destruct 
**    Description : Metodo para destruir el Timer
** ===================================================================
*/
void Timer_Delete(struct Timer* t,unsigned int NroList){
  BaseTimers_moveOut(t,NroList);
}


/*
** ===================================================================
**     Method      :  IncTimer 
**    Description : Incrementa el valor de la cuenta del Timer
** ===================================================================
*/
void TimerComp(void* timer,unsigned char NroTime){  
  struct Timer* t=(struct Timer*)timer;
  bool comp;
  ulong cuentaTmp = BaseTimers_getCuenta(NroTime);
  ulong nextCuentaTmp = t->next_cuenta;
  ulong tiempoTmp = t->tiempo;
   
  if(t->state != _timerStart || !t)
    return;
  
  if(t->of)
    comp= cuentaTmp <= nextCuentaTmp;
  else
    comp= cuentaTmp >= nextCuentaTmp;
  
  if (comp ){
    int error = cuentaTmp - nextCuentaTmp;
    error = (error<0)?-error:error;
    nextCuentaTmp +=   error + tiempoTmp;
    t->next_cuenta=nextCuentaTmp;    
    
    if(cuentaTmp >= nextCuentaTmp){
      t->of=TRUE;        
    }
    t->onTime(t->Obj);
  }
}

/*
** ===================================================================
**     Method      :  Timer_getCuenta 
**    Description : Regresa la cuenta actual que se resetea al llegar al tiempo
** ===================================================================
*/
ulong Timer_getCuenta(struct Timer* t,unsigned int NroList,unsigned char NroTime){
 
  ulong cuentaTmp;
  ulong nextCuentaTmp;
  ulong tiempoTmp;
  
  if(Timer_isfinish(t,NroList) )
    return 0;
  
  cuentaTmp = BaseTimers_getCuenta(NroTime);

  Cpu_DisableInt();
  nextCuentaTmp = t->next_cuenta;
  tiempoTmp = t->tiempo;
  Cpu_EnableInt();
   
  return cuentaTmp - (nextCuentaTmp - tiempoTmp);
}

/*
** ===================================================================
**     Method      :  Timer_isfinish 
**    Description : Indica si ya termino la cuenta
** ===================================================================
*/
uchar Timer_isfinish(struct Timer* t,unsigned int NroList){
  
  if(t)
    return !LinkedList_contains(t,NroList);
    
  return TRUE;
}

/*
** ===================================================================
**     Method      :  Timer_Restart 
**    Description : Reinicia la cuenta del tiempo
** ===================================================================
*/
void Timer_Restart(struct Timer* t,unsigned int NroList,unsigned char NroTime){
  
  ulong cuenta = BaseTimers_getCuenta(NroTime);
  
  t->next_cuenta= cuenta + t->tiempo ;
  
  if(cuenta >= t->next_cuenta)
    t->of=TRUE;  
  else
    t->of=FALSE;
  
  t->state=_timerStart;
  
  if(Timer_isfinish(t,NroList))
    LinkedList_aniadirFinal(t,NroList);
}

/*
** ===================================================================
**     Method      :  Timer_setTime 
**    Description : Setea un tiempo nuevo y reinicia la cuenta del tiempo
** ===================================================================
*/
void Timer_setTime(struct Timer* t,unsigned int NroList,unsigned char NroTime,ulong _tiempo){					 
  
  t->tiempo=_tiempo;
  Timer_Restart(t,NroList,NroTime);
}

/*
** ===================================================================
**     Method      :  Timer_getTime 
**    Description : Obtiene el tiempo a alcanzar
** ===================================================================
*/
ulong Timer_getTime(struct Timer* t){
 
  return t->tiempo;
}

/*
** ===================================================================
**     Method      :  Timer_Stop 
**    Description : Detiene la cuenta del timer
** ===================================================================
*/
void Timer_Stop(struct Timer* t){  
  
   t->state=_timerStop;
  
}



#pragma CODE_SEG Timer_CODE

