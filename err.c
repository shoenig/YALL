/* err.c
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#include <stdio.h>
#include <stdarg.h>
#include "err.h"

/* error printing.
   don't worry about this, it works fine */
void
yyerror(char* s, ...) {
  va_list ap;
  va_start(ap, s);
  fprintf(stderr, "error: ");
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}
