#ifndef _THREAD_ATACHABLE_H
#define _THREAD_ATACHABLE_H

#include "MethodContainer.h"
#include "Method.h"

void ThreadAttachable_adjuntar(struct Method * method);

void ThreadAttachable_executeThreads();
    
void ThreadAttacher(ThreadAttachable*);

void ThreadAttacher_adjuntar(struct Method * method);
      
ThreadAttacher * ThreadAttacher_getAdjuntador();
     
  

#endif