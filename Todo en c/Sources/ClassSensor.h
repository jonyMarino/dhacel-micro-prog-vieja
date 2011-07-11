#ifndef _CLASS_SENSOR_H
#define _CLASS_SENSOR_H

#include "Object.h"
#include "GetterWName.h"
#include "stdtypes.h"
#include "SearchLib.h"


struct TSensor{
  struct GetterWNameClass _base;
  uchar (*_AddOnnewVal)(void *,struct Method *);  // marca si hay algun valor nuevo
};

struct Sensor{
  void * classOf;
};

#define _Sensor_AddOnNewVal(sensor,method) \
          ((*(const struct TSensor **)sensor)->_AddOnnewVal(sensor,method))




typedef enum {
  SENSOR_OK=OK,  //OK
  SENSOR_OF=OF,  //Over Flow
  SENSOR_UF=UF   //Under Flow
}TSensorState;

struct TSensorWState{
  struct TSensor _sens;  
  TSensorState (*_get_State)(void *);
};

struct SensorWState{
  struct Sensor _base;
};

#define _getState(sensor) \
          ((*(struct TSensorWState **)sensor)->_get_State(sensor))


struct TSensorDec{
  struct TSensorWState _sens;
  uchar (*_get_Dec)(void *); // Obtiene los decimales con que se muestra el sensor
  int (*_AdaptVal)(void*,int);  //Adapta el valor que viene con los mismos decimales con el que se muestra el sensor y lo devuelve con los decimales que se obtienen al hacer un getVal del sensor 
};

struct SensorDec{
  struct SensorWState _base;
};

#define _getDec(sensor) \
          ((*(struct TSensorDec **)sensor)->_get_Dec(sensor))

#define _Sensor_AdaptVal(sensor,val) \
          ((*(struct TSensorDec **)sensor)->_AdaptVal(sensor,val))


struct TSensorDec_Lim{					//Decimales y limites
  struct TSensorDec _sens;
  int (*_get_Lim_Sup)(void *); 
  int (*_get_Lim_Inf)(void *);
  uint (*getMeasurePeriod)(void*);
};

struct SensorDec_Lim{
  struct SensorDec _base;
};

#define _Sensor_getLimSup(sensor) \
          ((*(struct TSensorDec_Lim **)sensor)->_get_Lim_Sup(sensor))
#define _Sensor_getLimInf(sensor) \
          ((*(struct TSensorDec_Lim **)sensor)->_get_Lim_Inf(sensor))
#define _Sensor_getMeasurePeriod(sensor) \
          ((*(struct TSensorDec_Lim **)sensor)->getMeasurePeriod(sensor))


struct TSensor_Visual{					//Decimales y limites
  struct TSensorDec_Lim _sens; 
};

struct SensorVisual{
  struct SensorDec_Lim _base;
  char * description;
};

#define _SensorVisual_setDescription(sensor,desc) \
          ((struct SensorVisual *)sensor)->description=desc
          
#define _SensorVisual_getDescription(sensor) \
          ((struct SensorVisual *)sensor)->description

#endif