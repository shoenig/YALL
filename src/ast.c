/* ast.c
   Definitions for AST
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/
#include <stdlib.h>
#include "stdtype.h"
#include "ast.h"
#include "err.h"
#include "symbol.h"
#include "symtable.h"
#include "builtins.h"
#include "utilz.h"
#include "typedecoder.h"
#include "list.h"

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
new_boolval(bool b) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = 'Z';
  ast->e.val.bool = b;
  return ast;
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
new_tribif(char bif, AST* l, AST* r, AST* aux) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = 'B';
  ast->e.val.b = bif;
  ast->left = l;
  ast->right = r;
  ast->aux = aux;
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

#include <stdio.h>
AST*
new_lf(char lf, AST* listA, AST* listB) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = 'w';
  ast->e.val.t = lf;
  ast->left = listA;
  ast->right = listB;
  return ast;
}

AST*
new_ref(char* refname) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = 'R';
  ast->e.val.str = refname;
  return ast;
}

AST* /* a list points to first element */
new_list(AST* e1) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = 'L';
  ast->e.val.list = list_make(e1);
  return ast;
}

AST* /* each element points to contents and next element */
new_list_element(AST* e, AST* next) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = 'E';
  ast->left = e;
  ast->right = next;
  return ast;
}

/* allocate an AST, but only big enough for the type
   of node that is actually going to be created
*/
AST*
alloc_ast(uint64 size) {
  AST* ast = malloc(size);
  ast->left = NULL;
  ast->right = NULL;
  ast->aux = NULL;
  if(!ast)
    crash("Out of Memory building AST");
  return ast;
}

/* basically assume we have a ref type, which we can't eval because
   the symtable doesn't have anything, we just want the char* name */
char* get_ref_name(AST* tree) {
  if(tree->nodetype != 'R')
    crash("not a ref type in get_ref_name: %s", type_decode(tree->nodetype));
  return tree->e.val.str;
}

/* Does type inferenceing.. or something */
/* TODO: pull type checking out of here and do it statically
   rather than at run time, that way we can assume everything
   is okay here and get rid of all this nasty branching */
