#include "BaseTimers.h"
#include "Timer.h"



void incrementar(unsigned int ms){
  cuenta+=ms;
}

bool moveOut(void * timer){
  byte err;
  lockInc();
  err = LinkedList::moveOut(timer);
  unlockInc();
  return err;
}


void BaseTimers::actualizarTimers(void){
  LinkedList::LinkedListIterator iterator;
  
  linkedListIterator(&iterator);
  
  while(iterator.hasNext()){
    TimerComp((struct Timer *)iterator.next());      
  }
}

unsigned long BaseTimers::getCuenta(){
  unsigned long cuentatmp;
  //lockInc(_self);
  cuentatmp = cuenta;
  //unlockInc(_self);   
  return cuentatmp;
}
