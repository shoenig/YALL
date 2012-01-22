/* ufunc.c
   Seth Hoenig 2012 (seth.a.hoenig@gmail.com)
*/

#include <stdlib.h>
#include "ufunc.h"

UserFunc*
new_user_func(char* fname, int argc, char** args, AST* body) {
  UserFunc* uf = malloc(sizeof(UserFunc));
  uf->argc = argc;
  uf->args = args;
  uf->func_name = fname;
  uf->func_body = dup_ast(body);
  return uf;
}

void
delete_user_func(UserFunc* uf) {
  int i;
  for(i=0; i<uf->argc; i++)
    free(uf->args[i]);
  free(uf->args);
  free(uf->func_name);
  free(uf->func_body);
}
