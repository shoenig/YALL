/* symtable.h
   Symbol Table
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#ifndef YALL_SYMTABLE_H
#define YALL_SYMTABLE_H
#define TABLESIZE 997

#include "symbol.h"

Symbol* table[TABLESIZE];

void smt_init();
Symbol* smt_lookup(char* name);
void smt_with_entry(char* name, AST* ast);
void smt_with_exit(char* name);
void smt_put(char* name, AST* ast);
void smt_del(char* name);

#endif
