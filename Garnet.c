#define __GARNET_CC
#include "garnet.h"


// GarnetObject class objects.
// need these so they can be referenced globally within this file.
GarnetObject classObject;
GarnetObject objectObject;
GarnetObject methodObject;
GarnetObject floatObject;
GarnetObject integerObject;
GarnetObject stringObject;
GarnetObject arrayObject;
GarnetObject trueClassObject;
GarnetObject falseClassObject;
GarnetObject nilClassObject;
GarnetObject trueObject;
GarnetObject falseObject;
GarnetObject nilObject;

void GarnetObject::do_init()
{
	create_environment();

	allocateBuiltInClasses();



}

void allocateBuiltInClasses()
{
  // allocate GarnetObject class objects for each of the built in classes
  // String, method, integer, float, object, truecClass, falseClass, NilClass

  //temp variables for class slots
  vector<GarnetPtr> * classSlotsVector = new vector<GarnetPtr>();
  GarnetPtr classSlots = GarnetPtr(classSlotsVector);
  //GarnetObject integerObject = GarnetObject(classSlots);

  
  

/*
5. Allocate a vector for the class slotnames, namely:
Superclass, Method_names, Method_objects, Slot_names,
Slot_initializers, Class_name, Sealed.
*/


    vector<GarnetPtr> *class_SlotNames = new vector<GarnetPtr>();
    vector<GarnetPtr> *class_SlotInits = new vector<GarnetPtr>();


    bind_slot(class_SlotNames, class_SlotInits, "SUPERCLASS", NilObject);
    bind_slot(class_SlotNames, class_SlotInits, "METHOD_NAMES", NilObject);
    bind_slot(class_SlotNames, class_SlotInits, "METHOD_OBJECTS", NilObject);
    bind_slot(class_SlotNames, class_SlotInits, "SLOT_NAMES", NilObject);
    bind_slot(class_SlotNames, class_SlotInits, "SLOT_INITS", NilObject);
    bind_slot(class_SlotNames, class_SlotInits, "CLASSNAME", NilObject);
    bind_slot(class_SlotNames, class_SlotInits, "SEALED", NilObject);


    vector<GarnetPtr> *classMethodNames = new vector<GarnetPtr>();
    vector<GarnetPtr> *classMethodObjects = new vector<GarnetPtr>();
    bind_method(classMethodNames, classMethodObjects, "to_s", classToString);
    bind_method(classMethodNames, classMethodObjects, "superclass", classSuperclass);

  TrueClass= GarnetPtr(&trueObject);
  FalseClass= GarnetPtr(&falseObject);
  NilClass= GarnetPtr(&nilObject);

  //3. Allocate the NilObject (a GarnetPtr whose u value is a GarnetObject
  //belonging to class NilClass, and having no slots.

  // not to be confused witht he nilClass
  NilObject = GarnetPtr(&nilObject);
  nilObject.myclass = NilClass;

  TrueObject = GarnetPtr(&trueObject);
  trueObject.myclass = TrueClass;

  FalseObject = GarnetPtr(&falseObject);
  falseObject.myclass = FalseClass;

  //Class 7
     // Class slots
  Class = GarnetPtr(&classObject);
  Object = GarnetPtr(&objectObject);
  classObject.myclass = Object;
  classObject.slotsvec = make_class_slots(
                                Object,
                                classMethodNames,
                                classMethodObjects,
                                class_SlotNames,
                                class_SlotInits,
                                std::string("Class"),
                                FalseObject);
 // GarnetObject *ClassBuiltIn = new GarnetObject(Integer, classSlots);

  //Obejct Slots 8
    // Object slots
  vector<GarnetPtr> *objectMethodNames = new vector<GarnetPtr>();
  vector<GarnetPtr> *objectMethodObjects = new vector<GarnetPtr>();
  vector<GarnetPtr> *objectSlotNames = new vector<GarnetPtr>();
  vector<GarnetPtr> *objectSlotInits = new vector<GarnetPtr>();
  bind_method(objectMethodNames, objectMethodObjects, "to_s", objectToString);
  bind_method(objectMethodNames, objectMethodObjects, "print", objectPrint);
    objectObject.slotsvec = make_class_slots(
                                NilObject,
                                objectMethodNames,
                                objectMethodObjects,
                                objectSlotNames,
                                objectSlotInits,
                                std::string("Object"),
                                FalseObject);


    //String 

  String= GarnetPtr(&stringObject);
  Array= GarnetPtr(&arrayObject);
     stringObject.myclass = Class;
    vector<GarnetPtr> *stringMnames = new vector<GarnetPtr>();
    vector<GarnetPtr> *stringMobjs = new vector<GarnetPtr>();
    vector<GarnetPtr> *stringSnames = new vector<GarnetPtr>();
    vector<GarnetPtr> *stringSinits = new vector<GarnetPtr>();
    bind_slot(stringSnames, stringSinits, "$rep", make_object_from_prim(""));
    // help from connor
    bind_method(stringMnames, stringMobjs, "to_s", objectToString);
    stringObject.slotsvec = make_class_slots(
                                Object,
                                stringMnames,
                                stringMobjs,
                                stringSnames,
                                stringSinits,
                                std::string("String"),
                                TrueObject);

    //Integer
     Integer = GarnetPtr(&integerObject);
  
     integerObject.myclass = Class;
    vector<GarnetPtr> *integerMnames = new vector<GarnetPtr>();
    vector<GarnetPtr> *integerMobjs = new vector<GarnetPtr>();
    vector<GarnetPtr> *integerSnames = new vector<GarnetPtr>();
    vector<GarnetPtr> *integerSinits = new vector<GarnetPtr>();
    bind_slot(integerSnames, integerSinits, "$rep", make_object_from_prim(0));
    // help from connor
    bind_method(integerMnames, integerMobjs, "to_s", objectToString);
    stringObject.slotsvec = make_class_slots(
                                Object,
                                integerMnames,
                               integerMobjs,
                               integerSnames,
                                integerSinits,
                                std::string("Integer"),
                                TrueObject);
    //floating point value

  Float= GarnetPtr(&floatObject);
     floatObject.myclass = Class;
    vector<GarnetPtr> *floatMnames = new vector<GarnetPtr>();
    vector<GarnetPtr> *floatMobjs = new vector<GarnetPtr>();
    vector<GarnetPtr> *floatSnames = new vector<GarnetPtr>();
    vector<GarnetPtr> *floatSinits = new vector<GarnetPtr>();
    bind_slot(floatSnames, floatSinits, "$rep", make_object_from_prim(0.0));
    // help from connor
    bind_method(floatMnames, floatMobjs, "to_s", objectToString);
    stringObject.slotsvec = make_class_slots(
                                Object,
                                floatMnames,
                               floatMobjs,
                               floatSnames,
                                floatSinits,
                                std::string("float"),
                                TrueObject);

    //Method
    Method= GarnetPtr(&methodObject);
     methodObject.myclass = Class;
    vector<GarnetPtr> *methodMnames = new vector<GarnetPtr>();
    vector<GarnetPtr> *methodMobjs = new vector<GarnetPtr>();
    vector<GarnetPtr> *methodSnames = new vector<GarnetPtr>();
    vector<GarnetPtr> *methodSinits = new vector<GarnetPtr>();
    bind_slot(methodSnames, methodSinits, "$rep", NilObject);
    // help from connor
    bind_method(methodMnames, methodMobjs, "to_s", objectToString);
    stringObject.slotsvec = make_class_slots(
                                Object,
                                methodMnames,
                               methodMobjs,
                               methodSnames,
                                methodSinits,
                                std::string("method"),
                                TrueObject);

    // UnComment to print the object hierarchy
    // Class.briefprint(std::cout);
   //  std::cout << std::endl;
    // Object.briefprint(std::cout);
    // std::cout << std::endl;
    // Integer.briefprint(std::cout);
    // std::cout << std::endl;
    // Float.briefprint(std::cout);
   //  std::cout << std::endl;
    // Array.briefprint(std::cout);
    // std::cout << std::endl;
    // Method.briefprint(std::cout);
   //  std::cout << std::endl;
   //  TrueClass.briefprint(std::cout);
    // std::cout << std::endl;
    // FalseClass.briefprint(std::cout);
    // std::cout << std::endl;
    // NilClass.briefprint(std::cout);
    // std::cout << std::endl;
    // TrueObject.briefprint(std::cout);
    // std::cout << std::endl;
    // FalseObject.briefprint(std::cout);
    // std::cout << std::endl;
    // NilObject.briefprint(std::cout);
    // std::cout << std::endl;
}

