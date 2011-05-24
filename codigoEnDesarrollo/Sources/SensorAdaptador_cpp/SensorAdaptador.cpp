#include "SensorAdaptador.hpp"
#include "Mydefines.h"
#include "str_lib.h"
#include "Sensores.h"
#include "cnfbox.h"

extern volatile const int PRom[R_ATA+1];


TSensorState SensorAdaptador::getState(){
  extern word Result[4];	
  return Result[numSensor];
}


int SensorAdaptador::getValue(){
  extern int ValFinal[4];
  return ValFinal[numSensor];
}


void SensorAdaptador::imprimirValor(OutputStream& os){
  byte decimales= getDecimalesMostrados();
  int Val= getValue(); 
  
  if(cartel){
 // os.write("\n");
	os.write(cartel);										 
  os.write(" ");
  }
    
  switch (getState()){
    case SENSOR_OK:
        if(Val>9999)
          os.write(" OF ");
        else{
          char str[7];
          FloatToStr(Val,str,6,decimales);
          os.write(str);
        }
    break;

    case SENSOR_OF:
        os.write(" OF ");
    break;

    case SENSOR_UF:
        os.write(" UF ");
    break;
  }
}


int SensorAdaptador::getDecimalesMostrados(){
  
  t_sensor sensor=PRom[R_Sensor+numSensor];
  return SENSOR_Decimales_Mostrar(sensor);
}


