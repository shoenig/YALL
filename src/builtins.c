/* builtins.c
   Definitions for built-in functions
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#include <stdlib.h>
#include <math.h>
#include "ast.h"
#include "builtins.h"
#include "builtintypes.h"
#include "err.h"
#include "symtable.h"
#include "ufunc.h"
#include "functable.h"
#include "typedecoder.h"

evaltype
call_builtin(AST* bifcall) {
  evaltype eleft;
  evaltype eright;
  evaltype eret;
  eret.type = ET_INVALID; /* make gcc a happy compiler */

  switch(bifcall->e.val.b) {

    /* sqrt (returns F) */
  case B_sqrt: {
    eleft = eval(bifcall->left);
    eret.type = ET_FLOAT;
    if(eleft.type == ET_FLOAT)
      eret.val.f = sqrt(eleft.val.f);
    else if(eleft.type == ET_INT) {
      float64 conv = (float64)eleft.val.i;
      eret.val.f = sqrt(conv);
    } else
      crash("need FLOAT or INT in sqrt, got: %s", etdec(eleft));
    break;
  }

    /* logical negation */
  case B_negate: {
    eleft = eval(bifcall->left);
    eret.type = ET_BOOL;
    if(eleft.type == ET_BOOL)
      eret.val.bool = !eleft.val.bool;
    else
      crash("need BOOLEAN in logical negation, got: %s", etdec(eleft));
    break;
  }

    /* abs (returns I or F) */
  case B_abs: {
    eleft = eval(bifcall->left);
    if(eleft.type == ET_FLOAT) {
      eret.type = ET_FLOAT;
      eret.val.f = eleft.val.f < 0 ? -eleft.val.f : eleft.val.f;
    } else if (eleft.type == ET_INT) {
      eret.type = ET_INT;
      eret.val.i = eleft.val.i < 0 ? -eleft.val.i : eleft.val.i;
    } else
      crash("need INT or FLOAT in abs, got: %s", etdec(eleft));
    break;
  }

    /* log (base e, returns F) */
  case B_log: {
    eleft = eval(bifcall->left);
    eret.type = ET_FLOAT;
    if(eleft.type == ET_FLOAT)
      eret.val.f = log(eleft.val.f);
    else if(eleft.type == ET_INT) {
      float64 conv = (float64)eleft.val.i;
      eret.val.f = log(conv);
    } else
      crash("need INT or FLOAT in log, got: %s", etdec(eleft));
    break;
  }

    /* log10 (base 10, returns F) */
  case B_log10: {
    eleft = eval(bifcall->left);
    eret.type = ET_FLOAT;
    if(eleft.type == ET_FLOAT)
      eret.val.f = log10(eleft.val.f);
    else if(eleft.type == ET_INT) {
      float64 conv = (float64)eleft.val.i;
      eret.val.f = log10(conv);
    } else
      crash("need INT or FLOAT in log10, got: %s", etdec(eleft));
    break;
  }

    /* log2 (base 2, returns F) */
  case B_log2: {
    eleft = eval(bifcall->left);
    eret.type = ET_FLOAT;
    if(eleft.type == ET_FLOAT) {
      eret.val.f = (log10(eleft.val.f)/log10(2.0));
    } else if(eleft.type == ET_INT) {
      float64 conv = (float64)eleft.val.i;
      eret.val.f = (log10(conv)/log10(2.0));
    } else
      crash("need INT or FLOAT in log2, got: %s", etdec(eleft));
    break;
  }

    /* pow (a**b), returns I iff a and b are I, F otherwise */
  case B_pow: {
    eleft = eval(bifcall->left);
    eright = eval(bifcall->right);
    if( (eleft.type != ET_INT && eleft.type != ET_FLOAT) ||
        (eright.type != ET_INT && eright.type != ET_FLOAT))
      crash("need INT or FLOAT in pow, got: %s, %s",
            etdec(eleft), etdec(eright));
    if(eleft.type == ET_INT && eright.type == ET_INT) { /* return an int */
      float64 f = pow(eleft.val.i, eright.val.i);
      int64 i = (int64)f;
      eret.type = ET_INT;
      eret.val.i = i;
    } else { /* return a float */
      float64 base, power;
      if(eleft.type == ET_INT)
        base = (float64)eleft.val.i;
      else
        base = eleft.val.f;
      if(eright.type == ET_INT)
        power = (float64)eright.val.i;
      else
        power = eright.val.f;
      eret.type = ET_FLOAT;
      eret.val.f = pow(base, power);
    }
    break;
  }
    /* n! if n is an I */
  case B_fact: {
    eleft = eval(bifcall->left);
    if(eleft.type != ET_INT)
      crash("need INT in !, got: %s", etdec(eleft));
    else {
      int64 i = eleft.val.i;
      int64 v = 1;
      while(i>0)
        v *= i--;
      eret.type = ET_INT;
      eret.val.i = v;
    }
    break;
  }

    /* returns F if n is F or I */
  case B_float: {
    eleft = eval(bifcall->left);
    if(eleft.type != ET_INT && eleft.type != ET_FLOAT)
      crash("need INT or FLOAT in float conversion, got: %s",
            etdec(eleft));
    else {
      /* must be I or F, if it's F, no need to do anything */
      eret.type = ET_FLOAT;
      if(eleft.type == ET_INT)
        eret.val.f = (float64)eleft.val.i;
      else
        eret.val.f = eleft.val.f;
    }
    break;
  }

    /* returns I if n is F or I (TRUNCATES F) */
  case B_int: {
    eleft = eval(bifcall->left);
    if(eleft.type != ET_INT && eleft.type != ET_FLOAT)
      crash("need INT or FLOAT in int conversion, got: %s",
            etdec(eleft));
    else {
      /* must be I or F, if I, no need to do anything */
      eret.type = ET_INT;
      if(eleft.type == ET_FLOAT)
        eret.val.i = (int64)eleft.val.f;
      else
        eret.val.i = eleft.val.i;
    }
    break;
  }

    /* returns floor(F) */
  case B_floor: {
    eleft = eval(bifcall->left);
    eret.type = ET_FLOAT;
    if(eleft.type == ET_FLOAT)
      eret.val.f = floor(eleft.val.f);
    else
      crash("need FLOAT in floor, got: %s", etdec(eleft));
    break;
  }

    /* returns ceil(F) */
  case B_ceil: {
    eleft = eval(bifcall->left);
    eret.type = ET_FLOAT;
    if(eleft.type == ET_FLOAT)
      eret.val.f = ceil(eleft.val.f);
    else
      crash("need FLOAT in ceil, got: %s", etdec(eleft));
    break;
  }

    /* defines an int */
  case B_defint: {
    if(bifcall->right == NULL)
      crash("need expression in defint");
    eright = eval(bifcall->right);
    if(eright.type != ET_INT)
      crash("need INT in defint, got: %s", etdec(eright));
    smt_put(get_ref_name(bifcall->left) , new_intval(eright.val.i));
    eret.type = ET_INT;
    eret.val.i = eright.val.i;
    break;
  }

    /* define a float */
  case B_deffloat: {
    if(bifcall->right == NULL)
      crash("need expression in deffloat");
    eright = eval(bifcall->right);
    if(eright.type != ET_FLOAT)
      crash("need FLOAT in deffloat, got: %s", etdec(eright));
    smt_put(get_ref_name(bifcall->left), new_floatval(eright.val.f));
    eret.type = ET_FLOAT;
    eret.val.f = eright.val.f;
    break;
  }

    /* define a boolean */
  case B_defbool: {
    if(bifcall->right == NULL)
      crash("need expression in defbool");
    eright = eval(bifcall->right);
    if(eright.type != ET_BOOL)
      crash("need BOOLEAN in defbool, got: %s", etdec(eright));
    smt_put(get_ref_name(bifcall->left), new_boolval(eright.val.bool));
    eret.type = ET_BOOL;
    eret.val.bool = eright.val.bool;
    break;
  }

    /* define a list */
  case B_deflist: {
    if(bifcall->right == NULL)
      crash("need expression in deflist");
    eright = eval(bifcall->right);
    if(eright.type != ET_LIST)
      crash("need LIST in deflist, got: %s", etdec(eright));
    smt_put(get_ref_name(bifcall->left), new_list(eright.val.list->head));
    eret.type = ET_LIST;
    eret.val.list = eright.val.list;
    break;
  }

    /* if statement */
  case B_if: {
    if(bifcall->left == NULL || bifcall->right == NULL)
      crash("no contents in if\n");
    eleft = eval(bifcall->left); /* the boolean conditional */
    if(eleft.type != ET_BOOL)
      crash("need BOOLEAN in if conditional, got: %s",
            etdec(eleft));
    if(eleft.val.bool) { /* execute right (the true condition) */
      eret = eval(bifcall->right);
    } else {
      if(bifcall->aux != NULL) { /* the else branch */
        eret = eval(bifcall->aux);
      } else { /* condition was not true and there was no else, false*/
        eret.type = ET_BOOL;
        eret.val.bool = false;
      }
    }
    break;
  }
    /* with statement TODO: better checking */
  case B_with: {
    yasrt((!!bifcall->left) && (bifcall->left->nodetype == AST_LIST),
          "need LIST in lhs of with statement");
    yasrt((!!bifcall->right), "need expression in with statement");

    /* need to setup symbol table */
    AST* outerlist_ast = (bifcall->left);
    evaltype outerlistetype = eval(outerlist_ast);
    List* outerlist = outerlistetype.val.list;

    AST* innerlist_ast = outerlist->head;
    while(innerlist_ast) {
      List* innerlist = innerlist_ast->left->e.val.list;
      if(!innerlist->head)
        goto no_args; /* please forgive me */
      char* lhs =    get_ref_name(innerlist->head->left);
      AST* rhs = innerlist->head->right->left;
      smt_with_entry(lhs, rhs);
      innerlist_ast = innerlist_ast->right;
    }

    eret = eval(bifcall->right);

    /* need to tear down symbol table */
    innerlist_ast = outerlist->head;
    while(innerlist_ast) {
      List* innerlist = innerlist_ast->left->e.val.list;
      char* lhs = get_ref_name(innerlist->head->left);
      smt_with_exit(lhs);
      innerlist_ast = innerlist_ast->right;
    }
    goto finished;

    no_args:
    eret = eval(bifcall->right);

    finished:

    break;
  }

    /* create and store a new user-defined function. need to keep track of the names
       of the variables. (so that we know how to handle their scope later on) */
  case B_defufunc: {
    yasrt( (bifcall->left->nodetype == AST_REFERENCE) ,
           "defining a function requires valid function name");
    char* fname = get_ref_name(bifcall->left);
    /* handle args list */
    /*evaltype listeval = eval(bifcall->right); // CANNOT call eval, b/c vars are dummy refs! */
    if(!bifcall->right || bifcall->right->nodetype != AST_LIST)
      crash("defining a function requires parameter list");
    if(!bifcall->aux)
      crash("defining a function requires a function body");
    List* arglist = bifcall->right->e.val.list; // oh snap, a list with unevaluated elements
    AST* temp = arglist->head;
    int argc = 0;
    while(temp) {
      AST* content = temp->left;
      if(content->nodetype != AST_REFERENCE)
        crash("arglist must contain only references");
      argc++;
      temp = temp->right;
    }
    char** reflist = malloc(sizeof(char*) * argc);
    temp = arglist->head;
    int i = 0;
    while(temp) {
      char* ref = get_ref_name(temp->left);
      reflist[i] = ref;
      temp = temp->right;
      i++;
    }

    AST* fbody = bifcall->aux;
    UserFunc* uf = new_user_func(fname, argc, reflist, fbody);
    uft_put(uf);

    eret.type = ET_BOOL;
    eret.val.bool = false;
    break;
  }

  default:
    crash("invalid built-in function: %d", bifcall->e.val.b);
  }
  return eret;
}

