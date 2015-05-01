#ifndef __AST__H_
#define __AST__H_

#include <iostream>
#include "garnet.h"
#include <string>
using namespace std;

struct LineSpec {
  int linenum;
  int charnum;
};

class ASTExpression {

  // This class is the superclass for all Abstract Syntax Tree nodes.
  // Every Garnet AST node is some form of expression that has a value.
  // The only attributes common to every node are that it corresponds
  // to some starting position in the program file.

private:
  int linenum;
  char charnum;

public:
  ASTExpression(int line_init, int char_init)
    :linenum(line_init),charnum(char_init)
      {}

  int getline();
  int getchar();

  virtual void ASTPrint(ostream &o) const =0;

};

class ASTExpressionList: public ASTExpression {
 public:
  ASTExpressionList(int line_init, int char_init)
    :ASTExpression(line_init,char_init)
  {}

  virtual void ASTPrint(ostream &o) const =0;
};


class ASTExpressionListEmpty: public ASTExpressionList {
 public:
  ASTExpressionListEmpty(int line_init, int char_init)
    :ASTExpressionList(line_init, char_init)
  {}
  void ASTPrint(ostream &o) const;
};

class ASTExpressionListSimple: public ASTExpressionList {
 private:
  ASTExpression *expr;
 public:
  ASTExpressionListSimple(ASTExpression *_expr)
    :ASTExpressionList(_expr->getline(), _expr->getchar()), expr(_expr)
  {}
  void ASTPrint(ostream &o) const;
};

class ASTExpressionListCompound: public ASTExpressionList {
 private:
  ASTExpressionList *expr_list;
  ASTExpression *expr;
 public:
  ASTExpressionListCompound(ASTExpressionList *_expr_list,
			    ASTExpression *_expr)
    :ASTExpressionList(_expr_list->getline(),_expr_list->getchar()),
     expr_list(_expr_list),expr(_expr)
  {}
  void ASTPrint(ostream &o) const;
};

class ASTFloat: public ASTExpression {
private:
  double value;
public:
  ASTFloat(double _value, int line_init, int char_init):ASTExpression(line_init, char_init),value(_value)
  {value = _value;}
  void ASTPrint(ostream &o) const;
};

class ASTInteger: public ASTExpression {
  // ASTInteger nodes represent integer literals encountered in
  // input.

  int value;

public:  
  ASTInteger(int i, int line_init, int char_init);
  
  void ASTPrint(ostream &o) const;
};

class ASTString: public ASTExpression {
  // ASTInteger nodes represent integer literals encountered in
  // input.

  string value;

public:  
  ASTString(string i, int line_init, int char_init);
  
  void ASTPrint(ostream &o) const;
};
GarnetPtr ASTEval();

class ASTArgList: public ASTExpression {
  // ASTArgList represents a comma separated argument
  // list used either in a method call or in an array
  // literal
  //
  // Note that this class is abstract.
  // Its subclasses provide representations for
  // empty argument lists, i.e., ()
  // single element lists, i.e., (arg1)
  // compound argument lists, i.e., (arg1, arg2, arg3)

public:
  ASTArgList(int line_init, int char_init)
    :ASTExpression(line_init,char_init)
      {

      }
  virtual void ASTPrint(ostream &o) const =0;

  ASTExpression * head();
  ASTExpression * tail();

  // virtual void generate(strstream &s) const =0;
};
class ASTIdentifier: public ASTExpression {
  // ASTIdentifier is a cruel beast
private:
  string id;
public:
  ASTIdentifier(string yytext, int line_init, int char_init):ASTExpression(line_init, char_init), id(yytext) // create a function or some shit to handle this.
  {
    id = yytext;
  }
  ASTIdentifier(string yytext):ASTExpression(0, 0), id(yytext)
  {id = yytext;}
  void ASTPrint(ostream &o) const;


};
class ASTArgListEmpty: public ASTArgList {
  // ASTArgListEmpty
  // This 
public:
  ASTArgListEmpty(int line_init, int char_init)
    :ASTArgList(line_init,char_init)
      {}

  virtual void ASTPrint(ostream &o) const =0;

};

class ASTArgListSimple: public ASTArgList {
private:
  ASTExpression *expr;
public:
  ASTArgListSimple(ASTExpression *expr_init)
    :ASTArgList(expr_init->getline(),expr_init->getchar()), expr(expr_init)
      {
        expr = expr_init;
      }

