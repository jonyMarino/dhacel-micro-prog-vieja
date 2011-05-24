#include "System.h"
#include "ManejadorImpresionPersistente.hpp"
//#include "ManejadorImpresion_protected.h"
//#include "OutputStream.h"
#include "EPM203Manejador.hpp"
#include "BaseTiempo.h"
#include "Getter.h"



static void ManejadorImpresionPersistente::printHora(void * _self){
  ManejadorImpresionPersistente * self =  (ManejadorImpresionPersistente *)_self;
  self->ManejadorImpresion::imprimir();
  Timer_Stop(&(self->mt));  
}

ManejadorImpresionPersistente::ManejadorImpresionPersistente(OutputStream& os,struct MIPConf * configuracion):ManejadorImpresion(os),conf(configuracion){

  newAlloced(&mt,&MethodTimer,(ulong)2000,ManejadorImpresionPersistente::printHora,this);
  Timer_Stop(&mt);
  setIntervalo(_MIPConf_getIntervalo(conf));
  if(_MIPConf_getHabilitado(conf))
    setHabilitado(TRUE);
}



void ManejadorImpresionPersistente::imprimir(){
  
  if(! isConfigurado(Sys_getBaseTiempo()) && getCuentaParcial() ==0){
    char str[30];

  //  if( class (getOS()) != EPM203Manejador){
      
      sprintf(str,"******************\n");
      getOS().write(str);
      sprintf(str,"Fecha no ingresada\n");
      getOS().write(str);
      ManejadorImpresion::imprimir();
      return;
  /*  }else{     
      sprintf(str,"*** Fecha no ingresada ***\n");
      getOS().write(str);
      Timer_Restart(&mt);
      return;
    }  */
  
    
  }
  ManejadorImpresion::imprimir();   
}

/**/
int ManejadorImpresionPersistente::getIntervalo(){
  return _MIPConf_getIntervalo(conf);
}

/**/
void ManejadorImpresionPersistente::setIntervalo(int intervalo){
  if( _MIPConf_setIntervalo(conf,intervalo)== ERR_OK ){
    ManejadorImpresion::setIntervalo(intervalo);
  }
    
}

/**/
bool ManejadorImpresionPersistente::getHabilitado(){
  return _MIPConf_getHabilitado(conf);
}

/**/
void ManejadorImpresionPersistente::setHabilitado(bool habilitar){
 
  if( _MIPConf_setHabilitado(conf,habilitar) == ERR_OK) 
    ManejadorImpresion::setHabilitado(habilitar);
}



