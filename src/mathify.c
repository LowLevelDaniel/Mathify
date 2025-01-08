#include "io.h"
#include "mathify/mathify.h"
#include "mathify/string.h"


// Creation
bool mathify_create_file(MATHIFY *obj, FILE *file) {
  obj->_ptr = false;
  obj->src.file = file;
  return false;
}
bool mathify_create_ptr(MATHIFY *obj, char *ptr) {
  obj->_ptr = true;
  obj->src.bytes = ptr;
  return false;
}

// Update
int mathify_getc(MATHIFY *obj) {
  if (!obj->_ptr) {
    int in;
    for (in = fgetc(obj->src.file);; in = fgetc(obj->src.file)) {
      if (in == EOF) return EOF;
      ++obj->_consumed;
      if (in != ' ' && in != '\n' && in != '\t') break;
    }
    return in;
  } else {
    char in;
    for (in = *(obj->src.bytes++);; in = *(obj->src.bytes++)) {
      if (in == '\0') { --obj->src.bytes; return EOF; }
      ++obj->_consumed;
      if (in != ' ' && in != '\n' && in != '\t') break;
    }
    return in;
  }
}

// Stage 1
int mathify_getbase(int in) {
  switch (in) {
  case 'b': // binary
  case '1': return 2;
  case '2': return 3;
  case '3': return 4;
  case '4': return 5;
  case '5': return 6;
  case '6': return 7;
  case '7': return 8;
  case 'o': // octal
  case '8': return 9;
  case 'd': // decimal
  case '9': return 10;
  case 'A': return 11;
  case 'B': return 12;
  case 'C': return 13;
  case 'D': return 14;
  case 'E': return 15;
  case 'x': // hexadecimal
  case 'F': return 16;
  case 'G': return 17;
  case 'H': return 18;
  case 'I': return 19;
  case 'J': return 20;
  case 'K': return 21;
  case 'L': return 22;
  case 'M': return 23;
  case 'N': return 24;
  case 'O': return 25;
  case 'P': return 26;
  case 'Q': return 27;
  case 'R': return 28;
  case 'S': return 29;
  case 'T': return 30;
  case 'U': return 31;
  case 'V': return 32;
  case 'W': return 33;
  case 'X': return 34;
  case 'Y': return 35;
  case 'Z': return 36;
  // specials
  default:
    ERR("Failed at attempting to find base in a value, %c", in);
    return 0;
  }
}

