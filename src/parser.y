  /* parser.y
     A parser for YALL
     Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
  */

%{
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "err.h"
#include "stdtype.h"
#include "ast.h"
#include "symbol.h"
%}

%union {
  AST* a;     /* abstract syntax tree */
  float64 f;  /* 64 bit floating point */
  int64 i;    /* 64 bit integral */
  bool b;     /* boolean value */
  char bfunc; /* built in func */
  char tfunc; /* boolean func */
  char* c; /* name, reference name */
  /*  Symbol* s; /* which symbol */
  /*Symlist* sl; /* list o' symbols */
  /*uint64_t func; /* which function */
}

 /* tokens */
%token <f> FLOAT
%token <i> INT
%token <b> BOOLEAN
%token <c> NAME
%token <tfunc> CMP
%token <bfunc> BFUNC
%token EOL
%token PI

 /* associativities */

%nonassoc UMINUS
 /*%nonassoc <fn> CMP
 /*%right '='*/
%left '+' '-'
%left '*' '/'

%type <a> sexp
/* %type <a> list */
/* %type <a> rlist */
 /* explist at some point is <a> */
 /*%type <sl> symlist */

%start yalllist

%%

 /* grammer */

 /* sexp is a symbolic expression */
 /* an exp is an (going to be an) INFIX expression */
sexp: CMP sexp sexp      { $$ = new_cmp($1, $2, $3); }
|     '+' sexp sexp    { $$ = new_ast('+', $2, $3); }
|     '-' sexp sexp    { $$ = new_ast('-', $2, $3); }
|     '*' sexp sexp    { $$ = new_ast('*', $2, $3); }
|     '/' sexp sexp    { $$ = new_ast('/', $2, $3); }
|     '&' sexp sexp    { $$ = new_ast('&', $2, $3); }
|     '|' sexp sexp    { $$ = new_ast('|', $2, $3); }
|      PI              { $$ = new_floatval(3.14159); }
|      FLOAT           { $$ = new_floatval($1); }
|      INT             { $$ = new_intval($1); }
|      BOOLEAN         { $$ = new_boolval($1); }
|      '(' '-' sexp ')'  { $$ = new_ast('M', $3, NULL); }
|     '(' sexp ')'    { $$ = $2; }
|      BFUNC sexp     { $$ = new_bif($1, $2, NULL); }
|      '(' BFUNC sexp sexp ')' { $$ = new_bif($2, $3, $4); }
|    NAME            { $$ = new_ref($1); }
;

 /* yalllist, top level */ 
yalllist: /* nothing */
| yalllist sexp EOL {

  evaltype e = eval($2);
  switch(e.type) {
  case 'I':
    printf("%lld\n", e.val.i);
    break;
  case 'F':
    printf("%4.2f\n", e.val.f);
    break;
  case 'Z':
    if(e.val.bool)
      printf("true\n");
    else
      printf("false\n");
    break;
  default:
    printf("Unknown evaled type: %c\n> ", e.type);
  }
  if(reading_stdin)
    printf("> ");
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
