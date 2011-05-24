#ifndef	OBJECT_H
#define	OBJECT_H



#ifdef __cplusplus
extern "C"
{
#endif

#include <stdarg.h>
#include <stddef.h>
#include <stdtypes.h>
#include <stdio.h>
#include "Object_r.h"

//extern const void * Object;		/* new(&Object); */
extern const struct Class Object;

void * _new (const void * _class, ...);
/* crea una instancia de classOf en el espacio de memoria _self*/
void * newAlloced (void * _self,const void * classOf, ...);
void _delete (void * self);
void deleteAlloced (void * _self);
void deleteAndNil (void ** _self);
                                 
const void * classOf (const void * self);
uchar instanceOf(const void * _self,const struct Class * _class);
size_t sizeOf (const void * self);

void ctor (void * self, va_list * app);
void dtor (void * self);
unsigned char differ (const void * _self, const void * b);
int puto (const void * _self, struct OutputStream * os);

//extern const void * Class;	/* new(&Class, "name", superPointer, size
//										sel, meth, ... 0); */
extern const struct Class Class;

const void * super (const void * self);	/* _class' superclass */

#ifdef __cplusplus
}
#endif

#endif
