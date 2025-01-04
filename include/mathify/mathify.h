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

#ifndef MATHIFY_ARENA
#define MATHIFY_ARENA arena_buf_t
#define MATHIFY_ARENA_PUSH(Parena, Pval, size) if (mathify_arena_push(Parena,Pval,size)) { return true; }
#define MATHIFY_ARENA_COPY(Parena, Pnewarena) mathify_arena_create(Pnewarena, Parena->bufsize)
#define MATHIFY_ARENA_DELETE(Parena) mathify_arena_delete(Parena)
#define MATHIFY_ARENA_BUFFER(Parena) ((Parena)->buf)
#include "mathify/arena.h"
#endif


// Enums
  enum TOKEN_TYPE : char {
    // Value
    TOKEN_TYPE_INT = 0,
    TOKEN_TYPE_UINT = 1,
    TOKEN_TYPE_FLOAT = 2,
    TOKEN_TYPE_MATHIFY_FUNCTION = 3,
    TOKEN_TYPE_MATHIFY_VARIABLE = 4,
    TOKEN_TYPE_VARIABLE = 5,
    TOKEN_TYPE_SYMBOL = 6,
    TOKEN_TYPE_FUNCTION = 7,

    // Arithmetic  
    TOKEN_TYPE_ADD = 8,
    TOKEN_TYPE_SUB = 9,
    TOKEN_TYPE_MUL = 10,
    TOKEN_TYPE_DIV = 11,
    TOKEN_TYPE_MOD = 12,

    // Bit Manipulation

    // Comparison

    // Control Flow
    TOKEN_TYPE_LPAREN = 13,
    TOKEN_TYPE_RPAREN = 14,

    // Auxiliary
    TOKEN_TYPE_EOF = -1
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
    struct token *next;
    union {
      long long int i;
      unsigned long long int u;
      long double f;
    } val;
    enum TOKEN_TYPE type;
  };
// END

// Creation
bool mathify_create_file(MATHIFY *obj, FILE *file);
bool mathify_create_ptr(MATHIFY *obj, char *ptr);

// Update
int mathify_getc(MATHIFY*);

// Main
bool mathify_main(MATHIFY*, MATHIFY_TOKEN*, MATHIFY_ARENA*);

// debug
void mathify_debug(MATHIFY_TOKEN*);

#ifdef __cplusplus
}
#endif
#endif // __MATHIFY_IMPLEMENTATION