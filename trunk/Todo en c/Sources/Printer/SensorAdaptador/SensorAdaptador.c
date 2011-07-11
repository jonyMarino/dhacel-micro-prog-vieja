
#include "Mydefines.h"
#include "str_lib.h"
#include "Sensores.h"
#include "cnfbox.h"
#include "SensorAdaptador.h"
#include "Parametros.h"
#include "Medicion.h"

byte numSensor;
char * cartel;


void SensorAdaptadorInit(byte numeroSensor,char * nombre){
   numSensor = numeroSensor;
   cartel = nombre;   
}  

TSensorState SensorAdaptador_getState(){
  return (TSensorState)getEstatusResult(numSensor);
}


int SensorAdaptador_getValue(){
 
  return getValCalculado(numSensor);
}


int SensorAdaptador_getDecimalesMostrados(){
  
  t_sensor sensor=PRom[R_Sensor+numSensor];
  return SENSOR_Decimales_Mostrar(sensor);
}


void SensorAdaptador_imprimirValor(){
  byte decimales= SensorAdaptador_getDecimalesMostrados();
  int Val= SensorAdaptador_getValue(); 
  
  if(cartel){
 
	OutputStream_writeStr(cartel);										 
  OutputStream_writeStr(" ");
  }
    
  switch (SensorAdaptador_getState()){
    case SENSOR_OK:
        if(Val>9999)
          OutputStream_writeStr(" OF ");
        else{
          char str[7];
          FloatToStr(Val,str,6,decimales);
          OutputStream_writeStr(str);
        }
    break;

    case SENSOR_OF:
        OutputStream_writeStr(" OF ");
    break;

    case SENSOR_UF:
        OutputStream_writeStr(" UF ");
    break;
  }
}




