#ifndef OBJECT_R
#define OBJECT_R

/*C++ 
class Object:public Object
{
 public:
  virtual int differ (const void * b);
  virtual int puto (OutputStream * os);

}
*/
struct Object {
	const struct Class * class;	/* object's description */
};

void * Object_ctor (void * _self, va_list * app);
void * Object_dtor (void * _self);
int Object_differ (const void * _self, const void * b);
int Object_puto (const void * _self, struct OutputStream * os);

struct Class {
	const struct Object _;			/* class' description */
	const char * name;				/* class' name */
	const struct Class * super;		/* class' super class */
	size_t size;					/* class' object's size */
	void * (* ctor) (void * self, va_list * app);
	void * (* dtor) (void * self);
	int (* differ) (const void * self, const void * b);
	int (* puto) (const void * self, struct OutputStream * os);
};

void * super_ctor (const void * class, void * self, va_list * app);
void * super_dtor (const void * class, void * self);
int super_differ (const void * class, const void * self, const void * b);
int super_puto (const void * class, const void * self, FILE * fp);


#define CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto)\
  NULL/*&description*/,          \
  "",/*#name,*/                 \
  &super,                \
  sizeof(struct name),  \
  contructor,           \
  destructor,           \
  differ,               \
  puto                 
//#define CLASS_NEW(description,contructor,destructor,differ,puto)\


#endif