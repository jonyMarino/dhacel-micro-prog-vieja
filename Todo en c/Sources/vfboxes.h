#ifndef _VF_BOXES_H
#define _VF_BOXES_H 

#define REP_TIME  6    //tiempo de repeticion de carteles en segundos
#define MAX_PROGRAMAS_VF 10
#define MAX_ETAPAS_VF 10
#define NRO_PARAMETROS_VF 6

#define R_Vrmp1 0
#define R_Tmpm1 1
#define R_Tiem1 2
#define R_NPRG  3
#define R_ETP   4
#define R_NPC   5

typedef struct{
 unsigned int vel; //velocidad 
          int tem; //temperatura de meseta
 unsigned int tiem;//tiempo  
}ParamVF;


 void vitroFusionHandler(void);
 
 void ProcesoTeclasVF(void);
 
 bool cartelesHandler (void);
 
 void muestraUnaVez (char * msj);

 extern const TDato ParametrosVF_Prog[NRO_PARAMETROS_VF];
 
 extern const Numerico Vrmp1;
 extern const Numerico Tmpm1;
 extern const Numerico etapas;
 extern const Numerico crearProgN;
 extern const Numerico nroProg;
 extern const Titulo ProgramaEnAccion;
 
 extern char flagCartel;
 extern char tecla_d;
 extern char flagComCartel;
 extern const ParamVF  DatosVF[MAX_PROGRAMAS_VF][MAX_ETAPAS_VF];
 extern volatile const int PRomVF[11];
 extern unsigned char etapasActual;
 extern int nroProgEnAccion;
 extern int crearProg;
    
   
 
#endif