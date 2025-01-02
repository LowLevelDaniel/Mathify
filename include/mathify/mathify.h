#ifdef __cplusplus
#pragma once
#endif

#ifndef __MATHIFY_IMPLEMENTATION
#define __MATHIFY_IMPLEMENTATION

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>

#define MATHIFY struct mathify
#define MATHIFY_TOKEN struct token

// Enums
  enum TOKEN_TYPE {
    // Value
    TOKEN_TYPE_INT,
    TOKEN_TYPE_UINT,
    TOKEN_TYPE_FLOAT,

    // Arithmetic  
    TOKEN_TYPE_ADD,
    TOKEN_TYPE_SUB,
    TOKEN_TYPE_MUL,
    TOKEN_TYPE_DIV,
    TOKEN_TYPE_MOD
  };
// Types
  struct mathify {
    // src
    int _ptr;
    union {
      FILE* file;
      char* bytes;
    } src;

  };

  struct token {
    enum TOKEN_TYPE type;
    union {
      long long int i;
      unsigned long long int u;
      long double f;
    } val;
  };
// END

// Creation
bool mathify_create_file(MATHIFY *obj, FILE *file);
bool mathify_create_ptr(MATHIFY *obj, char *ptr);

// Update
int mathify_getc(MATHIFY*);

// Main
bool mathify_main(MATHIFY*, MATHIFY_TOKEN*);

#ifdef __cplusplus
}
#endif
#endif // __MATHIFY_IMPLEMENTATION