bool mathify_lex(MATHIFY *obj, MATHIFY_ARENA *arena) {
  bool neg = false;
  MATHIFY_TOKEN prevtok = { .type = MATHIFY_TOKEN_TYPE_EOF };
  MATHIFY_TOKEN tmptok;
  for (int in = mathify_getc(obj); in != EOF;) {
    switch (in) {
    case '+': // IMPLEMENTED
      in = mathify_getc(obj);
      if (in == '+') {
        tmptok.type = MATHIFY_TOKEN_TYPE_POSTFIX_ADD;
        in = mathify_getc(obj);
      } else {
        tmptok.type = MATHIFY_TOKEN_TYPE_ADD;
      }
      break;
    case '-': // IMPLEMENTED
      in = mathify_getc(obj);
      if (in == '+') {
        tmptok.type = MATHIFY_TOKEN_TYPE_POSTFIX_SUB;
        in = mathify_getc(obj);
      } else {
        tmptok.type = MATHIFY_TOKEN_TYPE_SUB;
      }
      break;
    case '*': // IMPLEMENTED
      tmptok.type = MATHIFY_TOKEN_TYPE_MUL;
      in = mathify_getc(obj);
      break;
    case '/': // IMPLEMENTED
      tmptok.type = MATHIFY_TOKEN_TYPE_DIV;
      in = mathify_getc(obj);
      break;
    case '%': // IMPLEMENTED
      tmptok.type = MATHIFY_TOKEN_TYPE_MOD;
      in = mathify_getc(obj);
      break;
    case '|': // IMPLEMENTED
      in = mathify_getc(obj);
      if (in == '|') { // logical and
        tmptok.type = MATHIFY_TOKEN_TYPE_LOGICAL_OR;
        in = mathify_getc(obj);
      } else {
        tmptok.type = MATHIFY_TOKEN_TYPE_BITWISE_OR;
      }
      break;
    case '^': // IMPLEMENTED (could have a special arithmetic operation)
      in = mathify_getc(obj);
      // if (in == '^') {
      //   tmptok.type = MATHIFY_TOKEN_TYPE_POWER;
      //   in = mathify_getc(obj);
      // } else {
        tmptok.type = MATHIFY_TOKEN_TYPE_BITWISE_XOR;
      // }
      break;
    case '&': // IMPLEMENTED
      in = mathify_getc(obj);
      if (in == '&') { // logical and
        tmptok.type = MATHIFY_TOKEN_TYPE_LOGICAL_AND;
        in = mathify_getc(obj);
      } else {
        /*LOG("Second Ampersand Not Reached");
        if (
          prevtok.type != MATHIFY_TOKEN_TYPE_EOF && (
            prevtok.type = MATHIFY_TOKEN_TYPE_RPAREN  // bitwise and
            || 
            (prevtok.type >= MATHIFY_TOKEN_TYPE_ADD && prevtok.type < MATHIFY_TOKEN_TYPE_LPAREN)
          )
        ) { */ 
          tmptok.type = MATHIFY_TOKEN_TYPE_BITWISE_AND;
        /* } else { // address of
          tmptok.type = MATHIFY_TOKEN_TYPE_ADDRESS_OF;
        } */
      }
      break;
    case '~': // IMPLEMENTED
      tmptok.type = MATHIFY_TOKEN_TYPE_BITWISE_NOT;
      in = mathify_getc(obj);
      break;
    case '<': // IMPLEMENTED
      in = mathify_getc(obj);
      if (in == '<') {
        tmptok.type = MATHIFY_TOKEN_TYPE_BITWISE_SHIFT_LEFT;
        in = mathify_getc(obj);
      } else if (in == '=') {
        tmptok.type = MATHIFY_TOKEN_TYPE_LTE;
        in = mathify_getc(obj);
      } else {
        tmptok.type = MATHIFY_TOKEN_TYPE_LT;
      }
      break;
    case '>': // IMPLEMENTED
      in = mathify_getc(obj);
      if (in == '>') {
        tmptok.type = MATHIFY_TOKEN_TYPE_BITWISE_SHIFT_RIGHT;
        in = mathify_getc(obj);
      } else if (in == '=') {
        tmptok.type = MATHIFY_TOKEN_TYPE_MTE;
        in = mathify_getc(obj);
      } else {
        tmptok.type = MATHIFY_TOKEN_TYPE_MT;
      }
      break;
    case '!': // IMPLEMENTED
      in = mathify_getc(obj);
      tmptok.type = MATHIFY_TOKEN_TYPE_LOGICAL_NOT;
      break;
    case '=': // Assignments Not Possible Yet
      ERR("Invalid token combination");
      return true;
    case '(': // IMPLEMENTED
      // if previous value isn't an operator assume multiplication
      if (prevtok.type != MATHIFY_TOKEN_TYPE_EOF && !(prevtok.type >= MATHIFY_TOKEN_TYPE_ADD && prevtok.type < MATHIFY_TOKEN_TYPE_LPAREN)) {
        MATHIFY_TOKEN __switch_scope_tok = { .type = MATHIFY_TOKEN_TYPE_MUL };
        MATHIFY_ARENA_PUSH(arena,&__switch_scope_tok,sizeof(MATHIFY_TOKEN));
      }
      tmptok.type = MATHIFY_TOKEN_TYPE_LPAREN;
      in = mathify_getc(obj);
      break;
    case ')': // IMPLEMENTED
      tmptok.type = MATHIFY_TOKEN_TYPE_RPAREN;
      in = mathify_getc(obj);
      break;
    case 'i':
    case 'I': {
      if (prevtok.type != MATHIFY_TOKEN_TYPE_EOF && !(prevtok.type >= MATHIFY_TOKEN_TYPE_ADD && prevtok.type < MATHIFY_TOKEN_TYPE_LPAREN)) {
        MATHIFY_TOKEN __switch_scope_tok = { .type = MATHIFY_TOKEN_TYPE_MUL };
        MATHIFY_ARENA_PUSH(arena,&__switch_scope_tok,sizeof(MATHIFY_TOKEN));
      }
      int base = mathify_getbase(mathify_getc(obj));
      if (base == 0) return true;
      tmptok.type = MATHIFY_TOKEN_TYPE_INT;
      tmptok.val.i = 0;
      in = stoi(obj, &tmptok.val.i, base);
      if (neg) tmptok.val.i = -tmptok.val.i;
      neg = false;
      break;
    } case 'u':
    case 'U': {
      if (prevtok.type != MATHIFY_TOKEN_TYPE_EOF && !(prevtok.type >= MATHIFY_TOKEN_TYPE_ADD && prevtok.type < MATHIFY_TOKEN_TYPE_LPAREN)) {
        MATHIFY_TOKEN __switch_scope_tok = { .type = MATHIFY_TOKEN_TYPE_MUL };
        MATHIFY_ARENA_PUSH(arena,&__switch_scope_tok,sizeof(MATHIFY_TOKEN));
      }
      int base = mathify_getbase(mathify_getc(obj));
      if (base == 0) return true;
      tmptok.type = MATHIFY_TOKEN_TYPE_UINT;
      tmptok.val.u = 0;
      in = stou(obj, &tmptok.val.u, base);
      if (neg) tmptok.val.u = -tmptok.val.u;
      neg = false;
      break;
    } case 'f':
    case 'F': {
      if (prevtok.type != MATHIFY_TOKEN_TYPE_EOF && !(prevtok.type >= MATHIFY_TOKEN_TYPE_ADD && prevtok.type < MATHIFY_TOKEN_TYPE_LPAREN)) {
        MATHIFY_TOKEN __switch_scope_tok = { .type = MATHIFY_TOKEN_TYPE_MUL };
        MATHIFY_ARENA_PUSH(arena,&__switch_scope_tok,sizeof(MATHIFY_TOKEN));
      }
      int base = mathify_getbase(mathify_getc(obj));
      if (base == 0) return true;
      tmptok.type = MATHIFY_TOKEN_TYPE_FLOAT;
      tmptok.val.f = 0;
      in = stof(obj, &tmptok.val.f, base);
      if (neg) tmptok.val.f = -tmptok.val.f;
      neg = false;
      break;
    }
    default:
      ERR("Invalid Token in Mathify Expression");
      return true;
    };
    MATHIFY_ARENA_PUSH(arena,&tmptok,sizeof(MATHIFY_TOKEN));
    prevtok = tmptok;
    
  }
  tmptok.type = MATHIFY_TOKEN_TYPE_EOF;
  MATHIFY_ARENA_PUSH(arena,&tmptok,sizeof(MATHIFY_TOKEN));
  return false;
}

