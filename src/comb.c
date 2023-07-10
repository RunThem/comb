#include "comb.h"

#include "u/u.h"
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
    space[level * 4] = '\0';
    printf("%s'%s'\n", space, ast->match->c_str);
  } else {
    vec_for(&ast->children, it) {
      __ast_dump(level + 1, *it);
    }
  }
}

void ast_dump(ast_t* ast) {
  __ast_dump(-1, ast);
}

void __comb_dump(int level, comb_t* comb) {
  char space[128] = {0};

  u_ret_no_if(comb == nullptr);

  printf("%p: ", comb);

  memset(space, ' ', sizeof(space));
  space[level * 4] = '\0';

  if (comb->tag == C_MATCH) {
    printf("%s'%s'\n", space, comb->match->c_str);
  } else if (comb->tag == C_COMB) {
    printf("%s(%p)\n", space, comb->forward);
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
  int res    = 0;
  ast_t* ast = nullptr;

  res = strncmp(&in->input->c_str[in->idx], comb->match->c_str, comb->match->len);
  u_ret_if(res != 0, nullptr);

  in->idx += comb->match->len;
  ast        = ast_new();
  ast->match = str_new(comb->match->c_str);

  return ast;
}

static ast_t* comb_or(input_t* in, comb_t* comb) {
  ast_t* ast   = nullptr;
  size_t __idx = in->idx;

  vec_for(&comb->children, it) {
    ast = parse(in, *it);

    u_ret_if(ast != nullptr, ast);

    in->idx = __idx;
  }

  return nullptr;
}

static ast_t* comb_and(input_t* in, comb_t* comb) {
  ast_t* ast   = ast_new();
  ast_t* _ast  = nullptr;
  size_t __idx = in->idx;

  for (size_t i = 0; i < comb->children->len; i++) {
    auto it = vec_at(&comb->children, i);
    _ast    = parse(in, *it);

    if (_ast == nullptr) {
      if (_(it)->tag == C_MAYBE) {
        continue;
      }

      /* free ast */
      in->idx = __idx;
      return nullptr;
    }

    if (_(it)->tag == C_MAYBE) {
      ast_add(&ast, _ast);
      // vec_for(&_ast->children, __ast) {
      //   ast_add(&ast, *__ast);
      // }
    } else {
      ast_add(&ast, _ast);
    }
  }

  return ast;
}

static ast_t* comb_maybe(input_t* in, comb_t* comb) {
  tag_t tag    = comb->tag;
  ast_t* ast   = nullptr;
  ast_t* _ast  = nullptr;
  size_t __idx = in->idx;

  comb->tag = C_AND;
  while (true) {
    _ast = parse(in, comb);
    if (_ast == nullptr) {
      break;
    }

    if (ast == nullptr) {
      ast = ast_new();
    }

    __idx = in->idx;
    vec_for(&_ast->children, it) {
      ast_add(&ast, *it);
    }
  }

  in->idx   = __idx;
  comb->tag = tag;
  return ast;
}

ast_t* parse(input_t* in, comb_t* comb) {
  u_ret_if(in->idx == in->input->len, nullptr);

  printf("in('%s'), idx(%ld)\n", in->input->c_str, in->idx);
  switch (comb->tag) {
    case C_COMB:
      return parse(in, *comb->forward);
    case C_MATCH:
      return comb_match(in, comb);
    case C_OR:
      return comb_or(in, comb);
    case C_AND:
      return comb_and(in, comb);
    case C_MAYBE:
    case C_MAYB1:
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

comb_t* __forward(comb_t** comb) {
  comb_t* c = u_talloc(sizeof(comb_t), comb_t*);

  c->tag     = C_COMB;
  c->forward = comb;

  return c;
}