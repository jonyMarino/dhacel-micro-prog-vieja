#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Object.h"

/*
 *	Object
 */


int Object_differ (const void * _self, const void * b)
{
	return _self != b;
}


void _delete (void * _self)
{
	if (_self){
	  
	  free(_self);
	}
}

void deleteAndNil (void ** _self)
{
	if (_self && *_self){	  

	  free(*_self);
	}
	*_self = NULL;
}