// Stage 2
int get_precedence(enum MATHIFY_TOKEN_TYPE type) {
  switch (type) {
    case MATHIFY_TOKEN_TYPE_LOGICAL_OR: return 3;
    case MATHIFY_TOKEN_TYPE_LOGICAL_AND: return 4;
    case MATHIFY_TOKEN_TYPE_BITWISE_OR: return 5;
    case MATHIFY_TOKEN_TYPE_BITWISE_XOR: return 6;
    case MATHIFY_TOKEN_TYPE_BITWISE_AND: return 7;
    case MATHIFY_TOKEN_TYPE_EQ:
    case MATHIFY_TOKEN_TYPE_NEQ: return 8;
    case MATHIFY_TOKEN_TYPE_LT:
    case MATHIFY_TOKEN_TYPE_LTE:
    case MATHIFY_TOKEN_TYPE_MT:
    case MATHIFY_TOKEN_TYPE_MTE: return 9;
    case MATHIFY_TOKEN_TYPE_BITWISE_SHIFT_LEFT:
    case MATHIFY_TOKEN_TYPE_BITWISE_SHIFT_RIGHT: return 10;
    case MATHIFY_TOKEN_TYPE_ADD: 
    case MATHIFY_TOKEN_TYPE_SUB: return 11;
    case MATHIFY_TOKEN_TYPE_MUL: 
    case MATHIFY_TOKEN_TYPE_DIV: 
    case MATHIFY_TOKEN_TYPE_MOD: return 12;
    case MATHIFY_TOKEN_TYPE_BITWISE_NOT:
    case MATHIFY_TOKEN_TYPE_LOGICAL_NOT: return 13;
    case MATHIFY_TOKEN_TYPE_POSTFIX_ADD:
    case MATHIFY_TOKEN_TYPE_POSTFIX_SUB: return 14;
    // Add Functions, Derferencing, etc...
    default:
      return 0;
  }
}

