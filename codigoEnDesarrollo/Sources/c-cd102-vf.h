//////////////////// APARATOS ESPECIALES /////////////////////////////

//#define SP_EXTERNO							//va con CANTIDAD_CANALES 2
//#define EQA                     //va con sp externo            
//#define EQA1                    //solo cambia el stune en el menu
//#define SIMCIC
 //#define COFACO                   //solo las alarmas que esten en EoC funcionan  
/*el setpoint es el interno mas la entrada del segundo canal
usar con cantidad de canales 2*/
//////////////////// MODELOS /////////////////////////////
//#define SERIE101
#ifdef SERIE101
#define outc1 0
#define outc2
#define outa1 2
#define outa2 1
#define outa3 3

#define outc1L 0
#define outc2L
#define outa1L 2
#define outa2L 1
#define outa3L 3
#endif

//#define SERIE100
#ifdef SERIE100
#define outc1 0
#define outc2
#define outa1 2
#define outa2 1
#define outa3 3

#define outc1L 0
#define outc2L
#define outa1L 3
#define outa2L 2
#define outa3L 1
#endif

#define SERIECD101
#ifdef SERIECD101
#define outc1 0
#define outc2
#define outa1 3
#define outa2 2
#define outa3 1

#define outc1L 0
#define outc2L
#define outa1L 1
#define outa2L 3
#define outa3L 2
#endif



//////////MODIFICACIONES//////////////////
#define jony_25_06		/*  Teclas para el Programador*/
#define jony_28_06		/*  1_En manual, que no se respete periodo on-off 
													2_Alarmas tengan Periodo igual al control
													3_Mensajes en el Display al poner estados en el Programador
                          4_Agegado de Box para decidir variable a mostrar en el dpy inferior
                      */
#define jony_05_07  /*
											1_ Problemas como funcion de Flash
											2_ Agregado de comportamiento ante Condicion de Emergencia
                      3_ desconectar los rele cuando se desconecta el aparato
                    */
#define jony_06_07 /*
										1_ Problemas al grabar mientras rueda el programa
										2_ Luego de q desaparece el PWSN tiene q mantenerse prendido el aparato 
                    */
#define jony_08_07  /*
											1_ Problemas al reestablecer el programa
                    */
#define jony_13_07  /*
										1_Arreglo SP Externo
										2_Refresh de parametro inferior en Principal1(MainHandler) siempre
                    */
                    
#define jony_18_07 /*
										1_ Cambio de accion en EndP (queda en el final del programa)
										2_ Los Tiempos con los que se calculan la integral y la derivada 
										dependen del tiempo de entrada ente una lectura y otra del mismo canal 
										3_ Agegado de DIF en Version
                    */
#define jony_03_08  /*
                    1_ Problema con CRC de la comunicacion
                    */
#define jony_15_08  /*
										1_  Problema al mostrar Stop
										2_  Problema al restaurar Programador (se aumento tiempo a 2 seg)
										3_  Problema al ingresar mas de 500 minutos en el programador (problema de cast)
                    */
#define jony_17_08  /*
                    1_  Puesta en marcha de prog nuevo
                    */
#define jony_22_08  /*
										1_  Problema al arrancar programador
										2_  Problema en el modulo de la Flash al grabar un Byte
                    */
#define jony_23_08  /*
                    1_  Agregado de las direcciones de ajuste de sensor
                    */  
//////////////////// Tipo de Aparato /////////////////////////////
  #define VF   
  #ifdef VF
  #define F_VF 1
  #else
  #define F_VF 0
  #endif
  //#define VF_PROG
  #define VF_BKR
  #define VF_BKR_C //si se define aparece la vel, temp, tiem, de lo contrario solo  temp, tiem (vel fija 100.0) 
//#define MSJ_CORTO
//#define debug
//#define _TEST1 // test del tiempo de conversion del aparato
//#define HD90
//#define adquisidor
//#define programador
  #ifdef programador
  #define prog_viejo																								 
  #endif
//#define pirani
#define _APARATO_VIEJO
#define _NMODBUS		 // no modbus
//#define _COM_VIEJA  //compatibilidad DH100 en la comunicacion

//#define BKR

/*
#define CHAN_1 0
#define CHAN_2 1
#define AL_1   2
#define AL_2   3
#define CHAN_3 4
#define CHAN_4 5
#define AL_3   6
#define AL_4   7
  */
#define CHAN_1 0
#define CHAN_2 1
#define AL_1   2
#define AL_2   3
#define CHAN_3 4
#define CHAN_4 5
#define AL_3   6
#define AL_4   7




