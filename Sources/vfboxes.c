#include "Mydefines.h"
#include "cnfbox.h"
#include "boxes.h"
#include "PE_Types.h"
#include "Sensores.h"
#include "display.h"
#include "In1.h"
#include "IFsh10.h"
#include "FuncionVF.h"
#include "vfboxes.h"
#include "In1.h"
#include "In2.h"

extern PunteroF PtrTmp;	

#ifdef VF

volatile const int PRomVF[11]@(FLASH_VF_START) ={  //valor iniciales
10, //cantidad de programas
1,  //etapas progr 1
1,  //etapas progr 2
1,  //etapas progr 3
1,  //etapas progr 4
1,  //etapas progr 5
1,  //etapas progr 6
1,  //etapas progr 7
1,  //etapas progr 8
1,  //etapas progr 9
1,  //etapas progr 10
};

const ParamVF  DatosVF[MAX_PROGRAMAS_VF][MAX_ETAPAS_VF] @(FLASH_VF_START+24)={		 //Programas guardados en FLASH
/*etapas:*/
/* 1       2       3      4       5       6       7       8       9      10   */
1000,200,1, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0,   //parametros del programa 1
1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0,    //parametros del programa 2
1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0,    //parametros del programa 3 
1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0,    //parametros del programa 4
1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0,    //parametros del programa 5  
1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0,    //parametros del programa 6
1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0,    //parametros del programa 7
1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0,    //parametros del programa 8 
1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0,    //parametros del programa 9
1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0, 1,35,0,    //parametros del programa 10 
};

  

#pragma CONST_SEG DEFAULT 
#pragma STRING_SEG DEFAULT


extern byte BackupArray[MEM_PAGINA];
extern bool flagAlarma; 
extern word screen_cont;
extern int ValFinal[4];
bool flag_tecla_up=0;
bool flag_tecla_down=0;
extern dword rampa_mestaTime;
extern dword timeCartel;
extern byte Tecla;
word minutos = 1;
char flagCartel=1; 
char flagComCartel=FALSE; 
char unaVez=0;
char tecla_d=1;
extern byte Nletras[CANTIDAD_DISPLAYS];

char resetMst=0;
char resetRamp=0;
char cantDig=0;
char _msj[MAX_DIGITOS];
extern byte b; //variable de scrolling 
char unaVuelta=0;
extern bool SaveNow;

extern const Numerico Vrmp1;
extern const Numerico Tmpm1;
extern const Numerico Tiem1;
extern const Numerico etapas;
extern const Numerico nroProg;
extern const Numerico crearProgN;
extern const Titulo ProgramaEnAccion;
  
int velocidadVF,temperaturaVF,tiempoVF;
#ifdef VF_PROG
int nroProgEnAccion=0;
#else
int nroProgEnAccion=1;
#endif
int cantEtapas=1,crearProg=0;
unsigned char etapasActual=0;
int proxBox=0;
char textDincVel[26];
char textDincTie[26];
char textDincTem[26];

const TDato ParametrosVF_Prog[NRO_PARAMETROS_VF]={ 
#ifdef SIMCIC_1
/*vramp1*/{&velocidadVF,NO_FUNCTION,&PRom[R_LIVel],&Lim_80,0},
#else  
/*vramp1*/{&velocidadVF,NO_FUNCTION,&PRom[R_LIVel],&Lim_1000,0},
#endif                           
/*tmpmes1*/{&temperaturaVF,NO_FUNCTION,&PRom[R_LIMs],&PRom[R_Lim_Sup+0],0},
/*tiemes1*/{&tiempoVF,NO_FUNCTION,&Lim_0,&Lim_9999,0},
/*R_Nro_prog*/{&nroProgEnAccion,NO_FUNCTION,&Lim_0,&Lim_10,0},
/*R_etapas*/{&cantEtapas,NO_FUNCTION,&Lim_1,&Lim_10,0},
/*R_Nro_prog_crear*/{&crearProg,NO_FUNCTION,&Lim_0,&Lim_10,0},   
};
   
