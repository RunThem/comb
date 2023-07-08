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
  if (_(ast)->children == nullptr) {
    vec_init(&_(ast)->children);
  }

  vec_push_b(&_(ast)->children, a);
}

static void __ast_dump(int level, ast_t* ast) {
  char space[128] = {0};

  u_ret_no_if(ast == nullptr);

  if (ast->children == nullptr) {
    memset(space, ' ', sizeof(space));
    space[(level - 1) * 4] = '\0';
    printf("%s'%s'\n", space, ast->match->c_str);
  } else {
    vec_for(&ast->children, it) {
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

    vec_for(&comb->children, it) {
      __comb_dump(level + 1, *it);
    }
  }
}

void comb_dump(comb_t* comb) {
  __comb_dump(0, comb);
}

static ast_t* comb_match(input_t* in, comb_t* comb) {
  ast_t* ast = nullptr;

  if (0 != strncmp(&in->input->c_str[in->idx], comb->match->c_str, comb->match->len)) {
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

  vec_for(&comb->children, it) {
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

  _ast = parse(in, *vec_at(&comb->children, 0));
  u_goto_if(_ast == nullptr);

  ast = ast_new();
  ast_add(&ast, _ast);

  for (size_t i = 1; i < comb->children->len; i++) {
    auto it = vec_at(&comb->children, i);
    _ast    = parse(in, *it);

    u_goto_if(_ast == nullptr);

    ast_add(&ast, _ast);
  }

  return ast;

err:
  /* FIX: free ast */
  in->idx = __idx;
  return nullptr;
}

static ast_t* comb_maybe(input_t* in, comb_t* comb) {
  tag_t tag    = comb->tag;
  ast_t* ast   = nullptr;
  ast_t* _ast  = nullptr;
  size_t __idx = in->idx;

  comb->tag = C_AND;
  ast       = parse(in, comb);
  if (ast == nullptr) {
    comb->tag = tag;
    in->idx   = __idx;

    return nullptr;
  }

  while (true) {
    _ast = parse(in, comb);
    if (_ast == nullptr) {
      /* free _ast */
      in->idx = __idx;
      break;
    }

    __idx = in->idx;
    vec_for(&_ast->children, it) {
      ast_add(&ast, *it);
    }
  }

  comb->tag = tag;
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
    case C_MAYBE:
      return comb_maybe(in, comb);
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
  vec_init(&c->children, cnt);

  for (size_t i = 0; i < cnt; i++) {
    vec_push_b(&c->children, va_arg(ap, comb_t*));
  }

  va_end(ap);

  return c;
}