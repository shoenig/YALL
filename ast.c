/* ast.c
   Definitions for AST
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#include <stdlib.h>
#include "stdtype.h"
#include "ast.h"
#include "err.h"
#include "builtins.h"

AST*
new_ast(char ntype, AST* l, AST* r) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = ntype;
  ast->left = l;
  ast->right = r;
  ast->e.type = ntype;
  return ast;
}

AST*
new_floatval(float64 f) {
  AST* fast = alloc_ast(sizeof(AST));
  fast->nodetype = 'F';
  fast->e.type = 'F';
  fast->e.val.f = f;
  return fast;
}

AST*
new_intval(int64 i) {
  AST* iast = alloc_ast(sizeof(AST));
  iast->nodetype = 'I';
  iast->e.type = 'I';
  iast->e.val.i = i;
  return iast;
}

AST*
new_bif(char bif, AST* l, AST* r) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = 'B';
  ast->e.val.b = bif;
  ast->left = l;
  ast->right = r;
  return ast;
}

AST*
new_cmp(char cmp, AST* l, AST* r) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = 'T';
  ast->e.val.t = cmp;
  ast->left = l;
  ast->right = r;
  return ast;
}

/* allocate an AST, but only big enough for the type
   of node that is actually going to be created
*/
AST*
alloc_ast(uint64 size) {
  AST* ast = malloc(size);
  if(!ast)
    crash("Out of Memory building AST");
  return ast;
}

/* Does type inferenceing.. or something */
/* TODO: pull type checking out of here and do it statically
   rather than at run time, that way we can assume everything
   is okay here and get rid of all this nasty branching */
evaltype
eval(AST* tree) {
  if(!tree)
    crash("internal error, null tree in eval");

  switch(tree->nodetype) {
  case 'I':
    tree->e.type = 'I';
    break;
  case 'F':
    tree->e.type = 'F';
    break;
  case 'M':
    tree->e = eval(tree->left);
    switch(tree->e.type) {
    case 'I': tree->e.val.i = -tree->e.val.i; break;
    case 'F': tree->e.val.f = -tree->e.val.f; break;
    default:
      crash("typing error (I,F,M), e: %c", tree->e.type);
    }
    break;

  case 'B':
    tree->e = call_builtin(tree);
    break;

  case 'T':
    tree->e = call_boolfunc(tree);
    break;

  case '&':
  case '|': {
    evaltype le = eval(tree->left);
    evaltype re = eval(tree->right);
    if(le.type != 'I' || re.type != 'I')
      crash("typing error (&,|): le: %c, re: %c", le.type, re.type);
    tree->e.type = 'I';
    if(tree->nodetype == '&')
      tree->e.val.i = le.val.i & re.val.i;
    else
      tree->e.val.i = le.val.i | re.val.i;
    break;
  }

    /* BINARY FUNCTION cases that involve type specific operations */
  case '+':
  case '-':
  case '*':
  case '/': { /* compiler needs this {} b/c of nested switch */

    evaltype le = eval(tree->left);
    evaltype re = eval(tree->right);

    if(le.type != re.type)
      crash("typing error (+,-,*,/), le: %c, re: %c", le.type, re.type);

    if(le.type == 'I')
      tree->e.type = 'I';
    else if(le.type == 'F')
      tree->e.type = 'F';

    switch(tree->nodetype) {
    case '+':
      if(le.type == 'I')
        tree->e.val.i = le.val.i + re.val.i;
      else if(le.type == 'F')
        tree->e.val.f = le.val.f + re.val.f;
      break;
    case '-':
      if(le.type == 'I')
        tree->e.val.i = le.val.i - re.val.i;
      else if(le.type == 'F')
        tree->e.val.f = le.val.f - re.val.f;
      break;
    case '*':
      if(le.type == 'I')
        tree->e.val.i = le.val.i * re.val.i;
      else if(le.type == 'F')
        tree->e.val.f = le.val.f * re.val.f;
      break;
    case '/':
      if(le.type == 'I')
        tree->e.val.i = le.val.i / re.val.i;
      else if(le.type == 'F')
        tree->e.val.f = le.val.f / re.val.f;
      break;

    default:
      crash("invalid binary built-in function: %c", tree->nodetype);
    }
  }
  }
  return tree->e;
}

void
freeTREE(AST* tree) {
  if(tree == NULL)
    return;

  switch(tree->nodetype) {
    /* types that have THREE subtrees */
    /* (currently there are none */
    
    /* fall-through to types that have TWO subtrees */
  case '+':
  case '-':
  case '*':
  case '/':
  case '&':
  case '|':
  case 'T':
  case 'B':
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
    /* you probably added a node type and forgot to add it here */
    crash("internal error in freeing tree, bad node type: %c",
            tree->nodetype);
  }
  /* actually delete this node, (all children have been deleted) */
  free(tree);
}
