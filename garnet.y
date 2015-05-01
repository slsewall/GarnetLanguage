%{
#include "AST.h"
#include "garnet.h"

#include <iostream>
extern void init_keywords();

extern int Linenum;
extern int Charnum;

using namespace std;
int yylex();
void yyerror(string);

void yyerror(string)
{
  cerr << "Error at line " << Linenum << " character " << Charnum << endl;
}

extern FILE *yyin;
%}

%union {
       ASTExpressionList  *ExpressionList;
       ASTExpression 	  *Expression;
       ASTArgList    	  *ArgList;
       ASTInteger 	   *Integer;
       ASTFloat 	   *Float;
       ASTIdentifier	*Identifier;
       ASTIdentifier	*LHS;
       ASTExpression 	*Range;
       ASTString		*String;
       ASTWhile			*While;
       ASTelsif			*Elseif;
       ASTelse 			*Else;
       ASTFor			*For;
       LineSpec LS;  // This needs to change!
}
%type <ExpressionList> start
%type <ExpressionList> expression_list
%type <ExpressionList> expression_list_ety
%type <Expression>     expression

%type <Expression>	   logic_expr
%type <Expression>     rel_expr
%type <Expression>     term
%type <Expression>     factor


%type <ArgList>        arglist_ety
%type <ArgList>	       arglist
%type <Expression>     sub_expr
%type <Expression>	   if_stamt
%type <Expression>     elsif_part

%token IF
%token WHILE
%token THEN
%token INTEGER
%token END
%token FOR
%token IN
%token DO
%token ELSE
%token ELSIF
%type <Integer> INTEGER
%token FLOAT
%type <Float> FLOAT
%token  STRING
%type <String> STRING
%token IDENTIFIER
%type <Identifier> IDENTIFIER
%token VARIABLE
%token CONSTANT
%type <Identifier> VARIABLE
%type <Identifier> CONSTANT

%token ANDAND "&&"
%token OROR "||"
%token EQUALEQUAL "=="
%token LESSEQUAL "<="
%token GREATEREQUAL ">="
%token COLONCOLON "::"
%token LESSTHAN "<"
%token GREATERTHAN ">"


%%
start: expression_list { $1->ASTPrint(cout); cout << endl; }


expression_list : expression { $$ = new ASTExpressionListSimple($1); }
		| expression_list ';' expression
		  { $$ = new ASTExpressionListCompound($1, $3); }
		

expression_list_ety: /*empty */ { $$ = new ASTExpressionListEmpty(); }
		     | expression_list { $$ = $1; }
		     

expression : logic_expr { $$ = $1; }
			


logic_expr : rel_expr ANDAND rel_expr {$$ =new ASTMethodCall("&&", new ASTArgListCompound($1, new ASTArgListSimple($3))); }
			| rel_expr OROR rel_expr {$$ =new ASTMethodCall("||", new ASTArgListCompound($1, new ASTArgListSimple($3))); }
			| rel_expr {$$ = $1; } 


rel_expr : sub_expr EQUALEQUAL sub_expr {$$ =new ASTMethodCall("==", new ASTArgListCompound($1, new ASTArgListSimple($3))); }
		  | sub_expr LESSEQUAL sub_expr {$$ =new ASTMethodCall("<=", new ASTArgListCompound($1, new ASTArgListSimple($3))); }
		  | sub_expr GREATEREQUAL sub_expr {$$ =new ASTMethodCall(">=", new ASTArgListCompound($1, new ASTArgListSimple($3))); }
		  | sub_expr '<' sub_expr {$$ =new ASTMethodCall("<", new ASTArgListCompound($1, new ASTArgListSimple($3))); }
		  | sub_expr '>' sub_expr {$$ =new ASTMethodCall(">", new ASTArgListCompound($1, new ASTArgListSimple($3))); }
		  | sub_expr {$$ = $1; } 

sub_expr :
	term { $$ = $1; }
	| sub_expr '+' term {
	    $$ = new ASTMethodCall("+",
	         new ASTArgListCompound($1, new ASTArgListSimple($3)));
	  }
	| sub_expr '-' term {
	    $$ = new ASTMethodCall("-",
	         new ASTArgListCompound($1, new ASTArgListSimple($3)));
	  }
	;


term :
	factor { $$ = $1; }
     	| term '*' factor {
	    $$ = new ASTMethodCall("*",
	         new ASTArgListCompound($1, new ASTArgListSimple($3)));
     	//$$ = $3;
	  }
	| term '/' factor {
	    $$ = new ASTMethodCall("/",
	         new ASTArgListCompound($1, new ASTArgListSimple($3)));
	  }
	;

factor :
	 '!' factor { $$ = new ASTMethodCall("!", new ASTArgListSimple($2)); }
        | INTEGER { $$ = $1; }
	| FLOAT { $$ = $1; }
	| '-' factor
	  { $$ = new ASTMethodCall("-", new ASTArgListSimple($2));
	  }
	| VARIABLE { $$ = $1; }
	| CONSTANT { $$ = $1; }
	| factor '.' IDENTIFIER
	| factor '[' expression ']'
	| STRING {$$ = $1; }
	| '(' expression ')' { $$ = $2; }
	| WHILE expression DO expression_list END {$$ = new ASTWhile($2,$4, Linenum, Charnum);}
	| FOR expression IN expression_list DO expression_list END {$$ = new ASTFor($2,$4, $6, Linenum, Charnum);}
	| if_stamt {$$ = $1;}
	;

if_stamt : 
	IF expression THEN expression_list elsif_part END {$$ = new ASTIf($2, $4, $5, Linenum, Charnum);}
	;

elsif_part : END {$$ = new ASTIfNo(0,0); }
  			| ELSE expression_list {$$ = new ASTelse($2, Linenum, Charnum);}
			| ELSIF expression THEN expression_list elsif_part  {$$ = new ASTelsif($2, $4, $5, Linenum, Charnum);}
			;

arglist_ety :
        /* empty */ { $$ = new ASTArgListEmpty()}
	| arglist { $$ = $1; }
	;

arglist :  expression { $$ = new ASTArgListSimple($1); }
	| expression ',' arglist {
	    $$ = new ASTArgListCompound( $1, $3);
	  }
	;

%%
int main(int argc, char **argv)
{
    init_keywords();
    GarnetObject::do_init();
    yyparse();
}
