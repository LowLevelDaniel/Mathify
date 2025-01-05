#include "mathify/string.h"

#include <stdbool.h>

int stoi(struct mathify *obj, long long int *val, int base) {
  *val = 0;
  bool neg = false;
  bool value_section = false;
  for (int in = mathify_getc(obj); in != EOF && in != ';'; in = mathify_getc(obj)) {
    if (in >= '0' && in <= '9') {
      value_section = true;
      *val = (*val * base) + (in - '0');
    } else if ((in >= 'A' && in <= 'Z') || (in >= 'a' && in <= 'z')) {
      value_section = true;
      int digit = (in >= 'A' && in <= 'Z') ? (in - 'A' + 10) : (in - 'a' + 10);
      *val = (*val * base) + digit;
    } else if (in == '-' && !value_section) {
      neg = !neg;
    } else if (in == '+' && !value_section) {
      continue;
    } else {
      if (neg) *val = -(*val);
      return in; // leave parsing to lexer
    }
  }
  if (neg) *val = -(*val);
  return EOF; // return EOF if end of statement
}

int stou(struct mathify *obj, unsigned long long int *val, int base) {
  *val = 0;
  bool neg = false;
  bool value_section = false;
  for (int in = mathify_getc(obj); in != EOF && in != ';'; in = mathify_getc(obj)) {
    if (in >= '0' && in <= '9') {
      value_section = true;
      *val = (*val * base) + (in - '0');
    } else if ((in >= 'A' && in <= 'Z') || (in >= 'a' && in <= 'z')) {
      value_section = true;
      int digit = (in >= 'A' && in <= 'Z') ? (in - 'A' + 10) : (in - 'a' + 10);
      *val = (*val * base) + digit;
    } else if (in == '-' && !value_section) {
      neg = !neg;
    } else if (in == '+' && !value_section) {
      continue;
    } else {
      if (neg) *val = -(*val);
      return in; // leave parsing to lexer
    }
  }
  if (neg) *val = -(*val);
  return EOF; // return EOF if end of statement
}

int stof(struct mathify *obj, long double *val, int base) {
  *val = 0;
  long double factor = 1;
  bool decimal_point = false;

  bool neg = false;
  bool value_section = false;

  for (int in = mathify_getc(obj); in != EOF && in != ';'; in = mathify_getc(obj)) {
    if (in >= '0' && in <= '9') {
      value_section = true;
      if (decimal_point) {
        factor /= base;
        *val += (in - '0') * factor;
      } else {
        *val = (*val * base) + (in - '0');
      }
    } else if (in == '.') {
      value_section = true;
      if (decimal_point) {
        return in;
      } else {
        decimal_point = true;
      }
    } else if ((in >= 'A' && in <= 'Z') || (in >= 'a' && in <= 'z')) {
      value_section = true;
      int digit = (in >= 'A' && in <= 'Z') ? (in - 'A' + 10) : (in - 'a' + 10);
      if (decimal_point) {
        factor /= base;
        *val += digit * factor;
      } else {
        *val = (*val * base) + digit;
      }
    } else if (in == '-' && !value_section) {
      neg = !neg;
    } else if (in == '+' && !value_section) {
      continue;
    } else {
      if (neg) *val = -(*val);
      return in; // leave parsing to lexer
    }
  }
  if (neg) *val = -(*val);
  return EOF; // return EOF if end of statement
}

