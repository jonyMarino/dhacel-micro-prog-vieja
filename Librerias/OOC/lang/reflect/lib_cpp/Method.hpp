#ifndef _METHOD_H
#define _METHOD_H


#pragma DATA_SEG METHOD_DATA                                            
#pragma CODE_SEG METHOD_CODE 
#pragma CONST_SEG DEFAULT



class Method{
  public:
    typedef void (*pMethod)(void*);
    
    Method(pMethod method,void * obj);   
    
    void execute(void);

    inline pMethod getMethod(void){return pmethod;}

    void setMethod(pMethod method);

    inline void * getObj(void){return obj;}; 
    
    void setObj(void *); 
  
  private:
    pMethod pmethod;
    void* obj;
};


#pragma DATA_SEG DEFAULT                                            
#pragma CODE_SEG DEFAULT 

#endif