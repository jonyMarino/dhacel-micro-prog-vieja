#ifndef _cnfbox_h
#define _cnfbox_h

#include "Mydefines.h"
#include "PE_Types.h"
#include "BoxesStruct.h"

#define adquisidor
#define programador

/* Definicion del instrumento. seleccionar solo una */        
//#define 1ch_1al
//#define 1ch_3al
//#define 2ch_2al
/* Definicion de las estructuras de los boxes de navegacion */
/************************************************************/
/*		void(*DirTask)(void);		funcion que ejecuta tareas especiales de c/box*/

///VARIABLES EXTERNAS QUE UTILIZAN LOS BOXES //////////////
extern int Estado_Adquisicion;
extern int PRam[Parametros+1];
int	 V_Programa, V_Tempi, V_Segmento,  V_Tipo, V_Tempf,
	   V_tiempo, V_Velocidad, V_tol, V_Emer;
///////////////////////////////////////////////////////////


typedef void (**PunteroF)(void);
#define NULL ((void *) 0)


//tipos de boxes manipulados
union{
	Numerico * Num;
	Textual	* Txt;
	Titulo * Tit;
	}PtrBox;
//tipos de boxes manipulados para obteber valores de otros boxes
union{
	Numerico * Num;
	Textual	* Txt;
	Titulo * Tit;
	}PtrBox2;


/* estas son las funciones que procesan el box correspondiente */
void TitleHandler(void);//manejador boxes de titulos
void NumHandler(void);  //manejador boxes tipo numérico
void TxtHandler(void);  //manejador boxes tipo textual
void MainHandler(void); //manejador Pantalla principal
void CodHandler(void);  //manejador Pantalla Codigo
void EstadoHandler(void); //manejador de las pantallas de estado
void ProgHandler(void); //manejador de las pantallas de estado
void SegHandler(void); //manejador de las pantallas de estado

#ifdef adquisidor
void DiaHandler(void);   //manejador Pantalla ingreso de dia
void HoraHandler(void);  //manejador Pantalla ingreso de Hora
#endif	

/************************************/
/* Definicion y armado de los boxes */
/************************************/
const Numerico Principal1;

const Numerico C1SP;
const Numerico C2SP;
const Numerico C1AL1;
const Numerico C2AL1;

const Numerico Codigo1;

const Titulo Sintonia;
const Numerico C1RES;
const Numerico C2RES;
const Numerico C1H;
const Numerico C2H;
const Numerico C1AB;
const Numerico C2AB;
const Textual C1PER;
const Textual C2PER;
const Textual C1PRA;
const Textual C2PRA;
const Numerico C1IN;
const Numerico C2IN;
const Numerico C1DR;
const Numerico C2DR;
const Numerico C1HA;
const Numerico C2HA;
const Numerico C1ABA;
const Numerico C2ABA;


const Titulo Calibracion;
const Textual TipoSensor1;
const Textual TipoSensor2;
const Numerico Decimales1;
const Numerico Decimales2;
const Numerico Offset1;
const Numerico Offset2;
const Numerico Ganancia1;
const Numerico Ganancia2;

const Titulo Setting;
const Textual C1CN;
const Textual C2CN;
const Textual C1AL;
const Textual C2AL;
const Textual C1TAL;
const Textual C2TAL;
const Textual Version1;
const Numerico Id;
const Numerico SetC;
const Titulo Limites;
const Numerico LimiteInferior1;
const Numerico LimiteSuperior1;

const Numerico ACT1;
const Numerico ACT2;
const Numerico AGT1;
const Numerico AGT2;
const Numerico ATA1;
const Numerico ACP1;
const Numerico ACP2;
const Numerico AGP1;
const Numerico AGP2;

#ifdef adquisidor
const Titulo Adquisicion;
const Textual Adqui_State;
const Numerico ano;
const Numerico mes;
const Numerico dia;
const Numerico hora;
const Numerico intervalo;
const Textual Adquiriendo;
#endif

#ifdef programador
const Titulo Programas;
const Numerico Programa;
const Numerico Segmento;
const Textual Tipo;
const Numerico Temp_i;
const Numerico Temp_f;
const Numerico Tiempo;
const Numerico Velocidad;
const Numerico Tolerancia;
const Textual Cond_Emergencia;
#endif										

/* pantalla principal */
/*****************************/
#pragma INTO_ROM
const Numerico Principal1={
      MainHandler,								/* funcion que procesa al box*/
      &PRam[R_SetPoint],					/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Pri ",											/* texto del titulo */
			1,													/* punto decimal a enc */
			0, 32767,										//Limites independientes
			&PRam[R_Ver],								//Parametro que modifica
			&C1SP.DirProc,&Codigo1.DirProc,		//Proximos estados
			&Sintonia.DirProc,&Codigo1.DirProc,		      //Proximos estados
			&C1SP.DirProc,&Codigo1.DirProc,	  //Proximos estados
			NULL,&Codigo1.DirProc			                  //Proximos estados
			};

/*setpoints y alarmas*/			
/*****************************/
#pragma INTO_ROM
const Numerico C1SP={
      NumHandler,									/* funcion que procesa al box*/
      &PRam[R_SetPoint],					/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"SP 1",							        /* texto del titulo */
			Dec_Chan1,											 //pos punto dec
			Lim_Chan1, Lim_Chan1,								 //limites
			NULL,									      //Parametro que modifica
			&C1AL1.DirProc,NULL,						//Proximos estados
			&C1AL1.DirProc,NULL,						//Proximos estados
			&C1AL1.DirProc,NULL,						//Proximos estados
			&C1AL1.DirProc,NULL						//Proximos estados
			};

#pragma INTO_ROM
const Numerico C2SP={
      NumHandler,									/* funcion que procesa al box*/
      &PRam[R_SetPoint+1],				/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"SP 2",							        /* texto del titulo */
			Dec_Chan2,											 //pos punto dec
			Lim_Chan2, Lim_Chan2,								 //limites
			NULL,									      //Parametro que modifica
			&C2AL1.DirProc,NULL,						//Proximos estados
			&C2AL1.DirProc,NULL,						//Proximos estados
			&C2AL1.DirProc,NULL,						//Proximos estados
			&C2AL1.DirProc,NULL						  //Proximos estados
			};

