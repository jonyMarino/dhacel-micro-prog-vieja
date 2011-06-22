#include "PE_Types.h"
#include "cnfbox.h"
#include "Control.h"
#include "mydefines.h"
#include "piddefines.h"
#include <stdlib.h>

//#ifdef RPM
//int DELTAT=1000;

void MedidorFunciones_setDeltaTiempo(unsigned int delta){
  //DELTAT=delta;
}

int filtrar (int val_x,int Tf,int DEL_T,int TfMAX,long *buffer_fil){
 
  *buffer_fil = ((long)TfMAX*(long)DEL_T*(long)val_x + (long)Tf**buffer_fil)/((long)DEL_T+(long)Tf);
  return (int)(*buffer_fil/TfMAX);

}

 
long filtrarL (long val_x,int Tf,int DEL_T,int TfMAX,long *buffer_fil){
  /*
  *buffer_fil = ((long)TfMAX*(long)DEL_T*(long)val_x+ (long)Tf*(*buffer_fil))/((long)DEL_T+(long)Tf);
  return (long)(*buffer_fil/TfMAX);
 */ 
   
  //Nicolas
  if(Tf==0)
    return val_x;
//opcion 1:
  /*
  else {
    *buffer_fil= (*buffer_fil)*(Tf-1)/Tf + val_x;
      return (long)((*buffer_fil));
  }
    */
  
//opcion 2:
  else{  
  /*
  x(t)(n+dt)=m(t)dt + n x(t-1)
  m(t):valor medido
  x(t-1):valor calculado anterior
  dt: tiempo de repeticion del calculo(1seg)
  n:valor de filtrado / 10
  
  resultando:
  x(t)(tf+10)=m(t)10 + tf x(t-1)   
  */
   *buffer_fil= (*buffer_fil)*Tf + val_x*50;
    return (long)((*buffer_fil)/(Tf+50));
  }
   
} 
 
//#endif 
 /*****************************************************************************************************
                             Calculo de la integral poniendo los limites
 
 La señal de error es e = sp - vx
 La señal PID es K(e + kd de/dt + 1/ki smi)
 
 Supongamos una region activa de Vx entre sp+bi y sp+bs con bi y bs pos o neg.
 Entonces la region activa es
 
 sp+bi < vx < sp+bs
 sp-vx = e < -bi
 sp-vx = e > -bs
 
 y como la integral se suma al error, si hay int
 
 in+e<-bi; in<-bi-e
 in+e>-bs; in>-bs-e
 
 
 Si es cal solamente, los limites de la banda son 
 
 bi = -ab/2
 bs = ab/2
 
 Si es cal-refr hay que incluir la alarma, sea onoff o prop.
  
 bs = max( ab/2 ; al+abs(aba/2))
 bi = min(-ab/2 ; al-abs(aba/2))
 
 Las reglas para la integral son:
 
 
 1- Si Vx fuera de la banda -> int =0. La cond FUERA de banda es
 
 vx-sp < bi
 vx-sp > bs 
 

 2- La int esta limitada tal que e+int este en la banda. Por la regla ant. la cond
 		FUERA de banda es
 
 	 in > -bi-sp+vx
 	 in < -bs-sp+vx
 

se llama con

 ab     PRom[R_H1] 
 aba		PRom[R_HA1] 
 sp     SetPoint[0]
 al			PRom[R_AL]
 ki     PRom[R_IN1]
 vx			ValFinal[0]
 smi    &suminteg[0]
 int    tipo refr


*/
void integral ( int ab,     
                int aba,		
                int sp,    
                int al,			
                int ki,    
                int vfn,		
                long *smi,    
								int refr) {

#define FALSE 0
int bi;
int bs;

 if ((ab>0)&&(ki!=0)){            			                        //integro?

    if((ab/2 > al+abs(aba/2)) || refr == FALSE)									//limite sup de banda
      bs = ab/2;
    else  
			bs = al+abs(aba/2);
			
    if((-ab/2 < al-abs(aba/2)) || refr == FALSE)								//limite inf de banda
      bi = -ab/2;
    else  
			bi = al-abs(aba/2);
			
    
    *smi += ((sp-vfn)*(long)DELTAT)/1000;                             //acumulo
    
    if(vfn-sp > bs || vfn-sp <  bi)                   //1ra regla. 			
			*smi = 0;
		else{
		  if(*smi >(long)ki*(-bi-sp+vfn))												//2a regla
			   *smi =(long)ki*(-bi-sp+vfn);

      if(*smi <(long)ki*(-bs-sp+vfn))
			   *smi =(long)ki*(-bs-sp+vfn);
				 }
		} 
else *smi = 0;																																	 

								}


/*********************************************************************************************************
                       Calculo del valor de control
 
La señal PID es K(e + kd de/dt + 1/ki smi)
la salida tiene 1000 niveles, entonces la ganancia es K = 1000/ab

u = K(e + kd de/dt + 1/ki smi )



*/
 long cprop    (int ab,     
                int sp,    
                int ki,    
                int vfn,		
                long smi,    
								int der,
								int rest) {

int mab; 
long sc;

mab = abs(ab); 

      if(mab == 0)
        mab = 1;																														//para no div por 0

      if(ki != 0 )																													//idem
        sc = (1000*(long)(sp-vfn+der+mab/2))/mab+((long)1000*smi/mab/ki);
      else
		    sc = (1000*(long)(sp+rest-vfn+der+mab/2))/mab;

return sc;

								}

