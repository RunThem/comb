#pragma once

#include <u/str.h>
#include <u/vec.h>

typedef struct {
  str_t input;
  size_t idx;

  bool err;
} input_t;

typedef enum {
  C_MATCH,
  C_COMB,
  C_OR,
  C_AND,
  C_MAYBE,
  C_MAYB1,
} tag_t;

typedef struct __ast_t {
  vec(struct __ast_t*) children;
  str_t match;
} ast_t;

typedef struct __comb_t {
  tag_t tag;
  union {
    /* match */
    str_t match;

    /* children */
    vec(struct __comb_t*) children;
  };

  /* define */
  struct __comb_t** forward;
} comb_t;

void ast_dump(ast_t* ast);
void comb_dump(comb_t* comb);

comb_t* Match(c_str match);
comb_t* Comb(tag_t tag, size_t cnt, ...);

/* clang-format off */
#undef __va_size
#undef va_size
#define __va_size(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _n, ...) _n
#define va_size(...) __va_size("ignore" __VA_OPT__(, ) __VA_ARGS__, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
/* clang-format on */

#define P(s)      Match(s)
#define O(arg...) Comb(C_OR, va_size(arg), arg)
#define A(arg...) Comb(C_AND, va_size(arg), arg)
#define L(arg...) Comb(C_MAYB1, va_size(arg), arg)
#define M(arg...) Comb(C_MAYBE, va_size(arg), arg)

ast_t* parse(input_t* in, comb_t* comb);

comb_t* __forward(comb_t** comb);
#define forward(x)                                                                                 \
  comb_t* x;                                                                                       \
  x = __forward(&x)