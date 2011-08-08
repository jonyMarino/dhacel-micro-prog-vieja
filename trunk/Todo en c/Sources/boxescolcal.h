#ifndef _BOXES_COLCAL_H
#define _BOXES_COLCAL_H 

#define _END   0
#define _STRT  1
#define _TST 	 2
#define _SST   3
#define _TSN   4
#define _WSN   5

extern const Titulo Calibracion;
extern const Numerico Ganancia1;
extern const Textual Ccalstn;
extern const Titulo Colcal;
extern int flagsst;

void monit_colcal(void);

#endif