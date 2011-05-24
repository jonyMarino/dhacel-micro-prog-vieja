 #ifndef _PROM_BKP_H
 #define _PROM_BKP_H 
 
#include "ManejadorMemoria.h"
#include "LinkedList.hpp" 

struct PromBkpClass{
  struct ManejadorMemoriaClass super;
  byte(*borrarProm)(void*,void*);
  void *(*grabarProm)(void*); 
};



 struct PromBkp{
  struct ManejadorMemoria super;  
  bool aGrabar;									 //indica si hay para grabar en espera
  struct Timer * timer;
  word disableCont;
  LinkedList listaDireccionesABorrar;
  //struct LinkedList * preBorrarListenerContainer;
  //struct LinkedList * postBorrarListenerContainer;
  //struct LinkedList * postEscribirListenerContainer;
 };
 
extern const struct Class PromBkpClass;
extern const struct PromBkpClass PromBkp;
 
 
#define _PromBkp_grabarProm(self) \
  ((*(struct PromBkpClass **)self)->grabarProm)(self)

#define _PromBkp_borrarProm(self,direccion) \
  ((*(struct PromBkpClass **)self)->borrarProm)(self,direccion)

byte PromBkp_grabarOBorrar(void * self);	//template method	
bool PromBkp_listoParaGrabarOBorrar(void * self);
bool PromBkp_grabacionOBorradoEnEspera(void * self);
byte PromBkp_borrarPagina(void * self,void * address);

void PromBkp_deshabilitar(void* self);
void PromBkp_habilitar(void * self);
void PromBkp_demorarGrabado(void * self,word time);


byte PromBkp_grabarOBorrar(void * self);

bool PromBkp_estaEnPagina(void * address,void* pagina); //funcion para checkeo
 
byte PromBkp_addPreBorrarListener(void*,void * promListener);
byte PromBkp_deletePreBorrarListener(void*,void * promListener);
byte PromBkp_addPostBorrarObserver(void*,void * promListener);
byte PromBkp_deletePostBorrarObserver(void*,void * promListener);
byte PromBkp_addPostEscribirListener(void*,void * promListener);
byte PromBkp_deletePostEscribirListener(void*,void * promListener);

void * PromBkp_promListener(word address,void * pmethod,void*Obj); 
 
#define INITIALIZATION_PROM_BKP(_class)                     \
    INITIALIZATION_MANEJADOR_MEMORIA(_class),	          		\
    FALSE,																			\
    NULL,                                       \
    0,                                          \
    INICIALITATION_LINKED_LIST()																																									

#define PROM_BKP_CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto,getByte,setByte,getWord,setWord,getDWord,setDWord,grabarProm,borrarProm)  \
  	MANEJADOR_MEMORIA_CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto,getByte,setByte,getWord,setWord,getDWord,setDWord),  \
    borrarProm, \
    grabarProm \
    
#endif