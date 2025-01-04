#include "io.h"
#include "mathify/mathify.h"
#include "mathify/string.h"


// Creation
bool mathify_create_file(MATHIFY *obj, FILE *file) {
  obj->_ptr = false;
  obj->src.file = file;
  return true;
}
bool mathify_create_ptr(MATHIFY *obj, char *ptr) {
  obj->_ptr = true;
  obj->src.bytes = ptr;
  return true;
}

// Update
int mathify_getc(MATHIFY *obj) {
  if (!obj->_ptr) {
    int in;
    for (in = fgetc(obj->src.file);; in = fgetc(obj->src.file)) {
      if (in == EOF) return EOF;
      if (in != ' ' && in != '\n' && in != '\t') break;
    }
    return in;
  } else {
    char in;
    for (in = *(obj->src.bytes++);; in = *(obj->src.bytes++)) {
      if (in == '\0') { --obj->src.bytes; return EOF; }
      if (in != ' ' && in != '\n' && in != '\t') break;
    }
    return in;
  }
}

// Constexpr Operand
bool token_ioperate(MATHIFY_TOKEN *result, MATHIFY_TOKEN *tok1, MATHIFY_TOKEN *tok2, int operator) {
  switch (operator) {
    case '+':
      result->val.i = tok1->val.i + tok2->val.i; break;
    case '-':
      result->val.i = tok1->val.i - tok2->val.i; break;
    case '*':
      result->val.i = tok1->val.i * tok2->val.i; break;
    case '/':
      result->val.i = tok1->val.i / tok2->val.i; break;
    case '%':
      result->val.i = tok1->val.i % tok2->val.i; break;
    default:
      ERR("Invalid Operator");
      return true;
  }
  return false;
}
bool token_uoperate(MATHIFY_TOKEN *result, MATHIFY_TOKEN *tok1, MATHIFY_TOKEN *tok2, int operator) {
  switch (operator) {
    case '+':
      result->val.u = tok1->val.u + tok2->val.u; break;
    case '-':
      result->val.u = tok1->val.u - tok2->val.u; break;
    case '*':
      result->val.u = tok1->val.u * tok2->val.u; break;
    case '/':
      result->val.u = tok1->val.u / tok2->val.u; break;
    case '%':
      result->val.u = tok1->val.u % tok2->val.u; break;
    default:
      ERR("Invalid Operator");
      return true;
  }
  return false;
}
bool token_foperate(MATHIFY_TOKEN *result, MATHIFY_TOKEN *tok1, MATHIFY_TOKEN *tok2, int operator) {
  switch (operator) {
    case '+':
      result->val.f = tok1->val.f + tok2->val.f; break;
    case '-':
      result->val.f = tok1->val.f - tok2->val.f; break;
    case '*':
      result->val.f = tok1->val.f * tok2->val.f; break;
    case '/':
      result->val.f = tok1->val.f / tok2->val.f; break;
    case '%':
      ERR("Modulus not supported for double"); break;
    default:
      ERR("Invalid Operator");
      return true;
  }
  return false;
}
bool token_operate(MATHIFY_TOKEN *result, MATHIFY_TOKEN *tok1, MATHIFY_TOKEN *tok2, int operator) {
  if (tok1->type == tok2->type) {
    if (tok1->type == TOKEN_TYPE_INT) return token_ioperate(result, tok1, tok2, operator);
    if (tok1->type == TOKEN_TYPE_UINT) return token_uoperate(result, tok1, tok2, operator);
    if (tok1->type == TOKEN_TYPE_FLOAT) return token_foperate(result, tok1, tok2, operator);
    ERR("Token Type is the same but is not valid, maybe this isn't implemented");
    return true;
  }
  ERR("Token Type is different");
  return true;
}

