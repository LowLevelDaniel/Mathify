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
    // Auxiliary
    MATHIFY_TOKEN_TYPE_EOF = -1,

    // Value
    MATHIFY_TOKEN_TYPE_INT = 1,
    MATHIFY_TOKEN_TYPE_UINT = 2,
    MATHIFY_TOKEN_TYPE_FLOAT = 3,
    MATHIFY_TOKEN_TYPE_MATHIFY_FUNCTION = 4,
    MATHIFY_TOKEN_TYPE_MATHIFY_VARIABLE = 5,
    MATHIFY_TOKEN_TYPE_VARIABLE = 6,
    MATHIFY_TOKEN_TYPE_SYMBOL = 7,

    // Arithmetic  
    MATHIFY_TOKEN_TYPE_ADD = 10,
    MATHIFY_TOKEN_TYPE_SUB = 11,
    MATHIFY_TOKEN_TYPE_MUL = 12,
    MATHIFY_TOKEN_TYPE_DIV = 13,
    MATHIFY_TOKEN_TYPE_MOD = 14,

    // Bit Manipulation
    MATHIFY_TOKEN_TYPE_BITWISE_OR = 15,
    MATHIFY_TOKEN_TYPE_BITWISE_XOR = 16,
    MATHIFY_TOKEN_TYPE_BITWISE_AND = 17,
    MATHIFY_TOKEN_TYPE_BITWISE_SHIFT_LEFT = 20,
    MATHIFY_TOKEN_TYPE_BITWISE_SHIFT_RIGHT = 21,

    // Comparison
    MATHIFY_TOKEN_TYPE_EQ = 22,
    MATHIFY_TOKEN_TYPE_NEQ = 23,
    MATHIFY_TOKEN_TYPE_LT = 24,
    MATHIFY_TOKEN_TYPE_LTE = 25,
    MATHIFY_TOKEN_TYPE_MT = 26,
    MATHIFY_TOKEN_TYPE_MTE = 27,
    MATHIFY_TOKEN_TYPE_LOGICAL_AND = 28,
    MATHIFY_TOKEN_TYPE_LOGICAL_OR = 29,

    // Control Flow
    MATHIFY_TOKEN_TYPE_LPAREN = 30,
    MATHIFY_TOKEN_TYPE_RPAREN = 31,

    // Unary Operators
    MATHIFY_TOKEN_TYPE_BITWISE_NOT = 32,
    MATHIFY_TOKEN_TYPE_EQ_TRUE = 33, // is there a prpoer name for (if (val))
    MATHIFY_TOKEN_TYPE_LOGICAL_NOT = 34, // !val
    MATHIFY_TOKEN_TYPE_PREFIX_ADD = 35,
    MATHIFY_TOKEN_TYPE_PREFIX_SUB = 36,
    MATHIFY_TOKEN_TYPE_TYPE_CAST = 37,
    MATHIFY_TOKEN_TYPE_DEREFENCE = 38,
    MATHIFY_TOKEN_TYPE_ADDRESS_OF = 39,
    MATHIFY_TOKEN_TYPE_POSTFIX_ADD = 40,
    MATHIFY_TOKEN_TYPE_POSTFIX_SUB = 41,

    // Assignment Operators (right to left)
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