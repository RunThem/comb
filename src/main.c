/* libs */

#include "comb.h"
#include "u/str.h"

#include <u/vec.h>

int main(int argc, const char** argv) {

  forward(factor);
  forward(term);
  forward(expr);

  inf("factor(%p), expr(%p)", factor, expr);

#if 1
  input_t in = {
      .err   = false,
      .idx   = 0,
      .input = str_new("x+(x-x)*x-x"),
  };

  /* FIX: expr == nullptr */
  factor         = O(P("x"), A(P("("), expr, P(")")));
  term           = A(factor, M(O(P("*"), P("/")), factor));
  *expr->forward = A(term, M(O(P("+"), P("-")), term));

#else
  input_t in = {
      .err   = false,
      .idx   = 0,
      .input = str_new("xxxyxx"),
  };

  expr = M(P("x"));
#endif

  // comb_dump(expr);

  auto ast = parse(&in, expr);

  ast_dump(ast);

  return 0;
}
