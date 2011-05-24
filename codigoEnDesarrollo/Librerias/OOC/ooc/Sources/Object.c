#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Object.h"

/*
 *	Object
 */

void Object_ctor (void * _self, va_list * app)
{
	//return _self;
}

void Object_dtor (void * _self)
{
	//return _self;
}

int Object_differ (const void * _self, const void * b)
{
	return _self != b;
}

int Object_puto (const void * _self, struct OutStream * os)
{	const struct Class * _class = (const struct Class *)classOf(_self);
 // itostr
  char dir[10];

	write(os, _class -> name);
	write(os, " at ");
	sprintf(dir,"%x",_self);
	write(os,dir);
	return 0;
}


const void * classOf (const void * _self)
{	const struct Object * self = (const struct Object *)_self;

	assert(self && self -> _class);
	return self -> _class;
}

uchar instanceOf(const void * _self,const struct Class * _class){
  
  if (_self)
  { 
    void * myClass = classOf(_self);
    if (_class != &Object)
      while (myClass != _class)
        if (myClass != &Object)
          myClass = super(myClass);
        else
          return FALSE;
      return TRUE;
  }
  return FALSE; //No deberia llegar a aqui
}

size_t sizeOf (const void * _self)
{	const struct Class * _class = (const struct Class *) classOf(_self);

	return _class -> size;
}

/*
 *	Class
 */

static void * Class_ctor (void * _self, va_list * app)
{	struct Class * self = (struct Class *)_self;
	const size_t offset = offsetof(struct Class, ctor);

	self -> name = va_arg(* app, char *);
	self -> superPointer = va_arg(* app, struct Class *);
	self -> size = va_arg(* app, size_t);

	assert(self -> superPointer);

	memcpy((char *) self + offset, (char *) self -> superPointer
					+ offset, sizeOf(self -> superPointer) - offset);
{
	typedef void (* voidf) ();	/* generic function pointer */
	voidf selector;
	va_list ap = * app;

	while ((selector = va_arg(ap, voidf)))
	{	voidf method = va_arg(ap, voidf);

		if (selector == (voidf) ctor)
			* (voidf *) & self -> ctor = method;
		else if (selector == (voidf) dtor)
			* (voidf *) & self -> dtor = method;
		else if (selector == (voidf) differ)
			* (voidf *) & self -> differ = method;
		else if (selector == (voidf) puto)
			* (voidf *) & self -> puto = method;

	}

	return self;
}}

static void * Class_dtor (void * _self)
{	struct Class * self = (struct Class *)_self;

	fprintf(stderr, "%s: cannot destroy _class\n", self->name);
	return 0;
}

const void * super (const void * _self)
{	const struct Class * self = (const struct Class *)_self;

	assert(self && self -> superPointer);
	return self -> superPointer;
}

/*
 *	initialization
 */

const struct Class Object  = 
	{ { &Class },
	  "Object", &Object, sizeof(struct Object),
	  Object_ctor, Object_dtor, Object_differ, Object_puto
	};
	
const struct Class Class  = 
	{ { &Class },
	  "Object", &Object, sizeof(struct Object),
	  Object_ctor, Object_dtor, Object_differ, Object_puto
	};
/*	
static const struct Class object [] = {
	{ { object + 1 },
	  "Object", object, sizeof(struct Object),
	  Object_ctor, Object_dtor, Object_differ, Object_puto
	},
	
	{ { object + 1 },
	  "Class", object, sizeof(struct Class),
	  Class_ctor, Class_dtor, Object_differ, Object_puto
	}
};
  */
//const void * Object = object;
//const void * Class = object + 1;

/*
 *	object management and selectors
 */

void * _new (const void * __class, ...)
{	const struct Class * _class = (const struct Class *)__class;
	struct Object * object;
	va_list ap;

	assert(_class && _class -> size);
	object = (struct Object *)malloc(_class -> size);
	assert(object);
	object -> _class = _class;
	va_start(ap, __class);
	ctor(object, & ap);
	va_end(ap);
	return object;
}

void * newAlloced (void * _self,const void * __class, ...)
{	const struct Class * _class =(const struct Class *) __class;
	struct Object * object;
	va_list ap;

	assert(_class);
	object = (struct Object *)_self;
	assert(object);
	object -> _class = _class;
	va_start(ap, __class);
	/*object = */ctor(object, & ap);
	va_end(ap);
	return object;
}



void _delete (void * _self)
{
	if (_self){
	  
		dtor(_self);
	  free(_self);
	}
}

void deleteAndNil (void ** _self)
{
	if (_self && *_self){	  
		dtor(*_self);
	  free(*_self);
	}
	*_self = NULL;
}

void deleteAlloced (void * _self)
{
	if (_self)
		dtor(_self);
}

void ctor (void * _self, va_list * app)
{	const struct Class * _class = (const struct Class *)classOf(_self);

	if(_class -> ctor)
	  _class -> ctor(_self, app);
}

void super_ctor (const void * __class,
				void * _self, va_list * app)
{	const struct Class * superclass = (const struct Class *)super(__class);

	if(_self && superclass -> ctor)
	  superclass -> ctor(_self, app);
}

void dtor (void * _self)
{	const struct Class * _class = (const struct Class *)classOf(_self);

	if(_class -> dtor)
	  _class -> dtor(_self);
}

void super_dtor (const void * __class, void * _self)
{	const struct Class * superclass = (const struct Class *)super(__class);

	if(_self && superclass -> dtor)
	  superclass -> dtor(_self);
}

int differ (const void * _self, const void * b)
{	const struct Class * _class = (const struct Class *)classOf(_self);

	assert(_class -> differ);
	return _class -> differ(_self, b);
}

int super_differ (const void * __class, const void * _self, const void * b)
{	const struct Class * superclass = (const struct Class *)super(__class);

	assert(_self && superclass -> differ);
	return superclass -> differ(_self, b);
}

int puto (const void * _self, struct OutStream * os)
{	const struct Class * _class = (const struct Class *)classOf(_self);

	assert(_class -> puto);
	return _class -> puto(_self, os);
}

int super_puto (const void * __class, const void * _self, struct OutStream * fp)
{	const struct Class * superclass = (const struct Class *) super(__class);

	assert(_self && superclass -> puto);
	return superclass -> puto(_self, fp);
}