#pragma INTO_ROM			
const Numerico C1AL1=	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_ALa],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AL 1",							 //nombre display
			Dec_Chan1,											 //pos punto dec
			Lim_Chan1, Lim_Chan1,								 //limites
			NULL,							 //parametro que modifica. 
			&C2SP.DirProc,NULL,					 //enter rap,enter mant
			&C2SP.DirProc,NULL,					 //para los dist valores del
			&C2SP.DirProc,NULL,					 //parm que modifica
			&C2SP.DirProc,NULL
			};

#pragma INTO_ROM			
const Numerico C2AL1 =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_ALa+1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AL 2",							 //nombre display
			Dec_Chan2,											 //pos punto dec
			Lim_Chan2, Lim_Chan2,								 //limites
			&PRam[R_Ver],							 //parametro que modifica. 
			&Principal1.DirProc,NULL,					 //enter rap,enter mant
			&Principal1.DirProc,NULL,					 //para los dist valores del
			&Principal1.DirProc,NULL,					 //parm que modifica
			&Principal1.DirProc,NULL
			};
			
			




/* caja de ingreso de codigo */
#pragma INTO_ROM
const Numerico Codigo1={
      CodHandler,				/* funcion que procesa al box*/
      &PRam[R_none],						/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Cod ",				    //nombre display
			0,								/* punto decimal a enc */
			0, 9999,					//limites
			NULL,						  //parametro que modifica.
			NULL,&Adquisicion.DirProc,			//Proximos estados en bl1, primero toque rápido luego toque prolongado
			NULL,&Adquisicion.DirProc,	    //Proximos estados en bl2
			NULL,&Adquisicion.DirProc,			//Proximos estadso en bl3
			NULL,&Adquisicion.DirProc 			//Proximos estadso en bl4
			};
			
						
/* Definicion de los titulos */
/*****************************/

#pragma INTO_ROM
const Titulo Adquisicion={ 
      TitleHandler,										/* funcion que procesa al box*/
			"MEM ",													//nombre display
			" -- ",													//nombre display
			&PRam[R_Ver],											  //parametro que modifica.
			&Adqui_State.DirProc,&Programas.DirProc,		//Proximos estados
			&Adqui_State.DirProc,&Programas.DirProc,		//Proximos estados
			&Adqui_State.DirProc,&Programas.DirProc,		//Proximos estados
			&Adqui_State.DirProc,&Programas.DirProc		//Proximos estados	
			};


#pragma INTO_ROM
const Titulo Programas={ 
      TitleHandler,										/* funcion que procesa al box*/
			"Prg ",													//nombre display
			" -- ",													//nombre display
			&PRam[R_Ver],											  //parametro que modifica.
			&Programa.DirProc,&Sintonia.DirProc,		//Proximos estados
			&Programa.DirProc,&Sintonia.DirProc,		//Proximos estados
			&Programa.DirProc,&Sintonia.DirProc,		//Proximos estados
			&Programa.DirProc,&Sintonia.DirProc		//Proximos estados	
			};


#pragma INTO_ROM
const Titulo Sintonia={ 
      TitleHandler,							/* funcion que procesa al box*/
			"tun ",										//nombre display
			" -- ",										//nombre display
			&PRam[R_Ver],								//parametro que modifica.
			&C1RES.DirProc,&Calibracion.DirProc,					//Proximos estados
			&C1RES.DirProc,&Calibracion.DirProc,					//Proximos estados
			&C1RES.DirProc,&Calibracion.DirProc,					//Proximos estados
			&C1RES.DirProc,&Calibracion.DirProc					//Proximos estados
			};

#pragma INTO_ROM
const Titulo Calibracion={ 
      TitleHandler,						/* funcion que procesa al box*/
			"CAL ",									//nombre display
			" -- ",									//nombre display
			&PRam[R_Ver],							//parametro que modifica.
			&TipoSensor1.DirProc,&Setting.DirProc,  //Proximos estados
			&TipoSensor1.DirProc,&Setting.DirProc,  //Proximos estados
			&TipoSensor1.DirProc,&Setting.DirProc,  //Proximos estados
			&TipoSensor1.DirProc,&Setting.DirProc		//Proximos estados
			};

#pragma INTO_ROM
const Titulo Setting={ 
      TitleHandler,									/* funcion que procesa al box*/
			"SEt ",												//nombre display
			" -- ",												//nombre display
			NULL,												//parametro que modifica.
			&C1CN.DirProc,&Limites.DirProc,				//Proximos estados
			&C1CN.DirProc,&Limites.DirProc,				//Proximos estados
			&C1CN.DirProc,&Limites.DirProc,				//Proximos estados
			&C1CN.DirProc,&Limites.DirProc					//Proximos estados
			};

#pragma INTO_ROM
const Titulo Limites={ 
      TitleHandler,										/* funcion que procesa al box*/
			"LiM ",													//nombre display
			" -- ",													//nombre display
			&PRam[R_Ver],											//parametro que modifica.
			&LimiteInferior1.DirProc,&Principal1.DirProc,		//Proximos estados
			&LimiteInferior1.DirProc,&Principal1.DirProc,		//Proximos estados
			&LimiteInferior1.DirProc,&Principal1.DirProc,		//Proximos estados
			&LimiteInferior1.DirProc,&Principal1.DirProc		//Proximos estados	
			};
			


/* Parametros de Programador */
/*****************************/

#pragma INTO_ROM			
const Numerico Programa =	         //nombre variable
      {
      ProgHandler,						     /* funcion que procesa al box*/
      &V_Programa,									     /* direccion en la E2Prom */
			"Prg ",							         //nombre display
			0,									    		 //pos punto dec
			1, 100,				  				     //limites
			NULL,					               //parametro que modifica. 
			&Temp_i.DirProc,NULL,			 //enter rap,enter mant
			&Temp_i.DirProc,NULL,			 //para los dist valores del
			&Temp_i.DirProc,NULL,			 //parm que modifica
			&Temp_i.DirProc,NULL
			};
			
#pragma INTO_ROM
const Numerico Temp_i =	             //nombre variable
      {NumHandler,						     /* funcion que procesa al box*/
      &V_Tempi,									     /* direccion en la E2Prom */
			"t  i",							         //nombre display
			0,									    		 //pos punto dec
			-1999, 9999,				  				 //limites
			NULL,					 //parametro que modifica. 
			&Segmento.DirProc,NULL,					 //enter rap,enter mant
			&Segmento.DirProc,NULL,					 //para los dist valores del
			&Segmento.DirProc,NULL,					 //parm que modifica
			&Segmento.DirProc,NULL
			};
			
 			
