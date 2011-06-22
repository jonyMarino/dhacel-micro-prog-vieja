#ifndef _CONTROL_H
#define _CONTROL_H
#include "PE_Types.h"


#define CMAX 1000000
#define OPEN 1
#define CLOSE 2
#define REST 0

//#ifdef RPM
//extern int DELTAT;

void MedidorFunciones_setDeltaTiempo(unsigned int delta);

int filtrar (int val_x,int Tf,int DEL_T,int TfMAX,long *buffer_fil);

long filtrarL (long val_x,int Tf,int DEL_T,int TfMAX,long *buffer_fil);
//#endif

int duty_prop( int valcontrol, 	    
              int ab,
              int pinf,
              int psup
              );

 int duty_onoff( int duty_old, 	            //setpoint
              int valcontrol, 	            //variable
              int his 	                    //histeresis
              );

int derivada   (int val_x,									//calcula derivada de vx
                int kder,
                byte modocont,
                int hister_ab,  
							  long *buffer_deriv,
								int *deriv
								);




 int derivada1   (int val_x,									//calcula derivada de vx
                int kder,
                byte modocont,
                int hister_ab,  
							  long *buffer_deriv,
								int *deriv
								);




int proporcional (int val_x,
                  int spt);



void integral ( int ab,     								//calcula integral, antiwindup teniendo en cta refrig
                int aba,		
                int sp,    
                int al,			
                int ki,    
                int vfn,		
                long *smi,    
								int refr);
								
long cprop     ( int ab,     								//calcula duty en prop con limites de pot
                int sp,    
                int ki,    
                int vfn,		
                long smi,    
								int der,
								int rest); 								


int conoff     (int sc,     
                int dtold);
								
int dtprop  (int sc,
             int pmin,
             int pmax);								



int outvprop(int dutyc,								    //lleva apertura y comanda vprop
             int topen,											
             int dbnd,											
             int smpl,											
             long *cont_pos);
             
int dtonoff  (int sc,
                int dtold) ;             

#endif