void push_environment(Frame *env)
{
	envStack->push(*env);
}
void create_environment()
{
	Frame *local_frame = new Frame();
	envStack = new environmentStack();

	envStack->push(*local_frame);
}

GarnetPtr::GarnetPtr(){
  this->d = GarnetPtrDiscriminator::d_none;
}

GarnetPtr::GarnetPtr(const GarnetPtr &init){
	CopyAll(init);
}
GarnetPtr &GarnetPtr::operator=(const GarnetPtr &rhs){
	CopyAll(rhs);
	return *this;
}

const GarnetObject &GarnetObject::operator=(const GarnetObject& rhs) 
{
  GarnetObject *copiedObject = new GarnetObject();
  GarnetPtr yourClass = rhs.getclass();
  copiedObject->myclass = yourClass;

  vector<GarnetPtr> *vecInit = new vector<GarnetPtr>();
  
  for(int i = 0; i < rhs.getslot(0).vp()->size(); i++)
  {
    GarnetPtr toPush = rhs.getslot(i);
    vecInit->push_back(toPush);

    // todo: vector names
  }
  copiedObject->slotsvec = GarnetPtr(vecInit);
  return *copiedObject;
}
GarnetPtr make_class_slots(GarnetPtr super,
                 GarnetPtr mnames,
                 GarnetPtr mobjs,
                 GarnetPtr slotnames,
                 GarnetPtr slotinits,
                 std::string classname,
                 GarnetPtr sealed){
   vector<GarnetPtr> *v = new vector<GarnetPtr>();
   v->push_back(super);
   v->push_back(mnames);
   v->push_back(mobjs);
   v->push_back(slotnames);
   v->push_back(slotinits);
   v->push_back(GarnetPtr(&classname));
   v->push_back(sealed);
   return GarnetPtr(v);
}