bool mathify_shunting_yard(MATHIFY_TOKEN *input, MATHIFY_TOKEN *output, MATHIFY_TOKEN **outputend, MATHIFY_TOKEN *operator_stack) {
  int operator_top = -1; // Empty stack
  int output_top = 0;

  for (; input->type != MATHIFY_TOKEN_TYPE_EOF; ++input) {
    if (input->type < MATHIFY_TOKEN_TYPE_ADD) { // Numbers go directly to output
      output[output_top++] = *input;
    } else if (input->type > MATHIFY_TOKEN_TYPE_RPAREN) { // unary operators
      operator_stack[++operator_top] = *input;
    } else if (input->type < MATHIFY_TOKEN_TYPE_LPAREN) { // Operators
      while (operator_top >= 0 &&
             operator_stack[operator_top].type != MATHIFY_TOKEN_TYPE_LPAREN &&
             get_precedence(operator_stack[operator_top].type) >= get_precedence(input->type)) {
        output[output_top++] = operator_stack[operator_top--];
      }
      operator_stack[++operator_top] = *input;
    } else if (input->type == MATHIFY_TOKEN_TYPE_LPAREN) { // Left parenthesis
      operator_stack[++operator_top] = *input;
    } else if (input->type == MATHIFY_TOKEN_TYPE_RPAREN) { // Right parenthesis
      while (operator_top >= 0 && operator_stack[operator_top].type != MATHIFY_TOKEN_TYPE_LPAREN) {
        output[output_top++] = operator_stack[operator_top--];
      }
      if (operator_top < 0 || operator_stack[operator_top].type != MATHIFY_TOKEN_TYPE_LPAREN) {
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
    if (operator_stack[operator_top].type == MATHIFY_TOKEN_TYPE_LPAREN) {
      fprintf(stderr, "Mismatched parentheses!\n");
      return true;
    }
    output[output_top++] = operator_stack[operator_top--];
  }

  *outputend = &output[output_top];
  output[output_top].type = MATHIFY_TOKEN_TYPE_EOF;
  return false;
}


// Stage 3 (Expression)
// arith
bool mathify_add_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i += val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.u += val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
    result->val.f += val->val.f;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  return false;
}
bool mathify_sub_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i -= val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.u -= val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
    result->val.f -= val->val.f;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  return false;
}
bool mathify_mul_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i *= val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.u *= val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
    result->val.f *= val->val.f;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  return false;
}
bool mathify_div_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i /= val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.u /= val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
    result->val.f /= val->val.f;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  return false;
}
bool mathify_mod_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i %= val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.u %= val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
    ERR("Modulus not supported for floats");
    return true;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  return false;
}
bool mathify_or_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i |= val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.u |= val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
    ERR("OR not supported for floats");
    return true;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  return false;
}
bool mathify_xor_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i ^= val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.u ^= val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
   ERR("XOR not supported for floats");
    return true;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  return false;
}
bool mathify_and_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i &= val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.u &= val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
    ERR("AND (bitwise) not supported for floats");
    return true;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  return false;
}
// comparsion
bool mathify_eq_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i = result->val.i == val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.i = result->val.u == val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
    result->val.i = result->val.f == val->val.f;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  result->type = MATHIFY_TOKEN_TYPE_INT;
  return false;
}
bool mathify_neq_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i = result->val.i != val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.i = result->val.u != val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
    result->val.i = result->val.f != val->val.f;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  result->type = MATHIFY_TOKEN_TYPE_INT;
  return false;
}
bool mathify_lt_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i = result->val.i < val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.i = result->val.u < val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
    result->val.i = result->val.f < val->val.f;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  result->type = MATHIFY_TOKEN_TYPE_INT;
  return false;
}
bool mathify_lte_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i = result->val.i <= val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.i = result->val.u <= val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
    result->val.i = result->val.f <= val->val.f;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  result->type = MATHIFY_TOKEN_TYPE_INT;
  return false;
}
bool mathify_mt_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i = result->val.i > val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.i = result->val.u > val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
    result->val.i = result->val.f > val->val.f;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  result->type = MATHIFY_TOKEN_TYPE_INT;
  return false;
}
bool mathify_mte_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i = result->val.i >= val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.i = result->val.u >= val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
    result->val.i = result->val.f >= val->val.f;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  result->type = MATHIFY_TOKEN_TYPE_INT;
  return false;
}
bool mathify_logical_and_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i = result->val.i && val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.i = result->val.u && val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
    result->val.i = result->val.f && val->val.f;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  result->type = MATHIFY_TOKEN_TYPE_INT;
  return false;
}
bool mathify_logical_or_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i = result->val.i || val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.i = result->val.u || val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
    result->val.i = result->val.f || val->val.f;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  result->type = MATHIFY_TOKEN_TYPE_INT;
  return false;
}

