#include "mathify/string.h"

#include <stdbool.h>

int stoi(struct mathify *obj, long long int *val, int base) {
  *val = 0;
  for (int in = mathify_getc(obj); in != EOF && in != ';'; in = mathify_getc(obj)) {
    if (in >= '0' && in <= '9') {
      *val = (*val * base) + (in - '0');
    } else if ((in >= 'A' && in <= 'Z') || (in >= 'a' && in <= 'z')) {
      int digit = (in >= 'A' && in <= 'Z') ? (in - 'A' + 10) : (in - 'a' + 10);
      *val = (*val * base) + digit;
    } else {
      return in; // leave parsing to lexer
    }
  }
  return EOF; // return EOF if end of statement
}

int stou(struct mathify *obj, unsigned long long int *val, int base) {
  *val = 0;
  for (int in = mathify_getc(obj); in != EOF && in != ';'; in = mathify_getc(obj)) {
    if (in >= '0' && in <= '9') {
      *val = (*val * base) + (in - '0');
    } else if ((in >= 'A' && in <= 'Z') || (in >= 'a' && in <= 'z')) {
      int digit = (in >= 'A' && in <= 'Z') ? (in - 'A' + 10) : (in - 'a' + 10);
      *val = (*val * base) + digit;
    } else {
      return in; // leave parsing to lexer
    }
  }
  return EOF; // return EOF if end of statement
}

int stof(struct mathify *obj, long double *val, int base) {
  *val = 0;
  long double factor = 1;
  bool decimal_point = false;

  for (int in = mathify_getc(obj); in != EOF && in != ';'; in = mathify_getc(obj)) {
    if (in >= '0' && in <= '9') {
      if (decimal_point) {
        factor /= base;
        *val += (in - '0') * factor;
      } else {
        *val = (*val * base) + (in - '0');
      }
    } else if (in == '.') {
      if (decimal_point) {
        return in;
      } else {
        decimal_point = true;
      }
    } else if ((in >= 'A' && in <= 'Z') || (in >= 'a' && in <= 'z')) {
      int digit = (in >= 'A' && in <= 'Z') ? (in - 'A' + 10) : (in - 'a' + 10);
      if (decimal_point) {
        factor /= base;
        *val += digit * factor;
      } else {
        *val = (*val * base) + digit;
      }
    } else {
      return in; // leave parsing to lexer
    }
  }
  return EOF; // return EOF if end of statement
}