void bind_slot(vector <GarnetPtr> *snames,
      vector <GarnetPtr> *sobjs,
      string slot_name,
      GarnetPtr slot_initializer)
{
  snames->push_back(GarnetPtr(&slot_name));
  sobjs->push_back(slot_initializer);
}
void bind_method(vector <GarnetPtr> *mnames,
        vector <GarnetPtr> *mobjs,
        string method_name,
        MethodPtr method)
{
    mnames->push_back(GarnetPtr(&method_name));
    mobjs->push_back(GarnetPtr(method));
}
GarnetPtr GarnetObject::getslot(int n) const
{

  return this->slotsvec.vp()->at(n);
}

// This function assigns a value to a slot in an object
  // referred to by the GarnetPtr obj
/*void GarnetObject::bind_slot(GarnetPtr obj, string slot_name, GarnetPtr slot_initializer)
{
    this->slotnames->push_back(slot_name);
  //  this->slotsvec.vp()
}*/

//copy all function
void GarnetPtr::CopyAll(const GarnetPtr &other ){
	this->d = other.disc();
	if(other.is_undefined() == false)
	{
		if(other.is_object())				// is it an object?
		{
			this->u.objptr = other.gobjp();
		}
		else if(other.is_vector())			// is it a vector?
		{
			this->u.vecptr = other.vp();
		}
		else if(other.is_prim_method())	//is it a primitive method?
		{
			this->u.methptr = other.mp();
		}
		else if(other.is_string())		// is it a primitive string?
		{
			this->u.strptr = other.sp();
		}
		else if(other.is_int())		// is it a primitive int?
		{
			this->u.intval = other.iv();
		}
		else if(other.is_float())		// is it a primitive float?
		{
			this->u.floatval = other.fv();
		}
		else if(other.is_methDef())		// is it an ASTMethodDefinition pointer
		{
			this->u.methoddefptr = other.mdp();
		}
	}
}
//GarnetPtr Constructors:
  GarnetPtr::GarnetPtr(GarnetObject *arg)
  {
  	this->d = GarnetPtrDiscriminator::d_objptr;
  	this->u.objptr = arg;
  }
  GarnetPtr::GarnetPtr(string *arg)
  {
  	this->d = GarnetPtrDiscriminator::d_strptr;
  	this->u.strptr = arg;
  }
  GarnetPtr::GarnetPtr(int arg)
  {
  	this->d = GarnetPtrDiscriminator::d_intval;
  	this->u.intval = arg;
  }
  GarnetPtr::GarnetPtr(double arg)
  {
  	this->d = GarnetPtrDiscriminator::d_floatval;
  	this->u.floatval = arg;
  }
  GarnetPtr::GarnetPtr(MethodPtr arg)
  {
  	this->d = GarnetPtrDiscriminator::d_methptr;
  	this->u.methptr = arg;
  }
  GarnetPtr::GarnetPtr(vector<GarnetPtr> *arg)
  {
  	this->d = GarnetPtrDiscriminator::d_vecptr;
  	this->u.vecptr = arg;
  }
  GarnetPtr::GarnetPtr(void *arg)
  {
  	this->d = GarnetPtrDiscriminator::d_methoddefptr;
  	this->u.methoddefptr = arg;
  }



  GarnetObject *GarnetPtr::gobjp() const   // get an object pointer
  {
  		return this->u.objptr;
  }
  string *GarnetPtr::sp() const            // get a string pointer
  {
  		return this->u.strptr;
  }
  int GarnetPtr::iv() const                // get a primitive integer
  {
  		return this->u.intval;
  }
  double GarnetPtr::fv() const             	// get a primitive float
  {
  		return this->u.floatval;
  }
  MethodPtr GarnetPtr::mp() const         				// get a primitive method
  {
  		return this->u.methptr;
  }
  vector<GarnetPtr> *GarnetPtr::vp() const  // get a vector pointer
  {
  		return this->u.vecptr;
  }
  void *GarnetPtr::mdp() const              // get an ASTMethodDefinition pointer
  {
  		return this->u.methoddefptr;
  }

  // Tests on discriminant for various types
  bool GarnetPtr::is_undefined() const  // comparse against d_none
  {
  	if(this->d == GarnetPtrDiscriminator::d_none)
  	{
  		return true;
  	}
  	else{
  		return false;
  	}
  }
  bool GarnetPtr::is_object() const
  {
  	if(this->d == GarnetPtrDiscriminator::d_objptr)
  	{
  		return true;
  	}
  	else{
  		return false;
  	}
  }
  bool GarnetPtr::is_prim_method() const
  {
	if(this->d == GarnetPtrDiscriminator::d_methptr)
  	{
  		return true;
  	}
  	else{
  		return false;
  	}
  }
  bool GarnetPtr::is_vector() const
  {
  	if(this->d == GarnetPtrDiscriminator::d_vecptr)
  	{
  		return true;
  	}
  	else{
  		return false;
  	}
  }

  bool GarnetPtr::is_string() const
  {
  	if(this->d == GarnetPtrDiscriminator::d_strptr)
  	{
  		return true;
  	}
  	else{
  		return false;
  	}
  }
  bool GarnetPtr::is_int() const
  {
 	if(this->d == GarnetPtrDiscriminator::d_intval)
  	{
  		return true;
  	}
  	else{
  		return false;
  	}
  }
  bool GarnetPtr::is_float() const
  {
 	if(this->d == GarnetPtrDiscriminator::d_floatval)
  	{
  		return true;
  	}
  	else{
  		return false;
  	}
  }
  bool GarnetPtr::is_methDef() const
  {
  	if(this->d == GarnetPtrDiscriminator::d_methoddefptr)
  	{
  		return true;
  	}
  	else{
  		return false;
  	}
  }

