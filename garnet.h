#ifndef __GARNET__H_
#define __GARNET__H_


//
// Garnet.h header file
//
// Defines classes GarnetPtr and GarnetObject
// Defines types GarnetPtrDiscriminator, MethodPtr
//

#include <iostream>
#include <strstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <cassert>

#include "operations.h"
extern int Linenum;
extern int Charnum;

using namespace std;

// Declarations of these classes here allow me to use
// pointers to them in class declarations that follow.
class GarnetObject;   
class GarnetPtr;      

extern GarnetPtr MostRecentResult;    //probably not nessessary
extern GarnetPtr CurrentClass;

// A MethodPtr is a pointer to a void function of no arguments
// Such a function must grab its receiver and arguments from the
// stack and must place a return value on the stack.
typedef void (*MethodPtr)();

// The GarnetPtrDiscriminator class is used as the discriminant
// in the GarnetPtr class.  It lets me tell which kind of element
// is in the union field of the GarnetPtr.
typedef enum { d_none,
	       d_objptr,
	       d_strptr,
	       d_intval,
	       d_floatval,
	       d_methptr,
	       d_vecptr,
	       d_methoddefptr }
GarnetPtrDiscriminator;

typedef union {
  GarnetObject *objptr;
  string       *strptr;
  int          intval;
  double       floatval;
  MethodPtr    methptr;
  vector<GarnetPtr>  *vecptr;
  void         *methoddefptr;
} primitive;


// The GarnetPtr class is used to represent every different kind
// of value in the system.  There are essentially two cases:
// i)  the value is an object (and a pointer to the object is contained
//     in the objptr field of the union), or
// ii) the value is a primitive (and its value is stored directly in
//     the some other field of the union).
// 
class GarnetPtr {
private:
  GarnetPtrDiscriminator d;
  primitive u;
public:
  GarnetPtr();
  GarnetPtr(const GarnetPtr &init);
  GarnetPtr& operator= (const GarnetPtr &rhs);
  void CopyAll(const GarnetPtr &other );


  // We provide constructors for making a value from an
  // object and for making a value from each primitive type
  //
  GarnetPtr(GarnetObject *arg);
  GarnetPtr(string *arg);
  GarnetPtr(int arg);
  GarnetPtr(double arg);
  GarnetPtr(MethodPtr arg);
  GarnetPtr(vector<GarnetPtr> *arg);
  GarnetPtr(void *arg);

  GarnetPtrDiscriminator disc() const { return d; }
  
  bool operator==(const GarnetPtr &rhs) const;
  
  // We also provide methods to extract the value from the GarnetPtr.
  // Each of these methods checks the discriminant and fails if
  // it has the wrong value.
  //
  GarnetObject *gobjp() const;   // get an object pointer
  string *sp() const;            // get a string pointer
  int iv() const;                // get a primitive integer
  double fv() const;             // get a primitive float
  MethodPtr mp() const;          // get a primitive method
  vector<GarnetPtr> *vp() const; // get a vector pointer
  void *mdp() const;             // get an ASTMethodDefinition pointer
  long as_long() const;


  // Tests on discriminant for various types
  bool is_undefined() const;  // comparse against d_none
  bool is_object() const;
  bool is_prim_method() const;
  bool is_vector() const;
  bool is_string() const;
  bool is_int() const;
  bool is_float() const;
  bool is_methDef() const;

  // This primitive lets you print a GarnetPtr thing to an ostream
  void print(ostream &o) const;

  // I used briefprint for brief debugging output
  void briefprint(ostream &o) const;

  // We need a few friends to help construct the class hierarchy.
  // These may need to directly manipulate the contents of the GarnetPtr
  //
  friend class GarnetObject;
  friend void allocate_class_object(GarnetPtr &thisclass);
  friend GarnetPtr make_class_slots(GarnetPtr, GarnetPtr, GarnetPtr, GarnetPtr,
				    GarnetPtr, const char *, GarnetPtr);



};

// Provide ltstr so we can have a map on strings
struct ltstr
{
    bool operator()(const string s1, const string s2) const
    {
        return s1 < s2;
    }
};

// The Frame type is used to store local referencing environments
typedef map<string, GarnetPtr, ltstr> Frame;
//Full stack
typedef stack<Frame> environmentStack;

#ifdef __GARNET_CC
#define extern 
#endif

