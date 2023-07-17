/* libs */

#include "comb.h"
#include "u/str.h"

#include <re.h>

int main(int argc, const char** argv) {

  comb(factor);
  comb(term);
  comb(expr);

  input_t in = {
      .idx   = 0,
      .input = str_new("x+(x-x)*x-x"),
  };

  let(factor) = O(P("x"), A(P("("), expr, P(")")));
  let(term)   = A(factor, M(O(P("*"), P("/")), factor));
  let(expr)   = A(term, M(O(P("+"), P("-")), term));

  // comb_dump(expr);

  auto ast = parse(&in, expr);

  ast_dump(ast);

  re_t reg = nullptr;

  reg = re_compile("^hello.*world");

  int idx   = 0;
  c_str str = "iefahello isdhgawig worldgei";
  auto res  = re_matchp(reg, str, &idx);

  inf("res(%d)", res);
  inf("%d", idx);
  inf("%c", str[idx]);

  return 0;
}
