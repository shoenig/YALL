/* functable.h
   Function Table
   Seth Hoenig 2012 (seth.a.hoenig@gmail.com)
*/

#ifndef YALL_FUNCTABLE_H
#define YALL_FUNCTABLE_H
#define UFT_SIZE 97

#include "ufunc.h"

UserFunc* uft_table[UFT_SIZE];

void uft_init();
void uft_put(UserFunc* uf); /* todo, have a separate table for functions */
UserFunc* uft_lookup(char* fname);

#endif
