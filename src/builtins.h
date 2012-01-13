/* builtins.h
   Prototypes and Meta-info for built-in function in YALL
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#ifndef YALL_BUILTIN_FUNCS_H
#define YALL_BUILTIN_FUNCS_H

#include "ast.h"

evaltype call_builtin(AST* bifcall);
evaltype call_boolfunc(AST* boolcall);
evaltype call_listfunc(AST* listcall);

#endif
