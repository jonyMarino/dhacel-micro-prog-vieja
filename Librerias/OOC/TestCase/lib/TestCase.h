/** ###################################################################
**     @file TestCase.h
**     Project   : Controlador
**     Version   : 0.0.1
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 23/06/2008, 10:50
**     Abstract  :
**          Esta clase se utiliza como Framework para realizar los tests.
**     Contents  :
**         addTest - void TestCase_addTest(void * self, void (*test)(void*));
**				 runTest - void TestCase_runTest(void*self);
**     Dhacel S.R.L.
**     Argentina
**     mail      : marinon87@hotmail.com
** ###################################################################*/
#ifndef _TEST_CASE_H
#define _TEST_CASE_H

#include "Object.h"
#include "LinkedList.h"

/*
*/
struct TestCaseClass{
  struct Class super;
  void(*setup)(void*);		 //Definir para correr al inicio de cada prueba
  void(*tearDown)(void*);	 //Definir para correr al final de cada prueba
};


extern const struct Class TestCaseClass;
extern const struct TestCaseClass TestCase;

/**
  @struct TestCase
  @details Esta clase se utiliza como Framework para realizar los tests.
*/
struct TestCase{
  struct Object super; 
  struct LinkedList tests;
};


/**
@def TESTCASE_ADD_TEST(self,test)
@brief Macro que agrega el metodo \a test al conjunto de los tests
*/
#define TESTCASE_ADD_TEST(self,test) TestCase_addTest(self,test,#test)
/**
** ===================================================================
**     @fn void TestCase_constructor(void * self)
**     @brief constructor de TestCase, se debe llamar al inicio del 
**     constructor de cualquier clase derivada.
**     @param self puntero a la subclase de  CaseTest              
** ===================================================================
*/
void TestCase_constructor(void * self);

/*
** ===================================================================
**     @fn  void TestCase_addTest(void * self, void (*test)(void*)) 
**		 @deprecated
**     See @ref TESTCASE_ADD_TEST
**     @brief agrega el metodo "test" al conjunto de los tests.
**     @param self puntero a la subclase de  CaseTest
**     @param test puntero al metodo "test", como parametro recibira
**                 self               
** ===================================================================
*/
void TestCase_addTest(void * self, void (*test)(void*),const char * testName);
/**
** ===================================================================
**     @fn void TestCase_runTest(void*self)
**     @brief Corre todos los tests.
**     @param self puntero a la subclase de  CaseTest
** ===================================================================
*/
void TestCase_runTest(void*self);

#define TEST_CASE_CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto,setup,tearDown)\
  CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto),\
  setup,  \
  tearDown

#endif