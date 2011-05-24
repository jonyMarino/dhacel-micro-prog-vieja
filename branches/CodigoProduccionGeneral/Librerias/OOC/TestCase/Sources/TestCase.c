#include <assert.h>
#include <hidef.h>
#include <terminal.h>
#include <termio.h>
#include <inout.h>
#include <stdio.h>

#include "TestCase.h" 
#include "Method.h"
#include "PE_Types.h"

typedef bool (*Checker)(void*);


const struct Class TestCaseClass;
const struct TestCaseClass TestCase;

/* TestMethod*/
struct TestMethod{
  const void *class;
  struct Method method;
  char * methodName;
};



void TestMethod_constructor(void * self,va_list * args){
  struct TestMethod* _self = self;
  void * method = va_arg(*args,void*);
  void * obj = va_arg(*args,void*);
  char * name = va_arg(*args,void*);
  newAlloced(&_self->method,&Method,method,obj); 
  _self->methodName=name;
}

bool TestMethod_execute(void * self){
    struct TestMethod* _self = self;
    Checker m = Method_getMethod(&_self->method);
    void * o = Method_getObj(&_self->method);
    return (*m)(o);
}
const struct Class TestMethod={
  CLASS_INITIALIZATION(Class,TestMethod,Object,TestMethod_constructor,NULL,NULL,NULL)   
};

/* Fin TestMethod*/
 
/*
** ===================================================================
**     Method      :  TestCase_constructor 
**
**     Description :
**         constructor de TestCase
**     @param self puntero a la subclase de  CaseTest              
** ===================================================================
*/
void TestCase_constructor(void * self){
    struct TestCase * _self = self;
    newAlloced(&_self->tests,&LinkedList); 
    TERMIO_Init();
    TERM_WriteString("Test");
    TERM_WriteLn(); 
}

/*
** ===================================================================
**     Method      :  TestCase_destructor 
**
**     Description :
**         constructor de TestCase
**     @param self puntero a la subclase de  CaseTest              
** ===================================================================
*/
void TestCase_destructor(void * self){
    struct TestCase * _self = self;
    deleteAlloced(&_self->tests);  
}
/*
** ===================================================================
**     Method      :  setup 
**
**     Description :
**         Interno, llama al setup correspondiente al inicio de un test
** ===================================================================
*/
void setup(void* self){
  pMethod method= (*(struct TestCaseClass**)self)->setup;
  if(method)
    (*method)(self);
}

/*
** ===================================================================
**     Method      :  tearDown 
**
**     Description :
**         Interno, llama al setup correspondiente al inicio de un test
** ===================================================================
*/
void tearDown(void* self){
  pMethod method= (*(struct TestCaseClass**)self)->tearDown;
  if(method)
    (*method)(self);
}


/*
** ===================================================================
**     Method      :  TestCase_addTest 
**
**     Description :
**         agrega el puntero al metodo "test" para realizar el test.
**     @param self puntero a la subclase de  CaseTest
**     @param test puntero al metodo "test", como parametro recibira
**                 self               
** ===================================================================
*/
void TestCase_addTest(void * self, void (*test)(void*),const char * testName){
  struct TestCase * _self = self;
  struct TestMethod * method=new(&TestMethod,test,self,testName);
  LinkedList_add(& _self->tests,method);
   
}
/*
** ===================================================================
**     Method      :  TestCase_runTest 
**
**     Description :
**         Corre todos los tests.
** ===================================================================
*/
void TestCase_runTest(void*self){
  struct TestCase * _self = self;
  struct Iterator * iterator = LinkedList_iterator(&_self->tests);
  while(Iterator_hasNext(iterator)){
    struct TestMethod * testMethod= Iterator_next(iterator);
    bool err;
    setup(self);
    err=TestMethod_execute(testMethod);
    if(err)      
      printf("Falla en metodo: %s \n",testMethod->methodName);
     // assert(0);
    else
      printf("Prueba pasada: %s \n",testMethod->methodName);
    tearDown(self);    
  }
  delete(iterator);
}