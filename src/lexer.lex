/* lexer.lex
   A lexical analyzer (tokenizer) for YALL
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

%option noyywrap
%option nodefault
%option yylineno

%{
#include "stdtype.h"
#include "ast.h"
#include "err.h"
#include "builtins.h"
#include "parser.tab.h"
%}

  /* float exponent */
FEXP  ([Ee][-+]?[0-9]+)

%%

  /* single character ops */
"+" |
"-" |
"*" |
"/" |
"&" |
"|" |
"(" |
")"  { return yytext[0]; }

  /* comparison ops, (all use CMP token) */

">"  { yylval.tfunc = T_greater_than; return CMP; }
"<"  { yylval.tfunc = T_less_than; return CMP; }
"!=" { yylval.tfunc = T_not_equal; return CMP; }
"==" { yylval.tfunc = T_equal; return CMP; }
">=" { yylval.tfunc = T_greater_than_equal; return CMP; }
"<=" { yylval.tfunc = T_less_than_equal; return CMP; }
"or" { yylval.bfunc = T_or; return CMP; }
"and" { yylval.bfunc = T_and; return CMP; }

  /* names */
  /*
[a-zA-Z][a-zA-Z0-9_\-]* {
  yylval.s = lookup(yytext);
  return NAME;
}
  */

  /* floating point number */
[0-9]+"."[0-9]*{FEXP}? |
"."[0-9]+{FEXP}? {
  yylval.f = atof(yytext);
  return FLOAT;
}

  /* decimal integer (TODO: fit into 64bit) */
[0-9]+ {
  yylval.i = atoi(yytext);
  return INT;
}

  /* hex decimal integer (TODO: fit into 64bit) */
0?[Xx][0-9A-Fa-f]+ {
  yylval.i = htoi(yytext);
  return INT;
}

"//".* { /* C++-style comments, TODO: add C style comments */ }
[ \t]  { /* whitespace */ }

  /* line continuation */
\\\n {
  printf("c> ");
}


  /* keywords */
"pi" {
  return PI;
}

"true" {
  yylval.b = true;
  return BOOLEAN;
}

"false" {
  yylval.b = false;
  return BOOLEAN;
}

  /* built-in functions */
"sqrt" {
  yylval.bfunc = B_sqrt;
  return BFUNC;
}

"abs" {
  yylval.bfunc = B_abs;
  return BFUNC;
}

"log" {
  yylval.bfunc = B_log;
  return BFUNC;
}

"log2" {
  yylval.bfunc = B_log2;
  return BFUNC;
}

"log10" {
  yylval.bfunc = B_log10;
  return BFUNC;
}

"float" {
  yylval.bfunc = B_float;
  return BFUNC;
}

"int" {
  yylval.bfunc = B_int;
  return BFUNC;
}

"pow" {
  yylval.bfunc = B_pow;
  return BFUNC;
}

"fact" {
  yylval.bfunc = B_fact;
  return BFUNC;
}

"!" {
  yylval.bfunc = B_negate;
  return BFUNC;
}

"floor" {
  yylval.bfunc = B_floor;
  return BFUNC;
}

"ceil" {
  yylval.bfunc = B_ceil;
  return BFUNC;
}

  /* end of line */
\n {
  return EOL;
}

. {
  yyerror("Bad Character: %c\n", *yytext);
}

%%

  /* nothing */

