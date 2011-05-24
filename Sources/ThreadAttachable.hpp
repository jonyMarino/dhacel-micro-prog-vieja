#ifndef _THREAD_ATACHABLE_HPP
#define _THREAD_ATACHABLE_HPP

#include "MethodContainer.hpp"
#include "Method.h"

class ThreadAttachable:public MethodContainer{
  public:
    void adjuntar(struct Method * method);
    void executeThreads();
    
    class ThreadAttacher{
      public:
        ThreadAttacher(ThreadAttachable*);
        void adjuntar(struct Method * method);
      private:
        ThreadAttachable * threadAdjuntable;
    };
    
    ThreadAttacher * getAdjuntador();
     
  private:
    friend class ThreadAttacher; 
  
};


#endif