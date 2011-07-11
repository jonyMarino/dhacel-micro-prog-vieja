#include "BaseTiempoDS1307.h"
#include "EI2C1.h"

 TimeRegisters timeRecivido;
  word recibidos;

/*
** ===================================================================
**     Method      :  BaseTiempoDS1307_constructor (bean TimeDate)
**
**     Description :
** ===================================================================
*/
void BaseTiempoDS1307_Init()
{

  struct  {
    byte address;
    TimeRegisters time;
  }tiempoEnviar;
  
  word w;
  bool cambiado = FALSE;

  EI2C1_Init();
  
  EI2C1_SendChar(0);  // me posiciono en las horas
  EI2C1_SendStop();
  EI2C1_RecvBlock(&tiempoEnviar.time,sizeof(tiempoEnviar.time),&w);
  EI2C1_SendStop();

  
  if(tiempoEnviar.time.hours.bits.amPm){    //Esta configurado como ampm?
    tiempoEnviar.time.hours.bits.amPm = FALSE; 
    cambiado = TRUE;
  }
  if(tiempoEnviar.time.seconds.bits.CH){
    tiempoEnviar.time.seconds.bits.CH = FALSE;
    cambiado = TRUE;
  }
  
  if(cambiado){
    tiempoEnviar.address=0;
    EI2C1_SendBlock(&tiempoEnviar,sizeof(tiempoEnviar),&w);  
    EI2C1_SendStop();
  }
  
  
}

/*
** ===================================================================
**     Method      :  BaseTiempoDS1307_setTiempo
** ===================================================================
*/
byte BaseTiempoDS1307_setTiempoValidado(byte horas,byte min,byte segs){
  
  struct  {
    byte address;
    TimeRegisters time;
  }timeEnviar;

  word enviados;
  
  timeEnviar.address=0;
  
  timeEnviar.time.hours.bits.hourHigh = horas /10;
  timeEnviar.time.hours.bits.hourLow  = horas %10;
  timeEnviar.time.hours.bits.amPm=FALSE;
  
  timeEnviar.time.minutes.bits.minHigh = min/10;
  timeEnviar.time.minutes.bits.minLow = min%10;
  
  timeEnviar.time.seconds.bits.secHigh = segs / 10; 
  timeEnviar.time.seconds.bits.secLow = segs % 10;
  timeEnviar.time.seconds.bits.CH=FALSE;
  
  EI2C1_SendBlock(&timeEnviar,sizeof(timeEnviar),&enviados);
  EI2C1_SendStop();
  return 0;
}

/*
** ===================================================================
**     Method      :  BaseTiempoDS1307_getTiempo
** ===================================================================
*/
  
void BaseTiempoDS1307_getTiempo(TIMEREC *time){

  
  EI2C1_SendChar(0);  // me posiciono en los segundos
  EI2C1_SendStop();
  EI2C1_RecvBlock(&timeRecivido,sizeof(timeRecivido),&recibidos);
  EI2C1_SendStop();
  time->Hour= timeRecivido.hours.bits.hourHigh * 10 + timeRecivido.hours.bits.hourLow;
  time->Min = timeRecivido.minutes.bits.minHigh * 10 + timeRecivido.minutes.bits.minLow;
  time->Sec = timeRecivido.seconds.bits.secHigh * 10 + timeRecivido.seconds.bits.secLow;
}

/*
** ===================================================================
**     Method      :  BaseTiempoDS1307_setFechaValidada
** ===================================================================
*/
byte BaseTiempoDS1307_setFechaValidada(word year,byte month,byte day){
  
  struct  {
    byte address;
    DateRegister date;
  }fechaEnviar;
 
  word enviados;
  byte err;
   
  fechaEnviar.address=4;
  
  year -= 2000; 
  fechaEnviar.date.years.bits.yearHigh = year /10;
  fechaEnviar.date.years.bits.yearLow  = year %10;
  
  fechaEnviar.date.months.bits.monthHigh = month/10;
  fechaEnviar.date.months.bits.monthLow = month%10;
  
  fechaEnviar.date.days.bits.dayHigh = day / 10; 
  fechaEnviar.date.days.bits.dayLow = day % 10;  

  err=EI2C1_SendBlock(&fechaEnviar,sizeof(fechaEnviar),&enviados);
  EI2C1_SendStop();  
  
  return err;
}

/*
** ===================================================================
**     Method      :  BaseTiempoDS1307_getFechaValidada
** ===================================================================
*/
void BaseTiempoDS1307_getFecha(DATEREC *date){
  DateRegister fechaRecivida;

  word recibidos;
  
  EI2C1_SendChar(4);  // me posiciono en los dias
  EI2C1_SendStop();
  EI2C1_RecvBlock(&fechaRecivida,sizeof(fechaRecivida),&recibidos);
  EI2C1_SendStop();
  date->Year= 2000+fechaRecivida.years.bits.yearHigh * 10 + fechaRecivida.years.bits.yearLow;
  date->Month = fechaRecivida.months.bits.monthHigh * 10 + fechaRecivida.months.bits.monthLow;
  date->Day = fechaRecivida.days.bits.dayHigh * 10 + fechaRecivida.days.bits.dayLow;
  
}


/*
** ===================================================================
**     Method      :  EI2C1_OnNACK
** ===================================================================
*/
void EI2C1_OnNACK(void){

}

/*
** ===================================================================
**     Method      :  EI2C1_OnTxChar
** ===================================================================
*/
void EI2C1_OnTxChar(void){

}

/*
** ===================================================================
**     Method      :  EI2C1_OnRxChar
** ===================================================================
*/
void EI2C1_OnRxChar(void){

}
