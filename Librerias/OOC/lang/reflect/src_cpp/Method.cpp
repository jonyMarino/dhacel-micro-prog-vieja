#include <stdarg.h>
#include "Method.hpp"

#pragma DATA_SEG METHOD_DATA                                            
#pragma CODE_SEG METHOD_CODE 
#pragma CONST_SEG DEFAULT


Method::Method(pMethod method,void * _obj):obj(_obj),pmethod(method){

}

void Method::execute(void){
  (*pmethod)(obj);
}

void Method::setMethod(pMethod method){
  
  pmethod = method;
}

void Method::setObj(void* _obj){

  obj = _obj;

}
 
#pragma CODE_SEG METHOD_CODE


