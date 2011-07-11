#ifndef _METHOD_H
#define _METHOD_H


#pragma DATA_SEG METHOD_DATA                                            
#pragma CODE_SEG METHOD_CODE 
#pragma CONST_SEG DEFAULT


typedef void (*pMethod)(void*);
 
struct Method{
  pMethod pmethod;
  void* Obj;
}; 
 
struct Method*  initMethod(pMethod method,void * obj);   

void execute(struct Method* m);

pMethod getMethod(struct Method* m);

void setMethod(struct Method* m,pMethod method);

void * getObj(struct Method* m); 
    
void setObj(struct Method* m,void *); 
  

#pragma DATA_SEG DEFAULT                                            
#pragma CODE_SEG DEFAULT 

#endif