#pragma INTO_ROM			
const Numerico Segmento =	             //nombre variable
      {
      SegHandler,						     /* funcion que procesa al box*/
      &V_Segmento,									     /* direccion en la E2Prom */
			"SEgMEnto ",							         //nombre display
			0,									    		 //pos punto dec
			1, 200,				  				 //limites
			NULL,					 //parametro que modifica. 
			&Tipo.DirProc,NULL,					 //enter rap,enter mant
			&Tipo.DirProc,NULL,					 //para los dist valores del
			&Tipo.DirProc,NULL,					 //parm que modifica
			&Tipo.DirProc,NULL
			};

#pragma INTO_ROM
static const char *RMP_MST[]={
      "rMPt",
      "rMPV",									
			"MSt ",
			"Step"

};
#pragma INTO_ROM
const Textual Tipo=
      {
      TxtHandler,						/* funcion que procesa al box*/
			&V_Tipo,											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"tiPo",									//nombre display
			2,											//Cantidad de textos validos
			RMP_MST,             // Array donde estan los textos
			&V_Tipo,						 //parametro que modifica.
			&Temp_f.DirProc,NULL,					 //Proximos estados
			&Temp_f.DirProc,NULL,					 //Proximos estados
			&Temp_i.DirProc,NULL,					 //Proximos estados
			&Temp_i.DirProc,NULL						 //Proximos estados
			};


			
#pragma INTO_ROM
const Numerico Temp_f =	             //nombre variable
      {NumHandler,						     /* funcion que procesa al box*/
      &V_Tempf,									     /* direccion en la E2Prom */
			"t  F",							         //nombre display
			0,									    		 //pos punto dec
			-1999, 9999,				  				 //limites
			&V_Tipo,					 //parametro que modifica. 
			&Tiempo.DirProc,NULL,					 //enter rap,enter mant
			&Tiempo.DirProc,NULL,					 //para los dist valores del
			&Tiempo.DirProc,NULL,					 //parm que modifica
			&Tiempo.DirProc,NULL
			};

#pragma INTO_ROM
const Numerico Tiempo =	             //nombre variable
      {NumHandler,						     /* funcion que procesa al box*/
      &V_tiempo,									     /* direccion en la E2Prom */
			"tEMP",							         //nombre display
			0,									    		 //pos punto dec
			1, 9999,				  				 //limites
			&V_Tipo,					 //parametro que modifica. 
			&Tolerancia.DirProc,NULL,					 //enter rap,enter mant
			&Tolerancia.DirProc,NULL,					 //para los dist valores del
			&Tolerancia.DirProc,NULL,					 //parm que modifica
			&Tolerancia.DirProc,NULL
			};


#pragma INTO_ROM
const Numerico Velocidad =	             //nombre variable
      {NumHandler,						     /* funcion que procesa al box*/
      &V_Velocidad,									     /* direccion en la E2Prom */
			"VEL ",							         //nombre display
			0,									    		 //pos punto dec
			1, 9999,				  				 //limites
			&V_Tipo,					 //parametro que modifica. 
			&Tolerancia.DirProc,NULL,					 //enter rap,enter mant
			&Tolerancia.DirProc,NULL,					 //para los dist valores del
			&Tolerancia.DirProc,NULL,					 //parm que modifica
			&Tolerancia.DirProc,NULL
			};

#pragma INTO_ROM
const Numerico Tolerancia =	             //nombre variable
      {
      NumHandler,						     /* funcion que procesa al box*/
      &V_tol,									     /* direccion en la E2Prom */
			"toL ",							         //nombre display
			0,									    		 //pos punto dec
			0, 10,				  				 //limites
			&V_Tipo,					 //parametro que modifica. 
			&Cond_Emergencia.DirProc,NULL,					 //enter rap,enter mant
			&Cond_Emergencia.DirProc,NULL,					 //para los dist valores del
			&Cond_Emergencia.DirProc,NULL,					 //parm que modifica
			&Cond_Emergencia.DirProc,NULL
			};
			


#pragma INTO_ROM
static const char *Emergencia[]={
      "End ",									
			"cc  ",
			"rc  ",
			"rS  ",
			"Ec  ",
			"EE  ",
			"rPd "

};


#pragma INTO_ROM
const Textual Cond_Emergencia=
      {TxtHandler,						/* funcion que procesa al box*/
			&V_Emer,											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"EMEr",									//nombre display
			7,											//Cantidad de textos validos
			Emergencia,             // Array donde estan los textos
			&PRam[R_Ver],						 //parametro que modifica.
			&Segmento.DirProc,NULL,					 //Proximos estados
			&Segmento.DirProc,NULL,					 //Proximos estados
			&Segmento.DirProc,NULL,					 //Proximos estados
			&Segmento.DirProc,NULL						 //Proximos estados
			};


/* Parametros de Adquisición */
/*****************************/


#pragma INTO_ROM
static const char *Adq_Estados[]={
      "no  ",									
			"Si  ",
			"HLt ",
			"FULL"

};
#pragma INTO_ROM
const Textual Adqui_State=
      {EstadoHandler,						/* funcion que procesa al box*/
			&Estado_Adquisicion,											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"ESt ",									//nombre display
			4,
			Adq_Estados,                 // Array donde estan los textos
			&PRam[R_Ver],					 //parametro que modifica.
			&ano.DirProc,NULL,					 //Proximos estados
			&ano.DirProc,NULL,					 //Proximos estados
			&ano.DirProc,NULL,					 //Proximos estados
			&ano.DirProc,NULL						 //Proximos estados
			};


#pragma INTO_ROM			
const Numerico ano =	             //nombre variable
      {NumHandler,						     /* funcion que procesa al box*/
      &PRam[R_Ano],									     /* direccion en la E2Prom */
			"A;o ",							         //nombre display
			0,									    		 //pos punto dec
			2000, 2099,				  				 //limites
			&PRam[R_Ver],					 //parametro que modifica. 
			&mes.DirProc,NULL,					 //enter rap,enter mant
			&mes.DirProc,NULL,					 //para los dist valores del
			&mes.DirProc,NULL,					 //parm que modifica
			&mes.DirProc,NULL
			};
			
