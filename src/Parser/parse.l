
/* This is the Lexer, it makes tokens */

/* be careful with comments, each needs whitespace before it */
%{
  #define YY_NO_UNISTD_H

  class expression; // predeclare this class as it is needed later

  #include <string.h>
  #include <stdio.h>
  #include "parse.tab.h" // This file is created by flex
  #include "expression.h"
  #include "mathCore.h"
%}

%option never-interactive
%option noyywrap

/* Rules section, uses REGEX, grammar rules defined in the docs folder*/
%%

[ \t\n] ;                                 // Catches all whitespace
[\(\[\{]               return O_BRACE;    // Open Parenthesis
[\}\]\)]               return C_BRACE;    // Close Parenthesis
[!~¬']{2}                                 // Discard Double Negatives
[!~¬]                  return PRE_NEG;    // Negation Prefix Operator
[']                    return POST_NEG;   // Negation Postfix Operator
(\^d)                  return DUAL;       // Dual Operator
(->)|(=>)              return FWD_IMPL;   // Forward Implication Operator
(<-)|(<=)              return BCK_IMPL;   // Forward Implication Operator
[\^*·]|(\&)+           return AND;        // AND Operator
[∨V+]|[\|]+            return OR;         // Or Operator
[⊕⊻@]|(xor)|(XOR)     return XOR;        // Xor Operator
[≡=]|(xnor)|(XNOR)     return XNOR;       // Equivalence Operator
[01]                   {yylval.ival=atoi(yytext); return LITERAL;}    // Literals
[a-zA-Z][0-9]*         {yylval.sval=strdup(yytext); return VARIABLE;} // This expression catches variables
[$]                    return 0;          // End of File
%%