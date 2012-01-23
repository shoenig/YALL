/* typedecoder.h
   Accepts char types and sends back the full name
   This is just for prettying up some error messages
   Seth Hoenig 2012 (seth.a.hoenig@gmail.com)
*/

#ifndef YALL_TYPEDECODER_H
#define YALL_TYPEDECODER_H

#include "ast.h"

const char* etdec(evaltype);
const char* astdec(AST*);

#endif