void cartelVelHandler(void){

#ifdef MSJ_CORTO2
      
       textDincVel[0]=' '; 
       textDincVel[1]=' '; 
       textDincVel[2]='r';
       textDincVel[3]='m';
       textDincVel[4]='P';
       if((etapasActual+1)/10){
          textDincVel[5]=((etapasActual+1)/10+'0'); 
          textDincVel[6]=((etapasActual+1)%10+'0');
      } else  {  
          textDincVel[5]=((etapasActual+1)+'0');
          textDincVel[6]=' ';   
      }
       
       textDincVel[7]=' ';
       textDincVel[8]='v';
       textDincVel[9]='E';
       textDincVel[10]='L';
       textDincVel[11]='o';
       textDincVel[12]='c';
       textDincVel[13]='i';
       textDincVel[14]='d';
       textDincVel[15]='A';
       textDincVel[16]='d';
       textDincVel[17]=' ';
       textDincVel[18]='<';
       textDincVel[19]='C';
       textDincVel[20]='/';  
       textDincVel[21]='M'; 
       textDincVel[22]='i'; 
       textDincVel[23]='n';
       textDincVel[24]=' '; 
       textDincVel[25]='\0'; 
      
#else      
       textDincVel[0]=' '; 
       textDincVel[1]=' '; 
       textDincVel[2]=' '; 
       textDincVel[3]=' '; 
       textDincVel[4]='U';
       textDincVel[5]='E';
       textDincVel[6]='L';
       textDincVel[7]=' ';
       textDincVel[8]='r';
       textDincVel[9]='A';
       textDincVel[10]='m';
       textDincVel[11]='P';
       textDincVel[12]='A';
       if((etapasActual+1)/10){
          textDincVel[13]=((etapasActual+1)/10+'0'); 
          textDincVel[14]=((etapasActual+1)%10+'0');
      } else  {  
          textDincVel[13]=((etapasActual+1)+'0');
          textDincVel[14]=' ';   
      }
       textDincVel[15]='<';
       textDincVel[16]='C';
       textDincVel[17]='/';  
       textDincVel[18]='M'; 
       textDincVel[19]='i'; 
       textDincVel[20]='n';
       textDincVel[21]=' '; 
       textDincVel[22]=' '; 
       textDincVel[23]=' '; 
       textDincVel[24]=' ';  
       textDincVel[25]='\0'; 
 #endif      
  

} 

void cartelTemHandler(void){
 
  #ifdef MSJ_CORTO2
      //"   MSt tEMPErAtUrA  <C   ",
      
       textDincTem[0]=' ';         
       textDincTem[1]=' '; 
       textDincTem[2]=' '; 
       textDincTem[3]='M';
       textDincTem[4]='S';
       textDincTem[5]='t';
       if((etapasActual+1)/10){
          textDincTem[6]=((etapasActual+1)/10+'0'); 
          textDincTem[7]=((etapasActual+1)%10+'0');
      } else  {  
          textDincTem[6]=((etapasActual+1)+'0');
          textDincTem[7]=' ';   
      }
       
       textDincTem[8]=' ';
       textDincTem[9]='t';
       textDincTem[10]='E';
       textDincTem[11]='M';
       textDincTem[12]='P';
       textDincTem[13]='E';
       textDincTem[14]='r';
       textDincTem[15]='A';
       textDincTem[16]='t';
       textDincTem[17]='U';
       textDincTem[18]='r';
       textDincTem[19]='A';
       textDincTem[20]=' ';
       textDincTem[21]='<';
       textDincTem[22]='C';
       textDincTem[23]=' '; 
       textDincTem[24]=' '; 
       textDincTem[25]='\0'; 
      
      
 #else      
       textDincTem[0]=' ';         
       textDincTem[1]=' '; 
       textDincTem[2]=' '; 
       textDincTem[3]=' '; 
       textDincTem[4]='t';
       textDincTem[5]='E';
       textDincTem[6]='M';
       textDincTem[7]='P';
       textDincTem[8]='E';
       textDincTem[9]='r';
       textDincTem[10]=' ';
       textDincTem[11]='M';
       textDincTem[12]='E';
       textDincTem[13]='S';
       textDincTem[14]='E';
       textDincTem[15]='t';
       textDincTem[16]='A';
       if((etapasActual+1)/10){
          textDincTem[17]=((etapasActual+1)/10+'0'); 
          textDincTem[18]=((etapasActual+1)%10+'0');
      } else  {  
          textDincTem[17]=((etapasActual+1)+'0');
          textDincTem[18]=' ';   
      }
       textDincTem[19]='<';
       textDincTem[20]='C';
       textDincTem[21]=' '; 
       textDincTem[22]=' '; 
       textDincTem[23]=' '; 
       textDincTem[24]=' ';  
       textDincTem[25]='\0'; 
   #endif

} 


