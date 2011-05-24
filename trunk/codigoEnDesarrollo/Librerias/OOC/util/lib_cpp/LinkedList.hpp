#ifndef _LINKED_LIST_HPP
#define _LINKED_LIST_HPP

#include "stddef.h"
#include "nodo.hpp"
#include "Iterator.hpp"
#include "List.hpp"
#include "LinkedList.hpp"

#pragma CODE_SEG  LinkedList_CODE
#pragma DATA_SEG  LinkedList_DATA


class LinkedList:public List
{
 public:
  LinkedList();
  ~LinkedList();
  virtual bool add(void *dato);
  virtual bool aniadir(void *dato); // agerga un nodo al final de la Lista
  /*void addFirst(void * obj);
  void addLast(void * obj);
  void * getFirst();
  void * getLast();
  void * removeFirst();
  void * removeLast();
  */
  unsigned int size(void);
  bool contains(void * dato);
  void * eliminarPrimero(void);
  Iterator * iterator();
  virtual void * get(unsigned int index);
  bool moveOut(void * dato);
  bool isEmpty();
  void clear();
// private:
  unsigned char differ(LinkedList * _l);
  
  class LinkedListIterator:public Iterator{
    public:
      LinkedListIterator();
      LinkedListIterator(LinkedList * list);
      virtual bool hasNext(void);
      virtual void *next(void);
    private:  
      Nodo * nodoNext; 
      friend class LinkedList;
  };
    
  void linkedListIterator(LinkedListIterator * it);
  
 protected:
  Nodo * getPrimerNodo(); 
  void setPrimerNodo(Nodo *); 
 private: 
  Nodo * nodo;
  void deleteNodo(Nodo*tmpNodo,Nodo*tmpNodoAnterior);
  friend class LinkedListIterator;

};




#pragma CODE_SEG  DEFAULT
#pragma DATA_SEG  DEFAULT

#endif