#ifndef OBJECT_R_H
#define OBJECT_R_H

#include <stdarg.h>


#ifdef __cplusplus
extern "C"
{
#endif



struct Class;

const struct Object {
	 const struct Class * _class;	/* object's description */
};

void Object_ctor (void * _self, va_list * app);
void Object_dtor (void * _self);
unsigned char Object_differ (const void * _self, const void * b);
int Object_puto (const void * _self, struct OutStream * os);


struct Class {
	struct Object _super;			/* class' description */
	const char * name;				/* class' name */
	const struct Class * superPointer;		/* class' super class */
	size_t size;					/* class' object's size */
	void (* ctor) (void * self, va_list * app);
	void (* dtor) (void * self);
	unsigned char (* differ) (const void * self, const void * b);
	int (* puto) (const void * self, struct OutStream * os);
};

void  super_ctor (const void * _class, void * self, va_list * app);
void  super_dtor (const void * _class, void * self);
int super_differ (const void * _class, const void * self, const void * b);
int super_puto (const void * _class, const void * self, FILE * fp);


#define CLASS_INITIALIZATION(description,name,superPointer,contructor,destructor,differ,puto)\
  NULL/*&description*/,          \
  "",/*#name,*/                 \
  (const struct Class *)&superPointer,                \
  sizeof(struct name),  \
  contructor,           \
  destructor,           \
  differ,               \
  puto                 
//#define CLASS_NEW(description,contructor,destructor,differ,puto)\

#ifdef __cplusplus
}
#endif

#endif