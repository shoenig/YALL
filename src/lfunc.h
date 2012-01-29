/* lfunc.h
   Functions that operate on Lists (as defined in list.h/c
   Seth Hoenig 2012 (seth.a.hoenig@gmail.com)
*/

#ifndef YALL_LFUNC_H
#define YALL_LFUNC_H

#include "ast.h"
#include "list.h"

AST* list_wrap_evaltype_as_element(evaltype);
evaltype list_dup(List* orig);
int list_length(List* list);

#endif
