#include "mathify/mathify.h"

int main(int argc, char **argv) {
  char *mathexpr = "i  \n d \n\t 1 \t\t\t 0 \t\t\n\n\n  0";
  MATHIFY obj;
  mathify_create_ptr(&obj, mathexpr);
  
  MATHIFY_TOKEN tok;
  if (mathify_main(&obj, &tok)) return 1;

  if (tok.type == TOKEN_TYPE_INT) {
    printf("long long int - %lld\n", tok.val.i);
  } else if (tok.type == TOKEN_TYPE_INT) {
    printf("unsigned long long int - %llu\n", tok.val.u);
  } else if (tok.type == TOKEN_TYPE_INT) {
    printf("long double - %Lf\n", tok.val.f);
  }

  return 0;
}