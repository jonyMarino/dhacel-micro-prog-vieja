#include "Sensor.h"
#include "Mydefines.h"

#ifdef RPM

MethodContainer listeners;
const struct SensorClass Sensor={
  &SensorClass,
  "Sensor",
  &Object  
}; 

void Sensor_constructor(void * _self){
  struct Sensor * self=(struct Sensor *)_self;
  self->nuevaMedicionListeners=listeners;
  //newAlloced(&self->nuevaMedicionListeners,&MethodContainer);  
}

void Sensor_addNuevaMedicionListener(void * _self, struct Method * metodo){
  struct Sensor * self=(struct Sensor *)_self;
 // MethodContainer_add(&self->nuevaMedicionListeners,metodo);
  self->nuevaMedicionListeners.add(metodo);
}

void Sensor_deleteNuevaMedicionListener(void * _self, struct Method * metodo){
  struct Sensor * self=(struct Sensor *)_self;
  //MethodContainer_delete(&self->nuevaMedicionListeners,metodo);
}


void Sensor_notificarListeners(void * _self){
  struct Sensor * self=(struct Sensor *)_self;
  //MethodContainer_execute(&self->nuevaMedicionListeners);
  self->nuevaMedicionListeners.executeMethods();
}

#endif
