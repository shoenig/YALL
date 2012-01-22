/* funcuft_table.c
   Seth Hoenig 2012 (seth.a.hoenig@gmail.com)
*/

#include <stdlib.h>
#include <string.h>
#include "ufunc.h"
#include "functable.h"

void
uft_init() {
  size_t idx = 0;
  while(idx < UFT_SIZE) {
    uft_table[idx] = NULL;
    idx++;
  }
}

UserFunc*
uft_lookup(char* fname) {
  size_t idx = 0;
  while(idx < UFT_SIZE) {
    if(uft_table[idx] && strcmp(fname, uft_table[idx]->func_name) == 0)
      return uft_table[idx];
    idx++;
  }
  return NULL; /* function fname not found */
}

void
uft_put(UserFunc* uf) {
  UserFunc* temp = uft_lookup(uf->func_name);
  if(!temp) {
    /* find an empty slot and stick it in */
  } else { /* out with the old, in with the new */
    delete_user_func(temp);
    temp->argc = uf->argc;
    temp->args = uf->args;
    temp->func_name = uf->func_name;
    temp->func_body = uf->func_body;
  }
}
