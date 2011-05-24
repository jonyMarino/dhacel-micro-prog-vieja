
#include "System.h"


#ifdef BAJOCONSUMO

#include "BTBajoConsumoSafe.h"

struct BTBajoConsumo baseTiempo;

void System_init(void){
  newAlloced(&baseTiempo,&BTBajoConsumo,2008,1,1,0,0,0,TRUE);
}

#elif defined(DS1307)

struct BaseTiempoDS1307 baseTiempo;
//struct RelojInternoPersistenciaDS1307 baseTiempo;

void System_init(void){
  newAlloced(&baseTiempo,&BaseTiempoDS1307);
 //newAlloced(&baseTiempo,&RelojInternoPersistenciaDS1307,2008,1,1,0,0,0,TRUE);
}

#else
#include "BaseTiempo.h"

struct BaseTiempo baseTiempo;

void System_init(void){
  newAlloced(&baseTiempo,&BaseTiempo,2008,1,1,0,0,0,TRUE);
}
#endif

void Sys_getDate(DATEREC * date){
  getFecha(&baseTiempo , date);
}

void Sys_getTime(TIMEREC * time){
  getTiempo(&baseTiempo , time);
}