/*********************************************************************************************************/
                       /*Calculo del duty prop*/
 
 int dtprop (int sc,
             int pmin,
             int pmax){
  
 long tmp;
 if(sc > 1000)
    sc = 1000;
 if(sc < 0)   
 		sc = 0;
 
 tmp = (long)pmin + (long)sc * (pmax - pmin) /1000;
 
 return (int)tmp;
								}

/***************************************************************************************************/              
                         /* calculo valor del duty en onoff.FUNCIONA CON AB<0*/
               

//solo entra cuando ab es negativo

  int dtonoff  (int sc,
                int dtold) {
    
								
		if(sc<0)                                  //recordar que ab es negativo
			 return 0;
		else if(sc>1000) 
			 return 1000;
		else
		   return dtold;
								}

/****************************************************************************************************/
	                   			 /*Calculo de la derivada*/
 /*
DER:    Saco la referencia e = r - x = -x
        Pongo filtro de tiempo Td/N  1 / (1 + sTd/N)
        D = KsTd(-x) / (1 + sTd/N)
        D (1 + sTd/N) = -K s Td x = D + sDTd/N = D + sDTf
        D + sDTf = -sxTd
        en temporal
        D + (Tf) (dD/dt) = - Td (dx/dt)
        D(k) + Tf [D(k)-D(k-1)]/h = -Td [x(k) - x(k-1)]/h
        D(k)(1+Tf/h) = D(k-1) (Tf/h) - Td [x(k) - x(k-1)] /h
        D(k)(h+Tf) = D(k-1)Tf - Td deltax
o con la nomenclatura del programa

        D(k) = (D(k-1)kfil - kder deltax)/(DELTAT+kfil)
como DELTAT son milisegundos, trabajo todo en milis


  */

  int derivada (int val_x,
                int kder,
                byte modocont,
                int hister_ab,  
							  long *buffer_deriv,
                int *deriv
                )
{

long auxd;
long kder_10 = kder*10;
long kfil=kder_10/5; 
long DELTAT_10 = DELTAT/100; 

#ifdef _APARATO_VIEJO
   if ((hister_ab > 0)&&(kder!=0)){                                     //veo si hay derivada
#else
   if ((modocont==_pid)&&(kder!=0)){                                    //veo si hay derivada
#endif

          auxd = -(long)(val_x-*deriv)*(long)kder_10;                   // -kder*deltax
					*deriv = val_x;																					      // x(k-1) para la proxima
					*buffer_deriv = (kfil**buffer_deriv + auxd)/(DELTAT_10+kfil);    //(D(k-1)kfil - kder deltax)/(DELTAT+kfil)
					
   }
   else {
             *deriv=val_x; 
      			 *buffer_deriv=0;				
   }

  return (int)*buffer_deriv;
}
            
            
/**************************************************************************************************/


               

/* calculo valor del duty en onoff.*/

int duty_onoff(int duty_old, 	            //setpoint
              int valcontrol, 	          //variable
              int his 	                  //histeresis
              )
{
                
      	int duty;
        int his1;
#ifdef  _APARATO_VIEJO
         his1 = 0 - his;
#else
         his1 = his;
#endif        
        

				if(valcontrol>=his1/2)
				  duty = 1000;
				else if(valcontrol<=(- his1/2))  
					duty = 0;
				else duty = duty_old;             //codigo para indicar que no cambia
				
        return (int)duty;
}

/* calculo valor del duty en proporcional. Pongo
los limites */

//dan-12-02
/*
int duty_prop (int valcontrol,
              int ab,
              int pinf,           //pot inf
              int psup           //pot sup
              ) {
   			long duty;

				if(valcontrol>= ab/2)
				  duty = 1000;
				else if(valcontrol<= - ab/2)  
					duty = 0;
				else 
				duty=(1000*(long)(valcontrol + ab/2))/(long)ab;
				if(duty>=(10*psup))
				  duty = 10*psup;
				else if(duty<=10*pinf)
				  duty = 10*pinf;  

        return (int)duty;
}
*/

 int duty_prop (int valcontrol,
              int ab,
              int pinf,           //pot inf
              int psup           //pot sup
              ) {
   			long duty;

				if(valcontrol>= ab/2)
				  duty = psup;
				else if(valcontrol<= - ab/2)  
					duty = pinf;
				else 
				duty=pinf+(((psup-pinf)*(long)(valcontrol + ab/2))/(long)ab);
        return (int)duty;
              }

//dan-12-02


/* Calculo el termino integral

INT:    I = K/sTi e; sI = K/Ti e. En temporal dI/dt = K/Ti e
        [Ik-I(k-1)]/h = K/Ti ek
        Ik = I(k-1) + (Kh/Ti) ek
guardo 
        Ti Ik =  Ti I(k-1) + K h ek
y saco
        Ik = I(k-1) + (K h / Ti) ek
uso K=1 para ponerlo despues en otro lado

        Ik = I(k-1) + (K h / Ti) ek


Anti windup
limito el valor acumulado de la integral al borde de la banda ya sumada 
al error (y al reset?)

- AB/2 < Pr + In < AB/2
-AB/2 < sp - vx + In < AB/2
- TiAB/2 < Tisp - Tivxk + Ti Ik < Ti AB/2
Ti(-AB/2 - sp + vxk) < Ti Ik < Ti (AB/2 -sp + vx) 

como el integrador esta al borde de la banda, evito que 
integre con variaciones fuera de ella. Si no lo hago, integra
fuera de la banda

*/



               /*
PROP
es simplemente P = K (vr - vx)
uso K=1 para ponerlo despues en otro lado
*/


  int proporcional (int val_x,
                int spt
                )
{

  return (spt-val_x);

};

