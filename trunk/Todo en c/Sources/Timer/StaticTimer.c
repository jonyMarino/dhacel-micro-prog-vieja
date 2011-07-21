#include "StaticTimer.h"
#include "PE_Error.h"


byte nContadores = CONT_INIT;

T_CONTADOR Contador[CANTIDAD_CONTADORES];


/**/
byte Timer_Run(word tiempo,bool * flag, tipoContador tipo) {
  byte i;
  
  if (nContadores >= CANTIDAD_CONTADORES) 
    return ERR_RANGE;
  
  
  for(i=0;i<nContadores;i++){
    if (Contador[i].flag==flag){
      
      Contador[i].tiempo=tiempo;
      
      Contador[i].cuenta=tiempo;
    
      Contador[i].tipo = tipo;
    
      return ERR_OK;
    }
  }

  
  Contador[nContadores].tiempo=tiempo;
  
  Contador[nContadores].cuenta=tiempo;
  
  Contador[nContadores].tipo = tipo;
  
  Contador[nContadores].flag = flag; 
  
  nContadores++;
  
  return ERR_OK;
}




/**/
void HandlerStaticTimer(byte base) {
  byte i;
  
   for(i=0; i<nContadores; i++){
        if (Contador[i].tipo==FIN) 
          continue;
        
        if (Contador[i].cuenta>= base)
          Contador[i].cuenta-=base;
        else{
          Contador[i].cuenta=0;
          switch(Contador[i].tipo){
            case CONTINUO_SET:
              Contador[i].cuenta= Contador[i].tiempo;
              *(Contador[i].flag) = 1;
              break; 
            case UNICO_SET:
              *(Contador[i].flag) = 1;
              Contador[i].tipo = FIN;
              break;
            case  CONTINUO_TOG:
              Contador[i].cuenta= Contador[i].tiempo;
              *(Contador[i].flag) = ! *(Contador[i].flag);
            
          }
        }
    }

}
