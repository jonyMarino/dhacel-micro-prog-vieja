#ifndef _Base_Timers_1ms_40ms_hpp
#define _Base_Timers_1ms_40ms_hpp

#include "BaseTimers.hpp"
#include "Method.h"

//void inc1(void * _self);
//void inc40(void * _self);

class BaseTimers_1ms_40ms:public BaseTimers {
  public:
    static BaseTimers_1ms_40ms * getInstance();
      
  protected:
    virtual void lockInc();
    virtual void unlockInc();
    
  private:
    static  BaseTimers_1ms_40ms * instance;
    BaseTimers_1ms_40ms();
    ~BaseTimers_1ms_40ms();
   // static void inc1(void * _self);
   // static void inc40(void * _self);
    struct Method on1ms;
    struct Method on40ms;
    
    static void inc1(void * _self);
    static void inc40(void * _self);
   
};

#endif