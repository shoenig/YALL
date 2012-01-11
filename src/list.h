/* list.h
   List-specific operations
   Seth Hoenig 2012 (seth.a.hoenig@gmail.com)
*/

#ifndef YALL_LIST_H
#define YALL_LIST_H

typedef struct ast_ *ASTP; /* forward declare or something */

/* it's a linked list of list nodes  */
typedef struct {
  int size;
  ASTP head;
} List;

List* list_make(ASTP);

#endif
