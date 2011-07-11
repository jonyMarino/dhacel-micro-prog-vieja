#include <assert.h>
#include "OutputStream.h"
#include "stdio.h"

/*
 *	OutputStream
 */
 
void OutputStream_close(void){
}


void OutputStream_flush(void){
}


void OutputStream_writeStr(const char * b) {
  printf(b);
}


void OutputStream_write(uchar b) {
  printf("%c",b);
}
