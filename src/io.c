#include "src/io.h"

char getnext(FILE *srcf) {
  char in;
  for (in = fgetc(srcf);; in = fgetc(srcf)) {
    if (in == EOF) return EOF;
    if (in != ' ' && in != '\n' && in != '\t') break;
  }
  return in;
}