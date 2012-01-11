/* typedecoder.c
   Seth Hoenig 2012 (seth.a.hoenig@gmail.com)
*/

#include "typedecoder.h"

/* DO NOT CHANGE ORDER!!! */
char* typelist[] = {"UNKNOWN", "BOOLEAN", "INT", "FLOAT", "REFERENCE",
                    "LIST", "LISTELEMENT"};

char*
type_decode(char type) {
  switch(type) {
  case 'Z': return typelist[1];
  case 'I': return typelist[2];
  case 'F': return typelist[3];
  case 'R': return typelist[4];
  case 'L': return typelist[5];
  case 'E': return typelist[6];
  default : return typelist[0];
  }
}
