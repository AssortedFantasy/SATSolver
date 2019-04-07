%{
  /*
  * This file defines the parsing of expressions.
  * If this file is modified, run the make file in the Parser Directory
  * to automatically recompile the lexer. Then entire project will need to be
  * rebuilt as well.
  * If the DEBUG_INPUT_PARSE Flag is defined, then debug output will be printed
  */
  #include <iostream>
  #include <cstdio>
  #include "expression.h"
  #include "parse.tab.h"
  #include "mathCore.h"

  extern int yyparse();
  extern int yylex();
  extern FILE* yyin;
  expression* result;

   /* Function used to print error messages if one arises */
   /* Exits with error code -1 */
  void yyerror(const char* s) {
      std::cout << "ERROR FOUND: " << s << std::endl;
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
    return result;  // Returns the final expression
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
/* Paren -> Negation, Dual, Complement -> AND -> OR, XOR, XNOR -> IMPLICATION -> EQUIV evaluated left to right */

%left XNOR
%left FWD_IMPL BCK_IMPL
%left XOR OR
%left AND
%left POST_NEG DUAL
%right PRE_NEG

/* These tokens are not operators and have no associative property */
%token <ival> LITERAL
%token <sval> VARIABLE
%token O_BRACE C_BRACE
%start commands

/* Defines the data type of relevant expression types */
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
        /* Use top down parsing */
        /*| commands expression {
            #ifdef DEBUG_INPUT_PARSE
            std::cout << "commands " << std::endl;
            #endif
        } */
        | expression {
            result = $1; /* Stores the final result */
            #ifdef DEBUG_INPUT_PARSE
            std::cout << "MADE TO THE END" << std::endl;
            #endif
        }
;

/* Defines a basic expression */
expression:
    paren_expression {
        #ifdef DEBUG_INPUT_PARSE
        std::cout << "PAREN_EXPRESSION -> EXPRESSION\n";
        #endif 
    }
    | and_expression {
        #ifdef DEBUG_INPUT_PARSE
        std::cout << "AND -> EXPRESSION\n";
        #endif 
    }
    | or_expression  {
        #ifdef DEBUG_INPUT_PARSE
        std::cout << "OR -> EXPRESSION\n";
        #endif
        }
    | xor_expression {
        #ifdef DEBUG_INPUT_PARSE
        std::cout << "XOR -> EXPRESSION\n";
        #endif
        }
    | negated_expression {
        #ifdef DEBUG_INPUT_PARSE
        std::cout << "NEG -> EXPRESSION\n";
        #endif
        }
    | dualed_expression {
        #ifdef DEBUG_INPUT_PARSE
        std::cout << "DUAL -> EXPRESSION\n";
        #endif
        }
    | implication_expression {
        #ifdef DEBUG_INPUT_PARSE
        std::cout << "IMPL -> EXPRESSION\n";
        #endif
        }
    
    /* Interpreting Variables and Literals */
    | VARIABLE POST_NEG {
                        #ifdef  DEBUG_INPUT_PARSE
                        std::cout << "VARIABLE(NEG) -> EXPRESSION\n";
                        #endif
                        $$ = mathCore::variable($1);
                        mathCore::negate($$);
                    }
    | PRE_NEG VARIABLE {
                        #ifdef  DEBUG_INPUT_PARSE
                        std::cout << "(NEG)VARIABLE -> EXPRESSION\n";
                        #endif
                        $$ = mathCore::variable($2);
                        mathCore::negate($$);
                    }
    | VARIABLE      {
                        #ifdef  DEBUG_INPUT_PARSE
                        std::cout << "VARIABLE -> EXPRESSION\n";
                        #endif
                        $$ = mathCore::variable($1);
                    }
    | LITERAL POST_NEG {
                        #ifdef  DEBUG_INPUT_PARSE
                        std::cout << "LITERAL(NEG) -> EXPRESSION\n";
                        #endif
                        $$ = mathCore::literal((bool)$1);
                        mathCore::negate($$);
                    }
    | PRE_NEG LITERAL {
                        #ifdef  DEBUG_INPUT_PARSE
                        std::cout << "(NEG)LITERAL -> EXPRESSION\n";
                        #endif
                        $$ = mathCore::literal((bool)$2);
                        mathCore::negate($$);
                    }
    | LITERAL       {
                        #ifdef  DEBUG_INPUT_PARSE
                        std::cout << "LITERAL -> EXPRESSION\n";
                        #endif
                        $$ = mathCore::literal((bool)$1);
                    }
;

/* expressions closed in parenthesis */
paren_expression:
    O_BRACE expression C_BRACE {
        #ifdef  DEBUG_INPUT_PARSE
        std::cout << "EXPRESSION -> PAREN_EXPRESSION\n";
        #endif
        $$ = $2;
    }
    | O_BRACE C_BRACE {
        #ifdef  DEBUG_INPUT_PARSE
        std::cout << "EMPTY EXPRESSION\n";
        #endif
    }
;

/* Parses AND expressions */
and_expression:
    expression AND expression {
        #ifdef  DEBUG_INPUT_PARSE
        std::cout << "EXPRESSION -> AND\n";
        #endif
        $$ = mathCore::binary_and($1,$3);
    }
    /* This line parses tightly bound AND expressions, be wary of this line */
    | expression expression {
        #ifdef  DEBUG_INPUT_PARSE
        std::cout << "EXPRESSION -> AND_T\n";
        #endif
        $$ = mathCore::binary_and($1, $2);
    }
;

/* Parses OR Expressions */
or_expression:
    expression OR expression {
        #ifdef  DEBUG_INPUT_PARSE
        std::cout << "EXPRESSION -> OR\n";
        #endif
        $$ = mathCore::binary_or($1,$3);
    }
;

/* Parses XOR Expressions */
xor_expression:
    expression XOR expression {
        #ifdef  DEBUG_INPUT_PARSE
        std::cout << "EXPRESSION -> XOR\n";
        #endif
        $$ = mathCore::binary_xor($1,$3);
    }
    | expression XNOR expression {
        #ifdef  DEBUG_INPUT_PARSE
        std::cout << "EXPRESSION -> XNOR\n";
        #endif
        $$ = mathCore::binary_equiv($1,$3);
    }
;



/* parses implication expressions */
implication_expression:
    expression FWD_IMPL expression  {
        #ifdef  DEBUG_INPUT_PARSE
        std::cout << "EXPRESSION -> FWD_IMPL\n";
        #endif
        $$ = mathCore::imply($1,$3);
    }
    | expression BCK_IMPL expression  {
        #ifdef  DEBUG_INPUT_PARSE
        std::cout << "EXPRESSION -> BCK_IMPL\n";
        #endif
        mathCore::dual($1);
        mathCore::dual($3);
        $$ = mathCore::imply($3,$1);
        mathCore::negate($$);
        mathCore::dual($$);
    }
;

/* parses negated expressions, for both post/pre-fix negation */
negated_expression:
    PRE_NEG paren_expression POST_NEG {
        #ifdef  DEBUG_INPUT_PARSE
        std::cout << "DOUBLE_NEG PAREN_EXPRESSION -> PAREN_EXPRESSION\n";
        #endif
        $$ = $2;
    }
    | paren_expression POST_NEG   {
        #ifdef  DEBUG_INPUT_PARSE
        std::cout << "PAREN_EXPRESSION -> POST_NEG\n";
        #endif
        mathCore::negate($1);
        $$ = $1;
    }
    | PRE_NEG paren_expression  {
        #ifdef  DEBUG_INPUT_PARSE
        std::cout << "PAREN_EXPRESSION -> PRE_NEG\n";
        #endif
        mathCore::negate($2);
        $$ = $2;
    }
;

/* parses the dual of an expression postfix only */
dualed_expression:
    paren_expression DUAL   {
        #ifdef  DEBUG_INPUT_PARSE
        std::cout << "PAREN_EXPRESSION -> DUAL\n";
        #endif
        mathCore::dual($1);
        $$ = $1;
    }
;

%%

