/* builtintypes.h
   These are the possible types for evaltype and AST nodes.
   Don't read into their actual numeric value; I just wanted
   to keep them in separate ranges so as to stay sane while
   debuggining.
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#ifndef YALL_BUILTIN_TYPES_H
#define YALL_BUILTIN_TYPES_H

/* evaluatable types */
typedef enum {
  ET_INVALID = 0,
  ET_INT,
  ET_FLOAT,
  ET_BOOL,
  ET_LIST,
  ET_REFERENCE,
} EVALTYPE;

/* ast types */
typedef enum {
  AST_INVALID = 10,
  AST_INT,
  AST_FLOAT,
  AST_MINUS,
  AST_BOOL,
  AST_LIST,
  AST_LIST_ELEMENT,
  AST_REFERENCE,
  AST_LIST_FUNC,
  AST_BUILTIN_FUNC,
  AST_BOOLEAN_FUNC,
  AST_CALL_USERFUNC,
  AST_AMPER,
  AST_PIPE,
  AST_EQ,
  AST_PLUS,
  AST_SUB,
  AST_MULT,
  AST_DIV
} ASTTYPE;

/* built-in functions */
typedef enum {
  B_sqrt = 40,
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
  B_with,
  B_ufunc,
  B_defufunc
} bif;

/* {in}equalities  */
typedef enum {
  T_greater_than = 70,
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
  L_empty = 80,
  L_len,
  L_peek,
  L_copy,
  L_pop,
  L_push
} listf;

#endif