evaltype
call_boolfunc(AST* bftree) {
  evaltype l = eval(bftree->left);
  evaltype r = eval(bftree->right);
  evaltype res;
  res.type = ET_BOOL;
  switch(bftree->e.val.t) {
  case T_not_equal:
    if(l.type != r.type)
      res.val.bool = true;
    else {
      if(l.type == ET_INT)
        res.val.bool = (l.val.i != r.val.i);
      else if(l.type == ET_FLOAT)
        res.val.bool = (l.val.f != r.val.f);
      else if(l.type == ET_BOOL)
        res.val.bool = (l.val.bool != r.val.bool);
      else
        crash("invalid type in != : %s", etdec(l));
    }
    break;
  case T_equal:
    if(l.type != r.type)
      res.val.bool = false;
    else
      if(l.type == ET_INT)
        res.val.bool = (l.val.i == r.val.i);
      else if(l.type == ET_FLOAT)
        res.val.bool = (l.val.f == r.val.f);
      else if(l.type == ET_BOOL)
        res.val.bool = (l.val.bool == r.val.bool);
      else
        crash("invalid type in == : %s", etdec(l));
    break;
  case T_greater_than:
    if(l.type == ET_INT && r.type == ET_INT)
      res.val.bool = (l.val.i > r.val.i);
    else if(l.type == ET_FLOAT && r.type == ET_FLOAT)
      res.val.bool = (l.val.f > r.val.f);
    else
      crash("type conflict in > : %s, %s",
            etdec(l),
            etdec(r));
    break;
  case T_less_than:
    if(l.type == ET_INT && r.type == ET_INT)
      res.val.bool = (l.val.i < r.val.i);
    else if(l.type == ET_FLOAT && r.type == ET_FLOAT)
      res.val.bool = (l.val.f < r.val.f);
    else
      crash("type conflict in < : %s, %s",
            etdec(l),
            etdec(r));
    break;
  case T_greater_than_equal:
    if(l.type == ET_INT && r.type == ET_INT)
      res.val.bool = (l.val.i >= r.val.i);
    else if(l.type == ET_FLOAT && r.type == ET_FLOAT)
      res.val.bool = (l.val.f >= r.val.f);
    else
      crash("type conflict in >= : %s, %s",
            etdec(l),
            etdec(r));
    break;
  case T_less_than_equal:
    if(l.type == ET_INT && r.type == ET_INT)
      res.val.bool = (l.val.i <= r.val.i);
    else if(l.type == ET_FLOAT && r.type == ET_FLOAT)
      res.val.bool = (l.val.f <= r.val.f);
    else
      crash("type conflict in <= : %s, %s",
            etdec(l),
            etdec(r));
    break;
  case T_or:
    if(l.type == ET_BOOL && r.type == ET_BOOL)
      res.val.bool = (l.val.bool || r.val.bool);
    else
      crash("need BOOLEAN in or, got: %s, %s",
            etdec(l), etdec(r));
    break;
  case T_and:
    if(l.type == ET_BOOL && r.type == ET_BOOL)
      res.val.bool = (l.val.bool && r.val.bool);
    else
      crash("need BOOLEAN in and, got: %s, %s",
            etdec(l), etdec(r));
    break;

  default:
    crash("invalid boolean function: %d", bftree->e.val.t);
  }

  return res;
}

