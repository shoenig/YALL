/* main.c
   Where it YALL begins...
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "stdtype.h"
#include "functable.h"
#include "ast.h"

extern int yyparse();
extern FILE* yyin;

int
main(int argc, char** argv) {
  int parse;
  int i;
  smt_init(); /* initialize symbol table */
  uft_init(); /* init function table */
  if(argc > 1) {
    for(i=1; i<argc; i++) {
      FILE* f = fopen(argv[i], "r");
      if(!f) {
        perror(argv[i]);
        return EXIT_FAILURE;
      }
      reading_stdin = false;
      yyrestart(f);
      parse = yyparse(); // yee-haw!!
      fclose(f);
    }
    return parse;
  }
  reading_stdin = true;
  yyin = stdin;
  printf("> ");
  parse = yyparse();
  printf("\nBye.\n");
  return parse;
}
