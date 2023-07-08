#include "comb.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static ast_t* ast_new(ast_t* left, ast_t* right) {
  ast_t* ast = u_talloc(sizeof(ast_t), ast_t*);
  u_alloc_if(ast);

  ast->l = left;
  ast->r = right;

  return ast;

err:
  return nullptr;
}

void ast_dump(int level, ast_t* ast) {
  char space[128] = {0};

  u_ret_no_if(ast == nullptr);

  memset(space, ' ', sizeof(space));
  space[level * 4] = '\0';

  printf("%s'%s'\n", space, ast->match->c_str);

  ast_dump(level + 1, ast->l);
  ast_dump(level + 1, ast->r);
}

void comb_dump(int level, comb_t* comb) {
  char space[128] = {0};

  u_ret_no_if(comb == nullptr);

  memset(space, ' ', sizeof(space));
  space[level * 4] = '\0';

  if (comb->tag == C_MATCH) {
    printf("%s'%s'\n", space, comb->match->c_str);
  } else {
    printf("%s(%d)\n", space, comb->tag);

    vec_for(&comb->forward, it) {
      comb_dump(level + 1, *it);
    }
  }
}

comb_t* Match(c_str match) {
  comb_t* c = u_talloc(sizeof(comb_t), comb_t*);

  c->tag   = C_MATCH;
  c->match = str_new(match);

  return c;
}

comb_t* Comb(tag_t tag, size_t cnt, ...) {
  va_list ap;
  comb_t* c = u_talloc(sizeof(comb_t), comb_t*);

  va_start(ap, cnt);

  c->tag = tag;
  vec_init(&c->forward, cnt);

  for (size_t i = 0; i < cnt; i++) {
    vec_push_b(&c->forward, va_arg(ap, comb_t*));
  }

  va_end(ap);

  return c;
}