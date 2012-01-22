/* symbol.h
   Symbol
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#ifndef YALL_SYMBOL_H
#define YALL_SYMBOL_H

#include "ast.h"

typedef struct sym_ {
  char* name;
  AST* ast;
  bool isFunc;
  struct sym_* next;
} Symbol;

#endif
