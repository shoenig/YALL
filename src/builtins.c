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
#include "typedecoder.h"

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
      crash("need FLOAT or INT in sqrt, got: %s", type_decode(eleft.type));
    break;
  }

    /* logical negation */
  case B_negate: {
    eleft = eval(bifcall->left);
    eret.type = 'Z';
    if(eleft.type == 'Z')
      eret.val.bool = !eleft.val.bool;
    else
      crash("need BOOLEAN in logical negation, got: %s", type_decode(eleft.type));
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
      crash("need INT or FLOAT in abs, got: %s", type_decode(eleft.type));
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
      crash("need INT or FLOAT in log, got: %s", type_decode(eleft.type));
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
      crash("need INT or FLOAT in log10, got: %s", type_decode(eleft.type));
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
      crash("need INT or FLOAT in log2, got: %s", type_decode(eleft.type));
    break;
  }

    /* pow (a**b), returns I iff a and b are I, F otherwise */
  case B_pow: {
    eleft = eval(bifcall->left);
    eright = eval(bifcall->right);
    if( (eleft.type != 'I' && eleft.type != 'F') ||
        (eright.type != 'I' && eright.type != 'F'))
      crash("need INT or FLOAT in pow, got: %s, %s",
            type_decode(eleft.type), type_decode(eright.type));
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
      crash("need INT in !, got: %s", type_decode(eleft.type));
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
      crash("need INT or FLOAT in float conversion, got: %s", type_decode(eleft.type));
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
      crash("need INT or FLOAT in int conversion, got: %s", type_decode(eleft.type));
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
      crash("need FLOAT in floor, got: %s", type_decode(eleft.type));
    break;
  }

    /* returns ceil(F) */
  case B_ceil: {
    eleft = eval(bifcall->left);
    eret.type = 'F';
    if(eleft.type == 'F')
      eret.val.f = ceil(eleft.val.f);
    else
      crash("need FLOAT in ceil, got: %s", type_decode(eleft.type));
    break;
  }

    /* defines an int */
  case B_defint: {
    if(bifcall->right == NULL)
      crash("need expression in defint");
    eright = eval(bifcall->right);
    if(eright.type != 'I')
      crash("need INT in defint, got: %s", type_decode(eright.type));
    smt_put(get_ref_name(bifcall->left) , new_intval(eright.val.i)); /* don't eval lhs */
    eret.type = 'I';
    eret.val.i = eright.val.i;
    break;
  }

    /* define a float */
  case B_deffloat: {
    if(bifcall->right == NULL)
      crash("need expression in deffloat");
    eright = eval(bifcall->right);
    if(eright.type != 'F')
      crash("need FLOAT in deffloat, got: %s", type_decode(eright.type));
    smt_put(get_ref_name(bifcall->left), new_floatval(eright.val.f)); /* no eval lhs */
    eret.type = 'F';
    eret.val.f = eright.val.f;
    break;
  }

    /* define a boolean */
  case B_defbool: {
    if(bifcall->right == NULL)
      crash("need expression in defbool");
    eright = eval(bifcall->right);
    if(eright.type != 'Z')
      crash("need BOOLEAN in defbool, got: %s", type_decode(eright.type));
    smt_put(get_ref_name(bifcall->left), new_boolval(eright.val.bool)); /* no eval lhs */
    eret.type = 'Z';
    eret.val.bool = eright.val.bool;
    break;
  }

    /* if statement */
  case B_if: {
    if(bifcall->left == NULL || bifcall->right == NULL)
      crash("no contents in if\n");
    eleft = eval(bifcall->left); /* the boolean conditional */
    if(eleft.type != 'Z')
      crash("need BOOLEAN in if conditional, got: %s", type_decode(eleft.type));
    if(eleft.val.bool) { /* execute right (the true condition) */
      eret = eval(bifcall->right);
    } else {
      if(bifcall->aux != NULL) { /* the else branch */
        eret = eval(bifcall->aux);
      } else { /* condition was not true and there was no else, evalute to false for now */
        eret.type = 'Z';
        eret.val.bool = false;
      }
    }
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
        crash("invalid type in != : %s", type_decode(l.type));
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
        crash("invalid type in == : %s", type_decode(l.type));
    break;
  case T_greater_than:
    if(l.type=='I' && r.type=='I')
      res.val.bool = (l.val.i > r.val.i);
    else if(l.type=='F' && r.type=='F')
      res.val.bool = (l.val.f > r.val.f);
    else
      crash("type conflict in > : %s, %s", type_decode(l.type), type_decode(r.type));
    break;
  case T_less_than:
    if(l.type=='I' && r.type=='I')
      res.val.bool = (l.val.i < r.val.i);
    else if(l.type=='F' && r.type=='F')
      res.val.bool = (l.val.f < r.val.f);
    else
      crash("type conflict in < : %s, %s", type_decode(l.type), type_decode(r.type));
    break;
  case T_greater_than_equal:
    if(l.type=='I' && r.type=='I')
      res.val.bool = (l.val.i >= r.val.i);
    else if(l.type=='F' && r.type=='F')
      res.val.bool = (l.val.f >= r.val.f);
    else
      crash("type conflict in >= : %s, %s", type_decode(l.type), type_decode(r.type));
    break;
  case T_less_than_equal:
    if(l.type=='I' && r.type=='I')
      res.val.bool = (l.val.i <= r.val.i);
    else if(l.type=='F' && r.type=='F')
      res.val.bool = (l.val.f <= r.val.f);
    else
      crash("type conflict in <= : %s, %s", type_decode(l.type), type_decode(r.type));
    break;
  case T_or:
    if(l.type=='Z' && r.type=='Z')
      res.val.bool = (l.val.bool || r.val.bool);
    else
      crash("need BOOLEAN in or, got: %s, %s",
            type_decode(l.type), type_decode(r.type));
    break;
  case T_and:
    if(l.type=='Z' && r.type=='Z')
      res.val.bool = (l.val.bool && r.val.bool);
    else
      crash("need BOOLEAN in and, got: %s, %s",
            type_decode(l.type), type_decode(r.type));
    break;

  default:
    crash("invalid boolean function: %d", bftree->e.val.t);
  }

  return res;
}
