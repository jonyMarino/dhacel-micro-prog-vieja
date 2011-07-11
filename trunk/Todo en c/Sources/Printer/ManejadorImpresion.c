#include "FechaTiempo.h"
#include "ManejadorImpresion.h"
#include "SensorAdaptador.h"
#include "presentarValor.h"
#include "MethodTimer.h"


struct RlxMTimer * Rlxtimer;
byte cuentaParcial;

void imprimirPeriodico(){
  imprimir();
}

void ManejadorImpresionInit(){
  cuentaParcial = 0;
  Rlxtimer=RlxMTimer_Init(1000,imprimirPeriodico,NULL);
  RlxTimer_Stop(Rlxtimer);
}

void imprimir(){
  
  TIMEREC time;
  int pos = 0;
  char i;
  char str[13];
  
  
  if( cuentaParcial++ ==0){
    //Fecha
    DATEREC date;    
    getFecha(&date);
    
    //TODO: cambiar por date.write
    sprintf(str,"%02d/%02d/%d ",(int)date.Day,(int)date.Month,(int)date.Year );
    OutputStream_writeStr(str);
    pos = 11;   //son 10 lugares
  }
  
  if(cuentaParcial==10)
    cuentaParcial=0;
  
  
  getTiempo(&time);
  //TODO: cambiar: por time.write
  sprintf(str,"%d:%02d:%02d ",(int)time.Hour,(int)time.Min,(int)time.Sec );
  OutputStream_writeStr(str);
  
  pos += 11;
  
  while( pos++ < 19){
    OutputStream_write(' ');
  }
  
 executeMethods(ListaAimprimir);  // ejecuto las funciones de impresion agregadas en la lista
    
  OutputStream_write('\n');
  //sincronismo();
}

/**/
int getCuentaParcial(){
  return cuentaParcial;
}


/**/
int getIntervalo(){
  return (int)(RlxTimer_getTime(Rlxtimer)/1000);
}

/**/
void setIntervalo(int intervalo){
  if( getHabilitado() )
    RlxTimer_setTime(Rlxtimer,(ulong)intervalo*1000);
  else{
    RlxTimer_setTime(Rlxtimer,(ulong)intervalo*1000);
    RlxTimer_Stop(Rlxtimer);   
  }
    
}

/**/
bool getHabilitado(){
  if(isStartMTimer(Rlxtimer->_base))
    return TRUE;

  return FALSE;
}

/**/
void setHabilitado( bool habilitar){
  if(habilitar){ 
    if( !getHabilitado() )
      RlxTimer_Restart(Rlxtimer);  
  }else
    RlxTimer_Stop(Rlxtimer);
}

/**/
 void sincronismo(){
    RlxTimer_Restart(Rlxtimer); 
 }
 
 /**/
void addMethodAImprimir(void * Obj,TipoAimprimir tAi){
  struct Method* m;
  switch(tAi){
    case _Sensor: 
       m = (struct Method*)malloc(sizeof(struct Method));
      
      if(!m)   // tengo memoria??
        return;
       
       m->Obj=Obj;
       m->pmethod=SensorAdaptador_imprimirValor;
       
       MethodContainer_add(m,ListaAimprimir);
       break;
       
    case _Valor:
       
       if(!Obj)  // tengo argumentos a imprimir??
        return;
       
       m = (struct Method*)malloc(sizeof(struct Method));
      
      if(!m)   // tengo memoria??
        return;
       
       m->Obj=Obj;
       m->pmethod=imprimirValor;
       
       MethodContainer_add(m,ListaAimprimir);
       break;
  }
 }