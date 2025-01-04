#include "mathify/mathify.h"

int main(int argc, char **argv) {
  char *mathexpr = 
    "  i  \n d \n\t 1 \t\t\t 0 \t\t\n\n\n  0 + " 
    "ud 10 + ud10 - ud10 * ud10 / ud10 % ud10"
    "+ fd 100.10"
    "( id10 + id10 )"
  ;

/*
  "a = f(x) {}" // constexpr function
  "b = #d10" // immediate value
  "c = $var" // some local variable defined programatically
  "d = %isymbol" // some program memory
  ""
*/

  MATHIFY obj;
  mathify_create_ptr(&obj, mathexpr);

  MATHIFY_ARENA tokbuf;
  mathify_arena_create(&tokbuf, 1024);

  if (mathify_main(&obj, (MATHIFY_TOKEN*)tokbuf.buf, &tokbuf)) return 1;

  mathify_arena_delete(&tokbuf);
  return 0;
}