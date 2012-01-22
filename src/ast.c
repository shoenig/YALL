/* ast.c
   Definitions for AST
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#include <stdlib.h>
#include "builtintypes.h"
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
new_ast(int ntype, AST* l, AST* r) {
  AST* ast = alloc_ast(sizeof(AST));
  ASTTYPE atype;
  switch(ntype) {
  case '=': atype = AST_EQ; break;
  case '+': atype = AST_PLUS; break;
  case '-': atype = AST_SUB; break;
  case '*': atype = AST_MULT; break;
  case '/': atype = AST_DIV; break;
  case '&': atype = AST_AMPER; break;
  case '|': atype = AST_PIPE; break;
  case 'M': atype = AST_MINUS; break;
  default: atype = AST_INVALID;
  }
  ast->nodetype = atype;
  ast->left = l;
  ast->right = r;
  ast->e.type = atype;
  return ast;
}

AST*
new_floatval(float64 f) {
  AST* fast = alloc_ast(sizeof(AST));
  fast->nodetype = AST_FLOAT;
  fast->e.type = ET_FLOAT;
  fast->e.val.f = f;
  return fast;
}

AST*
new_intval(int64 i) {
  AST* iast = alloc_ast(sizeof(AST));
  iast->nodetype = AST_INT;
  iast->e.type = ET_INT;
  iast->e.val.i = i;
  return iast;
}

AST*
new_boolval(bool b) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = AST_BOOL;
  ast->e.type = ET_BOOL;
  ast->e.val.bool = b;
  return ast;
}

AST*
new_ref(char* refname) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = AST_REFERENCE;
  ast->e.type = ET_REFERENCE;
  ast->e.val.str = refname;
  return ast;
}

AST*
new_bif(bif bif, AST* l, AST* r) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = AST_BUILTIN_FUNC;
  ast->e.val.b = bif;
  ast->left = l;
  ast->right = r;
  return ast;
}

AST*
new_tribif(bif bif, AST* l, AST* r, AST* aux) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = AST_BUILTIN_FUNC;
  ast->e.val.b = bif;
  ast->left = l;
  ast->right = r;
  ast->aux = aux;
  return ast;
}

AST*
new_cmp(cmpf cmp, AST* l, AST* r) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = AST_BOOLEAN_FUNC;
  ast->e.val.t = cmp;
  ast->left = l;
  ast->right = r;
  return ast;
}

AST*
new_listfunc(listf lf, AST* listA, AST* listB) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = AST_LIST_FUNC;
  ast->e.val.t = lf;
  ast->left = listA;
  ast->right = listB;
  return ast;
}

AST* /* a list points to first element */
new_list(AST* e1) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = AST_LIST;
  ast->e.type = ET_LIST;
  ast->e.val.list = list_make(e1);
  ast->left = NULL;
  ast->right = NULL;
  ast->aux = NULL;
  return ast;
}

AST* /* each element points to contents and next element */
new_list_element(AST* e, AST* next) {
  AST* ast = alloc_ast(sizeof(AST));
  ast->nodetype = AST_LIST_ELEMENT;
  ast->left = e;
  ast->right = next;
  return ast;
}

/* AST* */
/* new_call_userfunc(char* funcname, AST* arglist, AST* body) { */
/*   AST* ast = alloc_ast(sizeof(AST)); */
/*   ast->nodetype = AST_USERFUNC; */
/*   ast->e.type = ET_USERFUNC; */
/*   ast->left = arglist; */
/*   ast->right = body; */
/*   return ast; */
/* } */

/* allocate an AST, but only big enough for the type
   of node that is actually going to be created
*/
AST*
alloc_ast(uint64 size) {
  AST* ast = malloc(size);
  ast->nodetype = '#'; /* a warning sign */
  ast->left = NULL;
  ast->right = NULL;
  ast->aux = NULL;
  if(!ast)
    crash("Out of Memory building AST");
  return ast;
}

/* basically assume we have a ref type, which we can't eval because
   the symtable doesn't have anything, we just want the char* name */
char*
get_ref_name(AST* tree) {
  if(tree->nodetype != AST_REFERENCE)
    crash("not a ref type in get_ref_name: %s", astdec(tree->nodetype));
  return tree->e.val.str;
}

