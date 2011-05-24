#ifndef _CLASS_SENSOR_H
#define _CLASS_SENSOR_H

#include "Object.h"
#include "GetterWName.h"
#include "stdtypes.h"
#include "MethodContainer.hpp"


struct SensorClass{
  struct GetterWNameClass super;
//  uchar (*_AddOnnewVal)(void *,struct Method *);  // marca si hay algun valor nuevo
};

struct Sensor{
  struct Object super;
   MethodContainer nuevaMedicionListeners;
};

extern const struct Class SensorClass;
extern const struct SensorClass Sensor; 

//#define _addNuevaMedicionListener(sensor,method) \
//          ((*(const struct TSensor **)sensor)->_AddOnnewVal(sensor,method))

//%+
void Sensor_addNuevaMedicionListener(void * _self, struct Method * metodo);
void Sensor_deleteNuevaMedicionListener(void * _self, struct Method * metodo);

//%#
void Sensor_constructor(void * _self);
void Sensor_notificarListeners(void * _self);


typedef enum {
  SENSOR_OK,  //OK
  SENSOR_OF,  //Over Flow
  SENSOR_UF   //Under Flow
}TSensorState;

struct SensorWStateClass{
  struct SensorClass super;  
  TSensorState (*_get_State)(void *);
};

struct SensorWState{
  struct Sensor super;
};

#define _getState(sensor) \
          ((*(struct SensorWStateClass **)sensor)->_get_State(sensor))


struct SensorDecClass{
  struct SensorWStateClass super;
  int (*_get_Dec)(void *); // Obtiene los decimales con que se muestra el sensor
  int (*getDifDecView)(void*);  //Adapta el valor que viene con los mismos decimales con el que se muestra el sensor y lo devuelve con los decimales que se obtienen al hacer un getVal del sensor 
};

struct SensorDec{
  struct SensorWState super;
};

#define _getDec(sensor) \
          ((*(struct SensorDecClass **)sensor)->_get_Dec(sensor))

#define _getDifDecView(sensor) \
          ((*(struct SensorDecClass **)sensor)->getDifDecView(sensor))


struct SensorDecLimClass{					//Decimales y limites
  struct SensorDecClass super;
  int (*_get_Lim_Sup)(void *); 
  int (*_get_Lim_Inf)(void *);
  uint (*getMeasurePeriod)(void*);
};

extern const struct Class SensorDecLimClass; 

struct SensorDecLim{
  struct SensorDec super;
};

#define _Sensor_getLimSup(sensor) \
          ((*(struct SensorDecLimClass **)sensor)->_get_Lim_Sup(sensor))
#define _Sensor_getLimInf(sensor) \
          ((*(struct SensorDecLimClass **)sensor)->_get_Lim_Inf(sensor))
#define _Sensor_getMeasurePeriod(sensor) \
          ((*(struct SensorDecLimClass **)sensor)->getMeasurePeriod(sensor))


struct SensorVisual{
  struct SensorDecLim super;
  char * description;
};

#define _SensorVisual_setDescription(sensor,desc) \
          ((struct SensorVisual *)sensor)->description=desc
          
#define _SensorVisual_getDescription(sensor) \
          ((struct SensorVisual *)sensor)->description

#endif