// part 2: GarnetObject Stuffs

  GarnetObject::GarnetObject(GarnetPtr cls, GarnetPtr slots)
  {
      myclass = cls;
      slotsvec = slots;
      // how the fuck do you name the slots!?
  }
  GarnetObject::GarnetObject()
  {
    myclass = GarnetPtr();
    slotsvec = GarnetPtr();
    slotnames = vector<string>();
  }
GarnetPtr make_object_from_prim(int val)
{
   GarnetPtr *ptr = new GarnetPtr(&val);
    vector<GarnetPtr> *v = new vector<GarnetPtr>();
    GarnetPtr *vec = new GarnetPtr(v);
    GarnetObject *obj = new GarnetObject(ptr, vec);
    return GarnetPtr(obj);
}

GarnetPtr make_object_from_prim(string val)
{
    // creates a GarnetPtr pointing to a String object
// That object will have class String and a slots vector containing
// a single value with is a primitive GarnetPtr string object
    GarnetPtr *ptr = new GarnetPtr(&val);
    vector<GarnetPtr> *v = new vector<GarnetPtr>();
    GarnetPtr *vec = new GarnetPtr(v);
    GarnetObject *obj = new GarnetObject(ptr, vec);
    return GarnetPtr(obj);
}

GarnetPtr make_object_from_prim(MethodPtr val)
{
  GarnetPtr *ptr = new GarnetPtr(&val);
  vector<GarnetPtr> *v = new vector<GarnetPtr>();
   GarnetPtr *vec = new GarnetPtr(v);
   GarnetObject *obj = new GarnetObject(ptr, vec);
   return GarnetPtr(obj);
}
GarnetPtr make_object_from_prim(void *val)
{
  GarnetPtr *ptr = new GarnetPtr(&val);
  vector<GarnetPtr> *v = new vector<GarnetPtr>();
   GarnetPtr *vec = new GarnetPtr(v);
   GarnetObject *obj = new GarnetObject(ptr, vec);
   return GarnetPtr(obj);
}
GarnetPtr make_object_from_prim(vector<GarnetPtr> *val)
{
  GarnetPtr *ptr = new GarnetPtr(&val);
  vector<GarnetPtr> *v = new vector<GarnetPtr>();
   GarnetPtr *vec = new GarnetPtr(v);
   GarnetObject *obj = new GarnetObject(ptr, vec);
   return GarnetPtr(obj);
}
GarnetPtr make_object_from_prim(double val)
{
  GarnetPtr *ptr = new GarnetPtr(&val);
  vector<GarnetPtr> *v = new vector<GarnetPtr>();
   GarnetPtr *vec = new GarnetPtr(v);
   GarnetObject *obj = new GarnetObject(ptr, vec);
   return GarnetPtr(obj);
}