#pragma INTO_ROM			
const Numerico mes =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_mes],											 /* direccion en la E2Prom */
			"mES ",							 //nombre display
			0,											 //pos punto dec
			0, 12,								 //limites
			&PRam[R_Ver],							 //parametro que modifica. 
			&dia.DirProc,NULL,					 //enter rap,enter mant
			&dia.DirProc,NULL,					 //para los dist valores del
			&dia.DirProc,NULL,					 //parm que modifica
			&dia.DirProc,NULL
			};
			
			
#pragma INTO_ROM			
const Numerico dia =	 //nombre variable
      {
      DiaHandler,						 /* funcion que procesa al box*/
      &PRam[R_dia],											 /* direccion en la E2Prom */
			"diA ",							 //nombre display
			0,											 //pos punto dec
			1, 30,								 //limites
			&PRam[R_Ver],							 //parametro que modifica. 
			&hora.DirProc,NULL,					 //enter rap,enter mant
			&hora.DirProc,NULL,					 //para los dist valores del
			&hora.DirProc,NULL,					 //parm que modifica
			&hora.DirProc,NULL
			};
			
			
#pragma INTO_ROM			
const Numerico hora =	 //nombre variable
      {
      HoraHandler,						 /* funcion que procesa al box*/
      &PRam[R_Hora],											 /* direccion en la E2Prom */
			"HorA",							 //nombre display
			2,											 //pos punto dec
			0, 2359,								 //limites
			&PRam[R_Ver],							 //parametro que modifica. 
			&intervalo.DirProc,NULL,					 //enter rap,enter mant
			&intervalo.DirProc,NULL,					 //para los dist valores del
			&intervalo.DirProc,NULL,					 //parm que modifica
			&intervalo.DirProc,NULL
			};
			
#pragma INTO_ROM			
const Numerico intervalo =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_int],											 /* direccion en la E2Prom */
			"int ",							 //nombre display
			0,											 //pos punto dec
			1, 7200,								 //limites
			&PRam[R_Ver],							 //parametro que modifica. 
			&Adquiriendo.DirProc,NULL,					 //enter rap,enter mant
			&Adquiriendo.DirProc,NULL,					 //para los dist valores del
			&Adquiriendo.DirProc,NULL,					 //parm que modifica
			&Adquiriendo.DirProc,NULL
			};


#pragma INTO_ROM
static const char *SN[]={
      "no  ",									
			"Si  "

};
#pragma INTO_ROM
const Textual Adquiriendo=
      {TxtHandler,						/* funcion que procesa al box*/
			&PRam[R_adq],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Adc ",									//nombre display
			2,											//Cantidad de textos validos
			SN,             // Array donde estan los textos
			&PRam[R_Ver],						 //parametro que modifica.
			&Adquisicion.DirProc,NULL,					 //Proximos estados
			&Adquisicion.DirProc,NULL,					 //Proximos estados
			&Adquisicion.DirProc,NULL,					 //Proximos estados
			&Adquisicion.DirProc,NULL						 //Proximos estados
			};
			

/* Parametros de sintonia    */
/*****************************/
#pragma INTO_ROM			
const Numerico C1RES =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_Reset],											 /* direccion en la E2Prom */
			"rES1",							 //nombre display
			Dec_Chan1,											 //pos punto dec
			Lim_Chan1, Lim_Chan1,								 //limites
			&PRam[R_Tcontrol1],							 //parametro que modifica. 
			&C1H.DirProc,NULL,					 //enter rap,enter mant
			&C1PER.DirProc,NULL,					 //para los dist valores del
			&C1PER.DirProc,NULL,					 //parm que modifica
			&C1PER.DirProc,NULL
			};

#pragma INTO_ROM			
const Numerico C2RES =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_Reset+1],											 /* direccion en la E2Prom */
			"rES2",							 //nombre display
			Dec_Chan2,											 //pos punto dec
			Lim_Chan2, Lim_Chan2,								 //limites
			&PRam[R_Tcontrol1+1],							 //parametro que modifica. 
			&C2H.DirProc,NULL,					 //enter rap,enter mant
			&C2PER.DirProc,NULL,					 //para los dist valores del
			&C2PER.DirProc,NULL,					 //parm que modifica
			&C2PER.DirProc,NULL
			};
#pragma INTO_ROM
static const char *Periodos[]={
      ".1  ",									
			".2  ",
			".5  ",
			"1   ",
			"2   ",
			"5   ",
			"10 ",
			"20 ",
			"50 ",
			"AnL ",
};
#pragma INTO_ROM
const Textual C1PER=
      {TxtHandler,						/* funcion que procesa al box*/
			&PRam[R_Per],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Pr 1",									//nombre display
			10,											//Cantidad de textos validos
			Periodos,             // Array donde estan los textos
			NULL,						 //parametro que modifica.
			&C1AB.DirProc,NULL,					 //Proximos estados
			&C1AB.DirProc,NULL,					 //Proximos estados
			&C1AB.DirProc,NULL,					 //Proximos estados
			&C1AB.DirProc,NULL						 //Proximos estados
			};

#pragma INTO_ROM
const Textual C2PER=
      {TxtHandler,						/* funcion que procesa al box*/
			&PRam[R_Per+1],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Pr 2",									//nombre display
			10,											//Cantidad de textos validos
			Periodos,
			NULL,						 //parametro que modifica.
			&C2AB.DirProc,NULL,					 //Proximos estados
			&C2AB.DirProc,NULL,					 //Proximos estados
			&C2AB.DirProc,NULL,					 //Proximos estados
			&C2AB.DirProc,NULL						 //Proximos estados
			};


#pragma INTO_ROM
const Textual C1PRA=
      {TxtHandler,						/* funcion que procesa al box*/
			&PRam[R_Pra],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"PrA1",									//nombre display
			10,											//Cantidad de textos validos
			Periodos,             // Array donde estan los textos
			NULL,						 //parametro que modifica.
			&C1ABA.DirProc,NULL,					 //Proximos estados
			&C1ABA.DirProc,NULL,					 //Proximos estados
			&C1ABA.DirProc,NULL,					 //Proximos estados
			&C1ABA.DirProc,NULL						 //Proximos estados
			};

#pragma INTO_ROM
const Textual C2PRA=
      {TxtHandler,						/* funcion que procesa al box*/
			&PRam[R_Pra+1],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"PrA2",									//nombre display
			10,											//Cantidad de textos validos
			Periodos,
		  NULL,						 //parametro que modifica.
			&C2ABA.DirProc,NULL,					 //Proximos estados
			&C2ABA.DirProc,NULL,					 //Proximos estados
			&C2ABA.DirProc,NULL,					 //Proximos estados
			&C2ABA.DirProc,NULL						 //Proximos estados
			};



