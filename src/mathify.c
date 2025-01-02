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


bool mathify_main(MATHIFY *obj, MATHIFY_TOKEN *tok) {
  for (int in = mathify_getc(obj); in != EOF; in = mathify_getc(obj)) {
    // get base
    int type = in;
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
      case '7':
      case 'o': // octal base = 8; break;
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
        ERR("Failed at attempting to find base");
        return true;
    }

    // complete value
    if (type == 'i' || type == 'I') {
      tok->type = TOKEN_TYPE_INT;
      if (stoi(obj, &tok->val.i, base)) {
        ERR("string to signed integer failed");
        return true;
      }
    } else if (type == 'u' || type == 'U') {
      tok->type = TOKEN_TYPE_UINT;
      if (stou(obj, &tok->val.u, base)) {
        ERR("string to signed integer failed");
        return true;
      }
    } else if (type == 'f' || type == 'F') {
      tok->type = TOKEN_TYPE_FLOAT;
      if (stof(obj, &tok->val.f, base)) {
        ERR("string to signed integer failed");
        return true;
      }
    }
  }
  return false;
}
