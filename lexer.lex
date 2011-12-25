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
  /*
">"  { yylval.fn = 1; return CMP; }
"<"  { yylval.fn = 2; return CMP; }
"!=" { yylval.fn = 3; return CMP; }
"==" { yylval.fn = 4; return CMP; }
">=" { yylval.fn = 5; return CMP; }
"<=" { yylval.fn = 6; return CMP; }
  */
  /* keywords */

  /* built-in values */

  /* built-in functions */

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

  /* PI */
"pi" {
  return PI;
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

"!" {
  yylval.bfunc = B_fact;
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


