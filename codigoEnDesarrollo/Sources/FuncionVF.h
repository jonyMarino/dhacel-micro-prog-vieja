#ifndef _FUNCIONVF_H
#define _FUNCIONVF_H

#define ENDVF 0
#define RUNVF 1
#define RMPVF 0
#define MSTVF 1
#define Ti_MES     DatosVF[nroProgEnAccion-1][EtapaCorr-1].tiem
extern char EtapaCorr;
extern char VFmod;
extern char VFstatus;
extern char CantEtapas;
extern long tempActVF;
extern bool fistTime;
extern bool spIsMayorMeseta;

void calculoTempInstYtiempoMesetaVF (void);

bool isFirstCalVF();

#endif
