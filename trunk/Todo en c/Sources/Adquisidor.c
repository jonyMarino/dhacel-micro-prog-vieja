#include "Mydefines.h"
#include "TmDt1.h"
#include "PE_Types.h"
#include "Adquisidor.h"
#include "IFsh10.h"

#ifdef adquisidor

extern bool save_parametros;
extern byte Error;
extern bool c1;
extern int Estado_Adquisicion;

#define Parametro1 PRom[0]
#define Parametro2 PRom[1]

void Prepare_new(void);
static word index=0; 
#define Mem_llena (Estado_Adquisicion==3)

void Grabar_Parametros(void) {
  if (!Mem_llena){
  Error=WriteWord(FLASH_ADQUISIDOR_START+index,Parametro1+4000); //Sumo 4000 para no tener numeros negativos
  Prepare_new();
  Error|=WriteWord(FLASH_ADQUISIDOR_START+index,Parametro2+4000); //Sumo 4000 para no tener numeros negativos
  Prepare_new();
  if (Error!=ERR_OK) c1=TRUE;
  }

}

void Escribir_Header(void){
DATEREC Fecha;
TIMEREC Tiempo;

if (!Mem_llena){

WriteWord(FLASH_ADQUISIDOR_START+index,30000);	 //Start word
Prepare_new();


TmDt1_GetDate(&Fecha); 
WriteWord(FLASH_ADQUISIDOR_START+index,(word)((Fecha.Year-2000)*100+Fecha.Month));   //Año y mes
Prepare_new();

TmDt1_GetTime(&Tiempo);
WriteWord(FLASH_ADQUISIDOR_START+index,(word)((Fecha.Day)*100+Tiempo.Hour)); //dia y hora
Prepare_new();
WriteWord(FLASH_ADQUISIDOR_START+index,(word)((Tiempo.Min)*100+Tiempo.Sec)); //min y seg
Prepare_new();
WriteWord(FLASH_ADQUISIDOR_START+index,PRom[R_int]);   //intervalo
Prepare_new();
}
}

void Escribir_Powerdown(void){
WriteWord(FLASH_ADQUISIDOR_START+index,30002);  //Power down word
}

void Prepare_new(void){

  index+=2;
  if (((index+FLASH_ADQUISIDOR_START)&65535)>49150) {
    Estado_Adquisicion = 3;
    EscribirParametro(R_adq,0);
  }
}

void Setup_index(void){
  for(index=0;index<49150;){
    if (*(volatile word*)(FLASH_ADQUISIDOR_START+index) == 65535)
    break;
    else Prepare_new(); 
  }

}

void Borrar_Adquisicion(void){
byte i;
for(i=0;FLASH_ADQUISIDOR_START+(i*512)<FLASH_ADQUISIDOR_END;i++)
EraseSectorInternal(FLASH_ADQUISIDOR_START+(i*512));

index=0;

}

#endif
 