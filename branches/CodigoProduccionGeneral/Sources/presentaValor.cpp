#include "presentaValor.hpp"
#include "Mydefines.h"
#include "str_lib.h"
#include "cnfbox.h"

extern volatile const int PRom[R_ATA+1];

 int PresentaValor::getValor(){
  
  return *valorApresentar;
}


void PresentaValor::imprimirValor(OutputStream& os){
  
  byte decimales= getDecimales();
  int Val= getValor(); 
	char str[7];
	os.write(cartel);
	
	if(valorApresentar!=NULL) {
	  os.write(" ");
	  FloatToStr(Val,str,6,decimales);
    os.write(str);
    os.write(" ");
	}else
	  os.write("\n");
}


int PresentaValor::getDecimales(){
  
  return PRom[R_Decimales]; 
}

