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
%token PI

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
 /* sexp is a symbolic expression */
 /* an exp is an (going to be an) INFIX expression */
sexp: /*exp CMP exp     { $$ = newcmp($2, $1, $3); } */
    '+' sexp sexp     { $$ = new_ast('+', $2, $3); }
|    '-' sexp sexp     { $$ = new_ast('-', $2, $3); }
|    '*' sexp sexp     { $$ = new_ast('*', $2, $3); }
|    '/' sexp sexp     { $$ = new_ast('/', $2, $3); }
|    PI              { $$ = new_floatval(3.14159); }
|    FLOAT           { $$ = new_floatval($1); }
|    INT             { $$ = new_intval($1); }
|    '(' sexp ')'    { $$ = $2; }
|    '(' '-' sexp ')' { $$ = new_ast('M', $3, NULL); }
/*|    NAME            { $$ = newref($1); }*/
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
