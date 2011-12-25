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
  char bfunc; /* built in func */
  /*Symbol* s;/* which symbol */
  /*Symlist* sl; /* list o' symbols */
  /*uint64_t func; /* which function */
}

 /* tokens */
%token <f> FLOAT
%token <i> INT
 /*%token <s> NAME*/
%token <bfunc> BFUNC
%token EOL
%token PI

 /* associativities */

%nonassoc UMINUS
 /*%nonassoc <fn> CMP*/
 /*%right '='*/
%left '+' '-'
%left '*' '/'

%type <a> sexp
/* %type <a> list */
/* %type <a> rlist */
 /* explist at some point is <a> */
 /*%type <sl> symlist */

%nonassoc BINF

%start yalllist

%%

 /* grammer */

 /* sexp is a symbolic expression */
 /* an exp is an (going to be an) INFIX expression */
sexp: /*exp CMP exp     { $$ = newcmp($2, $1, $3); } */
    '+' sexp sexp    { $$ = new_ast('+', $2, $3); }
|   '-' sexp sexp    { $$ = new_ast('-', $2, $3); }
|   '*' sexp sexp    { $$ = new_ast('*', $2, $3); }
|   '/' sexp sexp    { $$ = new_ast('/', $2, $3); }
|   '&' sexp sexp    { $$ = new_ast('&', $2, $3); }
|   '|' sexp sexp    { $$ = new_ast('|', $2, $3); }
|    PI              { $$ = new_floatval(3.14159); }
|    FLOAT           { $$ = new_floatval($1); }
|    INT             { $$ = new_intval($1); }
|    '(' '-' sexp ')'  { $$ = new_ast('M', $3, NULL); }
|   '(' sexp ')'    { $$ = $2; }
|    BFUNC sexp     { $$ = new_bif($1, $2, NULL); }
|    '(' BFUNC sexp sexp ')' { $$ = new_bif($2, $3, $4); }
/*|    NAME            { $$ = newref($1); }*/
;

 /* yalllist, top level */ 
yalllist: /* nothing */
| yalllist sexp EOL {

  evaltype e = eval($2);
  switch(e.type) {
  case 'I':
    printf("debug eval<I>: %4lld\n> ", e.val.i);
    break;
  case 'F':
    printf("debug eval<F>: %4.4g\n> ", e.val.f);
    break;
  default:
    printf("Unknown evaled type: %c\n> ", e.type);
  }
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
