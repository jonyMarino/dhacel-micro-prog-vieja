
#include "ManejadorImpresionPersistente.h"
#include "EPM203Manejador.h"

#ifdef _PRINTER

struct MIPConf * conf;


void ManejadorImpresionPersistenteInit(struct MIPConf * configuracion){

  conf=configuracion;
  ManejadorImpresionInit();
  ManejadorImpresionPersistente_setIntervalo(_MIPConf_getIntervalo(conf));
  if(_MIPConf_getHabilitado(conf))
    setHabilitado(TRUE);
}


/**/
int ManejadorImpresionPersistente_getIntervalo(){
  return _MIPConf_getIntervalo(conf);
}

/**/
void ManejadorImpresionPersistente_setIntervalo(int intervalo){
  if( _MIPConf_setIntervalo(conf,intervalo)== ERR_OK ){
    setIntervalo(intervalo);
  }
    
}


/**/
bool ManejadorImpresionPersistente_getHabilitado(){
  return _MIPConf_getHabilitado(conf);
}

/**/
void ManejadorImpresionPersistente_setHabilitado(bool habilitar){
 
  if( _MIPConf_setHabilitado(conf,habilitar) == ERR_OK) 
    setHabilitado(habilitar);
}

#endif

