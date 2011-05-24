#ifndef _SENSORES_H
#define _SENSORES_H

#include "PE_Types.h"
#include "Mydefines.h"
typedef struct{
  const int * xdat;
  const int * ydat;
  byte max_array;
  byte decimales_max;
  #ifdef _APARATO_VIEJO
  byte decimales_mostrar;
  #endif
}T_Sensor;

#ifdef pirani
//#define SEN_PIR 1
#define SEN_PIR 2

#else
#define SEN_PIR 0
#endif

#ifdef SENS_HUMEDAD
#define HUD 1
#else
#define HUD 0
#endif

#ifdef _APARATO_VIEJO 
#define NUM_SENSORES 15+SEN_PIR+HUD
//#define NUM_SENSORES 15+SEN_PIR

#else
#define NUM_SENSORES 9+SEN_PIR
#endif

extern const T_Sensor Sensores[NUM_SENSORES];
///////////Sensor//////////////////
#ifdef _APARATO_VIEJO
typedef enum{
 SENSOR_JB,
 SENSOR_JA,
 SENSOR_N,   
 SENSOR_KA,
 SENSOR_KB,   
 SENSOR_S,   
 SENSOR_R,
 SENSOR_B,   
 SENSOR_T,   
 SENSOR_PT,  
 #ifdef pirani
 SENSOR_PIR, 
 SENSOR_PR1, 
 #endif
 SENSOR_LIN1, 
 SENSOR_LIN2,
 SENSOR_LIN3,
 SENSOR_LIN4,
 SENSOR_MV,
 #ifdef SENS_HUMEDAD
 SENSOR_HUM,
 #endif    
}t_sensor;
#else
typedef enum{
 SENSOR_J,
 SENSOR_N,   
 SENSOR_K,   
 SENSOR_S,   
 SENSOR_R,   
 SENSOR_T,   
 SENSOR_PT,  
 #ifdef pirani
 SENSOR_PIR, 
 #endif
 SENSOR_LIN, 
 SENSOR_MV,  
}t_sensor;
#endif
/* Devuelve la cantidad de decimales con que se utiliza la tabla del sensor*/
#define SENSOR_Decimales(sensor) Sensores[sensor].decimales_max		
/* Devuelve la cantidad de decimales con que se muestran las lecturas del sensor*/
#ifdef _APARATO_VIEJO
#define SENSOR_Decimales_Mostrar(sensor) Sensores[sensor].decimales_mostrar		
#endif
/* Devulve el minimo valor de temperatura admisible(por el aparato) para Sensor*/
#define SENSOR_Minimo(sensor)  Sensores[sensor].ydat[0]			
/* Devulve el maximo valor de temperatura admisible(por el aparato) para Sensor*/
#define SENSOR_Maximo(sensor)  Sensores[sensor].ydat[Sensores[sensor].max_array]			

unsigned char Linealizar(long Vx, t_sensor sensor, int * Vy);
int CompTempAmb(t_sensor sensor);

int filtro (int val_x,
            int Tf,
            int DEL_T,
            int TfMAX,
            long *buffer_fil
            );  

#endif