void cartelTieHandler(void){


    #ifdef MSJ_CORTO2
     // "     MSt tiEMPo Min    ",
     
       textDincTie[0]=' ';             
       textDincTie[1]=' '; 
       textDincTie[2]=' '; 
       textDincTie[3]=' '; 
       textDincTie[4]='M';
       textDincTie[5]='S';
       textDincTie[6]='t';
       if((etapasActual+1)/10){
          textDincTie[7]=((etapasActual+1)/10+'0'); 
          textDincTie[8]=((etapasActual+1)%10+'0');
      } else  {  
          textDincTie[7]=((etapasActual+1)+'0');
          textDincTie[8]=' ';   
      }
       
       textDincTie[9]=' ';
       textDincTie[10]='t';
       textDincTie[11]='i';
       textDincTie[12]='E';
       textDincTie[13]='M';
       textDincTie[14]='P';
       textDincTie[15]='o';
       textDincTie[16]=' ';
       
       textDincTie[17]='M';
       textDincTie[18]='i';
       textDincTie[19]='n'; 
       textDincTie[20]=' '; 
       textDincTie[21]=' '; 
       textDincTie[22]=' ';  
       textDincTie[23]='\0'; 
      
    #else
       
       textDincTie[0]=' ';             
       textDincTie[1]=' '; 
       textDincTie[2]=' '; 
       textDincTie[3]=' '; 
       textDincTie[4]='t';
       textDincTie[5]='i';
       textDincTie[6]='E';
       textDincTie[7]='M';
       textDincTie[8]='P';
       textDincTie[9]='o';
       textDincTie[10]=' ';
       textDincTie[11]='M';
       textDincTie[12]='E';
       textDincTie[13]='S';
       textDincTie[14]='E';
       textDincTie[15]='t';
       textDincTie[16]='A';
       if((etapasActual+1)/10){
          textDincTie[17]=((etapasActual+1)/10+'0'); 
          textDincTie[18]=((etapasActual+1)%10+'0');
      } else  {  
          textDincTie[17]=((etapasActual+1)+'0');
          textDincTie[18]=' ';   
      }
       textDincTie[19]='M';
       textDincTie[20]='i';
       textDincTie[21]='n'; 
       textDincTie[22]=' '; 
       textDincTie[23]=' '; 
       textDincTie[24]=' ';  
       textDincTie[25]='\0'; 
  
  #endif

} 


void VelocidadHandler(void){
unsigned int *dir;
if(FstTime){
   cartelVelHandler();
}

dir=&DatosVF[crearProg-1][etapasActual].vel;
velocidadVF=*dir;     // tomo el valor corespondiente de la flash   

NumHandler();

if(Tecla=='r'){
  if(velocidadVF!=*dir){ //actualizo el valor en flash , si cambio
   EscribirWord((word)dir,((word)velocidadVF)); 
   SaveNow=TRUE;
  }
 }
 
 if (Tecla== 'k') 
   etapasActual=0;
   

}

void TemperaturaHandler(void){
unsigned int *dir;
if(FstTime){
   cartelTemHandler();
}

dir=&DatosVF[crearProg-1][etapasActual].tem;
temperaturaVF=*dir;     // tomo el valor corespondiente de la flash   
NumHandler();
if(Tecla=='r'){
  if(temperaturaVF!=*dir){ //actualizo el valor en flash , si cambio
   EscribirWord((word)dir,((word)temperaturaVF)); 
   SaveNow=TRUE;
  }
 }
 
 if (Tecla== 'k')
   etapasActual=0;
  
}

void TiempoHandler(void){
unsigned int *dir;

dir=&DatosVF[crearProg-1][etapasActual].tiem;
tiempoVF=*dir;     // tomo el valor corespondiente de la flash  

if(FstTime){
  cartelTieHandler();
} 

if(Tecla=='r'){
  etapasActual++;
  if((etapasActual+1)>cantEtapas){
    etapasActual=0;
    proxBox=0;         //retorno al box principal
  }else {
      
      proxBox=1;       //salto a la proxima etapa   
      }
}

NumHandler();

if(Tecla=='r'){
  if(tiempoVF!=*dir){ //actualizo el valor en flash , si cambio
   EscribirWord((word)dir,((word)tiempoVF)); 
   SaveNow=TRUE;
  }
 }
 


}