// unary
bool mathify_logical_not_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i = !val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.i = !val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
    result->val.i = !val->val.f;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  result->type = MATHIFY_TOKEN_TYPE_INT;
  return false;
}
bool mathify_not_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i = ~val->val.i;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.u = ~val->val.u;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
    ERR("NOT (bitwise) not supported for floats");
    return true;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  return false;
}
bool mathify_postfix_add_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i = val->val.i + 1;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.u = val->val.u + 1;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
   result->val.f = val->val.f + 1;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  return false;
}
bool mathify_postfix_sub_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type == MATHIFY_TOKEN_TYPE_INT) {
    result->val.i = val->val.i - 1;
  } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
    result->val.u = val->val.u - 1;
  } else if (result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
   result->val.f = val->val.f - 1;
  } else {
    ERR("Invalid Mathify Token Type in expression parsing");
    return true;
  }
  return false;
}

bool mathify_rpn_operation(MATHIFY_TOKEN **top, MATHIFY_TOKEN *result) {
  // Assign the operator to a variable for later use and predefine val1 and val2
  MATHIFY_TOKEN *operator = *top;
  MATHIFY_TOKEN val;

  // Remove the operator token
  --(*top);

  // Get the first operand
  if ((*top)->type < MATHIFY_TOKEN_TYPE_ADD) {
    if ((*top)->type > MATHIFY_TOKEN_TYPE_FLOAT) {
      ERR("Runtime Type %hhd in mathify expression", (*top)->type);
      return true;
    }
    val = **top;
  } else {
    if (mathify_rpn_operation(top, &val)) return true;
  }
  --(*top);

  if (operator->type > MATHIFY_TOKEN_TYPE_LOGICAL_OR && operator->type < MATHIFY_TOKEN_TYPE_POWER) {
    result->type = val.type;
    switch (operator->type) {
    case MATHIFY_TOKEN_TYPE_BITWISE_NOT:
      if (mathify_not_expr(result, &val)) return true;
      break;
    case MATHIFY_TOKEN_TYPE_LOGICAL_NOT:
      if (mathify_logical_not_expr(result, &val)) return true;
      break;
    case MATHIFY_TOKEN_TYPE_POSTFIX_ADD:
      ERR("NotImplementedYet - MATHIFY_TOKEN_TYPE_PREFIX_ADD");
      break;
    case MATHIFY_TOKEN_TYPE_POSTFIX_SUB:
      ERR("NotImplementedYet - MATHIFY_TOKEN_TYPE_PREFIX_SUB");
      break;
    default:
      ERR("Invalid Unary Operator");
      return true;
    };
    return false;
  }

  // Get the second operand
  if ((*top)->type < MATHIFY_TOKEN_TYPE_ADD) {
    if ((*top)->type > MATHIFY_TOKEN_TYPE_FLOAT) {
      ERR("Runtime Type %hhd in mathify expression", (*top)->type);
      return true;
    }
    *result = **top;
  } else {
    if (mathify_rpn_operation(top, result)) return true;
  }

  // Type Cast
  if (result->type != val.type) {
    if (result->type == MATHIFY_TOKEN_TYPE_INT && val.type == MATHIFY_TOKEN_TYPE_UINT) {
      result->type = MATHIFY_TOKEN_TYPE_UINT;
      result->val.u = result->val.i;
    } else if (result->type == MATHIFY_TOKEN_TYPE_INT && val.type == MATHIFY_TOKEN_TYPE_FLOAT) {
      result->type = MATHIFY_TOKEN_TYPE_FLOAT;
      result->val.f = result->val.i;
    } else if (val.type == MATHIFY_TOKEN_TYPE_INT && result->type == MATHIFY_TOKEN_TYPE_UINT) {
      val.type = MATHIFY_TOKEN_TYPE_UINT;
      val.val.u = val.val.i;
    } else if (val.type == MATHIFY_TOKEN_TYPE_INT && result->type == MATHIFY_TOKEN_TYPE_FLOAT) {
      val.type = MATHIFY_TOKEN_TYPE_FLOAT;
      val.val.f = val.val.i;
    } else if (result->type == MATHIFY_TOKEN_TYPE_UINT) {
      result->type = MATHIFY_TOKEN_TYPE_FLOAT;
      result->val.f = result->val.u;
    } else if (val.type == MATHIFY_TOKEN_TYPE_UINT) {
      val.type = MATHIFY_TOKEN_TYPE_FLOAT;
      val.val.f = val.val.u;
    }
  }

  switch (operator->type) {
  case MATHIFY_TOKEN_TYPE_ADD:
    if (mathify_add_expr(result, &val)) return true;
    break;
  case MATHIFY_TOKEN_TYPE_SUB:
    if (mathify_sub_expr(result, &val)) return true;
    break;
  case MATHIFY_TOKEN_TYPE_MUL:
    if (mathify_mul_expr(result, &val)) return true;
    break;
  case MATHIFY_TOKEN_TYPE_DIV:
    if (mathify_div_expr(result, &val)) return true;
    break;
  case MATHIFY_TOKEN_TYPE_MOD:
    if (mathify_mod_expr(result, &val)) return true;
    break;
  case MATHIFY_TOKEN_TYPE_BITWISE_OR:
    if (mathify_or_expr(result, &val)) return true;
    break;
  case MATHIFY_TOKEN_TYPE_BITWISE_XOR:
    if (mathify_xor_expr(result, &val)) return true;
    break;
  case MATHIFY_TOKEN_TYPE_BITWISE_AND:
    if (mathify_and_expr(result, &val)) return true;
    break;
  case MATHIFY_TOKEN_TYPE_EQ:
    if (mathify_eq_expr(result, &val)) return true;
    break;
  case MATHIFY_TOKEN_TYPE_NEQ:
    if (mathify_neq_expr(result, &val)) return true;
    break;
  case MATHIFY_TOKEN_TYPE_LT:
    if (mathify_lt_expr(result, &val)) return true;
    break;
  case MATHIFY_TOKEN_TYPE_LTE:
    if (mathify_lte_expr(result, &val)) return true;
    break;
  case MATHIFY_TOKEN_TYPE_MT:
    if (mathify_mt_expr(result, &val)) return true;
    break;
  case MATHIFY_TOKEN_TYPE_MTE:
    if (mathify_mte_expr(result, &val)) return true;
    break;
  case MATHIFY_TOKEN_TYPE_LOGICAL_AND:
    if (mathify_logical_and_expr(result, &val)) return true;
    break;
  case MATHIFY_TOKEN_TYPE_LOGICAL_OR:
    if (mathify_logical_or_expr(result, &val)) return true;
    break;
  default:
    ERR("Invalid Operator Type %hhd", operator->type);
    return true;
  };

  return false;
}

