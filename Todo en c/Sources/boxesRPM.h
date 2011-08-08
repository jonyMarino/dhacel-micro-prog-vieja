#ifndef BoxesRPM_H
#define BoxesRPM_H

#include "boxes.h"  

 extern const Titulo _RPM;
 extern const Numerico PulsosPorVuelta;
 extern const Numerico DecimalesRpm;
 extern const Numerico Filtro;
 extern const Numerico AjusteRpm;
 
 byte  setPulsosPorVuelta(int valor,byte chan);
 byte  setDecimalesRpm(int valor,byte chan);
 byte setFiltro(int valor,byte chan);
 byte setAjuste(int valor,byte chan);

#endif