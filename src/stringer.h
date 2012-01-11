/* stringer.h
   Turn things into strings 
   Seth Hoenig 2012 (seth.a.hoenig@gmail.com)
*/

#ifndef YALL_STRINGER_H
#define YALL_STRINGER_H

#include "ast.h"
#include "stdtype.h"

char* str_evaltype(evaltype);
char* str_int(int64);
char* str_float(float64);
char* str_bool(bool b);
char* str_ref(char* str);
char* str_list(List*);

#endif
