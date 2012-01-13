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
#include "stringer.h"
%}

%union {
  AST* ast;   /* abstract syntax tree */
  float64 f;  /* 64 bit floating point */
  int64 i;    /* 64 bit integral */
  bool b;     /* boolean value */
  char bfunc; /* built in func */
  char tfunc; /* boolean func */
  char lfunc; /* list func */
  char* c;    /* name, reference name */
}

 /* tokens */
%token <f> FLOAT
%token <i> INT
%token <b> BOOLEAN
%token <c> NAME
%token <tfunc> CMP
%token <bfunc> BFUNC
%token <lfunc> LFUNC
%token EOL
%token PI

 /* associativities */

%nonassoc UMINUS
%left '='
%left '+' '-'
%left '*' '/'

%type <ast> sexp
%type <ast> list

%start yalllist

%%

 /* grammer (consists of sexp, list, and yalllist) */
 /* sexp is a symbolic expression */
sexp: CMP sexp sexp                 { $$ = new_cmp($1, $2, $3); }
|     '=' sexp sexp                 { $$ = new_ast('=', $2, $3); }
|     '+' sexp sexp                 { $$ = new_ast('+', $2, $3); }
|     '-' sexp sexp                 { $$ = new_ast('-', $2, $3); }
|     '*' sexp sexp                 { $$ = new_ast('*', $2, $3); }
|     '/' sexp sexp                 { $$ = new_ast('/', $2, $3); }
|     '&' sexp sexp                 { $$ = new_ast('&', $2, $3); }
|     '|' sexp sexp                 { $$ = new_ast('|', $2, $3); }
|      PI                           { $$ = new_floatval(3.14159); }
|      FLOAT                        { $$ = new_floatval($1); }
|      INT                          { $$ = new_intval($1); }
|      BOOLEAN                      { $$ = new_boolval($1); }
|      '(' '-' sexp ')'             { $$ = new_ast('M', $3, NULL); }
|      '(' sexp ')'                 { $$ = $2; }
|      '(' BFUNC sexp ')'           { $$ = new_bif($2, $3, NULL); }
|      '(' BFUNC sexp sexp ')'      { $$ = new_bif($2, $3, $4); }
|      '(' BFUNC sexp sexp sexp ')' { $$ = new_tribif($2, $3, $4, $5); }
|      NAME                         { $$ = new_ref($1); }
|      '[' list ']'                 { $$ = new_list($2); }
|      '(' LFUNC sexp ')'   { $$ = new_lf($2, $3, NULL); }
;

 /* list (of the form: [a, b, c]) */
list: { $$ = NULL; } /* allow for empty list, ie: [] */
| sexp list { $$ = new_list_element($1, $2); }
;


 /* yalllist, top level */ 
yalllist: /* empty */
| yalllist sexp EOL {

  evaltype e = eval($2);
  char* str = str_evaltype(e);
  printf("%s\n", str);
  free(str);

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
