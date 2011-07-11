#include <stdarg.h>
#include "stddef.h"
#include "Method.h"

#pragma DATA_SEG METHOD_DATA                                            
#pragma CODE_SEG METHOD_CODE 
#pragma CONST_SEG DEFAULT


struct Method* initMethod(pMethod method,void * _obj){
    struct Method* tmpM=(struct Method*)malloc(sizeof(struct Method));
    
    if(!tmpM)  //tengo memoria??
      return NULL;
    
    tmpM->Obj=_obj;
    tmpM->pmethod=method;
    
    return tmpM;

}

void execute(struct Method* m){
  (*(m->pmethod))(m->Obj);
}

void setMethod(struct Method* m,pMethod method){
  
  m->pmethod = method;
}

void setObj(struct Method* m,void* _obj){

  m->Obj = _obj;

}
 
 
pMethod getMethod(struct Method* m){
  return m->pmethod;
}
 
 
void * getObj(struct Method* m){

  return m->Obj;

}  
 
#pragma CODE_SEG METHOD_CODE


