/* ast.h
   Prototypes and struct definitions for YALL's
   Abstract Syntax Tree
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#ifndef YALL_AST_H
#define YALL_AST_H

#include "stdtype.h"
/* #include "builtin_funcs.h" */

/* interface to lexer */
extern int yylineno;
void yyerror(char* s, ...);

/* TODO: fix the following nonsense */
/* node types
* + - * / |
* 0-7 comparison ops, bit coded 04 equal, 02 less, 01 greater
* M unary minus
* L expression or statement list
* I IF statement
* W WHILE statement
* N symbol ref
* = assignment
* S list of symbols
* F built in function call
* C user function call
*/


/* nodes in the AST all have common initial nodetype (AST) */
/* MUST follow format of AST when casting!! */

/*typedef struct ast AST;*/
typedef struct ast_ {
  char nodetype;
  struct ast_* left;
  struct ast_* right;
} AST;


/* typedef struct fncall { */
/*   uint8_t nodetype; */
/*   AST* left; */
/*   enum builtin_funcs functype; */
/* }; */

typedef struct {
  char nodetype;
  float64 fnumber;
} floatval;

typedef struct {
  char nodetype;
  int64 inumber;
} intval;


AST* new_ast(char ntype, AST* l, AST* r);
AST* new_floatval(float64 f);
AST* new_intval(int64 i);

/* allocate an AST given a size */
AST* alloc_ast(uint64 size);

/* recursively evaluate an AST tree */
float64 eval_f(AST*);
int64 eval_i(AST*);

/* recursively delete and free a tree of AST* */
void freeTREE(AST*);

#endif
