#include "ThreadAttachable.h"

ThreadAttachable * threadAdjuntable;
/*
** ===================================================================
**     Method      :  AdjuntadorAHilo_adjuntar 
**     Description :  adjunta el metodo al hilo de ejecucion
** ===================================================================
*/

void ThreadAttachable_adjuntar(struct Method * method){
  if(!method)
    return; //error thow exception
  add(method);
}

void ThreadAttachable_executeThreads(){
  executeMethods();
}

ThreadAttacher * ThreadAttacher_getAdjuntador(void){
  return new  ThreadAttacher(this);
}

/*
** ===================================================================
**     Method      :  AdjuntadorAHilo_constructor 
**     Description :  Constructor de la clase
** ===================================================================
*/
void ThreadAttacher(ThreadAttachable * thread){  
  threadAdjuntable = thread;   
}


/*
** ===================================================================
**     Method      :  AdjuntadorAHilo_adjuntar 
**     Description :  adjunta el metodo al hilo de ejecucion
** ===================================================================
*/

void ThreadAttacher_adjuntar(struct Method * method){
  if(!method)
    return; //error thow exception
  threadAdjuntable->add(method);
}