  void ASTPrint(ostream &o) const;

  ASTExpression *head();
};

class ASTArgListCompound: public ASTArgList {
private:
  ASTExpression *list_head;
  ASTArgList *list_tail;
public:
  ASTArgListCompound(ASTExpression *head_init,
		     ASTArgList *tail_init)
    :ASTArgList(head_init->getline(),head_init->getchar()),
     list_head(head_init),list_tail(tail_init)
      {
        list_head = head_init;
        list_tail = tail_init;
      }

  void ASTPrint(ostream &o) const;

  ASTExpression *head();
  ASTExpression *tail();
};

class ASTWhile: public ASTExpression {
private:

  ASTExpression *condition;
  ASTExpressionList *body;
public: 
  ASTWhile(ASTExpression *condition_Init, ASTExpressionList *body_init, int line_init, int char_init):ASTExpression(line_init, char_init), condition(condition_Init), body(body_init)
{
  condition = condition_Init;
  body = body_init;
}
 
 void ASTPrint(ostream &o) const;

};

class ASTFor: public ASTExpression {
private:

  ASTExpression *condition;
  ASTExpressionList *arrayExp;
  ASTExpressionList *body;
public: 
  ASTFor(ASTExpression *condition_Init, ASTExpressionList *arrayExp_init, ASTExpressionList *body_init, int line_init, int char_init):ASTExpression(line_init, char_init), condition(condition_Init), arrayExp(arrayExp_init), body(body_init)
{
  condition = condition_Init;
  arrayExp = arrayExp_init;
  body = body_init;
}
 
 void ASTPrint(ostream &o) const;

};

class ASTLoop: public ASTExpression {
private:

  ASTExpressionList *body;
public: 
  ASTLoop(ASTExpressionList *body_init, int line_init, int char_init):ASTExpression(line_init, char_init), body(body_init)
{
  body = body_init;
}
 
 void ASTPrint(ostream &o) const;

};

class ASTIf: public ASTExpression {
private:

  ASTExpression *condition;
  ASTExpressionList *body;
  ASTExpression *hacky;
public: 
  ASTIf(ASTExpression *condition_Init, ASTExpressionList *body_init, ASTExpression *hacky_init, int line_init, int char_init):ASTExpression(line_init, char_init), condition(condition_Init), body(body_init), hacky(hacky_init)
{
  condition = condition_Init;
  body = body_init;
  hacky = hacky_init;
}
 
 void ASTPrint(ostream &o) const;

};

class ASTIfNo: public ASTExpression {
private:

  /*NOPE*/

public: 
  ASTIfNo(int line_init, int char_init):ASTExpression(line_init, char_init)
{
}

 void ASTPrint(ostream &o) const;

};

class ASTelsif: public ASTExpression {
private:

  ASTExpression *condition;
  ASTExpressionList *body;
  ASTExpression *hacky;

public: 
  ASTelsif(ASTExpression *condition_Init, ASTExpressionList *body_init, ASTExpression *hacky_init, int line_init, int char_init):ASTExpression(line_init, char_init), condition(condition_Init), body(body_init), hacky(hacky_init)
{
  condition = condition_Init;
  body = body_init;
    hacky = hacky_init;
}

 void ASTPrint(ostream &o) const;

};

class ASTelse: public ASTExpression {
private:

 
  ASTExpressionList *body;
public: 
  ASTelse(ASTExpressionList *body_init, int line_init, int char_init):ASTExpression(line_init, char_init), body(body_init)
{
  body = body_init;
}
 
 void ASTPrint(ostream &o) const;

};


class ASTMethodCall: public ASTExpression {
private:
  string methodName;
  ASTArgList *arglist;
public:
  ASTMethodCall(string s,
		ASTArgList *_arglist)
    :ASTExpression(_arglist->getline(), _arglist->getchar())
  { 
    methodName = s;
    arglist = _arglist;
  }
  /*
  ASTMethodCall(ASTIdentifier s,
    ASTArgList *_arglist)
    :ASTExpression(_arglist->getline(), _arglist->getchar())
  {
    id = s;
    arglist = _arglist;
  }*/
  void ASTPrint(ostream &o) const;
};

#endif // __AST__H_
