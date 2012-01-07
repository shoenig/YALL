/* symtable.c
   Implementation of a Symbol Table
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#include <string.h>
#include "err.h"
#include "symtable.h"

/* basic hash-a-string */
/* not needed until proper implementation is used
uint64 symhash(char* symname) {
  uint64 hash = 0;
  uint64 c;
  while((c = *symname++))
    hash = hash*9 ^ c;
  return hash;
}
*/

/* Crappy Implementation */
void smt_init() {
  size_t idx = 0;
  while(idx < TABLESIZE) {
    table[idx].name = NULL;
    table[idx].ast = NULL;
    idx++;
  }
}

Symbol* smt_lookup(char* name) {
  size_t idx = 0;
  while(idx < TABLESIZE) {
    if(table[idx].name && strcmp(name, table[idx].name) == 0)
      return &table[idx];
    idx++;
  }
  return NULL; /* not found */
}

void smt_put(char* name, AST* ast) {
  size_t idx = 0;
  if(smt_lookup(name) == NULL) { /* new entry */
    while(idx < TABLESIZE) {
      if(table[idx].name == NULL) {
        table[idx].name = name;
        table[idx].ast = ast;
        return;
      }
      idx++;
    }
    crash("Out of symbol table space");
  } else { /* overwrite entry */
    while(idx < TABLESIZE) {
      if(table[idx].name && strcmp(name, table[idx].name) == 0) {
        table[idx].ast = ast;
        return;
      }
      idx++;
    }
  }
}

void smt_del(char* name) {
  size_t idx = 0;
  while(idx < TABLESIZE) {
    if(table[idx].name && strcmp(name, table[idx].name) == 0) {
      table[idx].name = NULL;
      table[idx].ast = NULL;
    }
    idx++;
  }
}

/* Proper Implementation (todo: finish)
void put(char* symname, AST* ast) {
  Symbol* s = lookup(symname);
  if(s) { // already in table, just update it 
    s->thing = ast;
  } else { // new entry 
    s = malloc(sizeof(Symbol));
    if(!s)
      crash("Out of Space");
    
  }
}

Symbol* lookup(char* symname) {
  Symbol* sp = &table[symhash(symname) % NHASH];
  int scount = NHASH; // how many syms we've looked at 
  while(--scount >= 0) {
    if(sp->name && ((!!strcmp(sp->name, symname))==0) )
      return sp;

    if(!sp->name) { // not in table 
      return NULL;
    }

    if(++sp >= table+NHASH) // loop around 
      sp = table;
    crash("Symbol Table Overflow");
  }
  return NULL; // never gets here 
}

*/
