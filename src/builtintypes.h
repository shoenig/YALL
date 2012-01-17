/* builtintypes.h
   This would normally be in builtins.h but C can't handle
   the cyclic references on the enum (extern doesn't work).
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#ifndef YALL_BUILTIN_TYPES_H
#define YALL_BUILTIN_TYPES_H

/* built-in functions */
typedef enum {
  B_sqrt = 10,
  B_abs,
  B_log,
  B_log2,
  B_log10,
  B_float,
  B_int,
  B_pow,
  B_fact,
  B_negate,
  B_floor,
  B_ceil,
  B_defint,
  B_deffloat,
  B_defbool,
  B_deflist,
  B_if,
  B_with
} bif;

/* {in}equalities  */
typedef enum {
  T_greater_than = 30,
  T_less_than,
  T_not_equal,
  T_equal,
  T_greater_than_equal,
  T_less_than_equal,
  T_and,
  T_or
} cmpf;

/* list functions */
typedef enum {
  L_empty = 50,
  L_len,
  L_peek,
  L_copy,
  L_pop,
  L_push
} listf;

#endif