void EtapasHandler(void){
unsigned int *dir;

//if(FstTime){
dir=&PRomVF[crearProg];
cantEtapas=*dir;     // tomo el valor corespondiente de la flash  
 
NumHandler();
if(Tecla=='r'){
  if(cantEtapas!=*dir){ //actualizo el valor en flash , si cambio
   EscribirWord((word)dir,((word)cantEtapas)); 
   SaveNow=TRUE;
  }
 }
 
 CantEtapas=cantEtapas; 
}


   /* Set vrmp1*/
/*****************/
const Numerico Vrmp1={
      VelocidadHandler,
      &ParametrosVF_Prog[R_Vrmp1],
      #ifdef MSJ_CORTO
      "Ur 1",
      #else
      #ifdef GOROSITO
      "     UEL rAMPA1 <C/HorA  " ,
      #elif defined(VF_BKR)
      "     UEL rAMPA  <C/Min   ",
      #else
	    textDincVel,
			#endif
			#endif
			1,
			NULL,
			(PunteroF*)&Tmpm1.DirProc,
			};


 /* Set tmeseta1*/
/*****************/
const Numerico Tmpm1={
      TemperaturaHandler,
      &ParametrosVF_Prog[R_Tmpm1],
      #ifdef MSJ_CORTO
      "<Cm1",
      
      #elif defined(VF_BKR)
      "     tEMPErAtUrA <C      ",
      #else
		//	"     tEMPEr MESEtA 1 <C  ",
		  textDincTem,
			#endif
			
			DECIMALES_CANAL1,
			NULL,
			(PunteroF*)&Tiem1.DirProc,
			};

const PunteroF R_tiemeseta1[2]={

		  &Principal1.DirProc,
		  &Vrmp1.DirProc
		  
};



 /* Set tiemeseta1*/
/*****************/
const Numerico Tiem1={
      TiempoHandler,
      &ParametrosVF_Prog[R_Tiem1],
      #ifdef MSJ_CORTO
      "tim1",
      
      #elif defined(VF_BKR)
      "     tiEMPo Min          ",
      #else
			//"     tiEMPo MESEtA 1 Min ",
			textDincTie,
			#endif
			0,
			&(proxBox),
			R_tiemeseta1,
			NULL
		  
			};
			

/* Etapas */
/**********/

const Numerico etapas={
      EtapasHandler,
      &ParametrosVF_Prog[R_ETP],
      #ifdef MSJ_CORTO
      "EtPS",
      #else
			"     EtAPAS     ",
			#endif
			DECIMALES_CANAL1,
			NULL,
			 (PunteroF*)&Vrmp1.DirProc,NULL
			};

/* NRO de Programa en Accion */
/**********/
#ifdef VF_PROG
const PunteroF R_NroDeProg[11]={

      &crearProgN.DirProc,	       
		  &Principal1_VF_PROG.DirProc,         
		  &Principal1_VF_PROG.DirProc,                      
			&Principal1_VF_PROG.DirProc,                                  
		  &Principal1_VF_PROG.DirProc,   
		  &Principal1_VF_PROG.DirProc,         
		  &Principal1_VF_PROG.DirProc,                      
			&Principal1_VF_PROG.DirProc,                                  
		  &Principal1_VF_PROG.DirProc,  
		  &Principal1_VF_PROG.DirProc,                                         
		  &Principal1_VF_PROG.DirProc            
   
   };
   
const Numerico nroProg={
      NumHandlervfPrincipal,
      &ParametrosVF_Prog[R_NPRG],
			"    SELEccionAr ProG    ",
			DECIMALES_CANAL1,
			&nroProgEnAccion,
			R_NroDeProg,
			&alarmas.DirProc
		
			};

const PunteroF R_CrearProg[11]={

      &Principal1.DirProc,	       
		  &etapas.DirProc,         
		  &etapas.DirProc,                      
			&etapas.DirProc,                                  
		  &etapas.DirProc,   
		  &etapas.DirProc,         
		  &etapas.DirProc,                      
			&etapas.DirProc,                                  
		  &etapas.DirProc,  
		  &etapas.DirProc,                                         
		  &etapas.DirProc            
   
   };

/*  Crear Programas */
/**********/

const Numerico crearProgN={
      NumHandlervf,
      &ParametrosVF_Prog[R_NPC],
			"    CrEAr ProG    ",
			DECIMALES_CANAL1,
			&crearProg,
			R_CrearProg,NULL
			
			};