#pragma INTO_ROM			
const Numerico C1H =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_H1],											 /* direccion en la E2Prom */
			"H  1",							 //nombre display
			Dec_Chan1,											 //pos punto dec
			Lim_Chan1, Lim_Chan1,								 //limites
			&PRam[R_Talar1],							 //parametro que modifica. 
			&C1HA.DirProc,NULL,					 //enter rap,enter mant
			&C1PRA.DirProc,NULL,					 //para los dist valores del
			&C1PRA.DirProc,NULL,					 //parm que modifica
			&C1PRA.DirProc,NULL
			};

#pragma INTO_ROM			
const Numerico C2H =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_H1+1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"H  2",							 //nombre display
			Dec_Chan2,											 //pos punto dec
			Lim_Chan2, Lim_Chan2,								 //limites
			&PRam[R_Talar1+1],							 //parametro que modifica. 
			&C2HA.DirProc,NULL,					 //enter rap,enter mant
			&C2PRA.DirProc,NULL,					 //para los dist valores del
			&C2PRA.DirProc,NULL,					 //parm que modifica
			&C2PRA.DirProc,NULL
			};
			
#pragma INTO_ROM			
const Numerico C1AB =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_AB1],											 /* direccion en la E2Prom */
			"Ab 1",							 //nombre display
			Dec_Chan1,											 //pos punto dec
			Lim_Chan1, Lim_Chan1,								 //limites
			NULL,							 //parametro que modifica. 
			&C1IN.DirProc,NULL,					 //enter rap,enter mant
			&C1IN.DirProc,NULL,					 //para los dist valores del
			&C1IN.DirProc,NULL,					 //parm que modifica
			&C1IN.DirProc,NULL
			};

#pragma INTO_ROM			
const Numerico C2AB =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_AB1+1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Ab 2",							 //nombre display
			Dec_Chan2,											 //pos punto dec
			Lim_Chan2, Lim_Chan2,								 //limites
			NULL,							 //parametro que modifica. 
			&C2IN.DirProc,NULL,					 //enter rap,enter mant
			&C2IN.DirProc,NULL,					 //para los dist valores del
			&C2IN.DirProc,NULL,					 //parm que modifica
			&C2IN.DirProc,NULL
			};

#pragma INTO_ROM			
const Numerico C1IN =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_IN1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"in 1",							 //nombre display
			0,											 //pos punto dec
			0, 1000,								 //limites
			NULL,							 //parametro que modifica. 
			&C1DR.DirProc,NULL,					 //enter rap,enter mant
			&C1DR.DirProc,NULL,					 //para los dist valores del
			&C1DR.DirProc,NULL,					 //parm que modifica
			&C1DR.DirProc,NULL
			};

#pragma INTO_ROM			
const Numerico C2IN =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_IN1+1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"in 2",							 //nombre display
			0,											 //pos punto dec
			0, 1000,								 //limites
			NULL,							 //parametro que modifica. 
			&C2DR.DirProc,NULL,					 //enter rap,enter mant
			&C2DR.DirProc,NULL,					 //para los dist valores del
			&C2DR.DirProc,NULL,					 //parm que modifica
			&C2DR.DirProc,NULL
			};

#pragma INTO_ROM			
const Numerico C1DR =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_DR1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"dr 1",							 //nombre display
			0,											 //pos punto dec
			0, 600,								 //limites
			&PRam[R_Talar1],							 //parametro que modifica. 
			&C1HA.DirProc,NULL,					 //enter rap,enter mant
			&C1PRA.DirProc,NULL,					 //para los dist valores del
			&C1PRA.DirProc,NULL,					 //parm que modifica
			&C1PRA.DirProc,NULL
			};

#pragma INTO_ROM			
const Numerico C2DR =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_DR1+1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"dr 2",							 //nombre display
			0,											 //pos punto dec
			0, 600,								 //limites
			&PRam[R_Talar1+1],							 //parametro que modifica. 
			&C2HA.DirProc,NULL,					 //enter rap,enter mant
			&C2PRA.DirProc,NULL,					 //para los dist valores del
			&C2PRA.DirProc,NULL,					 //parm que modifica
			&C2PRA.DirProc,NULL
			};

#pragma INTO_ROM			
const Numerico C1HA =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_HA1],											 /* direccion en la E2Prom */
			"HA 1",							 //nombre display
			Dec_Chan1,											 //pos punto dec
			Lim_Chan1, Lim_Chan1,								 //limites
			NULL,							 //parametro que modifica. 
			&C2RES.DirProc,NULL,					 //enter rap,enter mant
			&C2RES.DirProc,NULL,					 //para los dist valores del
			&C2RES.DirProc,NULL,					 //parm que modifica
			&C2RES.DirProc,NULL
			};

#pragma INTO_ROM			
const Numerico C2HA =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_HA1+1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"HA 2",							 //nombre display
			Dec_Chan2,											 //pos punto dec
			Lim_Chan2, Lim_Chan2,								 //limites
			NULL,							 //parametro que modifica. 
			&Principal1.DirProc,NULL,					 //enter rap,enter mant
			&Principal1.DirProc,NULL,					 //para los dist valores del
			&Principal1.DirProc,NULL,					 //parm que modifica
			&Principal1.DirProc,NULL
			};
			
#pragma INTO_ROM			
const Numerico C1ABA =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_ABA1],											 /* direccion en la E2Prom */
			"AbA1",							 //nombre display
			Dec_Chan1,											 //pos punto dec
			Lim_Chan1, Lim_Chan1,								 //limites
			NULL,							 //parametro que modifica. 
			&C2RES.DirProc,NULL,					 //enter rap,enter mant
			&C2RES.DirProc,NULL,					 //para los dist valores del
			&C2RES.DirProc,NULL,					 //parm que modifica
			&C2RES.DirProc,NULL
			};

#pragma INTO_ROM			
const Numerico C2ABA =	 //nombre variable
      {NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_ABA1+1],											 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AbA2",							 //nombre display
			Dec_Chan2,											 //pos punto dec
			Lim_Chan2, Lim_Chan2,								 //limites
			NULL,							 //parametro que modifica. 
			&Principal1.DirProc,NULL,					 //enter rap,enter mant
			&Principal1.DirProc,NULL,					 //para los dist valores del
			&Principal1.DirProc,NULL,					 //parm que modifica
			&Principal1.DirProc,NULL
			};

