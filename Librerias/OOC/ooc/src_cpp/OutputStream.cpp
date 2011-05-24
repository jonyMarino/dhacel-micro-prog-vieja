#include <assert.h>
#include "OutputStream.hpp"
#include "stdio.h"

/*
 *	OutputStream
 */
 
 OutputStream::OutputStream(){
 }
// OutputStream_close(void*_self)
 void OutputStream::close(void){
}

// OutputStream_flush(void * _self)
void OutputStream::flush(void){
}

// OutputStream_write(void * _self,uchar * b);
void OutputStream::write(const char * b) {
  printf(b);
}

//OutputStream_writeByte(void * _self,uchar b);
void OutputStream::write(uchar b) {
  printf("%c",b);
}
