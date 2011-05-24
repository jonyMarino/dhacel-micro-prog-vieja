#ifndef _MANEJADORIMPRESION_HPP
#define _MANEJADORIMPRESION_HPP

#include "LinkedList.hpp"
#include "RlxMTimer.h"
#include "OutputStream.hpp"


class ManejadorImpresion: public LinkedList {
  public:

    ManejadorImpresion(OutputStream& impresora);
    virtual int getIntervalo();
    virtual void setIntervalo(int intervalo);

    virtual bool getHabilitado();
    virtual void setHabilitado(bool habilitar);  
    int getCuentaParcial();
    void sincronismo(); 
  
  protected:
    virtual void imprimir();
    OutputStream& getOS();
  
  private:
    OutputStream& os;  
    struct RlxMTimer timer;
    byte cuentaParcial;
    static void imprimirPeriodico(void * _self);
    
};


#endif
