/* list.c
   Lists and things
   Seth Hoenig 2012 (seth.a.hoenig@gmail.com)
*/

#include <stdlib.h>
#include "list.h"

List* list_make(ASTP h) {
  List* l = malloc(sizeof(List));
  l->head = h;
  return l;
}
