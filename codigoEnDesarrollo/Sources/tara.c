#include "Mydefines.h"
#include "boxes.h"
#include "tara.h"
#include "In1.h"  

#ifdef TARA

extern long ValProv;
//extern char is_box_principal;
 
 void puestaCero (void){
  
   if(is_box_principal)
    if (!In1_GetVal())		  					      
	    EscribirWord((word)&PRom[R_Offset+0],(-ValProv));	
				
 }

#endif   