#define TRETURN 30000  //tiempo de retorno sin boton en ms

#define CANTIDAD_CANALES 1 /*Cantidad de canales */

//#define AD_2s /*  Un ciclo de AD en 1 segundo*/
#define AD_1s /*  Un ciclo de AD en 1 segundo*/
//#define AD_500ms /*  Un ciclo de AD en 1 segundo*/
//#define AD_250ms /*  Un ciclo de AD en 1 segundo*/

//#define Pt-200

#ifdef AD_2s
#define AD_TIME_EN_MILI_SEGUNDOS 2000
#endif

#ifdef AD_1s
#define AD_TIME_EN_MILI_SEGUNDOS 1000
#endif


#ifdef AD_500ms
#define AD_TIME_EN_MILI_SEGUNDOS 500
#endif


#ifdef AD_250ms
#define AD_TIME_EN_MILI_SEGUNDOS 250
#endif


#if CANTIDAD_CANALES<1
#error No hay canales de entrada
#endif
/*
** SET POINTS
*/
#define CANTIDAD_SAL_CONTROL   1 /*Cantidad de salidas control*/

/* 
**  ALARMAS
*/

#define ALARMAS_CH1 1
#define CANTIDAD_SAL_ALARMA  3 /*Cantidad de salidas alarma */
#define NUM_SALIDAS  CANTIDAD_SAL_CONTROL+CANTIDAD_SAL_ALARMA

#if CANTIDAD_SAL_CONTROL+ CANTIDAD_SAL_ALARMA>8
#error El programa fue pensado para hasta 8 salidas, revisar que modificaciones hay que hacer
#endif

#define PARAMETROS 255 /* Cantidad de PARAMETROS que utilizan memoria Flash */
#define COM_OFFSET 4   /* Offset a partir del cual empiezan las direcciones(de la com. serie) donde estan los parámetros extendidos */

#define MEM_PAGINA 512

#define FLASH_PARAMETROS_START 0x4000 /* ep */
#define FLASH_PARAMETROS_END 0x41FF 
  #ifdef programador
  #define FLASH_PROGRAMADOR_START   FLASH_PARAMETROS_END + 1  /* flash donde estan las adquisiciones */  
  #define FLASH_PROGRAMADOR_END   FLASH_PROGRAMADOR_START + 2560 
	#else
	  #ifdef VF
	  #define FLASH_VF_START  FLASH_PARAMETROS_END + 1
    #define FLASH_VF_END FLASH_VF_START + 1000
    #endif
  #endif
  
////BLOQUES A GRABAR POR APAGADO DEL APARATO/////
#define FLASH_APAGADO_START 0x5F00
#define FLASH_APAGADO_END   0x5FFF
/////////////////////////////////////////////////

#define RAM_COMUNICACION_END	 0x1200 /* donde termina la ram a la que se tiene acceso remoto (empieza en 0x1000) */ 
#define RAM_START 0x1000
#define RAM_END 0x1800
#define RAM_COMUNICACION_START 0x1000   /* donde empieza la ram a la que se tiene acceso remoto (empieza en 0x1000) */ 
#define CANTIDAD_DISPLAYS 2	 /* Numero de displays */

////////////////////VISUALIZACION//////////////////////////////////
#define MAX_DIGITOS 25 /* Tamaño del texto más largo*/
#define TIME_SCROLL 250 /* multiplicador de tiempo de scroling*/
#define MAIN_TEXT_PERIOD 4000 /* periodo del cambio de mensaje en el display inferior para la pantalla Main*/
#define MAIN_TEXT_TIME 2000 /* tiempo que estará el mensaje en el display inferior para la pantalla Main*/
#define REFESH_NUMBER 1000 /*periodo en el que se refrescan los valores de los boxes numericos de tiempo real*/

#define CHANGE_HD90_TEXT 2000

 #if  PARAMETROS>256
	 #error los parametros a grabar son mas del tamaño de una pagina, hay que modificar las funciones de FLASH!!
 #endif
 #if CANTIDAD_CANALES <1
  #error como minimo un canal
 #endif
 #if CANTIDAD_CANALES >2
  #error todavia no implementado
 #endif
 #if CANTIDAD_DISPLAYS !=2
  #error todavia no implementado
 #endif


/////////////////////////////////////////////////////////////////////////////
#define ERR_OF 1						 /*Error de la funcion de linealizacion*/
#define ERR_UF 2						 /*Error de la funcion de linealizacion*/

 #define numver "A001 cd102c vF    "
 #define tipoEquipo "vF"