/* builtins.c
   Definitions for built-in functions
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#include <stdlib.h>
#include <math.h>
#include "builtins.h"
#include "builtintypes.h"
#include "ast.h"
#include "err.h"


evaltype
call_builtin(AST* bifcall) {
  evaltype e = eval(bifcall->left);

  switch(bifcall->e.val.b) {

    /* sqrt (returns F) */
  case B_sqrt: {
    if(e.type == 'F')
      e.val.f = sqrt(e.val.f);
    else if(e.type == 'I') {
      float64 conv = (float64)e.val.i;
      e.val.f = sqrt(conv);
      e.type = 'F';
    } else {
      yyerror("wrong type in sqrt: %c", e.type);
      exit(EXIT_FAILURE);
    }
    break;
  }

    /* sqrt (returns F or I) */
  case B_abs: {
    if(e.type == 'F')
      e.val.f = e.val.f < 0 ? -e.val.f : e.val.f;
    else if (e.type == 'I')
      e.val.i = e.val.i < 0 ? -e.val.i : e.val.i;
    else {
      yyerror("wrong type in abs: %c", e.type);
      exit(EXIT_FAILURE);
    }
    break;
  }

    /* log (base e, returns F) */
  case B_log: {
    if(e.type == 'F')
      e.val.f = log(e.val.f);
    else if(e.type == 'I') {
      float64 conv = (float64)e.val.i;
      e.val.f = log(conv);
      e.type = 'F';
    } else {
      yyerror("wrong type in log: %c", e.type);
      exit(EXIT_FAILURE);
    }
    break;
  }

    /* log10 (base 10, returns F) */
  case B_log10: {
    if(e.type == 'F')
      e.val.f = log10(e.val.f);
    else if(e.type == 'I') {
      float64 conv = (float64)e.val.i;
      e.val.f = log10(conv);
      e.type = 'F';
    } else {
      yyerror("wrong type in log10: %c", e.type);
      exit(EXIT_FAILURE);
    }
    break;
  }

    /* log2 (base 2, returns F) */
  case B_log2: {
    if(e.type == 'F')
      e.val.f = (log10(e.val.f)/log10(2));
    else if(e.type == 'I') {
      float64 conv = (float64)e.val.i;
      e.val.f = (log10(conv)/log10(2));
      e.type = 'F';
    } else {
      yyerror("wrong type in log10: %c", e.type);
      exit(EXIT_FAILURE);
    }
    break;
  }

    /* pow (a**b), returns I iff a and b are I, F otherwise */
  case B_pow: {
    evaltype r = eval(bifcall->right);
    if(e.type == 'I' && r.type == 'I') { /* return an int */
      float64 f = pow(e.val.i, r.val.i);
      int64 i = (int64)f;
      e.val.i = i;
    } else { /* return a float */
      float64 base, power;
      if(e.type == 'I')
        base = (float64)e.val.i;
      else
        base = e.val.f;
      if(r.type == 'I')
        power = (float64)r.val.i;
      else
        power = r.val.f;
      e.type = 'F';
      e.val.f = pow(base, power);
    }
    break;
  }
    /* n! if n is an I */
  case B_fact: {
    if(e.type != 'I') {
      yyerror("wrong type in !: %c", e.type);
      exit(EXIT_FAILURE);
    } else {
      int64 i = e.val.i;
      int64 v = 1;
      while(i>0)
        v *= i--;
      e.val.i = v;
    }
    break;
  }

    /* returns F if n is F or I */
  case B_float: {
    if(e.type != 'I' && e.type != 'F') {
      yyerror("wrong type in conversion to float: %c", e.type);
      exit(EXIT_FAILURE);
    } else {
      /* must be I or F, if it's F, no need to do anything */
      if(e.type == 'I') {
        e.val.f = (float64)e.val.i;
        e.type = 'F';
      }
    }
    break;
  }

    /* returns I if n is F or I (TRUNCATES F) */
  case B_int: {
    if(e.type != 'I' && e.type != 'F') {
      yyerror("wrong type in conversion to int: %c", e.type);
      exit(EXIT_FAILURE);
    } else {
      /* must be I or F, if I, no need to do anything */
      if(e.type == 'F') {
        e.val.i = (int64)e.val.f;
        e.type = 'I';
      }
    }
    break;
  }

    /* returns floor(F) */
  case B_floor: {
    if(e.type == 'F') {
      e.val.f = floor(e.val.f);
    } else {
      yyerror("wrong type in floor: %c", e.type);
      exit(EXIT_FAILURE);
    }
    break;
  }

    /* returns ceil(F) */
  case B_ceil: {
    if(e.type == 'F') {
      e.val.f = ceil(e.val.f);
    } else {
      yyerror("wrong type in ceil: %c", e.type);
      exit(EXIT_FAILURE);
    }
    break;
  }

  default:
    yyerror("invalid built-in function: %d", bifcall->e.val.b);
    exit(EXIT_FAILURE);
  }
  return e;
}
