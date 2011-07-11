/*MODULE: MethodTimer.c*/

#include "MethodTimer.h"
#include <stdarg.h>
#include <stdlib.h>

#include "stddef.h"
#include "stdtypes.h"
#include "Cpu.h"
#include "Timer.h"
#include "Method.h"
#include "MethodTimer.h"
#include "RlxMTimer.h"

#pragma DATA_SEG MethodTimer_DATA                                            
#pragma CODE_SEG MethodTimer_CODE 
#pragma CONST_SEG DEFAULT


/* un methodTmer sirve para ejecutar una funcion cumplido el tiempo seleccionado
esta funcion se ejecuta dentro de la rutina de atencion de interrupcion de 1ms 
por lo tanto deben ser funciones pequeñas */

struct MethodTimer* _MethodTimer;  // contiene el ultimo MTimer creado
/*
** ===================================================================
**     Method      :  MTimer_Init 
**    Description : Metodo para setear los 
**                  valores de configuración del Timer (base 1ms)
** ===================================================================
*/


struct MethodTimer* MTimer_Init(ulong tiempo,void (*pf)(void*),void * Obj){
  static bool isFirst=TRUE; 
  struct MethodTimer* mt=(struct MethodTimer*)malloc(sizeof(struct MethodTimer));
   
  if(!mt)
    return;  //sin memoria 
 
 if(isFirst){
    isFirst=FALSE;
    setNroTimer(0);
 }
  
  _MethodTimer=mt;
  
  /*if(pf==RlxMTimer_OnTime){   // se trata de un RlxMtimer??
    struct RlxMTimer* rmt=(struct RlxMTimer*)Obj;
    rmt->_base=mt;
  } */
  
  mt->_base.onTime=pf;
  
  mt->_base.Obj=Obj;  
  
  mt->_base.tiempo=tiempo;
  
  mt->argBase.NroList=ListaInt1ms;  // siempre utilizo la lista de 1ms
   
  mt->argBase.NroTime=CuntaPrincipal;  // siempre utilizo el nro time cero(cuenta[0]) el resto los dejo para futuras funcionalidades
  
  BaseTimers_1ms_40ms_setArguMethod1ms(&(mt->argBase));
  
  InitBaseTimers_1ms_40ms();
  
  Timer_Init(mt,mt->argBase.NroList,mt->argBase.NroTime);
  
  return mt;
      
}



/*
** ===================================================================
**     Method      :  executeMethodsTimer 
**    Description : funcion a llamar cuando se llega al tiempo
** ===================================================================
*/

void executeMethodsTimer(unsigned char NroList){
  struct Nodo * tmpNodo;
  
  tmpNodo=LinkedList_getPrimerNodo(NroList);
  // ejecuto el primer elemento de la lista
  if(tmpNodo!=NULL ) {
     struct Method* m = (struct Method *)getDataObj(tmpNodo);
    (*(m->pmethod))(m->Obj);
  }
   
}


/**/

struct MethodTimer* getUltimoMTimer(){
  return _MethodTimer;
 }


/**/
void MTimer_setTime(struct MethodTimer*t, ulong _tiempo){
  Timer_setTime(&(t->_base),ListaInt1ms,CuntaPrincipal,_tiempo);
}

/**/ 
ulong MTimer_getTime(struct MethodTimer* t){
  return Timer_getTime(&(t->_base));
}
 
/**/
void MTimer_Stop(struct MethodTimer*t){
  Timer_Stop(&(t->_base));
}

/**/
uchar MTimer_isfinish(struct MethodTimer*t){
   return Timer_isfinish(&(t->_base),ListaInt1ms);
}

/**/
void MTimer_Restart(struct MethodTimer*t){
   Timer_Restart(&(t->_base),ListaInt1ms,CuntaPrincipal);    
}


bool isStartMTimer(struct MethodTimer*t){
  if(t->_base.state==_timerStart)
    return TRUE;
  
  return FALSE;
}

#pragma CODE_SEG MethodTimer_CODE

