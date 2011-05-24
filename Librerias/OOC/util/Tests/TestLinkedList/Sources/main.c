#include <hidef.h>      /* common defines and macros */
#include <MC9S12GC32.h>     /* derivative information */
#pragma LINK_INFO DERIVATIVE "mc9s12gc32"

#include <stddef.h>
#include "Object.h"
#include "LinkedList.h"
#include "Comparator.h"


struct TimerComparatorByTime{
  struct Comparator _base;
  struct Object * object;  
};

bool TimerComparatorByTime_Equals(void * self,void * obj){
  struct TimerComparatorByTime * _self = self;
  
  return _self->object==obj;
    
}

struct ComparatorClass TimerComparatorByTime={
    COMPARATOR_CLASS_INITIALIZATION(Class,
                       TimerComparatorByTime,
                       Object,
                       Object_ctor,
                       NULL,
                       NULL,
                       NULL,
                       TimerComparatorByTime_Equals,
                       NULL
                       )
};


void main(void) {
   // struct ArrayList arrayList;
  struct Object * t1=new(&Object); 
  struct Object * t2=new(&Object);
  struct Object * t3=new(&Object);
  struct Object * t4=new(&Object);
  struct Object * t5=new(&Object);
  struct Object * t6=new(&Object);
  struct Object * t7=new(&Object);
  struct LinkedList * array=new(&LinkedList);//&arrayList;//new(LinkedList);
  struct TimerComparatorByTime * comparator= new(&TimerComparatorByTime); 

  /* put your own code here */
  EnableInterrupts;

  
    for(;;){

      if(get(array,0)){        
        while(1); //texto fijo  
        break;
      }
      //agrego      
      
      add(array,t2);
      add(array,t1);
      
      if(LinkedList_get(array,0)!=t1 || LinkedList_get(array,1)!=t2){        
        while(1); //texto fijo   
        break;
      }
      
      LinkedList_Aniadir(array,t3);
      
      if(get(array,0)!=t1 
      || LinkedList_get(array,1)!=t2
      || get(array,2)!=t3){        
        while(1); //texto fijo  
        break;
      }
      
      comparator->object=t3;  //igual a t3
      /*
      if(LinkedList_search(array,comparator)!=t3){
        PasarASCII("Err Search 1",0); //texto fijo  
        break;        
      }
      
      comparator->time=4564645;  //Tiempo no existente 
      
      if(LinkedList_search(array,comparator)!=NULL){
        PasarASCII("Err Search 2",0); //texto fijo  
        break;        
      }
        */      
      break;
      

  }
  for(;;) {} /* wait forever */
  /* please make sure that you never leave this function */
}
