#ifndef _MANEJADOR_IMPRESION_PERSISTENTE_HPP
#define _MANEJADOR_IMPRESION_PERSISTENTE_HPP


#include "ManejadorImpresion.hpp"
#include "MIPConf.h"
#include "MethodTimer.h"


class ManejadorImpresionPersistente: public ManejadorImpresion {
    
    public:
      static void printHora(void * _self);
      ManejadorImpresionPersistente(OutputStream& os,struct MIPConf * configuracion);
      void imprimir();
      int getIntervalo();
      void setIntervalo(int intervalo);
      bool getHabilitado();
      void setHabilitado(bool habilitar);
    
    private:
      //OutputStream& os;
      struct MIPConf * conf;
      struct Timer mt;
};

#endif