const Titulo ProgramaEnAccion={ 
      TitleHandler,						                        /* funcion que procesa al box*/
			"PULSE ;^´ PArA CrEAr   ",										//nombre display
			" -- ",									//nombre display
			NULL,							//parametro que modifica.
			(PunteroF*)&crearProgN.DirProc,NULL //Proximos estados
			};
#endif
/* Procesador de las pantallas  */
/*****************************************************/
void vitroFusionHandler(void){
char repet=0;


  #ifdef MSJ_CORTO
    char new_text[5];
  
    
    if(VFmod==RMPVF || flag_tecla_up==1){
     
       flag_tecla_up=0;
       
       new_text[0]='r'; 
       new_text[1]='m';
       
       if((EtapaCorr)/10){
          new_text[2]=(EtapaCorr)/10+'0'; 
          new_text[3]=(EtapaCorr)%10+'0';
      } else  { 
          new_text[3]=(EtapaCorr+'0');
          new_text[2]=' ';   
      }
       new_text[4]='\0';
       
       if(resetRamp==0){
         resetRamp=1;
         resetMst=0; 
         PasarASCII("    ",1);   //borro la pantalla una ves
         //ResetScroll();
       }
       
    }else {
       
       new_text[0]='m';
       new_text[1]='S';
       if((EtapaCorr)/10){
          new_text[2]=(EtapaCorr)/10+'0'; 
          new_text[3]=(EtapaCorr)%10+'0';
      } else  {  
          new_text[3]=(EtapaCorr+'0');
          new_text[2]=' '; 
      } 
       
       new_text[4]='\0'; 
         
       if(resetMst==0){
         resetRamp=0;
         resetMst=1;
         //PasarASCII("    ",1);   //borro la pantalla una ves 
         //ResetScroll();
       } 
       
    }
  #elif(defined(VF) && !defined(VF_BKR))
    char new_text[22];
  
    
    if(VFmod==RMPVF || flag_tecla_up==1){
     
       flag_tecla_up=0;
       
       new_text[0]=' '; 
       new_text[1]=' ';
       new_text[2]=' ';
       new_text[3]=' ';
       new_text[4]=' ';
       new_text[5]='r';
       new_text[6]='A';
       new_text[7]='m';
       new_text[8]='P';
       new_text[9]='A';
       if((EtapaCorr)/10){
          new_text[10]=(EtapaCorr)/10+'0'; 
          new_text[11]=(EtapaCorr)%10+'0';
      } else  { 
          new_text[10]=(EtapaCorr+'0');
          new_text[11]=' ';   
      }
       
       new_text[12]=' ';
       new_text[13]=' ';
       new_text[14]=' ';
       new_text[15]=' ';
       new_text[16]=' ';
       new_text[17]=' ';
       new_text[18]=' ';
       new_text[19]=' ';
       new_text[20]=' ';
       new_text[21]='\0';
       
       if(resetRamp==0){
         resetRamp=1;
         resetMst=0; 
         PasarASCII("    ",1);   //borro la pantalla una ves
         ResetScroll();
       }
       
    }else {
       if(rampa_mestaTime>(60*minutos))
         minutos++;
     
       new_text[0]=' ';
       new_text[1]=' ';
       new_text[2]=' ';
       new_text[3]=' ';
       new_text[4]=' ';
       new_text[5]='m';
       new_text[6]='E';
       new_text[7]='S';
       new_text[8]='E';
       new_text[9]='t';
       new_text[10]='A';
       if((EtapaCorr)/10){
          new_text[11]=(EtapaCorr)/10+'0'; 
          new_text[12]=(EtapaCorr)%10+'0';
      } else  {  
          new_text[11]=(EtapaCorr+'0');
          //new_text[7]=' '; 
      } 
         new_text[12]=' ';
       
       if((minutos)/100){
          new_text[13]=(minutos)/100+'0';
          new_text[14]=((minutos)%100)/10+'0'; 
          new_text[15]=(minutos)%10+'0';
          new_text[16]='m';
          new_text[17]='i';
          new_text[18]='n';
          new_text[19]=' ';
          new_text[20]=' ';
          new_text[21]='\0';
       } else if((minutos/10) && (minutos/100) != 1) {
         // new_text[9]=' ';
          new_text[13]=(minutos)/10+'0'; 
          new_text[14]=(minutos)%10+'0';
          new_text[15]='m';
          new_text[16]='i';
          new_text[17]='n';
          new_text[18]=' ';
          new_text[19]=' ';
          new_text[20]=' ';
          new_text[21]='\0';
       }else{
          //new_text[9]=' ';
          //new_text[10]=' ';  
          new_text[13]=(minutos+'0');
          new_text[14]='m';
          new_text[15]='i';
          new_text[16]='n';
          new_text[17]=' ';
          new_text[18]=' ';
          new_text[19]=' ';
          new_text[20]=' ';
          new_text[21]='\0';
       }
       
       
       if(resetMst==0){
         resetRamp=0;
         resetMst=1;
         PasarASCII("    ",1);   //borro la pantalla una ves 
         ResetScroll();
       } 
       
    }
 #elif defined(VF_BKR)
   
    char new_text[22];
  
    
    if(VFmod==RMPVF || flag_tecla_up==1){
     
       flag_tecla_up=0;
       
       new_text[0]=' '; 
       new_text[1]=' ';
       new_text[2]=' ';
       new_text[3]=' ';
       new_text[4]=' ';
       new_text[5]='C';
       new_text[6]='A';
       new_text[7]='L';
       new_text[8]='E';
       new_text[9]='n';
       new_text[10]='t';
       new_text[11]='A';
       new_text[12]='n';
       new_text[13]='d';
       new_text[14]='o';
       new_text[15]=' ';
       new_text[16]=' ';
       new_text[17]=' ';
       new_text[18]=' ';
       new_text[19]=' ';
       new_text[20]=' ';
       new_text[21]='\0';
       
       if(resetRamp==0){
         resetRamp=1;
         resetMst=0; 
         PasarASCII("    ",1);   //borro la pantalla una ves
         ResetScroll();
       }
       
    }else {
       if(rampa_mestaTime>(60*minutos))
         minutos++;
     
       new_text[0]=' ';
       new_text[1]=' ';
       new_text[2]=' ';
       new_text[3]=' ';
       new_text[4]=' ';
       new_text[5]='F';
       new_text[6]='i';
       new_text[7]='n';
       new_text[8]='A';
       new_text[9]='L';
       new_text[10]=' ';
       new_text[11]='E';
       new_text[12]='n';
       new_text[13]=' ';
       
       if((Ti_MES-minutos+1)/100){
          new_text[14]=(Ti_MES-minutos+1)/100+'0';
          new_text[15]=((Ti_MES-minutos+1)%100)/10+'0'; 
          new_text[16]=(Ti_MES-minutos+1)%10+'0';
          new_text[17]=' ';
          new_text[18]='m';
          new_text[19]='i';
          new_text[20]='n';
          new_text[21]='\0';
       } else if(((Ti_MES-minutos+1)/10) && ((Ti_MES-minutos+1)/100) != 1) {
         // new_text[9]=' ';
          new_text[14]=(Ti_MES-minutos+1)/10+'0'; 
          new_text[15]=(Ti_MES-minutos+1)%10+'0';
          new_text[16]=' ';
          new_text[17]='m';
          new_text[18]='i';
          new_text[19]='n';
          new_text[20]=' ';
          new_text[21]='\0';
       }else{
          //new_text[9]=' ';
          //new_text[10]=' ';  
          new_text[14]=((Ti_MES-minutos+1)+'0');
          new_text[15]=' ';
          new_text[16]='m';
          new_text[17]='i';
          new_text[18]='n';
          new_text[19]=' ';
          new_text[20]=' ';
          new_text[21]='\0';
       }
       
       
       if(resetMst==0){
         resetRamp=0;
         resetMst=1;
         PasarASCII("    ",1);   //borro la pantalla una ves 
         ResetScroll();
       } 
       
    }
  #endif 
  #ifdef VF   
    set_MainText(new_text);
  #endif  
    
    //PasarASCII(new_text,1);
  }

