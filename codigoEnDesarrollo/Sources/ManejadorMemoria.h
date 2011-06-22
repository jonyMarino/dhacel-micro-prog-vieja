#ifndef _MANEJADOR_MEMORIA_H
#define _MANEJADOR_MEMORIA_H

#include "Object.h"
#include "Timer.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "FshTypeSizes.h"

struct ManejadorMemoriaClass{
  struct Class _base; 
  fbyte(*getByte)(void*,fbyte*);
  byte(*setByte)(void*,fbyte*,fbyte);  
  word(*getWord)(void*,word*);
  byte(*setWord)(void*,word*,word); 
  dword(*getDWord)(void*,dword*); 
  byte(*setDWord)(void*,dword*,dword);
};

struct ManejadorMemoria{
  struct Object super;
};

/*
byte ManejadorMemoria_setByte(void * self,fbyte* address,fbyte *data);
byte ManejadorMemoria_setWord(void * self,word address,word *data);
byte ManejadorMemoria_setDWord(void * self,word address,dword* data);

fbyte ManejadorMemoria_getByte(void * self,fbyte* address);
word ManejadorMemoria_getWord(void * self,word address);
dword ManejadorMemoria_getDWord(void * self,word address);
*/
#define _MANEJADOR_MEMORIA_SET(self,accion,dir,data)  \
    ((*(struct ManejadorMemoriaClass **)self)->accion)(self,dir,data)  

#define _MANEJADOR_MEMORIA_GET(self,accion,dir)  \
    ((*(struct ManejadorMemoriaClass **)self)->accion)(self,dir)     
    
    
#define _MANEJADOR_MEMORIA_SET_BYTE(self,dir,data) _MANEJADOR_MEMORIA_SET(self,setByte,dir,data)  
#define _MANEJADOR_MEMORIA_GET_BYTE(self,dir) _MANEJADOR_MEMORIA_GET(self,getByte,dir)

#define _MANEJADOR_MEMORIA_SET_WORD(self,dir,data) _MANEJADOR_MEMORIA_SET(self,setWord,dir,data)  
#define _MANEJADOR_MEMORIA_GET_WORD(self,dir) _MANEJADOR_MEMORIA_GET(self,getWord,dir)

#define _MANEJADOR_MEMORIA_SET_DWORD(self,dir,data) _MANEJADOR_MEMORIA_SET(self,setDWord,dir,data)  
#define _MANEJADOR_MEMORIA_GET_DWORD(self,dir) _MANEJADOR_MEMORIA_GET(self,getDWord,dir)


word super_getWord(void * _class,void*_self,word*);

byte super_setWord(void * _class,void*_self,word*,word); 


#define INITIALIZATION_MANEJADOR_MEMORIA(_class) \
    _class		
    
#define	MANEJADOR_MEMORIA_CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto,getByte,setByte,getWord,setWord,getDWord,setDWord)  \
      CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto),  \
      getByte,\
      setByte,\
      getWord,  \
      setWord,  \
      getDWord, \
      setDWord

#endif