/* Parametros de calibracion */
/*****************************/

////////////// Tabla de sensores//////////////////////////////////////////
#pragma INTO_ROM
static const int Sensor[9][2]={  
      -130,900, 	    /* Sensor J decimal, limite inferior y limite superior */
			-110,1300,			/* Sensor n decimal, limite inferior y limite superior */
			-110,1372,			/* Sensor K At decimal, limite inferior y limite superior */
			-31,1722,				/* Sensor S decimal, limite inferior y limite superior */
			-31,1728,				/* Sensor r decimal, limite inferior y limite superior */
			-270,400,			  /* Sensor t decimal, limite inferior y limite superior */
			-150,505,				/* Sensor Pt decimal, limite inferior y limite superior */
			-5000,9999,			/* Sensor Lin decimal, limite inferior y limite superior */
			-50,50				  /* Sensor Mv decimal, limite inferior y limite superior */
};
///////////////////////////////////////////////////
/* tipo de sensor canal 1*/
#pragma INTO_ROM
const char *T_Sensores[]={
      "J   ",									
			"n   ",
			"k   ",
			"S   ",
			"r   ",
			"t   ",
			"Pt  ",
			"Lin ",
			"MiLV",
};
#pragma INTO_ROM
const Textual TipoSensor1=
      {
      TxtHandler,						        /* funcion que procesa al box*/
			&PRam[R_Sensor],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"C  1",									      //nombre display
			9,											      //Cantidad de textos validos
			T_Sensores,                   // Array donde estan los textos
			&PRam[R_Ver],						//parametro que modifica.
			&TipoSensor2.DirProc,NULL,						 //Proximos estados
			&TipoSensor2.DirProc,NULL,						 //Proximos estados
			&TipoSensor2.DirProc,NULL,						 //Proximos estados
			&TipoSensor2.DirProc,NULL							 //Proximos estados
			};
			
#pragma INTO_ROM
const Textual TipoSensor2=
      {TxtHandler,						/* funcion que procesa al box*/
			&PRam[R_Sensor+1],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"C  2",									//nombre display
			10,											//Cantidad de textos validos
			T_Sensores,             // Array donde estan los textos
			&PRam[R_Ver],							 //parametro que modifica.
			&Decimales1.DirProc,NULL,						 //Proximos estados
			&Decimales1.DirProc,NULL,						 //Proximos estados
			&Decimales1.DirProc,NULL,						 //Proximos estados
			&Decimales1.DirProc,NULL							 //Proximos estados
			};


/* decimales*/ 
#pragma INTO_ROM
const Numerico Decimales1={
      NumHandler,					/* funcion que procesa al box*/
      &PRam[R_Decimales],									/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"dec1",							//nombre display
			0,									/* punto decimal a enc */
			0, 3,						     //limites
			&PRam[R_Ver],					//parametro que modifica.
			&Decimales2.DirProc,NULL,			//Proximos estados
			&Decimales2.DirProc,NULL,			//Proximos estados
			&Decimales2.DirProc,NULL,			//Proximos estados
			&Decimales2.DirProc,NULL				//Proximos estados
			};
			

#pragma INTO_ROM
const Numerico Decimales2={
      NumHandler,					/* funcion que procesa al box*/
      &PRam[R_Decimales+1],				/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"dec2",							//nombre display
			0,									/* punto decimal a enc */
			0, 3,						     //limites
			&PRam[R_Ver],					//parametro que modifica.
			&Offset1.DirProc,NULL,			//Proximos estados
			&Offset1.DirProc,NULL,			//Proximos estados
			&Offset1.DirProc,NULL,			//Proximos estados
			&Offset1.DirProc,NULL				//Proximos estados
			};

/* offset*/ 
#pragma INTO_ROM
const Numerico Offset1={
      NumHandler,					/* funcion que procesa al box*/
      &PRam[R_Offset],									/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"OFS1",							//nombre display
			Dec_Chan1,									/* punto decimal a enc */
			Lim_Chan1, Lim_Chan1,						//limites
			&PRam[R_Ver],					//parametro que modifica.
			&Offset2.DirProc,NULL,			//Proximos estados
			&Offset2.DirProc,NULL,			//Proximos estados
			&Offset2.DirProc,NULL,			//Proximos estados
			&Offset2.DirProc,NULL				//Proximos estados
			};
			
#pragma INTO_ROM
const Numerico Offset2={
      NumHandler,					/* funcion que procesa al box*/
      &PRam[R_Offset+1],					/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"OFS2",							//nombre display
			Dec_Chan2,									/* punto decimal a enc */
			Lim_Chan2, Lim_Chan2,						//limites
			&PRam[R_Ver],					//parametro que modifica.
			&Ganancia1.DirProc,NULL,			//Proximos estados
			&Ganancia1.DirProc,NULL,			//Proximos estados
			&Ganancia1.DirProc,NULL,			//Proximos estados
			&Ganancia1.DirProc,NULL				//Proximos estados
			};
			
/* ganancia*/ 
#pragma INTO_ROM
const Numerico Ganancia1={
      NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_Gan],										 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"GAn1",								 //nombre display
			3,										 /* punto decimal a enc */
			-1999, 9999,							 //limites
			&PRam[R_Ver],						 //parametro que modifica.
			&Ganancia2.DirProc,&ACT1.DirProc,			 //Proximos estados
			&Ganancia2.DirProc,&ACT1.DirProc,			 //Proximos estados
			&Ganancia2.DirProc,&ACT1.DirProc,			 //Proximos estados
			&Ganancia2.DirProc,&ACT1.DirProc				 //Proximos estados
			};
			
/* ganancia*/ 
#pragma INTO_ROM
const Numerico Ganancia2={
      NumHandler,						 /* funcion que procesa al box*/
      &PRam[R_Gan+1],										 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"GAn2",								 //nombre display
			3,										 /* punto decimal a enc */
			-1999, 9999,							 //limites
			&PRam[R_Ver],						 //parametro que modifica.
			&Calibracion.DirProc,&ACT2.DirProc,			 //Proximos estados
			&Calibracion.DirProc,&ACT2.DirProc,			 //Proximos estados
			&Calibracion.DirProc,&ACT2.DirProc,			 //Proximos estados
			&Calibracion.DirProc,&ACT2.DirProc				 //Proximos estados
			};
