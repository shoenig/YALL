/* ast.c
   Definitions for AST
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#include <stdlib.h>
#include "stdtype.h"
#include "ast.h"
#include "err.h"

AST*
new_ast(char ntype, AST* l, AST* r) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = ntype;
  ast->left = l;
  ast->right = r;
  return ast;
}

AST*
new_floatval(float64 f) {
  floatval* fv = (floatval*)alloc_ast(sizeof(floatval));
  fv->nodetype = 'F';
  fv->fnumber = f;
  return ((AST*)fv);
}

AST*
new_intval(int64 i) {
  intval* iv = (intval*)alloc_ast(sizeof(intval));
  iv->nodetype = 'I';
  iv->inumber = i;
  return ((AST*)iv);
}

/* allocate an AST, but only big enough for the type
   of node that is actually going to be created
*/
AST*
alloc_ast(uint64 size) {
  AST* ast = malloc(size);
  if(!ast) {
    yyerror("Heap Out of Memory building AST");
    exit(EXIT_FAILURE);
  }
  return ast;
}


/* evalute an AST that results in a float */
float64
eval_f(AST* tree) {
  float64 f;

  if(!tree) {
    yyerror("internal error, null tree in execute_f");
    exit(EXIT_FAILURE);
  }

  switch(tree->nodetype) {
    /* float */
  case 'F': f = ((floatval*)tree)->fnumber; break;
    /* int */
  case 'I': f = ((float) ((intval*)tree)->inumber); break;
    /* unary minus */
  case 'M': f = -eval_f(tree->left); break;
    /* expressions (int is upgrtreeded to float) */
  case '+': f = eval_f(tree->left) + eval_f(tree->right); break;
  case '-': f = eval_f(tree->left) - eval_f(tree->right); break;
  case '*': f = eval_f(tree->left) * eval_f(tree->right); break;
  case '/': f = eval_f(tree->left) / eval_f(tree->right); break;

  default:
    yyerror("internal error, bad node type: %c", tree->nodetype);
    exit(EXIT_FAILURE);
  }   

  return f;
}

int64
eval_i(AST* tree) {
  int64 i;

  if(!tree) {
    yyerror("internal error, null tree in eval_i");
    exit(EXIT_FAILURE);
  }

  switch(tree->nodetype) {
    /* float (cast to int) */
  case 'F': i = ((int) ((floatval*)tree)->fnumber); break;
    /* int */
  case 'I': i = ((intval*)tree)->inumber; break;
    /* unary minus */
  case 'M': i = -eval_i(tree->left); break;
  /* expressions (float is downgraded to int...) */
  case '+': i = eval_i(tree->left) + eval_i(tree->right); break;
  case '-': i = eval_i(tree->left) - eval_i(tree->right); break;
  case '*': i = eval_i(tree->left) * eval_i(tree->right); break;
  case '/': i = eval_i(tree->left) / eval_i(tree->right); break;

  default:
    yyerror("internal error, bad node type: %c", tree->nodetype);
    exit(EXIT_FAILURE);
  }
}

void
freeTREE(AST* tree) {
  switch(tree->nodetype) {
    /* types that have THREE subtrees */
    /* (currently there are none */
    
    /* fall-through to types that have TWO subtrees */
  case '+':
  case '-':
  case '*':
  case '/':
    freeTREE(tree->right);

    /* fall-through to types with ONE subtree */
  case 'M':
    freeTREE(tree->left);
    break;
    /* fall-through to types with NO subtree */
  case 'F':
  case 'I':
    break;

  default:
    yyerror("internal error in freeing tree, bad node type: %c",
            tree->nodetype);
    exit(EXIT_FAILURE);
  }
  /* actually delete this node, (all children have been deleted) */
  free(tree);
}
