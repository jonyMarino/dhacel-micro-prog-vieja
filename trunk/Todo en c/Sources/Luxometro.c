#include "Mydefines.h"
#include "Luxometro.h"
#include "ADC.h"
#include "cnfbox.h"
#include "IFsh10.h"
#include "Sensores.h"			
#include "TimerOld.h"

#define NO 0
#define SI 1

#ifdef LUX

extern bool flagCorte;
extern bool flag_1seg;
extern volatile const int PRom[PARAMETROS];
//extern long Vx[CANTIDAD_CANALES];
extern int ValFinal[4];
int AjutaDecimales[CANTIDAD_CANALES];
bool flagReset = FALSE;
bool firstTime = TRUE;
word parteAlta;

#if CANTIDAD_CANALES == 1
 long Acumulador[CANTIDAD_CANALES]={0};
 long valorAmostrar[CANTIDAD_CANALES]={0};
#elif CANTIDAD_CANALES == 2
  long Acumulador[CANTIDAD_CANALES]={0,0};
  long valorAmostrar[CANTIDAD_CANALES]={0,0};
#endif

#define valor(i) (ValFinal[i]) 
  
char contador = 0;

void luxometro_sumador(void){
  char i;
  flagSave = TRUE;         
  luxometro_decimales();
                 
  
  if(firstTime){                
    firstTime = FALSE;
    valorAmostrar[0] = (*(word*)ADR_CORTE_ENERGIA_BAJA);
    Acumulador[0] = (long)(valorAmostrar[0]*(120*AjutaDecimales[i])); 
  }
  
  if(PRom[R_RSTL]==SI && !flagReset)                        
     luxometro_reset();
  
  #ifdef AD_2s
   if( contador<15 ){
       flagReset = FALSE;
       contador++; 
    }else{
    contador = 1;
    for(i=0;i<CANTIDAD_CANALES;i++){
      Acumulador[i] += valor(i);
      valorAmostrar[i] = Acumulador[i]/(120*AjutaDecimales[i]);    
    }
   } 
  
  #elif defined AD_1s
    if( contador<30 ){
       flagReset = FALSE;
       contador++; 
    }else{
    contador = 1;
    for(i=0;i<CANTIDAD_CANALES;i++){
      Acumulador[i] += valor(i);
      valorAmostrar[i] = Acumulador[i]/(120*AjutaDecimales[i]);    
    }
    EscribirWord(ADR_CORTE_ENERGIA_BAJA,(word)valorAmostrar[0]);
   } 
  
  #elif defined AD_500ms
    if( contador<60 ){
       flagReset = FALSE;
       contador++; 
    }else{
    contador = 1;
    for(i=0;i<CANTIDAD_CANALES;i++){
      Acumulador[i] += valor(i);
      valorAmostrar[i] = Acumulador[i]/(120*AjutaDecimales[i]);    
    }
   } 
  
  #elif defined AD_250ms
    if( contador<120 ){
       flagReset = FALSE;
       contador++; 
    }else{
    contador = 1;
    for(i=0;i<CANTIDAD_CANALES;i++){
      Acumulador[i] += valor(i);
      valorAmostrar[i] = Acumulador[i]/(120*AjutaDecimales[i]);    
    }
   } 
  
  #endif
  

}

void luxometro_reset(void){
  
#if CANTIDAD_CANALES == 1
  Acumulador[0]=0;
  valorAmostrar[0]=0;
#elif CANTIDAD_CANALES == 2
  Acumulador[0]=0;
  Acumulador[1]=0;
  valorAmostrar[0]=0;
  valorAmostrar[1]=0; 
#endif
contador = 0; 
flagReset = TRUE;
EscribirWord((word)&PRom[R_RSTL],NO);

}


void luxometro_decimales(void){
   char i;
   
   for(i=0;i<CANTIDAD_CANALES;i++){
    switch (PRom[R_Sensor+i]) {
      case SENSOR_LIN1: AjutaDecimales[i] = 1;break;
      case SENSOR_LIN2: AjutaDecimales[i] = 10;break;
      case SENSOR_LIN3: AjutaDecimales[i] = 10;break;
      case SENSOR_LIN4: AjutaDecimales[i] = 10;break;
      default: AjutaDecimales[i] = 1;break;
    }
   }
}

#endif