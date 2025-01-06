#include "mathify/mathify.h"

int main(int argc, char **argv) {
  char *mathexpr = 
    // "i  \n d - + + - - - \n\t 1 \t\t\t 0 \t\t\n\n\n  0 + " 
    "ud10 + ud10 - ud10 * ud10 / ud10" // % ud10"
    // " + fd 100.10"
    // "( id10 + id10 )( id10 + id10 ) + ( id10 + id10 )"
    // "id10 + id10 - id10"
  ;

/*
  "a = f(x) {}" // constexpr function
  "b = id10" // immediate value
  "c = $var" // some local variable defined programatically
  "d = %isymbol" // some program memory
  ""
*/

  MATHIFY obj;
  mathify_create_ptr(&obj, mathexpr);

  MATHIFY_TOKEN tok;

  if (mathify_main_expr(&obj, &tok, 1024)) return 1;

  mathify_print_token(&tok);

  return 0;
}