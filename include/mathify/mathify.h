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
#define MATHIFY_TOKEN struct mathfy_lex_token

#ifndef MATHIFY_ARENA
  #define MATHIFY_ARENA arena_buf_t
  #define MATHIFY_ARENA_PUSH(Parena, Pval, size) if (mathify_arena_push(Parena,Pval,size)) { return true; }
  #define MATHIFY_ARENA_CREATE(Parena, size) if (mathify_arena_create(Parena, size)) { return true; }
  #define MATHIFY_ARENA_DELETE(Parena) mathify_arena_delete(Parena)
  #define MATHIFY_ARENA_BUFFER(Parena) ((Parena)->buf)
  #define MATHIFY_ARENA_SIZE(Parena) ((Parena)->bufsize)
  #include "mathify/arena.h"
#endif


// Enums
  enum MATHIFY_TOKEN_TYPE : char {
    // Value
    MATHIFY_TOKEN_TYPE_INT = 1,
    MATHIFY_TOKEN_TYPE_UINT = 2,
    MATHIFY_TOKEN_TYPE_FLOAT = 3,
    MATHIFY_TOKEN_TYPE_MATHIFY_FUNCTION = 4,
    MATHIFY_TOKEN_TYPE_MATHIFY_VARIABLE = 5,
    MATHIFY_TOKEN_TYPE_VARIABLE = 6,
    MATHIFY_TOKEN_TYPE_SYMBOL = 7,
    MATHIFY_TOKEN_TYPE_FUNCTION = 8,

    // Arithmetic  
    MATHIFY_TOKEN_TYPE_ADD = 9,
    MATHIFY_TOKEN_TYPE_SUB = 10,
    MATHIFY_TOKEN_TYPE_MUL = 12,
    MATHIFY_TOKEN_TYPE_DIV = 13,
    MATHIFY_TOKEN_TYPE_MOD = 14,

    // Bit Manipulation

    // Comparison

    // Control Flow
    MATHIFY_TOKEN_TYPE_LPAREN = 15,
    MATHIFY_TOKEN_TYPE_RPAREN = 16,

    // Unary

    // Auxiliary
    MATHIFY_TOKEN_TYPE_EOF = -1
  };
// Types
  struct mathfy_lex_token {
    union {
      long long int i;
      unsigned long long int u;
      long double f;
    } val;
    enum MATHIFY_TOKEN_TYPE type;
  };
  struct mathify {
    // src
    union {
      FILE* file;
      char* bytes;
    } src;
    unsigned long int _consumed;
    int _ptr;
  };
// END

// Creation
bool mathify_create_file(MATHIFY*, FILE*);
bool mathify_create_ptr(MATHIFY*, char*);

// Update
int mathify_getc(MATHIFY*);

// Backend
bool mathify_lex(MATHIFY*, MATHIFY_ARENA*arena);
bool mathify_shunting_yard(MATHIFY_TOKEN*, MATHIFY_TOKEN*start, MATHIFY_TOKEN **newtop, MATHIFY_TOKEN*);
bool mathify_parse(MATHIFY_TOKEN*, MATHIFY_ARENA*);
bool mathify_parse_expr(MATHIFY_TOKEN *top, MATHIFY_TOKEN *end, MATHIFY_TOKEN *resulttok);

// Main
bool mathify_main(MATHIFY*, MATHIFY_ARENA*);
bool mathify_main_expr(MATHIFY*, MATHIFY_TOKEN*, unsigned long int default_arena_size);

// debug
void mathify_print_token(MATHIFY_TOKEN *tok);
void mathify_debug(MATHIFY_TOKEN*);
void mathify_debug_stack(MATHIFY_TOKEN *end, MATHIFY_TOKEN *top);

#ifdef __cplusplus
}
#endif
#endif // __MATHIFY_IMPLEMENTATION