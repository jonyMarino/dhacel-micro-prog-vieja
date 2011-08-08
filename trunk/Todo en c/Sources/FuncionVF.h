#ifndef _FUNCIONVF_H
#define _FUNCIONVF_H

#include "PE_Types.h"

typedef enum{
  RMPVF,
  MSTVF
}VFstatus;


typedef enum{
  ENDVF,
  RUNVF
}VFmod;

typedef struct{
  long Time;
  long tempAct;
  VFmod mode;
  VFstatus status;
  char EtapaCorr;
  int CantEtapas;
  int nroProgEnAccion;
  bool fistTime;
} dataVF;


extern dataVF datosVF;

#ifdef GOROSITO
#define Unidad     60
#else
#define Unidad     1
#endif 

#define Te_MES_ANT  DatosVF[getNProgVF-1][getNEtapaActualVF-2].tem                              //temp meseta etapa anterior  
#define Te_MES      DatosVF[getNProgVF-1][getNEtapaActualVF-1].tem                               //temp meseta etapa actual
#define Ve_RMP      DatosVF[getNProgVF-1][getNEtapaActualVF-1].vel     
#define Ti_MES      DatosVF[getNProgVF-1][getNEtapaActualVF-1].tiem   
#define MaxMin      10                                                     //fija los limites de sobrepaso de la temperatura de meseta
#define PROTEC      2                                                      //cantidad de veces que compruebo la condicion de fin de rampa

#define getStatusVF          datosVF.status
#define getModeVF            datosVF.mode
#define getSpVF              datosVF.tempAct
#define getNEtapaActualVF    datosVF.EtapaCorr
#define getNProgVF           datosVF.nroProgEnAccion
#define getTimeVF            datosVF.Time
#define getCantidadEtapasVF  datosVF.CantEtapas



void intVF();
void calculoTempInstYtiempoMesetaVF (void);


#endif
