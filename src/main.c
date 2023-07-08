/* libs */

#include "comb.h"

#include <u/vec.h>

int main(int argc, const char** argv) {

  input_t in = {
      .idx   = 0,
      .input = str_new("hello world;!"),
  };

  auto hello = P("hello");
  auto world = P("world");

  auto expr = A(hello, P(" "), world, A(P(";"), P("!")));
  auto forw = A(hello, world, expr, P(";"));

  comb_dump(expr);

  auto ast = parse(&in, expr);

  ast_dump(ast);

  return 0;
}
