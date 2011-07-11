#ifndef _BASE_TIMERS_HPP
#define _BASE_TIMERS_HPP

#include "LinkedList.hpp"


class BaseTimers: public LinkedList{
  public:
    unsigned long getCuenta();
    virtual bool moveOut(void * timer);
  protected:
    void actualizarTimers(void);
    void incrementar(unsigned int ms);
    virtual void lockInc()=0;
    virtual void unlockInc()=0;    
  private:
    unsigned long cuenta;
    int index; 
};


#endif
