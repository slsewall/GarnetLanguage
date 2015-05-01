%{
#include "AST.h"

#include <iostream>
#include <sstream>
#include <map>

#include <climits>
#include <cerrno>
#include <cstdlib>

#include "garnet.tab.h"

using namespace std;

int Linenum = 1;
int Charnum = 1;

#define INCR_LINE() { Linenum += 1; Charnum = 1; }
#define INCR_CHAR() { Charnum += strlen(yytext); }

map<string,int> keywords;

void init_keywords() {
  keywords["&&"] = ANDAND;
  keywords["||"] = OROR;
  keywords["=="] = EQUALEQUAL;
  keywords["<="] = LESSEQUAL;
  keywords[">="] = GREATEREQUAL;
  keywords["::"] = COLONCOLON;
  keywords["while"] = WHILE;
  keywords["do"] = DO;
  keywords["end"] = END;
  keywords["if"] = IF;
  keywords["then"] = THEN;
  keywords["else"] = ELSE;
  keywords["elsif"] = ELSIF;
  keywords["for"] = FOR;
  keywords["in"] = IN;

}

int keyword_lookup(char *s, int tok_val)
{
  if (keywords.find(s) != keywords.end()) {
    return keywords[s];
  } else {
    return tok_val;
  }
}

string deunderscore(string s) {
  // Not provided
  for(string::iterator iter= s.begin(); iter != s.end(); ++iter)
  {
    if(*iter == '_')
    {
      s.erase(iter);
    }
  }
  return s;
}

double convert_to_float(string s){
  // did it
  const char *c = s.c_str();
  double entendre = atof(c);
  double d = 0.0; // I see what you did here XD
  d+= entendre;
  return d;
}	    

int convert_to_int(string s, unsigned b){ 
  long long_result; 
  char *endptr;

  if (0 == s.substr(0,2).compare("0x")) {
    stringstream ss;
    ss << hex << s.substr(2);
    ss >> long_result;
  } else {
    // convert return_val to int
    errno = 0; // Flush GLOBAL errno
    long_result = strtol(deunderscore(s).c_str(), &endptr, b);
    if (errno || long_result > INT_MAX) {
      cerr << "Integer literal (" << s << ") at line " <<
	Linenum << ", character " << Charnum << ":" << 
	strerror(errno) << endl;
      exit(1);
    }
  }
  return (int)long_result;
}

%}
DECIMAL  [0-9](_?[0-9])*
EXP_PART e"-"?{INTEGER}
INTEGER	[1-9](_?[0-9])*
			
/*
 * Had to comment these two rules out because the tokens aren't used in
 * garnet.y
 * Put them in as rules below the %% immediately following when you
 * have added definitions to your garnet.y for VARIABLE and CONSTANT
 * tokens.
*/

%%
[a-z_][a-zA-Z0-9_]*\??  {int tok_val = keyword_lookup(yytext, VARIABLE);
			 if (tok_val == VARIABLE) {
			   yylval.Identifier = new ASTIdentifier(yytext, Linenum, Charnum);
			 } else {
			   yylval.LS = LineSpec({Linenum,Charnum});
			 }
			 INCR_CHAR();
                         return tok_val;
                        }

[A-Z][a-zA-Z0-9_]*\??   { yylval.Identifier = new ASTIdentifier(yytext);
			  INCR_CHAR();
			  return CONSTANT;
			}

(0|{INTEGER}) { yylval.Integer = new ASTInteger(convert_to_int(yytext,10), Linenum, Charnum);
		      INCR_CHAR();
                      return INTEGER;
                    }

0[0-7](_?[0-7])* { yylval.Integer = new ASTInteger(convert_to_int(yytext,8), Linenum, Charnum);
	     INCR_CHAR();
             return INTEGER;
             }

(0x[0-9a-f](_?[0-9a-f])*) { yylval.Integer = new ASTInteger(convert_to_int(yytext,16), Linenum, Charnum);
	     INCR_CHAR();
             return INTEGER;

	       }

(0b[0-1]+) { yylval.Integer = new ASTInteger(convert_to_int(yytext+2,2), Linenum, Charnum);
	     INCR_CHAR();
             return INTEGER;
	   }

((0|({INTEGER}))"."{DECIMAL}{EXP_PART}?)|("-"?{INTEGER}{EXP_PART}) {
     yylval.Float = new ASTFloat(convert_to_float(yytext), Linenum, Charnum);
     INCR_CHAR();
     return FLOAT;
   }

"+"|"-"|"*"|"/"|"("|")"|"["|"]"|"{"|"}"|","|"."|"="|">"|"<"|"!"|";"|"&&"|"||"|"=="|"<="|">="|"::" { int tok_val = keyword_lookup(yytext, 0);
	 if (tok_val) {
	   /*
	    * Had to comment this out as well.
            * you can uncomment when you add an ASTIdentifier class with
	    * an appropriate constructor.
      */
      //cout << "before";
	    yylval.Identifier = new ASTIdentifier(yytext, Linenum, Charnum);
      //cout << "got into the tokThing";
	    
	 } else {
	   tok_val = *yytext;
	   yylval.LS = LineSpec({Linenum,Charnum});
	 }
	 INCR_CHAR();
         return tok_val;
	 }


"#".*         {}

[ \t\v\b\r\f]*  { INCR_CHAR(); }

\n   { INCR_LINE(); }

.    { INCR_CHAR(); cerr << "Erroneous token [" << yytext << "]" << endl; }
%%
