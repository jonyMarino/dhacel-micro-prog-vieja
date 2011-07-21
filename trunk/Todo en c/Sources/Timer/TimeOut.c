#include "TimeOut.h"


void setFlag(void * a){
  struct TimeOut * t=(struct TimeOut *)a;
  switch(t->modo){
    case CONTINUO:
      *(t->isTime)=TRUE;
      break;
    
    case ALTERNATIVE:  
      if(*(t->isTime)==TRUE)
        *(t->isTime)=FALSE;
      else
        *(t->isTime)=TRUE;
      
      break;
    case ONETIME:
      *(t->isTime)=TRUE;
      liberarTime(t);
      break;    
  
  }
    
}

/**/
struct TimeOut* InitTimeOut(word _tiempo,bool * flag,modoTimeOut modo){
  struct TimeOut* to=(struct TimeOut*)malloc(sizeof(struct TimeOut));
   
  if(!to)
    return;  //sin memoria
    
  to->isTime=flag;
  to->modo=modo;  
  to->_base=MTimer_Init(_tiempo,setFlag,to);  
  return to;   
}

/**/
void liberarTime(struct TimeOut * t){
  MTimer_Delete(t->_base);
   _delete (t);
  
}

/**/
void reStartTimeOut(struct TimeOut* to){
   if(to->modo != ONETIME) 
    MTimer_Restart(to->_base);
}