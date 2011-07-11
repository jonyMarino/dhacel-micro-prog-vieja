#include "Parametros.h"
#include "cnfbox.h"

////////////////////////////////////////////////////////////////////////////
// VARIABLES EN FLASH	 
////////////////////////////////////////////////////////////////////////////
#pragma CONST_SEG PARAMETERS_PAGE

volatile const int PRom[R_ATA+1] ={
10,    // R_SetPoint      //0
0,    // R_SetPoint+1

10,   // R_AL     
10,   // R_AL+1     
10,   // R_AL+2	         //4

-2,   // R_H1           //5
-2,   // R_H2      

-2,   // R_HA1           //7 
-2,   // R_HA2     
-2,   // R_HA3
    
0,    // R_Per1           //10   
0,    // R_Per2

SENSOR_JA,// R_Sensor1    //12
SENSOR_JA,// R_Sensor2

0,        // R_Decimales1 //14 
0,        // R_Decimales2          

0,        // R_Offset1     //16
0,        // R_Offset2  

1000,     // R_Gan1        //18
1000,     // R_Gan2  

0,        // R_ACT1        //20  
0,        // R_ACT2

0,        // R_AGT1       //22
0,        // R_AGT2

0,        // R_ACP1       //24
0,        // R_ACP2

0,        // R_AGP1       //26
0,        // R_AGP2

0,        // R_AAnlLow1    //28
0,        // R_AAnlLow2

100,      // R_AAnlHi1     //30
100,      // R_AAnlHi2

0,        // R_Lim_Inf1     //32
0,        // R_Lim_Inf2 

450,       
450,      // R_Lim_Sup 			 //35

0,          
0,        // R_Tcontrol1    //37  

0,          
0,        // R_Reset        //39
                            
0,          
0,        // R_IN1          //41

0,          
0,        // R_DR1          //43

0,         
0,
0,        // R_Pra          //46

0,          
0,       // R_Pot_Inf       //48

1000,       
1000,    // R_Pot_Sup      //50

0,          
0,       // R_Tsalcont      //52

0,          
0,        // R_Potman         //55

0,        
0,        // R_Stn          //56

_rel,         
_rel,       
_rel,     // R_T_AL          //59
                                
0,             
0,          
0,         //R_Talar          //62

1,          
2,          // R_Filtro1     //64


#ifdef programador
NO_PROGRAMA,
NO_PROGRAMA,// R_Programa 
0,          
0,
0,
0,
#endif         
#ifdef CCAL
200,         														// R_Pss 
120,         														// R_Tss
600,       															// R_SPlim
800,   																	// R_SPst
450,                                   // R_SPsb    
300,         														// R_Indf
30,        															// R_Drdf
200,   																	// R_Abdf
900,                                    // R_SPlimst
#endif         													 
#if defined(_PRINTER)
10,
0,
0,
0,
0,
0,
0,
0,
#endif
#ifdef RPM
1,
1000,
#endif
#ifdef VF
0,
35,
1,
#endif
#ifdef INTEGRADOR
0,
2,
0,
#endif
#ifdef ADQ
0,
0,
0,
0,
0,                                      // R_Adquirir
0,                                      // R_Borrar
0,                                      // R_Tranfer
5,                                      // R_periodoAdq 
#endif                                  
60,                                     // R_Topn 218
0,                                      // R_COM
0,                                      // R_Dbnd 219
0,                                      // R_SetC 220
0,                                      // R_Ver  221
1,                                      // R_Id   222 /* direccion del Id del aparato*/
0,                                      // R_ATA  229

};



