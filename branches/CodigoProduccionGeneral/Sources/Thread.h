#ifndef _THREAD_H
#define _THREAD_H


void pthread_create(void * _self,void * attr,void*(*pf)(void*),void* args);

#endif