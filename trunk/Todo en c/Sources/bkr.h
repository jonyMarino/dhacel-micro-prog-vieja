#ifndef _BKR_H
#define _BKR_H

extern long tempAct;
extern char estado;
extern char flagFinMeseta;
extern bool flagTempInicial;
extern bool flagComienzoRampa;
extern bool flagComieMeseta;
extern word tmpUltimaMeseta;
extern dword rampaTime;

#define RUN 0
#define END 1
#define RST 2 

#define BKR_  3
#define BKR_1 4

void calculoTempInstYtiempoMeseta (void);
void presentaTimeMeseta_SPact(void);

#endif
