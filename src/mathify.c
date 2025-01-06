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
  MATHIFY_TOKEN prevtok;
  MATHIFY_TOKEN tmptok;
  for (int in = mathify_getc(obj); in != EOF;) {
    switch (in) {
    case '+':
      tmptok.type = MATHIFY_TOKEN_TYPE_ADD;
      in = mathify_getc(obj);
      break;
    case '-':
      tmptok.type = MATHIFY_TOKEN_TYPE_SUB;
      in = mathify_getc(obj);
      break;
    case '*':
      tmptok.type = MATHIFY_TOKEN_TYPE_MUL;
      in = mathify_getc(obj);
      break;
    case '/':
      tmptok.type = MATHIFY_TOKEN_TYPE_DIV;
      in = mathify_getc(obj);
      break;
    case '%':
      tmptok.type = MATHIFY_TOKEN_TYPE_MOD;
      in = mathify_getc(obj);
      break;
    case '(':
      // if previous value isn't an operator assume multiplication
      if (!(prevtok.type >= MATHIFY_TOKEN_TYPE_ADD && prevtok.type < MATHIFY_TOKEN_TYPE_LPAREN)) {
        MATHIFY_TOKEN __switch_scope_tok = { .type = MATHIFY_TOKEN_TYPE_MUL };
        MATHIFY_ARENA_PUSH(arena,&__switch_scope_tok,sizeof(MATHIFY_TOKEN));
      }
      tmptok.type = MATHIFY_TOKEN_TYPE_LPAREN;
      in = mathify_getc(obj);
      break;
    case ')':
      tmptok.type = MATHIFY_TOKEN_TYPE_RPAREN;
      in = mathify_getc(obj);
      break;
    case 'i':
    case 'I': {
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
      int base = mathify_getbase(mathify_getc(obj));
      if (base == 0) return true;
      tmptok.type = MATHIFY_TOKEN_TYPE_FLOAT;
      tmptok.val.f = 0;
      in = stof(obj, &tmptok.val.f, base);
      if (neg) tmptok.val.f = -tmptok.val.f;
      neg = false;
      break;
    } default:
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
    case MATHIFY_TOKEN_TYPE_ADD: case MATHIFY_TOKEN_TYPE_SUB: return 1;
    case MATHIFY_TOKEN_TYPE_MUL: case MATHIFY_TOKEN_TYPE_DIV: case MATHIFY_TOKEN_TYPE_MOD: return 2;
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
bool mathify_add_expr(MATHIFY_TOKEN *result, MATHIFY_TOKEN *val) {
  if (result->type != val->type) {
    ERR("Type Mismatch in expression parsing");
    return true;
  }
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
  if (result->type != val->type) {
    ERR("Type Mismatch in expression parsing");
    return true;
  }
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
  if (result->type != val->type) {
    ERR("Type Mismatch in expression parsing");
    return true;
  }
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
  if (result->type != val->type) {
    ERR("Type Mismatch in expression parsing");
    return true;
  }
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
  if (result->type != val->type) {
    ERR("Type Mismatch in expression parsing");
    return true;
  }
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
  default:
    ERR("Invalid Operator Type");
    return true;
  };

  return false;
}

bool mathify_parse_expr(MATHIFY_TOKEN *top, MATHIFY_TOKEN *end, MATHIFY_TOKEN *resulttok) {
  --top;
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

  mathify_debug_stack(outputtoks_bottom, outputtoks_top);

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