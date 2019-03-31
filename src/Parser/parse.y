%{

  #include <iostream>
  //#include "parse.tab.h"

  extern int yyparse();
  extern int yylex();
  extern FILE *yyin;

  void yyerror(const char* s) {
      std::cout << "Well that's an error:" << s << std::endl;
      exit(-1);
  }


  int main() {
    std::cout << "Parsed: " << yyparse() << std::endl;
    return 0;
  }
%}

%union{
    int ival;
    char* sval;
}

/* Tokens delcared later will be group first */
/* Order of operations is: */
/* Paren -> Negation, Dual, Complement -> AND -> OR, XOR, XNOR -> EQUIV evaluated left to right */

%left XNOR
%left OR XOR
%left AND
%right PRE_NEG
%left POST_NEG DUAL COMPLEMENT
%token <ival> LITERAL
%token <sval> VARIABLE
%token O_BRACE C_BRACE
%start commands


%%

/* Recursivly parses expressions 1 at a time */
/* Parses from right to left, as it is more memory efficienct */
commands:
    | commands expression
;

/* Defines a basic expression */
expression:
    paren_expression
    | and_expression
    | or_expression
    | negated_expression
    | dualed_expression
    | complement_expression
    | VARIABLE
    | LITERAL
;

/* expressions closed in parenthesis */
paren_expression:
    O_BRACE expression C_BRACE
;

and_expression:
    expression AND expression
    | expression expression
;

or_expression:
    expression OR expression
;

/* For XNOR, change the negate flag */
xor_expression:
    expression XOR expression
    expression XNOR expression
;

negated_expression:
    paren_expression POST_NEG
    | PRE_NEG paren_expression
;

/* postfix only */
dualed_expression:
    paren_expression DUAL
;

/* postfix only */
complement_expression:
    paren_expression COMPLEMENT
;
%%

