#include "Mydefines.h"
#include "cnfbox.h"
#include "Humedad.h"
#include "Parametros.h"
 
#ifdef SENS_HUMEDAD

//#define TEST        //sirve para testear como funciona el aparato
                    //en el SP se carga la temp del bulbo seco
                    //en AL se carga la temp del bulbo humedo
#define ERROR 110
#define DetX  2
#define DetY  10

/*
Tabla de humedad relativa porcentual:
  filas: diferencia entre temperatura del bulbo seco y humedo (0,2,4,6,8,10,12,14,16,18,20)
  columnas: temperatura del bulbo seco (10,20,30,40,50,60,70,80,90,100)
*/
 const unsigned int tablaHumedad[11][10]={

  {1000,1000,1000,1000,1000,1000,1000,1000,1000,1000}, 
  {764,825,859,880,894,904,912,920,924,929}, 
  {542,662,728,769,796,815,830,843,853,863},
  {333,510,608,666,705,733,754,772,787,800},
  {135,369,496,571,621,656,684,706,724,741},
  {0,236,392,484,543,585,617,644,666,685},
  {0,112,295,403,471,520,556,586,611,633},
  {0,0,206,328,405,459,499,532,559,584},
  {0,0,122,258,344,403,447,482,512,538},
  {0,0,44,194,287,351,398,436,467,495}, 
  {0,0,0,134,235,303,353,392,425,454}
    
};

 const int diferTempBsBh[11]={0,20,40,60,80,100,120,140,160,180,200};

 const int TempBs[10]={100,200,300,400,500,600,700,800,900,1000};

//busca en la tabla indicada y devuelve la posicion mas cercana y menor al valor buscado

char buscarEnTabla (int x, char longTabla, int * tabla){
   
   char r = longTabla;
   
   if (x == tabla[0]) {			  //  menor igual que el valor minimo en la tabla???
    return 0;					                       // es = valor minimo en la tabla
   } 
   else if (x < tabla[0]){      //  es menor que el valor minimo en la tabla???
      return ERROR;				                      // devolver error
     }
   else if (x == tabla[r]) {	 // es mayor que el valor maximo en la tabla???
     return r;}                 // es =  valor maximo en la tabla
    else if (x > tabla[r]){       // es mayor que el valor maximo de la tabla 
     return ERROR;					              // devolver error
   }
   else {						 				           //  esta dentro de los rangos???
     int l = 0, i = 0, eq = 0;

     while ((l < r) && (r - l > 1)) {		// Algoritmo de busqueda de los 2 valores mas cercanos hacia arriba y hacia abajo
       i = (l + r) / 2;
       if (x > tabla[i])
         l = i;
       else
         if (x < tabla[i])
           r = i;
         else {
           eq = 1;
           break;
         }
     }
     if (eq)
       return i;		// Concuerda con un valor de la tabla
     else 
      return l;
   }

}


coordenadas buscarCordenadas (int xM, int yM){
  coordenadas resultado;
  
// busco la coordenada x mas cercana pero menor
   resultado.x = buscarEnTabla(xM,10,diferTempBsBh);
  
// busco la coordenada y mas cercana pero menor
   resultado.y = buscarEnTabla(yM,9,TempBs);

  
  return resultado;
}

unsigned int interpolacionDoble (void){
  
  char ptoDecimal;
  coordenadas datos;
  int xMed,yMed,X1,Y1;
  unsigned int humedad;
  int Z11,Z21,Z12;

  ptoDecimal = PRom[R_Decimales+1];
  
  #ifdef TEST
   xMed= PRom[R_SetPoint]-PRom[R_AL];
   yMed= PRom[R_SetPoint];
  #else
   xMed = getValCalculado(0)-getValCalculado(1);
   yMed = getValCalculado(0); 
  #endif  
  
  datos = buscarCordenadas(xMed,yMed);
  
  if(datos.x == ERROR || datos.y == ERROR) 
     return ERR_OF;
  
  if((diferTempBsBh[datos.x])== xMed && (TempBs[datos.y]) == yMed)  // el dato esta en la tabla "tablaHumedad" ??
    return tablaHumedad[datos.x][datos.y];  
  
   Z11 = tablaHumedad[datos.x][datos.y];
   Z21 = tablaHumedad[((datos.x)+1)][datos.y];
   Z12 = tablaHumedad[datos.x][((datos.y)+1)];
   X1 = diferTempBsBh[datos.x];
   Y1 = TempBs[datos.y];
   
   humedad = Z11 + ((long)((Z21-Z11)*(xMed-X1)))/(10*DetX) + ((long)((Z12-Z11)*(yMed-Y1)))/(10*DetY);
   
   return humedad;
}
#endif