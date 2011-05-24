#ifndef _OUTPUTSTREAM_HPP
#define _OUTPUTSTREAM_HPP


class OutputStream
{
 public:
  OutputStream();
  //Writes the specified byte to this output stream. The general contract for write is that one byte is written to the output stream.
 
 /**
  @fn  void close(void*_self);
  @details close the stream
  */
  virtual void close(void);
  
  //Flushes this output stream and forces any buffered output bytes to be written out. The general contract of flush is that calling it is an indication that, if any bytes previously written have been buffered by the implementation of the output stream, such bytes should immediately be written to their intended destination.
  //
  //If the intended destination of this stream is an abstraction provided by the underlying operating system, for example a file, then flushing the stream guarantees only that bytes previously written to the stream are passed to the operating system for writing; it does not guarantee that they are actually written to a physical device such as a disk drive.
  //
  //The flush method of OutputStream does nothing. 
  virtual void flush(void);
  
  //public void write(byte[] b)
  //           throws IOException
  //
  virtual void write(const char * b);
  
  //    Writes b.length bytes from the specified byte array to this output stream.
  virtual void write(unsigned char b);
};

#endif
