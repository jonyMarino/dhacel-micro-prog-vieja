#ifndef	OBJECT_H
#define	OBJECT_H




#include <stdarg.h>
#include <stddef.h>
#include <stdtypes.h>
#include <stdio.h>

void deleteAndNil (void ** _self);
int Object_differ (const void * _self, const void * b);                                 
void _delete (void * _self);

#endif