bool mathify_parse_expr(MATHIFY_TOKEN *top, MATHIFY_TOKEN *end, MATHIFY_TOKEN *resulttok) {
  --top;

  if (top->type < MATHIFY_TOKEN_TYPE_ADD) {
    *resulttok = *top;
    return false;
  }
  return mathify_rpn_operation(&top, resulttok);
}

// Stage 3 (Runtime)
bool mathify_parse(MATHIFY_TOKEN *rpn_toks, MATHIFY_ARENA *arena) {
  return true;
}

// Main
bool mathify_main(MATHIFY *obj, MATHIFY_ARENA *arena) {
  // lexer
  MATHIFY_ARENA lextoks_arena;
  MATHIFY_ARENA_CREATE(&lextoks_arena, MATHIFY_ARENA_SIZE(arena));

  if (mathify_lex(obj,&lextoks_arena)) { ERR("Error Lexing Mathematical Expression"); return true; }

  MATHIFY_TOKEN *lextoks = (MATHIFY_TOKEN*)MATHIFY_ARENA_BUFFER(&lextoks_arena);

  // ast
  MATHIFY_ARENA output_arena;
  MATHIFY_ARENA_CREATE(&output_arena, MATHIFY_ARENA_SIZE(&lextoks_arena));
  MATHIFY_ARENA tmpstack_arena;
  MATHIFY_ARENA_CREATE(&tmpstack_arena, MATHIFY_ARENA_SIZE(&lextoks_arena));

  MATHIFY_TOKEN *outputtoks = (MATHIFY_TOKEN*)MATHIFY_ARENA_BUFFER(&output_arena);
  MATHIFY_TOKEN *outputtoks_end;

  if (mathify_shunting_yard(
    lextoks,
    outputtoks,
    &outputtoks_end,
    (MATHIFY_TOKEN*)MATHIFY_ARENA_BUFFER(&tmpstack_arena)
  )) {
    ERR("Error converting mathify expression to Reverse Polish Notation, please report this issue");
    return true;
  }

  MATHIFY_ARENA_DELETE(&lextoks_arena);
  MATHIFY_ARENA_DELETE(&tmpstack_arena);

  // interpret
  mathify_parse(
    outputtoks,
    arena
  );

  // mathify_debug(outputtoks);

  MATHIFY_ARENA_DELETE(&output_arena);
  
  return false;
}

