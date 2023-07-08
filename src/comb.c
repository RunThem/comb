#include "comb.h"

#include "u/vec.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

ast_t* parse(input_t* in, comb_t* comb);

static ast_t* ast_new() {
  ast_t* ast = u_talloc(sizeof(ast_t), ast_t*);

  return ast;
}

static void ast_add(ast_t** ast, ast_t* a) {
  if (_(ast)->forward == nullptr) {
    vec_init(&_(ast)->forward);
  }

  vec_push_b(&_(ast)->forward, a);
}

static void __ast_dump(int level, ast_t* ast) {
  char space[128] = {0};

  u_ret_no_if(ast == nullptr);

  if (ast->forward == nullptr) {
    memset(space, ' ', sizeof(space));
    space[(level - 1) * 4] = '\0';
    printf("%s'%s'\n", space, ast->match->c_str);
  } else {
    vec_for(&ast->forward, it) {
      __ast_dump(level + 1, *it);
    }
  }
}

void ast_dump(ast_t* ast) {
  __ast_dump(0, ast);
}

void __comb_dump(int level, comb_t* comb) {
  char space[128] = {0};

  u_ret_no_if(comb == nullptr);

  memset(space, ' ', sizeof(space));
  space[level * 4] = '\0';

  if (comb->tag == C_MATCH) {
    printf("%s'%s'\n", space, comb->match->c_str);
  } else {
    printf("%s(%d)\n", space, comb->tag);

    vec_for(&comb->forward, it) {
      __comb_dump(level + 1, *it);
    }
  }
}

void comb_dump(comb_t* comb) {
  __comb_dump(0, comb);
}

static ast_t* comb_match(input_t* in, comb_t* comb) {
  ast_t* ast = nullptr;

  if (0 != str_comp(&comb->match, &in->input->c_str[in->idx])) {
    return nullptr;
  }

  ast        = ast_new();
  ast->match = comb->match;

  in->idx += comb->match->len;
  return ast;
}

static ast_t* comb_or(input_t* in, comb_t* comb) {
  ast_t* ast   = nullptr;
  size_t __idx = in->idx;

  vec_for(&comb->forward, it) {
    in->idx = __idx;
    ast     = parse(in, *it);
    u_ret_if(ast != nullptr, ast);
  }

  in->idx = __idx;
  return nullptr;
}

static ast_t* comb_and(input_t* in, comb_t* comb) {
  ast_t* ast   = nullptr;
  ast_t* _ast  = nullptr;
  size_t __idx = in->idx;

  ast = ast_new();

  _ast = parse(in, *vec_at(&comb->forward, 0));
  if (_ast != nullptr) {
    ast_add(&ast, _ast);
  }

  for (size_t i = 1; i < comb->forward->len; i++) {
    auto it = vec_at(&comb->forward, i);
    _ast    = parse(in, *it);

    if (_ast == nullptr) {
      /* FIX: free ast */
      in->idx = __idx;
      return nullptr;
    }

    ast_add(&ast, _ast);
  }

  return ast;
}

ast_t* parse(input_t* in, comb_t* comb) {
  switch (comb->tag) {
    case C_MATCH:
      return comb_match(in, comb);
    case C_OR:
      return comb_or(in, comb);
    case C_AND:
      return comb_and(in, comb);
    default:
      break;
  }

  return nullptr;
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