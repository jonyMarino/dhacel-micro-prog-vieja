#include <assert.h>
#include "OutStream.h"

const struct Class OutputStreamClass={
  CLASS_INITIALIZATION(Class,OutputStreamClass,Class,Object_ctor,Object_dtor,Object_differ,Object_puto)  
};

const struct OutputStreamClass OutStream={
  OUTPUT_STREAM_CLASS_INITIALIZATION(OutputStreamClass,
                                     OutStream,
                                     Object,
                                     Object_ctor,
                                     Object_dtor,
                                     Object_differ,
                                     Object_puto,
                                     OutputStream_close,
                                     OutputStream_flush,
                                     OutputStream_write,
                                     OutputStream_writeByte)
};


/*
 *	OutStream
 */
 
// OutputStream_close(void*_self)
void OutputStream_close(void*_self){
}

// OutputStream_flush(void * _self)
void OutputStream_flush(void * _self){
}

// OutputStream_write(void * _self,uchar * b);
void OutputStream_write(void * _self,const char * b) {
  printf(b);
}

//OutputStream_writeByte(void * _self,uchar b);
void OutputStream_writeByte(void * _self,uchar b) {
  printf("%c",b);
}


/*
 *	object management and selectors
 */
 
 
/**
  @fn  void close(void*_self);
  @details close the stream
*/
void close(void*_self){
  struct OutputStreamClass * _class = (struct OutputStreamClass *)classOf(_self); 
  
  assert(_class -> close);
	
	_class -> close(_self);
}

//Flushes this output stream and forces any buffered output bytes to be written out. The general contract of flush is that calling it is an indication that, if any bytes previously written have been buffered by the implementation of the output stream, such bytes should immediately be written to their intended destination.
//
//The flush method of OutStream does nothing. 
void flush(void * _self) {
  struct OutputStreamClass * _class =(struct OutputStreamClass *)classOf(_self); 
  
  assert(_class -> flush);
	
	_class -> flush(_self);
}

//public void write(byte[] b)
//           throws IOException
//
//    Writes b.length bytes from the specified byte array to this output stream.
void write(void * _self,const char * b) {
  struct OutputStreamClass * _class =(struct OutputStreamClass *)classOf(_self); 
  
  assert(_class -> write);
	
	_class -> write(_self,b);
}

//Writes the specified byte to this output stream. The general contract for write is that one byte is written to the output stream.
void writeByte(void * _self,uchar b) {
  struct OutputStreamClass * _class =(struct OutputStreamClass *)classOf(_self); 
  
  assert(_class -> writeByte);
	
	_class -> writeByte(_self,b);
}