extern environmentStack * envStack;
// These are used to create and destroy new referencing environments
// Remember we're introducing local referencing environments for
// blocks and all the looping constructs.
void push_environment();
void create_environment();
void pop_environment();

//Allocate classes:
void allocateBuiltInClasses();


// This makes it easy to concatenate GarnetPtr objects onto
// an ostream. Use any default printing approach that lets you see
// what the GarnetPtr contains
//
ostream& operator<<(ostream &o, const GarnetPtr &g);


// The OperandStack is used for evaluation of expressions
// Each argument we pass to a method is pushed onto the stack.
// The return value of the method is left on the stack.
//

// StackError and its subclasses are
// used to trap errors associated with
// the OperandStack.
class StackError{};
class StackOverflow: StackError{};
class StackUnderflow: StackError{};
class StackProtectionError: StackError{};

const int STACK_SIZE=1024;


class ValueStack {
private:
  // The operand stack holds argument values that are being used to
  // compute dependent method call (i.e., ones that are not fully
  // evaluated yet.
  GarnetPtr operand_stack[STACK_SIZE];
  int operand_stack_offset;

  // The frame stack holds indices of the operand stack that correspond
  // to the stack top at the beginning of evaluation of each frame.
  // frames are introduced at the beginnings of blocks that create new
  // (static) scopes.
  //
  // If a scope is exited by an exception, any arguments associated with
  // dependent evaluations in that frame may need to be popped.
  // I'm not sure this can actually occur given the constraints of this
  // simple language.
  int frame_stack[STACK_SIZE];
  int frame_stack_offset;
  
public:
  ValueStack()
    :operand_stack_offset(0),frame_stack_offset(0)
  {}
  GarnetPtr push(GarnetPtr new_object);
  GarnetPtr pop();
  void push_frame();
  void pop_frame();
  bool is_empty();
  
};
#ifdef __GARNET_CC
#define extern 
#endif
extern ValueStack OperandStack;


// These GarnetPtr variables comprise our class hierarchy;
//
#ifdef __GARNET_CC
#define extern 
#endif

extern GarnetPtr Class;
extern GarnetPtr Object;
extern GarnetPtr Method;
extern GarnetPtr Float;
extern GarnetPtr Integer;
extern GarnetPtr String;
extern GarnetPtr Array;
extern GarnetPtr TrueClass;
extern GarnetPtr FalseClass;
extern GarnetPtr NilClass;

// These unique objects are convenient to have around inside the program.
//
extern GarnetPtr NilObject;
extern GarnetPtr FalseObject;
extern GarnetPtr TrueObject;



// The GarnetObject class represents the structure and behavior 
// of all Garnet Object values.
//
class GarnetObject {
private:
 
  
  // The copy constructor of GarnetObject is made private so that
  // we cannot simply make a copy of an object (and point to the
  // same storage by accident).
  //
  GarnetObject(const GarnetObject& init);

public:

   GarnetPtr myclass; //supposed to be private, but yolo
  GarnetPtr slotsvec; //see above

  // do_init creates the class objects and any other objects that
  // must exist in order for a program to start running.
  // This *must* be called before you try to evaluate your
  // abstract syntax tree. (I forgot to initialize and wasted
  // a bit of time tracking that error down.)
  static void do_init();

  GarnetObject();
  // Initialize an object by giving its class and its slot vector
  GarnetObject(GarnetPtr cls, GarnetPtr slots);


  
  vector<string> slotnames;



  // If you don't know about C++ copy constructors, you need to find out
  // about them
  const GarnetObject& operator= (const GarnetObject& rhs); 

  // This gets the GarnetPtr associated with slot number n
  GarnetPtr getslot(int n) const;
  
  // This friend function allows you to get the slot associated with
  // a GarnetPtr value that refers to an object.
  // Make sure you *always* check the discriminant when you're accessing
  // GarnetPtrs with any assumption about the thing to which they refer.
  // The accessor functions should do this for you.
  // It makes debugging *much* easier
  friend GarnetPtr get_slot(GarnetPtr gp, int n);

  // This gets the slot number associated with a slot name
  friend int getslotnum(GarnetPtr gp, string name);

  // This function assigns a value to a slot in an object
  // referred to by the GarnetPtr obj
  friend void bind_slot(GarnetPtr obj,
			string slot_name,
			GarnetPtr slot_initializer);

  // This inds the index of a method name in the method names array
  int find_method_index(string method_name) const;

  // This returns the GarnetPtr of the method that corresponds to
  // name method_name
  GarnetPtr lookup_method(string method_name) const;

