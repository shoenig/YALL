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
  B_floor,
  B_ceil
} bif;


#endif