bool mathify_lex(MATHIFY *obj, MATHIFY_TOKEN *toks, MATHIFY_ARENA *arena) {
  MATHIFY_TOKEN tmptok;
  for (int in = mathify_getc(obj); in != EOF;) {
    switch (in) {
    case '+':
      tmptok.type = TOKEN_TYPE_ADD;
      in = mathify_getc(obj);
      break;
    case '-':
      tmptok.type = TOKEN_TYPE_SUB;
      in = mathify_getc(obj);
      break;
    case '*':
      tmptok.type = TOKEN_TYPE_MUL;
      in = mathify_getc(obj);
      break;
    case '/':
      tmptok.type = TOKEN_TYPE_DIV;
      in = mathify_getc(obj);
      break;
    case '%':
      tmptok.type = TOKEN_TYPE_MOD;
      in = mathify_getc(obj);
      break;
    case '(':
      tmptok.type = TOKEN_TYPE_LPAREN;
      in = mathify_getc(obj);
      break;
    case ')':
      tmptok.type = TOKEN_TYPE_RPAREN;
      in = mathify_getc(obj);
      break;
    default: {
      // save type
      int type = in;

      // get base
      in = mathify_getc(obj);

      int base = 0;
      switch (in) {
        case 'b': // binary
        case '1': base = 2; break;
        case '2': base = 3; break;
        case '3': base = 4; break;
        case '4': base = 5; break;
        case '5': base = 6; break;
        case '6': base = 7; break;
        case '7': base = 8; break;
        case 'o': // octal
        case '8': base = 9; break;
        case 'd': // decimal
        case '9': base = 10; break;
        case 'A': base = 11; break;
        case 'B': base = 12; break;
        case 'C': base = 13; break;
        case 'D': base = 14; break;
        case 'E': base = 15; break;
        case 'x': // hexadecimal
        case 'F': base = 16; break;
        case 'G': base = 17; break;
        case 'H': base = 18; break;
        case 'I': base = 19; break;
        case 'J': base = 20; break;
        case 'K': base = 21; break;
        case 'L': base = 22; break;
        case 'M': base = 23; break;
        case 'N': base = 24; break;
        case 'O': base = 25; break;
        case 'P': base = 26; break;
        case 'Q': base = 27; break;
        case 'R': base = 28; break;
        case 'S': base = 29; break;
        case 'T': base = 30; break;
        case 'U': base = 31; break;
        case 'V': base = 32; break;
        case 'W': base = 33; break;
        case 'X': base = 34; break;
        case 'Y': base = 35; break;
        case 'Z': base = 36; break;
        // specials
        default:
          ERR("Failed at attempting to find base in a value, %c", in);
          return true;
      }

      // get type and complete value
      if (type == 'i' || type == 'I') {
        tmptok.type = TOKEN_TYPE_INT;
        in = stoi(obj, &tmptok.val.i, base);
      } else if (type == 'u' || type == 'U') {
        tmptok.type = TOKEN_TYPE_UINT;
        in = stou(obj, &tmptok.val.u, base);
      } else if (type == 'f' || type == 'F') {
        tmptok.type = TOKEN_TYPE_FLOAT;
        in = stof(obj, &tmptok.val.f, base);
      } else {
        ERR("Invalid type given in number %c", type);
        return true;
      }

      // exit switch
      break;
    }
    };
    MATHIFY_ARENA_PUSH(arena,&tmptok,sizeof(MATHIFY_TOKEN));
  }
  tmptok.type = TOKEN_TYPE_EOF;
  MATHIFY_ARENA_PUSH(arena,&tmptok,sizeof(MATHIFY_TOKEN));
  return false;
}

int get_precedence(enum TOKEN_TYPE type) {
  switch (type) {
    case TOKEN_TYPE_ADD: case TOKEN_TYPE_SUB: return 1;
    case TOKEN_TYPE_MUL: case TOKEN_TYPE_DIV: case TOKEN_TYPE_MOD: return 2;
    default:
      return 0;
  }
}

