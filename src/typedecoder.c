/* typedecoder.c
   Seth Hoenig 2012 (seth.a.hoenig@gmail.com)
*/

#include "typedecoder.h"

const char*
etdec(EVALTYPE ettype) {
  switch(ettype) {
  case ET_INT: return "INT";
  case ET_FLOAT: return "FLOAT";
  case ET_BOOL: return "BOOL";
  case ET_LIST: return "LIST";
  case ET_REFERENCE: return "REF";
  case ET_INVALID:
  default:
    return "INVALID";
  }
}

const char*
astdec(ASTTYPE asttype) {
  switch(asttype) {
  case AST_INT: return "INT";
  case AST_FLOAT: return "FLOAT";
  case AST_MINUS: return "ARITH-NEGATE";
  case AST_BOOL: return "BOOL";
  case AST_LIST: return "LIST";
  case AST_LIST_ELEMENT: return "LIST-ELEM";
  case AST_REFERENCE: return "REF";
  case AST_LIST_FUNC: return "LIST-FUNC";
  case AST_BUILTIN_FUNC: return "BUILTIN-FUNC";
  case AST_BOOLEAN_FUNC: return "BOOLEAN-FUNC";
  case AST_AMPER: return "&";
  case AST_PIPE: return "|";
  case AST_EQ: return "=";
  case AST_PLUS: return "+";
  case AST_SUB: return "-";
  case AST_MULT: return "*";
  case AST_DIV: return "/";
  case AST_INVALID:
  default:
    return "INVALID";
  }
}
