/* ast.h
   Prototypes and struct definitions for YALL's
   Abstract Syntax Tree
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#ifndef YALL_AST_H
#define YALL_AST_H

#include "list.h"
#include "stdtype.h"
#include "builtintypes.h"


/* interface to lexer */
extern int yylineno;
void yyerror(char* s, ...);

/* evaltype is what every sexp eventually gets evaluated to. it contains
   info about itself with evaltype.type, which lets you know which value
   from the evaltype.val union you should be looking at. for example, if
   evaltype.type == 'F', then you want evaltype.val.f */
typedef struct {
  char type;
  union {
    int64 i;
    float64 f;
    bool bool;
    List* list;
    char* str;
    bif b; /* todo: rename this */
    cmpf t; /* and this */
    listf lfunc; /* and this as well */
  } val;
} evaltype;

/* AST */
typedef struct ast_ {
  char nodetype;
  evaltype e;
  struct ast_* left;
  struct ast_* right;
  struct ast_* aux; /* for ternary builtins */
} AST;


AST* new_ast(char ntype, AST* l, AST* r);
AST* new_floatval(float64 f);
AST* new_intval(int64 i);
AST* new_boolval(bool b);
AST* new_bif(char bif, AST* l, AST* r);
AST* new_tribif(char bif, AST* l, AST* m, AST* r);
AST* new_lf(char lf, AST* a, AST* b);
AST* new_cmp(char cmp, AST* l, AST* r);
AST* new_ref(char* ref);
AST* new_list(AST* first);
AST* new_list_element(AST* this, AST* next);

/* allocate an AST given a size */
AST* alloc_ast(uint64 size);

/* returns refname of a 'R' type (without eval-ing) */
char* get_ref_name(AST*);

/* recursively evaluate an AST tree */
evaltype eval(AST*);

/* recursively delete and free a tree of AST* */
void freeTREE(AST*);

#endif