evaltype
eval(AST* tree) {
  evaltype eleft;
  evaltype eright;
  evaltype eret;
  eret.type = '!'; /* gcc nonsense */

  if(tree == NULL)
    crash("internal error, null tree in eval");

  switch(tree->nodetype) {
  case 'I': /* int */
    eret.type = 'I';
    eret.val.i = tree->e.val.i;
    break;
  case 'F': /* float */
    eret.type = 'F';
    eret.val.f = tree->e.val.f;
    break;
  case 'Z': /* boolean (true/false) */
    eret.type = 'Z';
    eret.val.bool = tree->e.val.bool;
    break;
  case 'M': /* int or float */
    eleft = eval(tree->left);
    switch(eleft.type) {
    case 'I': eret.type = 'I'; eret.val.i = -eleft.val.i; break;
    case 'F': eret.type = 'F'; eret.val.f = -eleft.val.f; break;
    default:
      crash("typing error (I,F,M), e: %s", type_decode(tree->e.type));
    }
    break;

  case 'L': { /* a list */
    eret.type = 'L';
    eret.val.list = tree->e.val.list;
    break;
  }

  case 'w': /* a built-in list function */
    eret = call_listfunc(tree);
    break;

  case 'R': {
    Symbol* s;
    /* lookup in the table, if it's not there, crash */
    /* if it is there, evaluate what's in the table, set e to that */
    s = smt_lookup(tree->e.val.str);

    if(!s)
      crash("undefined variable: %s", tree->e.val.str);
    eret = eval(s->ast);
    break;
  }

  case 'B':
    eret = call_builtin(tree);
    break;

  case 'T':
    eret = call_boolfunc(tree);
    break;

  case '&':
  case '|': {
    eleft = eval(tree->left);
    eright = eval(tree->right);
    if(eleft.type != 'I' || eright.type != 'I')
      crash("typing error (&,|): le: %s, re: %s",
            type_decode(eleft.type), type_decode(eright.type));
    eret.type = 'I';
    if(tree->nodetype == '&')
      eret.val.i = eleft.val.i & eright.val.i;
    else
      eret.val.i = eleft.val.i | eright.val.i;
    break;
  }

  case '=': {
    Symbol* s;
    /* don't eval left, just want the name */
    s = smt_lookup(tree->left->e.val.str);
    if(!s)
      crash("cannot assign to undeclared variable: %s", s);
    eright = eval(tree->right);  /* value to be assigned */
    if(s->ast->nodetype != eright.type)
      crash("cannot assign type %s to var of type %s",
            type_decode(eright.type), type_decode(s->ast->nodetype));

    /* need an ast wrapper */
    AST* tmp;
    switch(eright.type) {
    case 'I':
      tmp = new_intval(eright.val.i);
      break;
    case 'F':
      tmp = new_floatval(eright.val.f);
      break;
    case 'Z':
      tmp = new_boolval(eright.val.bool);
      break;
    default:
      tmp = NULL;
      crash("cannot wrap type: (%d) %c", eright.type, eright.type);
    }
    smt_put(s->name, tmp);
    eret.type = eright.type;
    eret = eright;
    break;
  }

    /* BINARY FUNCTION cases that involve type specific operations */
  case '+':
  case '-':
  case '*':
  case '/': { /* compiler needs this {} b/c of nested switch */

    evaltype eleft = eval(tree->left);
    evaltype eright = eval(tree->right);

    if(eleft.type != eright.type)
      crash("typing error (+,-,*,/), le: %s, re: %s",
            type_decode(eleft.type), type_decode(eright.type));

    if(eleft.type == 'I')
      eret.type = 'I';
    else if(eleft.type == 'F')
      eret.type = 'F';

    switch(tree->nodetype) {
    case '+':
      if(eleft.type == 'I')
        eret.val.i = eleft.val.i + eright.val.i;
      else if(eleft.type == 'F')
        eret.val.f = eleft.val.f + eright.val.f;
      break;
    case '-':
      if(eleft.type == 'I')
        eret.val.i = eleft.val.i - eright.val.i;
      else if(eleft.type == 'F')
        eret.val.f = eleft.val.f - eright.val.f;
      break;
    case '*':
      if(eleft.type == 'I')
        eret.val.i = eleft.val.i * eright.val.i;
      else if(eleft.type == 'F')
        eret.val.f = eleft.val.f * eright.val.f;
      break;
    case '/':
      if(eleft.type == 'I')
        eret.val.i = eleft.val.i / eright.val.i;
      else if(eleft.type == 'F')
        eret.val.f = eleft.val.f / eright.val.f;
      break;

    default:
      crash("invalid binary built-in function: %s",
            type_decode(tree->nodetype));
    }
  }
  }
  if(eret.type == '!')
    crash("eval did not do something right, tree->nodetype: (%d) %c",
          tree->nodetype,
          tree->nodetype);
  return eret;
}

void
freeTREE(AST* tree) {
  if(tree == NULL)
    return;

  switch(tree->nodetype) {
    /* various builtin functions */
    /* types that have THREE subtrees */
  case 'B':
    if(tree->aux != NULL)
      freeTREE(tree->aux);

    /* fall-through to types that have TWO subtrees */
  case '=':
  case '+':
  case '-':
  case '*':
  case '/':
  case '&':
  case '|':
  case 'T': /* truth function */
  case 'w': /* list function */
    freeTREE(tree->right);

    /* fall-through to types with ONE subtree */
  case 'M': /* numeric negation */
    freeTREE(tree->left);
    break;
    /* fall-through to types with NO subtree */
  case 'F': /* float */
  case 'I': /* int */
  case 'Z': /* boolean */
  case 'R': /* reference name, maybe we should free e.val.str */
    break;

  case 'L': /* list */
    freeTREE(tree->left); /* go to first element */
    break;
  case 'E': /* list element */
    freeTREE(tree->right); /* go to next element as well */
    freeTREE(tree->left); /* clear this element */
    break;

  default:
    /* you probably added a node type and forgot to add it here */
    crash("internal error in freeing tree, bad node type: (%d) %s ",
          type_decode(tree->nodetype), type_decode(tree->nodetype));
  }
  /* actually delete this node, (all children have been deleted) */
  free(tree);
}
