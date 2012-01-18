/* symtable.c
   Implementation of a Symbol Table
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#include <string.h>
#include <stdlib.h>
#include "err.h"
#include "symtable.h"

/* clear everything */
void
smt_init() {
  size_t idx = 0;
  while(idx < TABLESIZE) {
    table[idx] = NULL;
    idx++;
  }
}

/* returns pointer to the Symbol, NULL if ref is not in the table */
Symbol*
smt_lookup(char* name) {
  size_t idx = 0;
  while(idx < TABLESIZE) {
    if(table[idx] && strcmp(name, table[idx]->name) == 0) {
      Symbol* sym = table[idx];
      while(sym->next)
        sym = sym->next;
      return sym;
    }
    idx++;
  }
  return NULL; /* not found */
}

void
smt_create_symbol(char* name, AST* ast) {
  size_t idx = 0;
  while(idx < TABLESIZE) {
    if(table[idx] == NULL) {
      table[idx] = malloc(sizeof(AST));
      table[idx]->name = name;
      table[idx]->ast = ast;
      table[idx]->next = NULL;
      return;
    }
    idx++;
  }
  crash("Out of symbol table space");
}

/* APPEND to the chain */
void
smt_with_entry(char* name, AST* ast) {
  Symbol* sym = smt_lookup(name);
  if(!sym)
    smt_create_symbol(name, ast);
  else {
    sym->next = malloc(sizeof(AST));
    sym->next->name = name;
    sym->next->ast = ast;
  }
}

/* REMOVE the end of the chain */
bool
smt_with_exit(char* name) {
  /* TODO, implement doubly linked list so i can just fiddle
     with back pointers instead of re-implementing(ish) smt_lookup */
  size_t idx = 0;
  while(idx < TABLESIZE) {
    if(table[idx] && strcmp(name, table[idx]->name) == 0) {
      Symbol* current  = table[idx];
      Symbol* trailer = table[idx];
      while(current->next != NULL) {
        trailer = current;
        current = current->next;
      }
      if(current == trailer) { /* top level */
        free(table[idx]);
        table[idx] = NULL;
        return true; /* all gone */
      }
      else {
        free(trailer->next);
        trailer->next = NULL;
        return false; /* at least one remains */
      }
    }
    idx++;
  }
  crash("cannot delete non-existent symbol: %s", name);
  return false;
}

/* UPDATE last element in chain */
void
smt_put(char* name, AST* ast) {
  Symbol* sym = smt_lookup(name);
  if(!sym)
    smt_create_symbol(name, ast);
  else
    sym->ast = ast;
}

  /* REMOVE ENTIRE chain */
void
smt_del(char* name) {
  while(smt_lookup(name))
    smt_with_exit(name); /* takes care of free-ing */
}
