/* libs */

#include "comb.h"

#include <u/vec.h>

int main(int argc, const char** argv) {

  input_t in = {
      .idx   = 0,
      .input = str_new("-=-=-=---==--=-="),
  };

  auto op   = O(P("-"), P("="));
  auto expr = M(op);

  comb_dump(expr);

  auto ast = parse(&in, expr);

  ast_dump(ast);

  return 0;
}