#pragma INTO_ROM
const Numerico ACT1={
      NumHandler,									/* funcion que procesa al box*/
      &PRam[R_ACT],													/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"ACT1",							/* texto del titulo */
			0,													/* punto decimal a enc */
			-1999, 9999,										//Limites independientes
			&PRam[R_Ver],									//Parametro que modifica
			&AGT1.DirProc,NULL,						//Proximos estados
			&AGT1.DirProc,NULL,						//Proximos estados
			&AGT1.DirProc,NULL,						//Proximos estados
			&AGT1.DirProc,NULL						//Proximos estados
			};
#pragma INTO_ROM			
const Numerico AGT1={
      NumHandler,									/* funcion que procesa al box*/
      &PRam[R_AGT],													/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AGt1",							/* texto del titulo */
			0,													/* punto decimal a enc */
			-1999, 9999,										//Limites independientes
			&PRam[R_Ver],									//Parametro que modifica
			&ACP1.DirProc,NULL,						//Proximos estados
			&ACP1.DirProc,NULL,						//Proximos estados
			&ACP1.DirProc,NULL,						//Proximos estados
			&ACP1.DirProc,NULL						//Proximos estados
			};
#pragma INTO_ROM			
const Numerico ACP1={
      NumHandler,									/* funcion que procesa al box*/
      &PRam[R_ACP],													/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"ACP1",							/* texto del titulo */
			0,													/* punto decimal a enc */
			-1999, 9999,										//Limites independientes
			&PRam[R_Ver],									//Parametro que modifica
			&AGP1.DirProc,NULL,						//Proximos estados
			&AGP1.DirProc,NULL,						//Proximos estados
			&AGP1.DirProc,NULL,						//Proximos estados
			&AGP1.DirProc,NULL						//Proximos estados
			};
#pragma INTO_ROM			
const Numerico AGP1={
      NumHandler,									/* funcion que procesa al box*/
      &PRam[R_AGP],													/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AGP1",							/* texto del titulo */
			0,													/* punto decimal a enc */
			-1999, 9999,										//Limites independientes
			&PRam[R_Ver],									//Parametro que modifica
			&ATA1.DirProc,NULL,						//Proximos estados
			&ATA1.DirProc,NULL,						//Proximos estados
			&ATA1.DirProc,NULL,						//Proximos estados
			&ATA1.DirProc,NULL						//Proximos estados
			};
			
#pragma INTO_ROM
const Numerico ATA1={
      NumHandler,									/* funcion que procesa al box*/
      &PRam[R_ATA],													/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AtA ",							/* texto del titulo */
			2,													/* punto decimal a enc */
			-1999, 9999,										//Limites independientes
			&PRam[R_Ver],									//Parametro que modifica
			&Ganancia1.DirProc,NULL,						//Proximos estados
			&Ganancia1.DirProc,NULL,						//Proximos estados
			&Ganancia1.DirProc,NULL,						//Proximos estados
			&Ganancia1.DirProc,NULL						//Proximos estados
			};
			
#pragma INTO_ROM
const Numerico ACT2={
      NumHandler,									/* funcion que procesa al box*/
      &PRam[R_ACT+1],													/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"ACT2",							/* texto del titulo */
			0,													/* punto decimal a enc */
			-1999, 9999,										//Limites independientes
			&PRam[R_Ver],									//Parametro que modifica
			&AGT2.DirProc,NULL,						//Proximos estados
			&AGT2.DirProc,NULL,						//Proximos estados
			&AGT2.DirProc,NULL,						//Proximos estados
			&AGT2.DirProc,NULL						//Proximos estados
			};
#pragma INTO_ROM			
const Numerico AGT2={
      NumHandler,									/* funcion que procesa al box*/
      &PRam[R_AGT+1],													/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AGt2",       							/* texto del titulo */
			0,													/* punto decimal a enc */
			-1999, 9999,										//Limites independientes
			&PRam[R_Ver],									//Parametro que modifica
			&ACP2.DirProc,NULL,						//Proximos estados
			&ACP2.DirProc,NULL,						//Proximos estados
			&ACP2.DirProc,NULL,						//Proximos estados
			&ACP2.DirProc,NULL						//Proximos estados
			};
#pragma INTO_ROM			
const Numerico ACP2={
      NumHandler,									/* funcion que procesa al box*/
      &PRam[R_ACP+1],													/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"ACP2",							/* texto del titulo */
			0,													/* punto decimal a enc */
			-1999, 9999,										//Limites independientes
			&PRam[R_Ver],									//Parametro que modifica
			&AGP2.DirProc,NULL,						//Proximos estados
			&AGP2.DirProc,NULL,						//Proximos estados
			&AGP2.DirProc,NULL,						//Proximos estados
			&AGP2.DirProc,NULL						//Proximos estados
			};
#pragma INTO_ROM			
const Numerico AGP2={
      NumHandler,									/* funcion que procesa al box*/
      &PRam[R_AGP+1],													/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AGP2",							/* texto del titulo */
			0,													/* punto decimal a enc */
			-1999, 9999,										//Limites independientes
			&PRam[R_Ver],									//Parametro que modifica
			&Ganancia2.DirProc,NULL,						//Proximos estados
			&Ganancia2.DirProc,NULL,						//Proximos estados
			&Ganancia2.DirProc,NULL,						//Proximos estados
			&Ganancia2.DirProc,NULL						//Proximos estados
			};
			
	
			
/* Parametros de "set" */
/***********************/

#pragma INTO_ROM
const char *T_Control[]={
      "On-O",									
			"Pid ",
			};
#pragma INTO_ROM
const Textual C1CN=
      {
      TxtHandler,						/* funcion que procesa al box*/
			&PRam[R_Tcontrol1],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"C  1",									//nombre display
			2,											//Cantidad de textos validos
			T_Control,             // Array donde estan los textos
			NULL,							 //parametro que modifica.
			&C1AL.DirProc,NULL,						 //Proximos estados
			&C1AL.DirProc,NULL,						 //Proximos estados
			&C1AL.DirProc,NULL,						 //Proximos estados
			&C1AL.DirProc,NULL							 //Proximos estados
			};