  // getter method for functions not in this class that
  // need to access the class of an object
  GarnetPtr getclass() const { return myclass; }



  // Print methods similar to the ones for GarnetPtr
  void print(ostream &o) const;
  void briefprint(ostream &o) const;

};


// ControlObject and its subclass are used to
// implement non-sequential control flow such
// as break, next, and return.
// These constructs are implemented using C++
// exception handling.  (If we didn't have
// C++ exception handling, we'd use setjmp
// and longjmp instead.)
class ControlObject{
 public:
  GarnetPtr value;
  
 ControlObject()
   :value(NilObject)
  {}

  ControlObject(GarnetPtr value_)
    :value(value_)
  {}

  virtual GarnetPtr
    get_value() {return value;}
};

class NextControl:public ControlObject{
 public:
  NextControl() {}
  NextControl(GarnetPtr value_)
    :ControlObject(value_)
  {}
};

class BreakControl:public ControlObject{
 public:
  BreakControl() {}
  BreakControl(GarnetPtr value_)
    :ControlObject(value_)
  {}
};

class ReturnControl:public ControlObject{
 public:
  ReturnControl() {}
  ReturnControl(GarnetPtr value_)
    :ControlObject(value_)
  {}
};






// These functions make full-fledged Garnet Objects
// from their arguments and return a GarnetPtr to
// the constructed object.
GarnetPtr make_object_from_prim(int val);
GarnetPtr make_object_from_prim(string val);
GarnetPtr make_object_from_prim(MethodPtr val);
GarnetPtr make_object_from_prim(void *val);
GarnetPtr make_object_from_prim(vector<GarnetPtr> *val);
GarnetPtr make_object_from_prim(double val);

// make_class_slots takes all the slot values associated with
// a class and wraps them up in a GarnetPtr vector.
GarnetPtr make_class_slots(GarnetPtr super,
                 GarnetPtr mnames,
                 GarnetPtr mobjs,
                 GarnetPtr slotnames,
                 GarnetPtr slotinits,
                 std::string classname,
                 GarnetPtr sealed);

// push_back_object appends an object to a GarnetPtr vector
void
push_back_object(vector <GarnetPtr> *v, GarnetPtr obj);

// push_back_vector appends a vector to a GarnetPtr vector
void
push_back_vector(vector <GarnetPtr> *v, GarnetPtr val);

// is_true checks to see that a GarnetPtr is not the FalseObject, or nil
int is_true(GarnetPtr p);

// insert_binding puts a variable binding into the local environment
GarnetPtr
insert_binding(string, GarnetPtr value);

// lookup_binding finds a variable in the local environment
GarnetPtr
lookup_binding(const string s);


Frame *
local_frame();

// Get the nth slot value in class thisclass
GarnetPtr
get_slot(GarnetPtr thisclass, int n);

// Find the slot number associated with the specified name
// in a GarnetPtr object
int getslotnum(GarnetPtr object, string name);

// Find the primitive string from a GarnetPtr String object
string* getprimstring(GarnetPtr string_object);

// Jam a method name and its associated GarnetPtr value
// into the given vectors in preparation for
// calling make_class_slots
void
bind_method(vector <GarnetPtr> *mnames,
	    vector <GarnetPtr> *mobjs,
	    string method_name,
	    MethodPtr method);

// Jam the specified slot name and initializer into
// the appropriate vectors in preparation for calling
// make_class_slots
void
bind_slot(vector <GarnetPtr> *snames,
	    vector <GarnetPtr> *sobjs,
	    string slot_name,
	    GarnetPtr slot_initializer);

// Jam a slot binding into an extant object.
void
bind_slot(GarnetPtr obj,
	  string slot_name,
	  GarnetPtr slot_initializer);

const int CLASS_SUPERCLASS_INX = 0;
const int CLASS_METHOD_NAMES_INX = 1;
const int CLASS_METHOD_OBJS_INX = 2;
const int CLASS_SLOT_NAMES_INX = 3;
const int CLASS_SLOT_INIT_INX = 4;
const int CLASS_CLASS_NAME_INX = 5;
const int CLASS_INSTANTIABLE_INX = 6;
const int CLASS_NUMSLOTS = 7;

// Implement a method call
GarnetPtr
do_method_call(GarnetPtr);
//TODO: UNCOMMENT THIS LATER

//#include "Operations.h"

#endif // __GARNET_H_