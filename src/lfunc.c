/* lfunc.c
   Seth Hoenig 2012 (seth.a.hoenig@gmail.com)
*/

#include <stdlib.h>
#include "err.h"
#include "lfunc.h"

AST*
_list_dup_elem(AST* orig_elm) {
  if(orig_elm) {
    if(orig_elm->nodetype != AST_LIST_ELEMENT) {
      crash("cannot duplicate non list element (%d) %c",
            orig_elm->nodetype,
            orig_elm->nodetype);
    }
    AST* new = malloc(sizeof(AST));
    new->nodetype = AST_LIST_ELEMENT;
    new->e = orig_elm->e;
    new->left = dup_ast(orig_elm->left);
    new->aux = dup_ast(orig_elm->right);
    new->right = _list_dup_elem(orig_elm->right);
    return new;
  } else
    return NULL;
}

AST*
list_wrap_evaltype_as_element(evaltype e) {
  AST* inner = ast_wrap(e);
  AST* elem = malloc(sizeof(AST));
  elem->nodetype = AST_LIST_ELEMENT;
  elem->left = inner;
  /* elem->right gets set later (to next element) */
  return elem;
}

evaltype
list_dup(List* orig) {
  evaltype ret;
  ret.type = ET_LIST;
  ret.val.list = malloc(sizeof(List));
  ret.val.list->head = _list_dup_elem(orig->head); /* kick off recursion */
  return ret;
}

int
list_length(List* list) {
  int size = 0;
  AST* elem_ptr = list->head;
  while(elem_ptr) {
    size++;
    elem_ptr = elem_ptr->right;
  }
  return size;
}