bool mathify_main_expr(MATHIFY *obj, MATHIFY_TOKEN *tok, unsigned long int default_arena_size) {
  // lexer
  MATHIFY_ARENA lextoks_arena;
  MATHIFY_ARENA_CREATE(&lextoks_arena, default_arena_size);

  if (mathify_lex(obj,&lextoks_arena)) { ERR("Error Lexing Mathematical Expression"); return true; }

  MATHIFY_TOKEN *lextoks = (MATHIFY_TOKEN*)MATHIFY_ARENA_BUFFER(&lextoks_arena);

  // ast
  MATHIFY_ARENA output_arena;
  MATHIFY_ARENA_CREATE(&output_arena, MATHIFY_ARENA_SIZE(&lextoks_arena));
  MATHIFY_ARENA tmpstack_arena;
  MATHIFY_ARENA_CREATE(&tmpstack_arena, MATHIFY_ARENA_SIZE(&lextoks_arena));

  MATHIFY_TOKEN *outputtoks = (MATHIFY_TOKEN*)MATHIFY_ARENA_BUFFER(&output_arena);
  MATHIFY_TOKEN *outputtoks_top;
  MATHIFY_TOKEN *outputtoks_bottom = outputtoks - 1;

  if (mathify_shunting_yard(
    lextoks,
    outputtoks,
    &outputtoks_top,
    (MATHIFY_TOKEN*)MATHIFY_ARENA_BUFFER(&tmpstack_arena)
  )) {
    ERR("Error converting mathify expression to Reverse Polish Notation, please report this issue");
    return true;
  }

  MATHIFY_ARENA_DELETE(&lextoks_arena);
  MATHIFY_ARENA_DELETE(&tmpstack_arena);

  // interpret
  if (mathify_parse_expr(
    outputtoks_top,
    outputtoks_bottom,
    tok
  )) {
    ERR("Error consildating the final mathify expression into a value");
    return true;
  }

  MATHIFY_ARENA_DELETE(&output_arena);
  
  return false;
}

