%{

  #include <iostream>
  #include "parse.tab.h"
  //#include "../../"

  extern int yyparse();
  extern int yylex();
  extern FILE *yyin;

  void yyerror(const char* s) {
      std::cout << "Well that's an error:" << s << std::endl;
      exit(-1);
  }


  int main() {
    int tag;
    tag = yyparse();
    std::cout << "Parsed: " << tag << std::endl;
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
%left IMPL
%left XOR OR
%left AND
%right PRE_NEG
%left POST_NEG DUAL
%token <ival> LITERAL
%token <sval> VARIABLE
%token O_BRACE C_BRACE
%start commands
//%type <expression> expression


%%

/* Recursivly parses expressions 1 at a time */
/* Parses from right to left, as it is more memory efficienct */
commands:
        | commands expression '\n'{std::cout << "commands ";}
;

/* Defines a basic expression */
expression:
    paren_expression {std::cout << "PAREN_EXPRESSION -> EXPRESSION\n";}
    | and_expression {std::cout << "AND -> EXPRESSION\n";}
    | or_expression  {std::cout << "OR -> EXPRESSION\n";}
    | xor_expression {std::cout << "XOR -> EXPRESSION\n";}
    | negated_expression {std::cout << "NEG -> EXPRESSION\n";}
    | dualed_expression {std::cout << "DUAL -> EXPRESSION\n";}
    | implication_expression {std::cout << "IMPL -> EXPRESSION\n";}
    | VARIABLE      {std::cout << "VARIABLE -> EXPRESSION\n";}
    | LITERAL       {std::cout << "LITERAL -> EXPRESSION\n";}
;

/* expressions closed in parenthesis */
paren_expression:
    O_BRACE expression C_BRACE      {std::cout << "EXPRESSION -> PAREN_EXPRESSION\n";}
;

and_expression:
    expression AND expression {std::cout << "EXPRESSION -> AND\n";}
    /*| expression expression {std::cout << "EXPRESSION -> AND_T\n";} */
;

or_expression:
    expression OR expression {std::cout << "EXPRESSION -> OR\n";}
;

/* For XNOR, change the negate flag */
xor_expression:
    expression XOR expression       {std::cout << "EXPRESSION -> XOR\n";}
    | expression XNOR expression    {std::cout << "EXPRESSION -> XNOR\n";}
;

implication_expression:
    expression IMPL expression      {std::cout << "EXPRESSION -> IMPL\n";}
;

negated_expression:
    paren_expression POST_NEG       {std::cout << "PAREN_EXPRESSION -> POST_NEG\n";}
    | PRE_NEG paren_expression      {std::cout << "PAREN_EXPRESSION -> PRE_NEG\n";}
;

/* postfix only */
dualed_expression:
    paren_expression DUAL           {std::cout << "PAREN_EXPRESSION -> DUAL\n";}
;

%%