void GarnetPtr::print(ostream &o) const
{
  switch(d) {
        case d_objptr:
            u.objptr->print(o);
            break;
        case d_intval:
            o <<"(GarnetPtr: intval=" << u.intval << ")"<< std::endl;;
            break;
        case d_floatval:
            o << "(GarnetPtr: floatval=" << u.floatval << ")" << std::endl;;
            break;
        case d_methptr:
            o << "(GarnetPtr: methodptr=" << u.methptr << ")" << std::endl;
            break;
        case d_vecptr:
            o << "(GarnetPtr: vecptr=" << u.vecptr << ")" << std::endl;
            break;
        case d_methoddefptr:
            o << "(GarnetPtr: methoddefptr=" << u.methoddefptr << ")" << std::endl;
            break;
        case d_strptr:
            o << "(GarnetPtr: strptr=" << u.strptr <<")" << std::endl;;
            break;
        case d_none:
             o << "(GarnetPtr: None)" << std::endl;
            break;
        default:
           o << "ERROR: GarnetPtr type is undefined" << std::endl;
            break;
    }
}

ostream&
operator<<(ostream &o, const GarnetPtr &g)
{
    g.briefprint(std::cout);
}

void GarnetPtr::briefprint(ostream &o) const
{
  switch(d) {
        case d_objptr:
            u.objptr->briefprint(o);
            break;
        case d_intval:
            o << u.intval;
            break;
        case d_floatval:
            o << u.floatval;
            break;
        case d_strptr:
            o << u.strptr;
            break;
        case d_none:
            o << "NONE TYPE";
            break;
        default:
            o << "NOT implemented";
            break;
    }
}

void GarnetObject::print(ostream &o) const
{
  myclass.print(o);
}
void GarnetObject::briefprint(ostream &o) const
{
  myclass.briefprint(o);
}