%{

  #include <iostream>
  #include <cstdio>
  #include "expression.h"
  #include "parse.tab.h"
  #include "mathCore.h"

  extern int yyparse();
  extern int yylex();
  extern FILE* yyin;
  expression* result;

  void yyerror(const char* s) {
      std::cout << "Well that's an error:" << s << std::endl;
      exit(-1);
  }

  /* 
  * Function to get an expression from a file
  * Takes a file name as an argument and attempts to open it
  * if the file is successfully opened, it attemps to read from it
  * a pointer to the resulting expression is returned
  * if the file fails to open, return NULL
  */
  expression* parse_expression(const char* filename) {
    if( (yyin = fopen(filename, "r")) == NULL) {
        // The file could not be read
        return NULL;
    }
    int tag = yyparse(); /* Parses the entire input file, returns the exit status */
    std::cout << "Parsed: " << tag << std::endl;
    return result;  // Returns 
  }
%}

/* Defines valid Token types */
%union{
    int ival;
    char* sval;
    expression* expr;
}

/* Tokens delcared later will be group first */
/* Order of operations is: */
/* Paren -> Negation, Dual, Complement -> AND -> OR, XOR, XNOR -> EQUIV evaluated left to right */

%left XNOR
%left IMPL
%left XOR OR
%left AND
%left POST_NEG DUAL
%right PRE_NEG
%token <ival> LITERAL
%token <sval> VARIABLE
%token O_BRACE C_BRACE
%start commands

%type <expr> expression
%type <expr> and_expression
%type <expr> or_expression
%type <expr> xor_expression
%type <expr> negated_expression
%type <expr> dualed_expression
%type <expr> implication_expression
%type <expr> paren_expression


%%

/* Recursivly parses expressions 1 at a time */
/* Parses from right to left, as it is more memory efficienct */
commands:
        /*| commands expression {std::cout << "commands " << std::endl;} */
        | expression {result = $1; std::cout << "MADE TO THE END" << std::endl;}  /* use this line to set the final result */
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
    
    /* Interpreting Variables and Literals */
    | VARIABLE POST_NEG {
                        #ifndef  DEBUG_INPUT_PARSE
                        std::cout << "VARIABLE(NEG) -> EXPRESSION\n";
                        #endif
                        $$ = mathCore::variable($1);
                        mathCore::negate($$);
                    }
    | PRE_NEG VARIABLE {
                        #ifndef  DEBUG_INPUT_PARSE
                        std::cout << "(NEG)VARIABLE -> EXPRESSION\n";
                        #endif
                        $$ = mathCore::variable($2);
                        mathCore::negate($$);
                    }
    | VARIABLE      {
                        #ifndef  DEBUG_INPUT_PARSE
                        std::cout << "VARIABLE -> EXPRESSION\n";
                        #endif
                        $$ = mathCore::variable($1);
                    }
    | LITERAL POST_NEG {
                        #ifndef  DEBUG_INPUT_PARSE
                        std::cout << "LITERAL(NEG) -> EXPRESSION\n";
                        #endif
                        $$ = mathCore::literal((bool)$1);
                        mathCore::negate($$);
                    }
    | PRE_NEG LITERAL {
                        #ifndef  DEBUG_INPUT_PARSE
                        std::cout << "(NEG)LITERAL -> EXPRESSION\n";
                        #endif
                        $$ = mathCore::literal((bool)$2);
                        mathCore::negate($$);
                    }
    | LITERAL       {
                        #ifndef  DEBUG_INPUT_PARSE
                        std::cout << "LITERAL -> EXPRESSION\n";
                        #endif
                        $$ = mathCore::literal((bool)$1);
                    }
;

/* expressions closed in parenthesis */
paren_expression:
    O_BRACE expression C_BRACE {
        #ifndef  DEBUG_INPUT_PARSE
        std::cout << "EXPRESSION -> PAREN_EXPRESSION\n";
        #endif
        $$ = $2;
    }
    | O_BRACE C_BRACE {
        #ifndef  DEBUG_INPUT_PARSE
        std::cout << "EMPTY EXPRESSION\n";
        #endif
    }
;

/* Parses AND expressions */
and_expression:
    expression AND expression {
        #ifndef  DEBUG_INPUT_PARSE
        std::cout << "EXPRESSION -> AND\n";
        #endif
        $$ = mathCore::binary_and($1,$3);
    }
    /* This line parses tightly bound AND expressions, be wary of this line */
    | expression expression {
        #ifndef  DEBUG_INPUT_PARSE
        std::cout << "EXPRESSION -> AND_T\n";
        #endif
        $$ = mathCore::binary_and($1, $2);
    }
;

/* Parses OR Expressions */
or_expression:
    expression OR expression {
        #ifndef  DEBUG_INPUT_PARSE
        std::cout << "EXPRESSION -> OR\n";
        #endif
        $$ = mathCore::binary_or($1,$3);
    }
;

/* Parses XOR Expressions */
xor_expression:
    expression XOR expression {
        #ifndef  DEBUG_INPUT_PARSE
        std::cout << "EXPRESSION -> XOR\n";
        #endif
        $$ = mathCore::binary_xor($1,$3);
    }
    | expression XNOR expression {
        #ifndef  DEBUG_INPUT_PARSE
        std::cout << "EXPRESSION -> XNOR\n";
        #endif
        $$ = mathCore::binary_equiv($1,$3);
    }
;



/* parses implication expressions */
implication_expression:
    expression IMPL expression  {
        #ifndef  DEBUG_INPUT_PARSE
        std::cout << "EXPRESSION -> IMPL\n";
        #endif
        $$ = mathCore::imply($1,$3);
    }
;

/* parses negated expressions, for both post/pre-fix negation */
negated_expression:
    PRE_NEG paren_expression POST_NEG {
        #ifndef  DEBUG_INPUT_PARSE
        std::cout << "DOUBLE_NEG PAREN_EXPRESSION -> PAREN_EXPRESSION\n";
        #endif
        $$ = $2;
    }
    | paren_expression POST_NEG   {
        #ifndef  DEBUG_INPUT_PARSE
        std::cout << "PAREN_EXPRESSION -> POST_NEG\n";
        #endif
        mathCore::negate($1);
        $$ = $1;
    }
    | PRE_NEG paren_expression  {
        #ifndef  DEBUG_INPUT_PARSE
        std::cout << "PAREN_EXPRESSION -> PRE_NEG\n";
        #endif
        mathCore::negate($2);
        $$ = $2;
    }
;

/* parses the dual of an expression postfix only */
dualed_expression:
    paren_expression DUAL   {
        #ifndef  DEBUG_INPUT_PARSE
        std::cout << "PAREN_EXPRESSION -> DUAL\n";
        #endif
        mathCore::dual($1);
        $$ = $1;
    }
;

%%

