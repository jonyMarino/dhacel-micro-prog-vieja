#include "presentarValor.h"
#include "Mydefines.h"
#include "str_lib.h"
#include "cnfbox.h"
#include "Parametros.h"


int getValor(struct ArgPresentaValor* arg){
  
  return *(arg->val);
}

char * getCartel(struct ArgPresentaValor* arg){
  
  return (arg->nombre);
}

void imprimirValor(struct ArgPresentaValor* arg){
  
  byte decimales= getDecimales();
  int Val= getValor(arg); 
	char str[7];
	OutputStream_writeStr(getCartel(arg));
	
	if(arg->val!=NULL) {
	  OutputStream_writeStr(" ");
	  FloatToStr(Val,str,6,decimales);
    OutputStream_writeStr(str);
    OutputStream_writeStr(" ");
	}else
	  OutputStream_writeStr("\n");
}


int getDecimales(){
  
  return PRom[R_Decimales]; 
}

