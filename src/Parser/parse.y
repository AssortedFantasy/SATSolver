%{

  #include <stdio.h>

  extern "C" {
    int yyparse(void);
    int yylex(void);
    int yywrap() {
    return 1;
    }
  }
  extern yylval;


  main() {
    yyparse();
  }
%}

%union
{
 int number;
 char* string;
}

%token <number> LITERAL
%token <string> VARIABLE
%token O_BRACE C_BRACE VARIABLE
%left AND OR XOR XNOR POST_NEG DUAL COMPLEMENT
%right PRE_NEG
%start commands



%%
commands:
    | commands expression
;

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

tightly_bound_expression:
    paren_expression
    | and_expression
    | negated_expression
    | dualed_expression
    | complement_expression
    | VARIABLE
    | LITERAL
;

paren_expression:
    | O_BRACE expression C_BRACE
;

and_expression:
    expression AND expression
;

or_expression:
    expression OR expression
;

xor_expression:
    expression XOR expression
;

/* If a LITERAL is negated, invert its value */
negated_literal:
    LITERAL POST_NEG  {$1 ^= 1}
    | PRE_NEG LITERAL   {$2 ^= 1}
;


negated_variable:
    paren_expression VARIABLE
    | PRE_NEG VARIABLE
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