void mathify_print_token(MATHIFY_TOKEN *tok) {
  switch (tok->type) {
  case MATHIFY_TOKEN_TYPE_INT:
    LOG("signed integer{%lld}", tok->val.i); break;
  case MATHIFY_TOKEN_TYPE_UINT:
    LOG("unsigned integer{%llu}", tok->val.u); break;
  case MATHIFY_TOKEN_TYPE_FLOAT:
    LOG("float{%Lf}", tok->val.f); break;
  case MATHIFY_TOKEN_TYPE_LPAREN:
    LOG("LPAREN"); break;
  case MATHIFY_TOKEN_TYPE_RPAREN:
    LOG("RPAREN"); break;
  case MATHIFY_TOKEN_TYPE_ADD:
    LOG("ADD"); break;
  case MATHIFY_TOKEN_TYPE_SUB:
    LOG("SUB"); break;
  case MATHIFY_TOKEN_TYPE_MUL:
    LOG("MUL"); break;
  case MATHIFY_TOKEN_TYPE_DIV:
    LOG("DIV"); break;
  case MATHIFY_TOKEN_TYPE_MOD:
    LOG("MOD"); break;
  case MATHIFY_TOKEN_TYPE_BITWISE_OR:
    LOG("OR"); break;
  case MATHIFY_TOKEN_TYPE_BITWISE_XOR:
    LOG("XOR"); break;
  case MATHIFY_TOKEN_TYPE_BITWISE_AND:
    LOG("AND"); break;
  case MATHIFY_TOKEN_TYPE_BITWISE_SHIFT_LEFT:
    LOG("SHIFT LEFT"); break;
  case MATHIFY_TOKEN_TYPE_BITWISE_SHIFT_RIGHT:
    LOG("SHIFT RIGHT"); break;
  case MATHIFY_TOKEN_TYPE_EQ:
    LOG("EQ"); break;
  case MATHIFY_TOKEN_TYPE_NEQ:
    LOG("NEQ"); break;
  case MATHIFY_TOKEN_TYPE_LT:
    LOG("LT"); break;
  case MATHIFY_TOKEN_TYPE_LTE:
    LOG("LTE"); break;
  case MATHIFY_TOKEN_TYPE_MT:
    LOG("MT"); break;
  case MATHIFY_TOKEN_TYPE_MTE:
    LOG("MTE"); break;
  case MATHIFY_TOKEN_TYPE_LOGICAL_AND:
    LOG("LOGICAL AND"); break;
  case MATHIFY_TOKEN_TYPE_LOGICAL_OR:
    LOG("LOGICAL OR"); break;
  case MATHIFY_TOKEN_TYPE_BITWISE_NOT:
    LOG("NOT"); break;
  case MATHIFY_TOKEN_TYPE_LOGICAL_NOT:
    LOG("LOGICAL NOT"); break;
  case MATHIFY_TOKEN_TYPE_POSTFIX_ADD:
    LOG("INCREMENT"); break;
  case MATHIFY_TOKEN_TYPE_POSTFIX_SUB:
    LOG("DECREMENT"); break;
  default:
    ERR("ERR");
    return;
  }
}

void mathify_debug_stack(MATHIFY_TOKEN *end, MATHIFY_TOKEN *top) {
  --top;
  for (; top != end; --top) {
    mathify_print_token(top);
  }
  LOG("\nEOS(end of stack)");
}
void mathify_debug(MATHIFY_TOKEN *tok) {
  for (; tok->type != MATHIFY_TOKEN_TYPE_EOF; ++tok) {
    mathify_print_token(tok);
  }
  LOG("\nEOF");
}