/********************Proceso de las teclas(inicio y fin)***************************/



void ProcesoTeclasVF(void){

#ifdef VF_BKR
  CantEtapas = 1;
#endif

if((is_box_principal==1 || is_box_principal==3) && nroProgEnAccion!=0 /*&& flagCartel!=0*/){
  
/* proceso Tecla UP */  // siempre pone la variable "NroEtapas" en 1 para inicar
#ifdef LLAVES_EXT
	 if (Tecla=='u' || !In1_GetVal()){
#elif defined VF_PROG
   if (Tecla=='r'){  
#else
	 if (Tecla=='u'){
#endif
  #ifdef VF_PROG	 
    if(is_box_principal==1){
     is_box_principal=3;
   #endif    
	   tecla_d=0; 
	   EtapaCorr = 1;
	   fistTime = TRUE;
	   flag_tecla_up=1;
	   rampa_mestaTime=0;
     VFstatus = RUNVF;
     #ifndef MSJ_CORTO
     PasarASCII("    ",1);   //borro la pantalla una ves 
	   ResetScroll();
	   #endif
	 #ifdef VF_PROG  
    }
   #endif 
  }
  
//#if defined(COFACO) || defined(VF101) || defined(SIMCIC_1)
  if (Tecla=='k'){ 
    if(flagAlarma==TRUE){ // si la alarma esta conectada y se presiona "R" se desconecta
      flagAlarma=FALSE;    // esto es asi solo en el aparato "COFACO"
      tecla_d=1;   
     }
  }
//#endif 
  
	/* proceso Tecla down */ // incrementa la variable "NroEtapas" (si es cero no)
#ifndef VF_PROG 	
#ifdef LLAVES_EXT	
	if (Tecla=='d' || !In2_GetVal()){
#else
  if (Tecla=='d'){ 
#endif    
	   #if defined(VF_BKR) && !defined(VF101)
     if(flagAlarma==TRUE){ // si la alarma esta conectada y se presiona "down" se desconecta
      flagAlarma=FALSE;    // esto es asi solo en el aparato "VF_BKR"
      tecla_d=1;   
     }
     #else
     tecla_d=0;
     #endif
	    
	   
	   if(EtapaCorr!=0 && EtapaCorr<CantEtapas){
	    EtapaCorr++;
	    VFmod=RMPVF;
	    tempActVF = DatosVF[nroProgEnAccion-1][EtapaCorr-2].tem;
	    rampa_mestaTime=0;
	    minutos=1;
	    flag_tecla_down=1;
	    #if !defined (MSJ_CORTO) && !defined (VF_PROG)
	    PasarASCII("    ",1);   //borro la pantalla una ves 
	    ResetScroll();
	    #endif
	    
	   }
	   else{
	    tempActVF=0;
	    minutos=1;
	    VFstatus = ENDVF;
	    #if !defined (MSJ_CORTO) && !defined (VF_PROG)
	    PasarASCII("    ",1);   //borro la pantalla una ves 
	    ResetScroll();
	    #endif
	    
	   }
	  
	}
	#endif
	}
	
	
	
 #ifdef VF_PROG 	
	if (Tecla=='u'){ 
	  if(PtrTmp==&Principal1.DirProc){
	      tempActVF=0;
	      minutos=1;
	      VFstatus = ENDVF;
	  }
	
	}
 #endif
}

/************************Carteles******************************************/

bool cartelesHandler (){
 
 if(timeCartel<=REP_TIME && flagCartel!=1){
 // flagComCartel=TRUE;
 #ifndef MSJ_CORTO 
  if(unaVez==0){
    PasarASCII("    ",1);   //borro la pantalla una ves
    unaVez=1 ;
  }
 #endif
  flagComCartel=TRUE;
 #ifdef MSJ_CORTO
  set_MainText("Pcom");
 #else 
  set_MainText("    ProGrAmA comPLEto   ");
  //muestraUnaVez ("PrOGrAmA cOmPLEtO");
 #endif
  
  return 0;   
 }else {
   flagComCartel=FALSE;
   timeCartel=0;
   unaVez=0;
   return 1;
 }
}

/**************************************************************************/

void muestraUnaVez (char * msj){
 
 char i,k;
 
 _msj[0]=' ';
 _msj[1]=' ';
 _msj[2]=' ';
 _msj[3]=' ';
// _msj[4]=' ';
 
 if(unaVez==0){
    PasarASCII("    ",1);   //borro la pantalla una ves
    unaVez=1 ;
    unaVuelta=0;
    
  for(i=4,k=0;msj[k]!='\0'&&i<MAX_DIGITOS;i++,k++){
   
    _msj[i]=msj[k];
  
 }    
   cantDig=k;
 }
 
  if(b<cantDig && unaVuelta ==0){
    set_MainText(_msj);
    
  }else{
    unaVuelta=1;
    PasarASCII("    ",1);   //borro la pantalla una ves 
     
  }
} 

#endif