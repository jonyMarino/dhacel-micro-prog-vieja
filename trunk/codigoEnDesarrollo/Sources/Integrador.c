#include "Mydefines.h"
#include "Integrador.h"
#include "ADC.h"
#include "cnfbox.h"
#include "IFsh10.h"
#include "Sensores.h"			
#include "TimerOld.h"

#define NO 0
#define SI 1
#define HORA 0
#define MIN  1
#define SEG  2
#define RESOL_1   0
#define RESOL_10  1
#define RESOL_100 2

#ifdef INTEGRADOR

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

void _sumador(void){
  char i;        
  
  integrador_decimales();
                 
  if(firstTime){                
    firstTime = FALSE;
    HabilitarAccionPagApagado();
    
  }
  
  if(PRom[R_RSTL]==SI && !flagReset)                        
     integrador_reset();
  
 
   if(PRom[R_UND]==HORA){
    
   //#ifdef AD_2s
    //if( contador<15 ){
   //#elif defined (AD_1s)  
    if( contador<30 ){
   //#elif defined (AD_500ms)
    //if( contador<60 ){
   //#elif defined (AD_250ms)
    //if( contador<120 ){
   //#endif
   
       flagReset = FALSE;
       contador++; 
    }else{
    contador = 1;
    
    for(i=0;i<CANTIDAD_CANALES;i++){
      Acumulador[i] += valor(i);
      valorAmostrar[i] = Acumulador[i]/(120*AjutaDecimales[i]);    
    }
   }
   }else if(PRom[R_UND]==MIN){
     flagReset = FALSE;
     for(i=0;i<CANTIDAD_CANALES;i++){
      Acumulador[i] += valor(i);
      valorAmostrar[i] = Acumulador[i]/(60*AjutaDecimales[i]);    
    }
      
   }else if(PRom[R_UND]==SEG){
     flagReset = FALSE;
     for(i=0;i<CANTIDAD_CANALES;i++){
      Acumulador[i] += valor(i);
      valorAmostrar[i] = Acumulador[i]/(AjutaDecimales[i]);    
    }
     
   }

}

void integrador_reset(void){
  
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


void integrador_decimales(void){
   char i;
   
   for(i=0;i<CANTIDAD_CANALES;i++){
    if(PRom[R_RSL] == RESOL_10 && PRom[R_Sensor+i]>=SENSOR_LIN2){
        AjutaDecimales[i] = 10;
        
    }else if(PRom[R_RSL] == RESOL_100 && PRom[R_Sensor+i]>=SENSOR_LIN3){
        AjutaDecimales[i] = 100;
        
    }else if(PRom[R_RSL] == RESOL_100 && PRom[R_Sensor+i]==SENSOR_LIN2){
        AjutaDecimales[i] = 10;
    
    }else
        AjutaDecimales[i] = 1;
    
   }
}

#endif