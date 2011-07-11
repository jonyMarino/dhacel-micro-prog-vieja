#ifndef _SENSOR_ADAPTADOR_H
#define _SENSOR_ADAPTADOR_H


#include "PE_Types.h"
#include "OutputStream.h"

typedef enum {
  SENSOR_OK,  //OK
  SENSOR_OF,  //Over Flow
  SENSOR_UF   //Under Flow
}TSensorState;

void SensorAdaptadorInit(byte numeroSensor,char * nombre);   
    
TSensorState SensorAdaptador_getState();

int SensorAdaptador_getValue();
    
void SensorAdaptador_imprimirValor();

int SensorAdaptador_getDecimalesMostrados();
    
#endif

