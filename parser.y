  /* parser.y
     A parser for YALL
     Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
  */

%{
#include <stdio.h>
#include <stdlib.h>
#include "err.h"
#include "stdtype.h"
#include "ast.h"
%}

%union {
  AST* a;
  float64 f;
  int64 i;
  /*Symbol* s;/* which symbol */
  /*Symlist* sl; /* list o' symbols */
  /*uint64_t func; /* which function */
}

 /* tokens */
%token <f> FLOAT
%token <i> INT
 /*%token <s> NAME*/
 /*%token <fn> FUNC*/
%token EOL
 /* %token PI */

 /* associativities */
 /*%nonassoc <fn> CMP*/
 /*%right '='*/
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%type <a> sexp /*explist*/
 /*%type <sl> symlist */

%start yalllist

%%

 /* grammer */

 /* an exp is an INFIX expression */
 /*
exp: exp CMP exp     { $$ = newcmp($2, $1, $3); }
|    exp '+' exp     { $$ = newast('+', $1, $3); }
|    exp '-' exp     { $$ = newast('-', $1, $3); }
|    exp '*' exp     { $$ = newast('*', $1, $3); }
|    exp '/' exp     { $$ = newast('/', $1, $3); }
|    '(' exp ')'     { $$ = $2; }
|    '-' exp $prec UMINUS { $$ = newast('M', $2, NULL); }
|    NUMBER          { $$ = newnum($1); }
|    PI              { $$ = newnum(3.14159); }
|    NAME            { $$ = newref($1); }
;
 */


 /* a sexp is an S-Expression (normal lisp style) */ 
sexp: '(' INT ')'       { $$ = new_intval($2); }
|     '(' FLOAT ')'     { $$ = new_floatval($2); }
;                                                     

 /* yalllist, top level */ 
yalllist: /* nothing */
| yalllist sexp EOL {
  printf("debug eval_f: %4.4g\n", eval_f($2));
  printf("debug eval_i: %4lld\n> ", eval_i($2));
  /* eval($2); */
  freeTREE($2);
 }
| yalllist error EOL {
  yyerrok;
  printf("> ");
 }
| yalllist EOL {
  printf("> ");
 }
;


%%

  /* nada */
