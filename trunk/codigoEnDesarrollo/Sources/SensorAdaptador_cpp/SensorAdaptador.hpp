#ifndef _SENSOR_ADAPTADOR_HPP
#define _SENSOR_ADAPTADOR_HPP

/*
#include "stddef.h"	
#include "Mydefines.h"

#include "MethodContainer.h"
*/
#include "Imprimible.hpp"
#include "PE_Types.h"
#include "OutputStream.hpp"
#include "Sensor.h"

/*typedef enum {
  SENSOR_OK,  //OK
  SENSOR_OF,  //Over Flow
  SENSOR_UF   //Under Flow
}TSensorState;
  */

#pragma CONST_SEG DEFAULT


class SensorAdaptador: public Imprimible{
  public:
    SensorAdaptador(byte numeroSensor,char * nombre){
      numSensor = numeroSensor;
      cartel = nombre;   
    }  
    
    /*
    ** ===================================================================
    **     Method      :  getState 
    **     Description :  Indica el estado del sensor(UF,OF,OK)
    ** ===================================================================
    */
    TSensorState getState();
    /*
    ** ===================================================================
    **     Method      :  SensorAdaptador_getValue 
    **     Description :  Devuelve el último valor procesado del AD
    ** ===================================================================
    */
    int getValue();
    
    /*
    ** ===================================================================
    **     Method      :  printVal 
    **     Description :  Imprime el valor del sensor
    ** ===================================================================
    */
    void imprimirValor(OutputStream& os);

    /*
    ** ===================================================================
    **     Method      :  getDecimalesMostrados 
    **    Description :   Obtiene la cantidad de decimales
    ** ===================================================================
    */
    int getDecimalesMostrados();
    
  private:
    byte numSensor;
    char * cartel;
};
/******************************************/



/// FUNCIONES DE SETEO Y OBTENCION DE VARIABLES 

/** ===================================================================
**     Method      :  get_LimInf_Sensor 
**     Description :  Limite inferior de temperatura del sensor
** ===================================================================


int get_LimInf_Sensor(struct SensorAdaptador * self);


** ===================================================================
**     Method      :  get_LimSup_Sensor 
**     Description :  Limite superior de temperatura del sensor
** ===================================================================


int get_LimSup_Sensor(struct SensorAdaptador * self);


  Decimales */
/*
** ===================================================================
**     Method      :  SENSOR_setDecimales 
**    Description : Setea la cantidad de decimales
** ===================================================================

byte SensorAdaptador_setDecimales(struct SensorAdaptador * self, int val);

** ==========================================================================
**     Method      :  get_LimInf_Decimales 
**    Type:   Static
**    Description :   Obtiene el minimo valor de decimales que puede setearse
** ==========================================================================

int get_LimInf_Decimales(void);

** ==========================================================================
**     Method      :  get_LimSup_Decimales 
**    Description :   Obtiene el maximo valor de decimales que puede setearse
** ==========================================================================

int get_LimSup_Decimales(const struct SensorAdaptador * self);




** ===================================================================
**     Method      :  SensorAdaptador_Print 
**     Description :  Imprime el valor del sensor
** ===================================================================

void SensorAdaptador_Print(struct SensorAdaptador * self,uchar num_display);


*/
#endif




