#include "Mydefines.h"
#include "boxes.h"
#include "cnfbox.h" 
#include "boxesset.h"
#include "Timer.h" 
#include "bkr.h"
#include "TimerOld.h"

#ifdef VF_BKR  

extern int ValFinal[4];
extern dword rampaTime;
extern word countMesetaTime;
extern byte DotNum[CANTIDAD_DISPLAYS];
bool flagComienzoRampa = FALSE;
bool flagComieMeseta = FALSE;
long tempAct = 0;
char puntoDecimal =0;
char estado=END;
bool flagTempInicial=FALSE;
int tempInicial=0;
char flagFinMeseta=0;      //0: nunca llego al fin de la meseta
                           //1: llego al fin de la meseta;
                           //2: corte de energia
                           //3: ya llego una ves o se corto la energia una ves   
char tiempoEstabilizacion=0;
char countProtecRuido=0;
word minutosMesta=1;

#define RUN 0
#define END 1
#define RST 2
#define TEMP_MESETA   PRom[R_Tmpm]
#define VELOC_RAMPA   PRom[R_Vrmp]
#define TIEMPO_MESETA PRom[R_Tiem]
#define MAX           PRom[R_Tmpm]+10
#define VERSION       PRom[R_Ver]
#define VALOR_MEDIDO  ValFinal[0]
#define TIEMPO_PROTEC 2

 //Nicolas 27/04/09
/************************************************************/

void calculoTempInstYtiempoMeseta (void){     
/* calculo de la temperatura de la rampa cada 1 segundos y             */
/* calculo del tiempo transcurrido una ves llegado a la temp de meseta */  
  estado = PRom[R_EST];
  puntoDecimal = PRom[R_Decimales];
 
 if( flagFinMeseta==2 && estado == RUN) {
  tiempoEstabilizacion++;
  if(tiempoEstabilizacion==3){
    tiempoEstabilizacion=0;
    tempAct = ValFinal[0];
    rampaTime = 0;
    countMesetaTime = 0;
    flagFinMeseta=3;
  }
  else
     return;
 }
 
if(flagFinMeseta==1 && estado == RUN) {
  
    tempAct = ValFinal[0];
    rampaTime = 0;
    countMesetaTime = 0;
    flagFinMeseta=3;
  } 
 
 
 if(estado == RUN){
 
  if(flagTempInicial==FALSE){
   flagTempInicial=TRUE;
   tempInicial=ValFinal[0];
  } 
  
  HabilitarAccionPagApagado();
  
  /*//////////////////CALCULO DEL SETPOINT(tempAct)//////////////////////////////*/
    
  if(VELOC_RAMPA != 0 && TEMP_MESETA > VALOR_MEDIDO && flagComieMeseta != TRUE) {
    flagComienzoRampa = TRUE;
    
      if(puntoDecimal == 0)
        tempAct = (long)((VELOC_RAMPA*rampaTime)+(((long)tempInicial)*600))/600;      //YA QUE TIENE UN PUNTO DECIMAL EL VALOR REAL ES  tempAct/10
      else                                                                        //COMO tempAct SE ACTUALIZA CADA 6 SEG Y LA VELOC ESTA EN °C/MIN ENTONCES /10
        tempAct = ((long)(VELOC_RAMPA*rampaTime)/60)+tempInicial;                     //TINE PUNTO DECIMAL 
  
      if(tempAct>=MAX)
         tempAct=MAX;
      
  }else{
     tempAct = TEMP_MESETA;
     flagComieMeseta = TRUE;
  }
  /*////////////////CONTROL DE FIN DE RAMPA/////////////////////////////////////////*/
  
  if(TEMP_MESETA <= VALOR_MEDIDO && flagComienzoRampa == TRUE){ 
    if(countProtecRuido==TIEMPO_PROTEC)
      countProtecRuido=0;
    else{
      countProtecRuido++;
      return;
    }
    flagComienzoRampa = FALSE;
    flagComieMeseta = TRUE;
    tempAct = TEMP_MESETA;
    rampaTime = 0;
    
  } /*////////////////CONTROL DE FIN DE MESETA////////////////////////////////////////*/
  
   if((TIEMPO_MESETA*60) <= countMesetaTime){
    flagComieMeseta = FALSE;
    flagComienzoRampa = FALSE;
    flagTempInicial=FALSE;
    flagFinMeseta=1;
    EscribirWord((word)&PRom[R_EST],END);
  }
  
 }else if(estado == RST){
     
     tempAct = 0;
     flagTempInicial=FALSE;
     rampaTime = 0;
     flagComienzoRampa = TRUE;
     flagComieMeseta = FALSE; 
     countMesetaTime = 0;
     flagFinMeseta=0;
     EscribirWord((word)&PRom[R_EST],RUN);
    
     
 }else if(estado == END){
     flagComienzoRampa = FALSE;
     flagComieMeseta = FALSE;
     flagTempInicial=FALSE;
    // if(VERSION==BKR_1)
       tempAct = 0;
 }
}

/******************************************************************/
/* ////////////////////presenta el tiempo de la meseta/////////////
/*/////////////////////////////////////////////////////////////////
/*******************************************************************/
 
 void presentaTimeMeseta_SPact(void){
  char new_text[21];
  
  if(flagComieMeseta == TRUE){
    
    if(countMesetaTime>(60*minutosMesta))
         minutosMesta++;
     
       new_text[0]=' ';
       new_text[1]=' ';
       new_text[2]=' ';
       new_text[3]=' ';
       new_text[4]=' ';
       new_text[5]='t';
       new_text[6]='i';
       new_text[7]='E';
       new_text[8]='m';
       new_text[9]='P';
       new_text[10]='o';
       new_text[11]=' ';
       
       if((minutosMesta)/100){
          new_text[12]=(minutosMesta)/100+'0';
          new_text[13]=(minutosMesta)/10+'0'; 
          new_text[14]=(minutosMesta)%10+'0';
          new_text[15]=' ';
          new_text[16]='m';
          new_text[17]='i';
          new_text[18]='n';
          new_text[19]=' ';
          new_text[20]='\0';
       } else if((minutosMesta/10) && (minutosMesta/100) != 1) {
         // new_text[9]=' ';
          new_text[12]=(minutosMesta)/10+'0'; 
          new_text[13]=(minutosMesta)%10+'0';
          new_text[14]=' ';
          new_text[15]='m';
          new_text[16]='i';
          new_text[17]='n';
          new_text[18]=' ';
          new_text[19]=' ';
          new_text[20]='\0';
       }else{
          //new_text[9]=' ';
          //new_text[10]=' ';  
          new_text[12]=(minutosMesta+'0');
          new_text[13]=' ';
          new_text[14]='m';
          new_text[15]='i';
          new_text[16]='n';
          new_text[17]=' ';
          new_text[18]=' ';
          new_text[19]=' ';
          new_text[20]='\0';
       }
       PasarASCII(new_text,1);
  } else{
     Pasar_Numero(tempAct,1,DotNum[1]);
     minutosMesta=1; 
  }
  
 
 }
 
#endif 














