#include "System.h"
#include "ManejadorImpresion.hpp"
#include "BaseTiempo.h"
#include "Imprimible.hpp"
//#include <iostream.h>
//#include <strstrea.h>

static void ManejadorImpresion::imprimirPeriodico(void * _self){
  struct ManejadorImpresion* self = (struct ManejadorImpresion*)_self;
  self->imprimir();
}

ManejadorImpresion::ManejadorImpresion(OutputStream& impresora):os(impresora){
  cuentaParcial = 0;
  newAlloced(&timer,&RlxMTimer,(ulong)1000,imprimirPeriodico,this);
  Timer_Stop((struct Timer*)&timer);
}

void ManejadorImpresion::imprimir(){
   LinkedListIterator it;
  TIMEREC time;
  int pos = 0;
  char i;
  char str[13];
  Imprimible * obj = (Imprimible*)get(0);
  
  if( cuentaParcial++ ==0){
    //Fecha
    DATEREC date;    
    Sys_getDate(&date);
    
    //TODO: cambiar por date.write
    sprintf(str,"%02d/%02d/%d ",(int)date.Day,(int)date.Month,(int)date.Year );
    os.write(str);
    pos = 11;   //son 10 lugares
  }
  
  if(cuentaParcial==10)
    cuentaParcial=0;
  
  
  Sys_getTime(&time);
  //TODO: cambiar: por time.write
  sprintf(str,"%d:%02d:%02d ",(int)time.Hour,(int)time.Min,(int)time.Sec );
  os.write(str);
  
  pos += 11;
  
  while( pos++ < 19){
    os.write(' ');
  }
  
  if(obj)
    obj->imprimirValor(os);
     
  
  for(i=1;obj=(Imprimible*)get(i);i++){
    //os.write('\n');
    os.write("           ");     
    obj->imprimirValor(os);
  }
  
  os.write('\n');
  sincronismo();
}

/**/
int ManejadorImpresion::getCuentaParcial(){
  return cuentaParcial;
}

/**/
OutputStream& ManejadorImpresion::getOS(){
  return os;
}
/**/
int ManejadorImpresion::getIntervalo(){
  return Timer_getTime((struct Timer*)&timer)/1000;
}

/**/
void ManejadorImpresion::setIntervalo(int intervalo){
  if( getHabilitado() )
    Timer_setTime((struct Timer*)&timer,(ulong)intervalo*1000);
  else{
    Timer_setTime((struct Timer*)&timer,(ulong)intervalo*1000);
    Timer_Stop((struct Timer*)&timer);   
  }
    
}

/**/
bool ManejadorImpresion::getHabilitado(){
  return !Timer_isfinish((struct Timer*)&timer);
}

/**/
void ManejadorImpresion::setHabilitado( bool habilitar){
  if(habilitar){ 
    if( !getHabilitado() )
      Timer_Restart((struct Timer*)&timer);  
  }else
    Timer_Stop((struct Timer*)&timer);
}



 void ManejadorImpresion::sincronismo(){
    Timer_Restart((struct Timer*)&timer);
 }