#pragma INTO_ROM
const Textual C2CN=
      {
      TxtHandler,						/* funcion que procesa al box*/
			&PRam[R_Tcontrol1+1],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"C  2",									//nombre display
			2,											//Cantidad de textos validos
			T_Control,             // Array donde estan los textos
			NULL,							 //parametro que modifica.
			&C2AL.DirProc,NULL,						 //Proximos estados
			&C2AL.DirProc,NULL,						 //Proximos estados
			&C2AL.DirProc,NULL,						 //Proximos estados
			&C2AL.DirProc,NULL							 //Proximos estados
			};

#pragma INTO_ROM
const char *T_Alar[]={
      "On-O",									
			"P   ",
			};
			
#pragma INTO_ROM
const Textual C1AL=
      {
      TxtHandler,						/* funcion que procesa al box*/
			&PRam[R_Talar1],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"A  1",									//nombre display
			2,											//Cantidad de textos validos
			T_Alar,             // Array donde estan los textos
			NULL,							 //parametro que modifica.
			&C1TAL.DirProc,NULL,						 //Proximos estados
			&C1TAL.DirProc,NULL,						 //Proximos estados
			&C1TAL.DirProc,NULL,						 //Proximos estados
			&C1TAL.DirProc,NULL							 //Proximos estados
			};

#pragma INTO_ROM
const Textual C2AL=
      {
      TxtHandler,						/* funcion que procesa al box*/
			&PRam[R_Talar1+1],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"A  2",									//nombre display
			2,											//Cantidad de textos validos
			T_Alar,             // Array donde estan los textos
			NULL,							 //parametro que modifica.
			&C2TAL.DirProc,NULL,						 //Proximos estados
			&C2TAL.DirProc,NULL,						 //Proximos estados
			&C2TAL.DirProc,NULL,						 //Proximos estados
			&C2TAL.DirProc,NULL							 //Proximos estados
			};


/* Tipo de alarma */
#pragma INTO_ROM
const char *Alarmas[]={
	    "rEL ",									
			"AbS ",
			"rEFr",
			"bAn ",
			"SG  ",
			"EoC ",
};

#pragma INTO_ROM
const Textual C1TAL=
      {TxtHandler,						/* funcion que procesa al box*/
			&PRam[R_T_AL],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AL 1",									//nombre display
			6,											//Cantidad de textos validos
			Alarmas,
			NULL,						 //parametro que modifica.
			&C2CN.DirProc,NULL,					 //Proximos estados
			&C2CN.DirProc,NULL,					 //Proximos estados
			&C2CN.DirProc,NULL,					 //Proximos estados
			&C2CN.DirProc,NULL						 //Proximos estados
			};
			
#pragma INTO_ROM
const Textual C2TAL=
      {TxtHandler,						/* funcion que procesa al box*/
			&PRam[R_T_AL+1],											/* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"AL 2",									//nombre display
			6,											//Cantidad de textos validos
			Alarmas,
			&PRam[R_Ver],						 //parametro que modifica.
			&Version1.DirProc,NULL,					 //Proximos estados
			&Version1.DirProc,NULL,					 //Proximos estados
			&Version1.DirProc,NULL,					 //Proximos estados
			&Version1.DirProc,NULL						 //Proximos estados
			};


			
/* version */
#pragma INTO_ROM
static const char *Bloqueos[]={
		  "bL1 ",								 
			"bL2 ",
			"bL3 ",
			"bL4 ",
};

#pragma INTO_ROM
const Textual Version1=
      {TxtHandler,					 /* funcion que procesa al box*/
			&PRam[R_Ver],										 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"VEr1",								 //nombre display
			4,										 //Cantidad de textos validos
			Bloqueos,
			NULL,						    //parametro que modifica.
			&Id.DirProc,NULL,					//Proximos estados
			&Id.DirProc,NULL,					//Proximos estados
			&Id.DirProc,NULL,					//Proximos estados
			&Id.DirProc,NULL						//Proximos estados
			};
			


//Id///
#pragma INTO_ROM
const Numerico Id={
      NumHandler,
      &PRam[R_Id],
			"id  ",
			0,
			1, 255,
			NULL,
			&SetC.DirProc,NULL,
			&SetC.DirProc,NULL,
			&SetC.DirProc,NULL,
			&SetC.DirProc,NULL
			};
			
#pragma INTO_ROM
const Numerico SetC={
      NumHandler,
      &PRam[R_SetC],
			"SetC",
			0,
			0, 9999,
			NULL,
			&Setting.DirProc,NULL,
			&Setting.DirProc,NULL,
			&Setting.DirProc,NULL,
			&Setting.DirProc,NULL
			};
			
/* Parametros de "Limites" */
/***************************/

/* limite inferior del set-point de control principal*/ 
#pragma INTO_ROM
const Numerico LimiteInferior1={
      NumHandler,
      &PRam[R_Lim_Inf],
			"Li 1",
			Dec_Chan1,
			Lim_Chan1, Lim_Chan1,
			NULL,
			&LimiteSuperior1.DirProc,NULL,
			&LimiteSuperior1.DirProc,NULL,
			&LimiteSuperior1.DirProc,NULL,
			&LimiteSuperior1.DirProc,NULL
			};
			
#pragma INTO_ROM
const Numerico LimiteInferior2={
      NumHandler,
      &PRam[R_Lim_Inf+1],
			"Li 2",
			Dec_Chan2,
			Lim_Chan2, Lim_Chan2,
			NULL,
			&LimiteSuperior1.DirProc,NULL,
			&LimiteSuperior1.DirProc,NULL,
			&LimiteSuperior1.DirProc,NULL,
			&LimiteSuperior1.DirProc,NULL
			};

/* limite superior del set-point de control principal*/  
#pragma INTO_ROM
const Numerico LimiteSuperior1={
      NumHandler,
      &PRam[R_Lim_Sup],
			"LS 1",
			Dec_Chan1,
			Lim_Chan1, Lim_Chan1,
			NULL,
			&Limites.DirProc,NULL,
			&Limites.DirProc,NULL,
			&Limites.DirProc,NULL,
			&Limites.DirProc,NULL
			};
			
#pragma INTO_ROM
const Numerico LimiteSuperior2={
      NumHandler,
      &PRam[R_Lim_Sup+1],
			"LS 2",
			Dec_Chan2,
			Lim_Chan2, Lim_Chan2,
			NULL,
			&Limites.DirProc,NULL,
			&Limites.DirProc,NULL,
			&Limites.DirProc,NULL,
			&Limites.DirProc,NULL
			};
			
#endif
/* fin  */


