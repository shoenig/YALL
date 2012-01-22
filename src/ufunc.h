/* ufunc.h
   For storing user-defined functions in the AST
   Seth Hoenig 2012 (seth.a.hoenig@gmail.com)
*/

#ifndef YALL_UFUNC_H
#define YALL_UFUNC_H

#include "ast.h"

typedef struct {
  int argc;
  char** args;
  char* func_name;
  AST* func_body;
} UserFunc;

UserFunc* new_user_func(char*, int, char**, AST*);
void delete_user_func(UserFunc*);

#endif
