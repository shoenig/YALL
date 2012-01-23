/* err.h
   Handling errors in the parser
   Seth Hoenig 2011 (seth.a.hoenig@gmail.com)
*/

#ifndef YALL_ERR_H
#define YALL_ERR_H

#include "stdtype.h"

void yyerror(char*, ...);
void crash(char*, ...);
void yasrt(bool, char*, ...);

#endif
