 //valores de T_Salcont			
#ifdef RPM

#define _drct 0
#define _inv 1
#define _man 2

#else

#define _cal 0
#define _ref 1
#define _man 2			

#endif

//dan1001
#ifdef _APARATO_VIEJO
//valores de T_Alar aparato viejo
#define _e 0
#define _d 1
#define _eret 2
#define _dret 3
#define _dbloq 4
#else
//valores de T_Alar aparato nuevo
#define _e 0
#define _d 1
#define _eret 2
#define _dret 3
#define _dbloq 4
#define _eprop 5
#define _dprop 6
#endif
//dan1001



//valores de Alarmas
#define _rel 0
#define _abs 1
#define _refr 2
#define _ban 3
#define _seg 4
#define _eoc 5
