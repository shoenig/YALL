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
"PI" {
  return PI;
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