AST*
_wrap_evaltype_as_element(evaltype e) {
  AST* inner = ast_wrap(e);
  AST* elem = malloc(sizeof(AST));
  elem->nodetype = AST_LIST_ELEMENT;
  elem->left = inner;
  /* elem->right gets set later (to next element) */
  return elem;
}

AST*
_list_dup_element(AST* orig_elm) {
  if(orig_elm) {
    if(orig_elm->nodetype != AST_LIST_ELEMENT) {
      crash("cannot duplicate non list element (%d) %c",
            orig_elm->nodetype,
            orig_elm->nodetype);
    }
    AST* new = malloc(sizeof(AST));
    new->nodetype = AST_LIST_ELEMENT;
    new->e = orig_elm->e;
    new->left = dup_ast(orig_elm->left);
    new->aux = dup_ast(orig_elm->right);
    new->right = _list_dup_element(orig_elm->right);
    return new;
  } else
    return NULL;
}

evaltype
_list_copy(List* orig) {
  evaltype ret;
  ret.type = ET_LIST;
  ret.val.list = malloc(sizeof(List));
  ret.val.list->head = _list_dup_element(orig->head); /* kick off recursion */
  return ret;
}

evaltype
call_listfunc(AST* listfunc) {
  /*  evaltype right;*/
  evaltype eleft;
  evaltype eret;
  eret.type = ET_INVALID; /* rid of gcc warnings */

  switch(listfunc->e.val.lfunc) {
    /* true if list is empty, false otherwise */
  case L_empty: {
    eleft = eval(listfunc->left);
    if(!eleft.type == ET_LIST)
      crash("need LIST in empty, got: %s", etdec(eleft));
    eret.type = ET_BOOL;
    eret.val.bool = (eleft.val.list->head) ? false : true;
    break;
  }

  case L_len: {
    eleft = eval(listfunc->left);
    if(eleft.type != ET_LIST)
      crash("need LIST in size, got: %s", etdec(eleft));
    eret.type = ET_INT;
    int size = 0;
    AST* elem_ptr = eleft.val.list->head;
    while(elem_ptr != NULL) {
      size++;
      elem_ptr = elem_ptr->right;
    }
    eret.val.i = size;
    break;
  }

  case L_peek: {
    eleft = eval(listfunc->left);
    if(eleft.type != ET_LIST)
      crash("need LIST in peek, got: %s", etdec(eleft));
    if(!eleft.val.list->head)
      crash("cannot peek empty list");
    eret = eval(eleft.val.list->head->left);
    break;
  }

  case L_copy: {
    eleft = eval(listfunc->left);
    if(eleft.type != ET_LIST)
      crash("need LIST in copy, got: %s", etdec(eleft));
    eret = _list_copy(eleft.val.list);
    break;
  }

  case L_pop: {
    eleft = eval(listfunc->left);
    if(eleft.type != ET_LIST)
      crash("need LIST in pop, got: %s", etdec(eleft));
    eret = _list_copy(eleft.val.list);
    if(!eret.val.list->head)
      crash("cannot pop empty LIST");
    else
      eret.val.list->head = eret.val.list->head->right;
    break;
  }

  case L_push: {
    evaltype eright; /* r is list, l is value to push */
    eright = eval(listfunc->right);
    if(eright.type != ET_LIST)
      crash("need LIST in push, got: %s", etdec(eright));
    eret = _list_copy(eright.val.list);
    if(eret.val.list->head == NULL) {
      eret.val.list->head = _wrap_evaltype_as_element(eval(listfunc->left));
      eret.val.list->head->right = NULL;
    } else {
      AST* nhead = _wrap_evaltype_as_element(eval(listfunc->left));
      nhead->right = eret.val.list->head;
      eret.val.list->head = nhead;
    }
    break;
  }

  default:
    crash("invalid list function: %d", listfunc->e.val.lfunc);
  }
  return eret;
}