/* wrap an evaltype in an AST */
AST*
ast_wrap(evaltype e) {
  AST* tmp;
  switch(e.type) {
  case ET_INT:
    tmp = new_intval(e.val.i);
    break;
  case ET_FLOAT:
    tmp = new_floatval(e.val.f);
    break;
  case ET_BOOL:
    tmp = new_boolval(e.val.bool);
    break;
  case ET_LIST:
    tmp = new_list(e.val.list->head);
    break;
  default:
    tmp = NULL;
    crash("cannot wrap type: (%d) %c", e.type, e.type);
  }
  return tmp;
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
  eret.type = ET_INVALID; /* gcc nonsense */

  if(tree == NULL)
    crash("internal error, null tree in eval");

  switch(tree->nodetype) {
  case AST_INVALID:
  case AST_LIST_ELEMENT:
    crash("invalid ast: %s", astdec(tree->nodetype));
    break;
  case AST_INT: /* int */
    eret.type = ET_INT;
    eret.val.i = tree->e.val.i;
    break;
  case AST_FLOAT: /* float */
    eret.type = ET_FLOAT;
    eret.val.f = tree->e.val.f;
    break;
  case AST_BOOL: /* boolean (true/false) */
    eret.type = ET_BOOL;
    eret.val.bool = tree->e.val.bool;
    break;
  case AST_MINUS: /* int or float */
    eleft = eval(tree->left);
    switch(eleft.type) {
    case ET_INT: eret.type = ET_INT; eret.val.i = -eleft.val.i; break;
    case ET_FLOAT: eret.type = ET_FLOAT; eret.val.f = -eleft.val.f; break;
    default:
      crash("typing error e: %s", astdec(tree->e.type));
    }
    break;

  case AST_LIST: { /* a list */
    eret.type = ET_LIST;
    eret.val.list = tree->e.val.list;
    break;
  }

  case AST_LIST_FUNC: /* a built-in list function */
    eret = call_listfunc(tree);
    break;

  case AST_REFERENCE: {
    Symbol* s;
    /* lookup in the table, if it's not there, crash */
    /* if it is there, evaluate what's in the table, set e to that */
    s = smt_lookup(tree->e.val.str);
    if(!s)
      crash("undefined variable: %s", tree->e.val.str);
    eret = eval(s->ast);
    break;
  }

  case AST_DEF_USERFUNC: {
    break;
  }

  case AST_CALL_USERFUNC: {
    break;
  }

  case AST_BUILTIN_FUNC:
    eret = call_builtin(tree);
    break;

  case AST_BOOLEAN_FUNC:
    eret = call_boolfunc(tree);
    break;

  case AST_AMPER:
  case AST_PIPE: {
    eleft = eval(tree->left);
    eright = eval(tree->right);
    if(eleft.type != ET_INT || eright.type != ET_INT)
      crash("typing error (&,|): le: %s, re: %s",
            etdec(eleft.type), etdec(eright.type));
    eret.type = ET_INT;
    if(tree->nodetype == AST_AMPER)
      eret.val.i = eleft.val.i & eright.val.i;
    else
      eret.val.i = eleft.val.i | eright.val.i;
    break;
  }

  case AST_EQ: {
    Symbol* s;
    /* don't eval left, just want the name */
    s = smt_lookup(tree->left->e.val.str);
    if(!s)
      crash("cannot assign to undeclared variable: %s", s);
    eright = eval(tree->right);  /* value to be assigned */

    /* TODO: fix this using cassert */
    /* if(s->ast->nodetype != eright.type) */
    /*   crash("cannot assign type %s to var of type %s", */
    /*         etdec(eright.type), astdec(s->ast->nodetype)); */

    /* need an ast wrapper */
    AST* tmp = ast_wrap(eright);
    smt_put(s->name, tmp);
    eret.type = eright.type;
    eret = eright;
    break;
  }

    /* BINARY FUNCTION cases that involve type specific operations */
  case AST_PLUS:
  case AST_SUB:
  case AST_MULT:
  case AST_DIV: { /* compiler needs this {} b/c of nested switch */

    evaltype eleft = eval(tree->left);
    evaltype eright = eval(tree->right);

    if(eleft.type != eright.type)
      crash("typing error (+,-,*,/), le: %s, re: %s",
            etdec(eleft.type), etdec(eright.type));

    if(eleft.type == ET_INT)
      eret.type = ET_INT;
    else if(eleft.type == ET_FLOAT)
      eret.type = ET_FLOAT;

    switch(tree->nodetype) {
    case AST_PLUS:
      if(eleft.type == ET_INT)
        eret.val.i = eleft.val.i + eright.val.i;
      else if(eleft.type == ET_FLOAT)
        eret.val.f = eleft.val.f + eright.val.f;
      break;
    case AST_SUB:
      if(eleft.type == ET_INT)
        eret.val.i = eleft.val.i - eright.val.i;
      else if(eleft.type == ET_FLOAT)
        eret.val.f = eleft.val.f - eright.val.f;
      break;
    case AST_MULT:
      if(eleft.type == ET_INT)
        eret.val.i = eleft.val.i * eright.val.i;
      else if(eleft.type == ET_FLOAT)
        eret.val.f = eleft.val.f * eright.val.f;
      break;
    case AST_DIV:
      if(eleft.type == ET_INT)
        eret.val.i = eleft.val.i / eright.val.i;
      else if(eleft.type == ET_FLOAT)
        eret.val.f = eleft.val.f / eright.val.f;
      break;

    default:
      crash("invalid binary built-in function: %s",
            astdec(tree->nodetype));
    }
  }
  }
  if(eret.type == ET_INVALID)
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
  case AST_BUILTIN_FUNC:
    if(tree->aux != NULL)
      freeTREE(tree->aux);

    /* fall-through to types that have TWO subtrees */
  case AST_EQ:
  case AST_PLUS:
  case AST_SUB:
  case AST_MULT:
  case AST_DIV:
  case AST_AMPER:
  case AST_PIPE:
  case AST_BOOLEAN_FUNC: /* truth function */
  case AST_LIST_FUNC: /* list function */
    freeTREE(tree->right);

    /* fall-through to types with ONE subtree */
  case AST_MINUS: /* numeric negation */
    freeTREE(tree->left);
    break;
    /* fall-through to types with NO subtree */
  case AST_FLOAT: /* float */
  case AST_INT: /* int */
  case AST_BOOL: /* boolean */
  case AST_REFERENCE: /* reference name, maybe we should free e.val.str */
    break;

  case AST_LIST: /* list */
    freeTREE(tree->left); /* go to first element */
    break;
  case AST_LIST_ELEMENT: /* list element */
    freeTREE(tree->right); /* go to next element as well */
    freeTREE(tree->left); /* clear this element */
    break;

  default:
    /* you probably added a node type and forgot to add it here */
    crash("internal error in freeing tree, bad node type: (%d) %s ",
          astdec(tree->nodetype), astdec(tree->nodetype));
  }
  /* actually delete this node, (all children have been deleted) */
  free(tree);
}
