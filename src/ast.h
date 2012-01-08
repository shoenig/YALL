/* ast.h
   Prototypes and struct definitions for YALL's
   Abstract Syntax Tree
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#ifndef YALL_AST_H
#define YALL_AST_H

#include "stdtype.h"
#include "builtintypes.h"

/* interface to lexer */
extern int yylineno;
void yyerror(char* s, ...);

/* TODO: fix the following nonsense */
/* node types
* + - * / | &
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


/* etypes:
  'I': int64
  'F': float64
  'B': built-in function
  'T': truth value (boolean)
  'R': reference (of symtable)
  other: who cares for now?
*/
typedef struct {
  char type;
  union {
    int64 i;
    float64 f;
    bif b;
    cmpf t;
    bool bool;
    char* str;
  } val;
} evaltype;


/* AST */
typedef struct ast_ {
  char nodetype;
  evaltype e;
  struct ast_* left;
  struct ast_* right;
  struct ast_* aux; /* for ternary things */
} AST;


AST* new_ast(char ntype, AST* l, AST* r);
AST* new_floatval(float64 f);
AST* new_intval(int64 i);
AST* new_boolval(bool b);
AST* new_bif(char bif, AST* l, AST* r);
AST* new_tribif(char bif, AST* l, AST* m, AST* r);
AST* new_cmp(char cmp, AST* l, AST* r);
AST* new_ref(char* ref);

/* allocate an AST given a size */
AST* alloc_ast(uint64 size);

/* returns refname of a 'R' type (without eval-ing) */
char* get_ref_name(AST*);

/* recursively evaluate an AST tree */
evaltype eval(AST*);

/* recursively delete and free a tree of AST* */
void freeTREE(AST*);

#endif
