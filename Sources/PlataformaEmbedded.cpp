#include <assert.h>
#include "PlataformaEmbedded.hpp"
#include "ThreadAttachable.hpp"
#include "Thread.h"
#include "Method.h"


ThreadAttachable threads;

void pthread_create(void * _self,void * attr,void*(*pf)(void*),void* args){
  _self = _new(&Method,pf,args);
  threads.adjuntar((struct Method *)_self);  
}


void PlataformaEmbedded::mainLoop(void){
  threads.executeThreads();
}


