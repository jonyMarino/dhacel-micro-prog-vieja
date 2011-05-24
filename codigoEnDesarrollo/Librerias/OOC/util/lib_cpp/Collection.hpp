#ifndef _COLLECTION_HPP
#define _COLLECTION_HPP

#include "Iterator.hpp"


class Collection
{
 public:
  virtual bool add(void * obj)=0;
  virtual void clear(void)=0;
  virtual bool contains(void * obj)=0;   
  virtual bool isEmpty(void)=0;
  virtual Iterator * iterator(void )=0;
  virtual bool moveOut(void * obj)=0; 
  virtual unsigned int size(void)=0; 
};

  
#endif