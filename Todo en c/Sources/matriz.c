#include "matriz.h"

///////////////////////////////////////////////////////////////////////////
// matriz de decodificacion para display de 7 segmentos
///////////////////////////////////////////////////////////////////////////


const unsigned char Car_Ini[]=	{
							

              //     a
							//	f		 b
							//		g
							//	e		 c
							//		 d

												//Pd,c,a,d,e,b,g,f,


0x02,					// -	ok  00000010
0x80,					// .	ok  10000000
0x0E,					// /			00001110
0x7D,					// 0	ok  01111101
0x44,					// 1	ok  01000100
0x3E,					// 2	ok  00111110
0x76,					// 3	ok  01110110
0x47,					// 4	ok  01000111
0x73,					// 5	ok  01110011
0x7B,					// 6	ok  01111011
0x64,					// 7	ok  01100100
0x7F,					// 8	ok  01111111
0x77,					// 9 	ok  01110111
0x46,					// : (lo uso como -1) ok
0x04,					// ;
0x27,					// <			00100111 es el grados cent
0x76,					// =
0x47,					// >			01000111
0x01,					// ?
0x7B,					// @
0x6F,					// A	ok
0x7F,					// B
0x39,					// C	0k
0x7B,					// D
0x3B,					// E	ok
0x2B,					// F	ok
0x79,					// G	ok
0x4F,					// H	ok
0x09,					// I	ok
0x5C,					// J	ok
0x4F,					// K	ok
0x19,					// L	ok
0x6D,			    // M
0x7F,					// N
0x7D,					// O	ok
0x2F,					// P	ok
0x64,					// Q
0x7F,					// R
0x73,					// S	ok
0x7B,					// T	
0x5D,					// U	ok
0x5D,					// V	ok (=U)
0x77,					// W
0x7B,					// X
0x47,					// Y	ok  01000111
0x7F,					// Z
0x77,					// [
0x7B,					// \
0x64,					// ]	// 5d
0x7F,					// ^
0x4A,					//  Agregado
0x10,					// _	ok	
0x04,					// `
0x7E,					// a	ok
0x5B,					// b	ok
0x1A,					// c	ok
0x5E,					// d	ok
0x3F,					// e	ok
0x7F,					// f	
0x77,					// g
0x4B,					// h	ok
0x08,					// i	ok
0x7F,					// j
0xF ,					// k
0x09,					// l	ok
0x6D,			    // m	ok
0x4A,					// n	ok
0x5A,					// o	ok										
0x7B,					// p
0x67,					// q
0x0A,					// r	ok
0x77,					// s
0x1B,					// t	ok
0x58,					// u	ok
0x58,					// v
0x77,					// w
0x7B,					// x
0x64,					// y
0x7F,					// z
					
						 };	
