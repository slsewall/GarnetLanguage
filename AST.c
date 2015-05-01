#include "AST.h"

//////////////////////////////////
// I/O Interface

ostream&
operator <<(ostream &o, const ASTExpression &a)
{
  // Concatenates an ASTExpression object to an ostream.
  //
  // This works by delegating the job back to the ASTExpression node.


  a.ASTPrint(o);
  return o;
}


//////////////////////////////////
// ASTExpression methods
int
ASTExpression::getline()
{
  return linenum;
}

int
ASTExpression::getchar()
{
  return charnum;
}

//////////////////////////////////
// ASTInteger methods

ASTInteger::ASTInteger(int i, int line_init, int char_init)
:ASTExpression(line_init,char_init),value(i)
{
  // The ASTInteger constructor uses the superclass initializer
  // to initialize its superclass linenum and charnum.  It
  // also initializes value from its argument.
  // No other code is necessary.
  value = i;
}

void
ASTInteger::ASTPrint(ostream &o) const {
  // To print an integer ASTExpression, print the integer
  // value associated with it.
  o << value;
}

ASTString::ASTString(string i, int line_init, int char_init)
:ASTExpression(line_init,char_init),value(i)
{
  // The ASTInteger constructor uses the superclass initializer
  // to initialize its superclass linenum and charnum.  It
  // also initializes value from its argument.
  // No other code is necessary.
  value = i;
}

void
ASTString::ASTPrint(ostream &o) const {
  // To print an integer ASTExpression, print the integer
  // value associated with it.
  o << value;
}
//GarnetPtr
//ASTInteger::eval() const
//{
//  // To evaluate an integer, make a garnet integer object.
//  // An integer is a primitive, so I call the make_prim
//  return make_object_from_prim(value);
//}


void ASTFloat::ASTPrint(ostream &o) const {
  o << value;
}

void ASTIdentifier::ASTPrint(ostream &o) const {
  
  o << id;
}

void ASTExpressionListSimple::ASTPrint(ostream &o) const
{
  o << "(" << *expr << ")";
}

void ASTExpressionListCompound::ASTPrint(ostream &o) const
{
  o << "(" << *expr_list << "; \n" << *expr << ")";
}

//////////////////////////////////
// ASTArgList methods

ASTExpression *
ASTArgList::head() {
  cerr << "Don't know how to take the head of this arglist" << endl;
  abort();
}

ASTExpression *
ASTArgList::tail() {
  cerr << "Don't know how to take the tail of this arglist" << endl;
  abort();
}



//////////////////////////////////
// ASTArgListSimple methods


ASTExpression *
ASTArgListSimple::head()
{
  return expr;
}

void
ASTArgListSimple::ASTPrint(ostream &o) const
{
  o << *expr;
}


//////////////////////////////////
// ASTArgListCompound methods

ASTExpression *
ASTArgListCompound::head()
{
  return list_head;
}

ASTExpression *
ASTArgListCompound::tail()
{
  return list_tail;
}

void ASTArgListCompound::ASTPrint(ostream &o) const
{
  o << *list_head<< ", " << *list_tail;
}



void ASTArgListEmpty::ASTPrint(ostream &o) const
{
  // define appropriately
}

void
ASTMethodCall::ASTPrint(ostream &o) const
{
  o << methodName << "(" << *arglist << ")";
}

GarnetPtr ASTEval()
{
  //evaluate the GarnetPtr in question

}

GarnetPtr ASTInteger::ASTEval()
{
  return make_object_from_prim(value);
}

void
ASTWhile::ASTPrint(ostream &o) const
{
  o << "While " << *condition << " " << " do \n" << *body << "\n end" ;
}

void
ASTFor::ASTPrint(ostream &o) const
{
  o << "for " << *condition << " in "<< *arrayExp << " do \n" << *body << "\n end" ;
}

void
ASTLoop::ASTPrint(ostream &o) const
{
  o << "loop " << "{ \n" << *body << "\n }" ;
}


void
ASTIf::ASTPrint(ostream &o) const
{
  o << "if(" << *condition << ") do" << "\n" << *body << "\n end" << *hacky ;
}

void
ASTelsif::ASTPrint(ostream &o) const
{
  o << "elsif(" << *condition << ") then" << "{ \n" << *body << "\n}" << *hacky;
}


void
ASTelse::ASTPrint(ostream &o) const
{
  o << "else" << "{ \n" << *body << "\n}" ;
}


void
ASTIfNo::ASTPrint(ostream &o) const
{
  o << "end" ;
}