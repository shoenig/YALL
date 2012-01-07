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

evaltype
call_builtin(AST* bifcall) {
  evaltype eleft;
  evaltype eright;
  evaltype eret;

  switch(bifcall->e.val.b) {

    /* sqrt (returns F) */
  case B_sqrt: {
    eleft = eval(bifcall->left);
    eret.type = 'F';
    if(eleft.type == 'F')
      eret.val.f = sqrt(eleft.val.f);
    else if(eleft.type == 'I') {
      float64 conv = (float64)eleft.val.i;
      eret.val.f = sqrt(conv);
    } else
      crash("wrong type in sqrt: %c", eleft.type);
    break;
  }

    /* logical negation */
  case B_negate: {
    eleft = eval(bifcall->left);
    eret.type = 'Z';
    if(eleft.type == 'Z')
      eret.val.bool = !eleft.val.bool;
    else
      crash("cannot invert non-boolean: %c", eleft.type);
    break;
  }

    /* abs (returns I or F) */
  case B_abs: {
    eleft = eval(bifcall->left);
    if(eleft.type == 'F') {
      eret.type = 'F';
      eret.val.f = eleft.val.f < 0 ? -eleft.val.f : eleft.val.f;
    } else if (eleft.type == 'I') {
      eret.type = 'I';
      eret.val.i = eleft.val.i < 0 ? -eleft.val.i : eleft.val.i;
    } else
      crash("wrong type in abs: %c", eleft.type);
    break;
  }

    /* log (base e, returns F) */
  case B_log: {
    eleft = eval(bifcall->left);
    eret.type = 'F';
    if(eleft.type == 'F')
      eret.val.f = log(eleft.val.f);
    else if(eleft.type == 'I') {
      float64 conv = (float64)eleft.val.i;
      eret.val.f = log(conv);
    } else
      crash("wrong type in log: %c", eleft.type);
    break;
  }

    /* log10 (base 10, returns F) */
  case B_log10: {
    eleft = eval(bifcall->left);
    eret.type = 'F';
    if(eleft.type == 'F')
      eret.val.f = log10(eleft.val.f);
    else if(eleft.type == 'I') {
      float64 conv = (float64)eleft.val.i;
      eret.val.f = log10(conv);
    } else
      crash("wrong type in log10: %c", eleft.type);
    break;
  }

    /* log2 (base 2, returns F) */
  case B_log2: {
    eleft = eval(bifcall->left);
    eret.type = 'F';
    if(eleft.type == 'F') {
      eret.val.f = (log10(eleft.val.f)/log10(2.0));
    } else if(eleft.type == 'I') {
      float64 conv = (float64)eleft.val.i;
      eret.val.f = (log10(conv)/log10(2.0));
    } else
      crash("wrong type in log10: %c", eleft.type);
    break;
  }

    /* pow (a**b), returns I iff a and b are I, F otherwise */
  case B_pow: {
    eleft = eval(bifcall->left);
    eright = eval(bifcall->right);
    if(eleft.type == 'I' && eright.type == 'I') { /* return an int */
      float64 f = pow(eleft.val.i, eright.val.i);
      int64 i = (int64)f;
      eret.type = 'I';
      eret.val.i = i;
    } else { /* return a float */
      float64 base, power;
      if(eleft.type == 'I')
        base = (float64)eleft.val.i;
      else
        base = eleft.val.f;
      if(eright.type == 'I')
        power = (float64)eright.val.i;
      else
        power = eright.val.f;
      eret.type = 'F';
      eret.val.f = pow(base, power);
    }
    break;
  }
    /* n! if n is an I */
  case B_fact: {
    eleft = eval(bifcall->left);
    if(eleft.type != 'I')
      crash("wrong type in !: %c", eleft.type);
    else {
      int64 i = eleft.val.i;
      int64 v = 1;
      while(i>0)
        v *= i--;
      eret.type = 'I';
      eret.val.i = v;
    }
    break;
  }

    /* returns F if n is F or I */
  case B_float: {
    eleft = eval(bifcall->left);
    if(eleft.type != 'I' && eleft.type != 'F')
      crash("wrong type in conversion to float: %c", eleft.type);
    else {
      /* must be I or F, if it's F, no need to do anything */
      eret.type = 'F';
      if(eleft.type == 'I')
        eret.val.f = (float64)eleft.val.i;
      else
        eret.val.f = eleft.val.f;
    }
    break;
  }

    /* returns I if n is F or I (TRUNCATES F) */
  case B_int: {
    eleft = eval(bifcall->left);
    if(eleft.type != 'I' && eleft.type != 'F')
      crash("wrong type in conversion to int: %c", eleft.type);
    else {
      /* must be I or F, if I, no need to do anything */
      eret.type = 'I';
      if(eleft.type == 'F')
        eret.val.i = (int64)eleft.val.f;
      else
        eret.val.i = eleft.val.i;
    }
    break;
  }

    /* returns floor(F) */
  case B_floor: {
    eleft = eval(bifcall->left);
    eret.type = 'F';
    if(eleft.type == 'F')
      eret.val.f = floor(eleft.val.f);
    else
      crash("wrong type in floor: %c", eleft.type);
    break;
  }

    /* returns ceil(F) */
  case B_ceil: {
    eleft = eval(bifcall->left);
    eret.type = 'F';
    if(eleft.type == 'F')
      eret.val.f = ceil(eleft.val.f);
    else
      crash("wrong type in ceil: %c", eleft.type);
    break;
  }

    /* defines an int */
  case B_defint: {
    if(bifcall->right == NULL)
      crash("need int expression in defint");
    eright = eval(bifcall->right);
    if(eright.type != 'I')
      crash("wrong type in defint: ref: %s, int: %s", eleft.type, eright.type);
    smt_put(get_ref_name(bifcall->left) , new_intval(eright.val.i)); /* don't eval lhs */
    eret.type = 'I';
    eret.val.i = eright.val.i;
    break;
  }

    /* define a float */
  case B_deffloat: {
    if(bifcall->right == NULL)
      crash("need float expression in deffloat");
    eright = eval(bifcall->right);
    if(eright.type != 'F')
      crash("wrong type in deffloat: ref: %s, int: %s", eleft.type, eright.type);
    smt_put(get_ref_name(bifcall->left), new_floatval(eright.val.f)); /* no eval lhs */
    eret.type = 'F';
    eret.val.f = eright.val.f;
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
  res.type = 'Z';
  switch(bftree->e.val.t) {
  case T_not_equal:
    if(l.type != r.type)
      res.val.bool = true;
    else {
      if(l.type=='I')
        res.val.bool = (l.val.i != r.val.i);
      else if(l.type=='F')
        res.val.bool = (l.val.f != r.val.f);
      else if(l.type=='Z')
        res.val.bool = (l.val.bool != r.val.bool);
      else
        crash("invalid type in != : %c", l.type);
    }
    break;
  case T_equal:
    if(l.type != r.type)
      res.val.bool = false;
    else
      if(l.type=='I')
        res.val.bool = (l.val.i == r.val.i);
      else if(l.type=='F')
        res.val.bool = (l.val.f == r.val.f);
      else if(l.type=='Z')
        res.val.bool = (l.val.bool == r.val.bool);
      else
        crash("invalid type in == : %c", l.type);
    break;
  case T_greater_than:
    if(l.type=='I' && r.type=='I')
      res.val.bool = (l.val.i > r.val.i);
    else if(l.type=='F' && r.type=='F')
      res.val.bool = (l.val.f > r.val.f);
    else
      crash("type conflict in > : %c, %c", l.type, r.type);
    break;
  case T_less_than:
    if(l.type=='I' && r.type=='I')
      res.val.bool = (l.val.i < r.val.i);
    else if(l.type=='F' && r.type=='F')
      res.val.bool = (l.val.f < r.val.f);
    else
      crash("type conflict in < : %c, %c", l.type, r.type);
    break;
  case T_greater_than_equal:
    if(l.type=='I' && r.type=='I')
      res.val.bool = (l.val.i >= r.val.i);
    else if(l.type=='F' && r.type=='F')
      res.val.bool = (l.val.f >= r.val.f);
    else
      crash("type conflict in >= : %c, %c", l.type, r.type);
    break;
  case T_less_than_equal:
    if(l.type=='I' && r.type=='I')
      res.val.bool = (l.val.i <= r.val.i);
    else if(l.type=='F' && r.type=='F')
      res.val.bool = (l.val.f <= r.val.f);
    else
      crash("type conflict in <= : %c, %c", l.type, r.type);
    break;
  case T_or:
    if(l.type=='Z' && r.type=='Z')
      res.val.bool = (l.val.bool || r.val.bool);
    else
      crash("non boolean in or  : %c, %c", l.type, r.type);
    break;
  case T_and:
    if(l.type=='Z' && r.type=='Z')
      res.val.bool = (l.val.bool && r.val.bool);
    else
      crash("non boolean in and : %c, %c", l.type, r.type);
    break;

  default:
    crash("invalid boolean function: %d", bftree->e.val.t);
  }

  return res;
}
