/* libs */

#include "comb.h"

#include <u/vec.h>

int main(int argc, const char** argv) {
  auto hello = P("hello");
  auto world = P("world");

  auto expr = A(hello, P(" "), world);
  auto forw = O(hello, world, expr, P(";"));

  comb_dump(0, forw);

  return 0;
}
