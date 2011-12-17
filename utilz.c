/* Seth Hoenig 2011 */
/* seth.a.hoenig@gmail.com */
/* bitonic.org */

#include "utilz.h"

/* convert hex str to dec int */
int
htoi(char* hex) {
  unsigned int slen = stlen(hex);
  unsigned int val = 0;
  unsigned int multi = 0;
  char* c;
  for(c = hex+slen-1;c>=hex; c--, multi++) {
    unsigned int pow = 1 << (4*multi);
    if(*c=='x' || *c=='X')
      return val; 
    else if((*c>='0') && (*c<='9'))
      val += (*c-'0') * pow;
    else if((*c>='a') && (*c<='f'))
      val += ((*c-'a')+10) * pow;
    else if((*c>='A') && (*c<='F'))
      val += ((*c-'A')+10) * pow;
  }
  return val;
}

/* length of str */
size_t
stlen(char* str) {
  int len = 0;
  for(;*str!='\0';str++)
    len++;
  return len;
}


/* duplicate string */
char*
strdup(char* str) {
  size_t len = stlen(str);
  char* nstr = (char*) malloc(len * sizeof(char) + 1);
  nstr[len] = '\0';
  for( ; len>0 ; len--)
    nstr[len] = str[len];
  nstr[0] = str[0];
  return nstr;
}
