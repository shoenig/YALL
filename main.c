/* main.c
   Where it YALL begins...
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#include <stdio.h>

/*#include "parser.tab.h"*/
#include "stdtype.h"
#include "ast.h"

extern int yyparse();

int
main(int argc, char** argv) {

  printf("> ");
  int parse = yyparse(); // yee-haw!!
  printf("\nBye.\n");
  return parse;
}
