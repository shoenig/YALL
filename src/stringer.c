/* stringer.c
   Seth Hoenig 2012 (seth.a.hoenig@gmail.com)
*/

#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "ast.h"
#include "stringer.h"
#include "utilz.h"

char*
str_evaltype(evaltype e) {
  switch(e.type) {
  case ET_INT: return str_int(e.val.i); break;
  case ET_FLOAT: return str_float(e.val.f); break;
  case ET_BOOL: return str_bool(e.val.bool); break;
  case ET_REFERENCE: return str_ref(e.val.str); break;
  case ET_LIST: return str_list(e.val.list); break;
  default: {
    char* tmp = malloc(sizeof(char) * 128);
    sprintf(tmp, "strERROR (%d)", e.type);
    return tmp;
  }
  }
}

char*
str_int(int64 i) {
  char* nstr = malloc(sizeof(char) * 32);
  sprintf(nstr, "%lld", i);
  return nstr;
}


char*
str_float(float64 f) {
  char* nstr = malloc(sizeof(char) * 32);
  sprintf(nstr, "%4.2f", f);
  return nstr;
}

char*
str_bool(bool b) {
  char* nstr = malloc(sizeof(char) * 6);
  if(!b)
    sprintf(nstr, "false");
  else
    sprintf(nstr, "true");
  return nstr;
}

char*
str_ref(char* str) {
  return str_dup(str);
}

/* this probably leaks memory. */
char*
str_list(List* list) {
  char* nstr = malloc(sizeof(char) * 3);
  AST* ast = list->head;
  while(ast != NULL) {
    nstr = str_ezcat(nstr, " ");
    evaltype e = eval(ast->left);
    nstr = str_ezcat(nstr, str_evaltype(e));
    ast = ast->right;
  }
  nstr[0] = '[';
  nstr = str_ezcat(nstr, "]");
  return nstr;
}
