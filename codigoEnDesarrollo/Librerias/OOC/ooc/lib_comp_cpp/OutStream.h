#ifndef _OUTPUTSTREAM_H
#define _OUTPUTSTREAM_H


#include "Object.h"

struct OutputStreamClass{
  struct Class super;
  void (*close)(void*_self);
  void (*flush)(void * _self);
  void (*write)(void * _self,const char * b);
  void (*writeByte)(void * _self,uchar b);
};

extern const struct Class OutputStreamClass;



/*C++ 
_class OutputStream:public Object
{
 public:
  OutputStream();
  void OutputStream_close(void);
  void OutputStream_flush(void);
  void OutputStream_write(uchar * b);
  void OutputStream_writeByte(uchar b);
  void close(void);
  void flush(void);
  void write(uchar * b);

}
*/
struct OutStream{
  struct Object super;
};

extern const struct OutputStreamClass OutStream;

/**
  @fn  void close(void*_self);
  @details close the stream
*/
void close(void*_self);

//Flushes this output stream and forces any buffered output bytes to be written out. The general contract of flush is that calling it is an indication that, if any bytes previously written have been buffered by the implementation of the output stream, such bytes should immediately be written to their intended destination.
//
//If the intended destination of this stream is an abstraction provided by the underlying operating system, for example a file, then flushing the stream guarantees only that bytes previously written to the stream are passed to the operating system for writing; it does not guarantee that they are actually written to a physical device such as a disk drive.
//
//The flush method of OutputStream does nothing. 
void flush(void * _self);
//public void write(byte[] b)
//           throws IOException
//
//    Writes b.length bytes from the specified byte array to this output stream.
void write(void * _self,const char * b);

//Writes the specified byte to this output stream. The general contract for write is that one byte is written to the output stream.
void writeByte(void * _self,uchar b);



// OutputStream_close(void*_self)
void OutputStream_close(void*_self);
// OutputStream_flush(void * _self)
void OutputStream_flush(void * _self);
// OutputStream_write(void * _self,uchar * b);
void OutputStream_write(void * _self,const char * b);
// OutputStream_writeByte(void * _self,uchar b);
void OutputStream_writeByte(void * _self,uchar b);


#define OUTPUT_STREAM_CLASS_INITIALIZATION(description,name,superPointer,contructor,destructor,differ,puto,close,flush,write,writeByte)\
  CLASS_INITIALIZATION(description,name,superPointer,contructor,destructor,differ,puto),\
  close,  \
  flush,  \
  write,  \
  writeByte
    
#endif
