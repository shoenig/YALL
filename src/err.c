/* err.c
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "err.h"
#include "globals.h"

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

/* will halt program execution
   TODO: resume execution if we're in repl mode */
void
crash(char* s, ...) {
  va_list ap;
  va_start(ap, s);
  fprintf(stderr, "FATAL ERROR: ");
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
  exit(EXIT_FAILURE);
}

/* if b is NOT true, warn on s, ...
   if we are running a file, crash program */
void
yasrt(bool b, char* s, ...) {
  if(!b) {
    va_list ap;
    va_start(ap, s);
    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
    if(!reading_stdin)
      exit(EXIT_FAILURE);
  }
}