bool mathify_shunting_yard(MATHIFY_TOKEN *input, MATHIFY_TOKEN *output, MATHIFY_TOKEN *operator_stack) {
  int operator_top = -1; // Empty stack
  int output_top = 0;

  for (; input->type != TOKEN_TYPE_EOF; ++input) {
    if (input->type < TOKEN_TYPE_ADD) { // Numbers go directly to output
      output[output_top++] = *input;
    } else if (input->type < TOKEN_TYPE_LPAREN) { // Operators
      while (operator_top >= 0 &&
             operator_stack[operator_top].type != TOKEN_TYPE_LPAREN &&
             get_precedence(operator_stack[operator_top].type) >= get_precedence(input->type)) {
        output[output_top++] = operator_stack[operator_top--];
      }
      operator_stack[++operator_top] = *input;
    } else if (input->type == TOKEN_TYPE_LPAREN) { // Left parenthesis
      operator_stack[++operator_top] = *input;
    } else if (input->type == TOKEN_TYPE_RPAREN) { // Right parenthesis
      while (operator_top >= 0 && operator_stack[operator_top].type != TOKEN_TYPE_LPAREN) {
        output[output_top++] = operator_stack[operator_top--];
      }
      if (operator_top < 0 || operator_stack[operator_top].type != TOKEN_TYPE_LPAREN) {
        fprintf(stderr, "Mismatched parentheses!\n");
        return true;
      }
      operator_top--; // Pop the left parenthesis
    } else {
      fprintf(stderr, "Unhandled token type!\n");
      return true;
    }
  }

  // Pop remaining operators to output
  while (operator_top >= 0) {
    if (operator_stack[operator_top].type == TOKEN_TYPE_LPAREN) {
      fprintf(stderr, "Mismatched parentheses!\n");
      return true;
    }
    output[output_top++] = operator_stack[operator_top--];
  }

  output[output_top].type = TOKEN_TYPE_EOF; // Mark the end of the output
  return false;
}

bool mathify_main(MATHIFY *obj, MATHIFY_TOKEN *toks, MATHIFY_ARENA *arena) {
  if (mathify_lex(obj,toks,arena)) { ERR("Error Lexing Mathematical Expression"); return true; }
  
  MATHIFY_ARENA output_arena;
  MATHIFY_ARENA_COPY(arena, &output_arena);
  MATHIFY_ARENA tmpstack_arena;
  MATHIFY_ARENA_COPY(arena, &tmpstack_arena);

  MATHIFY_TOKEN *outputtoks = (MATHIFY_TOKEN*)MATHIFY_ARENA_BUFFER(&output_arena);

  mathify_shunting_yard(
    toks, 
    outputtoks,
    (MATHIFY_TOKEN*)MATHIFY_ARENA_BUFFER(&tmpstack_arena)
  );

  // mathify_debug(toks);
  mathify_debug(outputtoks);

  MATHIFY_ARENA_DELETE(&output_arena);
  MATHIFY_ARENA_DELETE(&tmpstack_arena);
  return false;
}

void mathify_debug(MATHIFY_TOKEN *tok) {
  for (; tok->type != TOKEN_TYPE_EOF; ++tok) {
    switch (tok->type) {
    case TOKEN_TYPE_INT:
      LOG("signed integer{%lld}", tok->val.i); break;
    case TOKEN_TYPE_UINT:
      LOG("unsigned integer{%llu}", tok->val.u); break;
    case TOKEN_TYPE_FLOAT:
      LOG("float{%Lf}", tok->val.f); break;
    case TOKEN_TYPE_LPAREN:
      LOG("RPAREN"); break;
    case TOKEN_TYPE_RPAREN:
      LOG("LPAREN"); break;
    case TOKEN_TYPE_ADD:
      LOG("ADD"); break;
    case TOKEN_TYPE_SUB:
      LOG("SUB"); break;
    case TOKEN_TYPE_MUL:
      LOG("MUL"); break;
    case TOKEN_TYPE_DIV:
      LOG("DIV"); break;
    case TOKEN_TYPE_MOD:
      LOG("MOD"); break;
    default:
      ERR("ERR");
    }
  }
  LOG("\nEOF");
}