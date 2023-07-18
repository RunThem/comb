/* libs */

#include "comb.h"
#include "u/str.h"
#include "u/u.h"

int main(int argc, const char** argv) {
  ast_t* ast = nullptr;

  comb(number);
  comb(string);
  comb(factor);
  comb(term);
  comb(expr);
  comb(array);

  input_t in[] = {
      {str_new("1+3*(3-2)-32-3*21")},
      {str_new("[1,2,3,4,5]")},
      {str_new("[]")},
      {str_new("\"hello\"")},
      {str_new("23")},
      {str_new("23e")},
      {str_new("12.3")},
      {str_new("3234.42452")},
  };

  let(number) = R("[0-9]+(\\.[0-9]*)?");
  let(string) = R("\"[^\"]*\"");

  // let(factor) = O(number, A(P("("), expr, P(")")));
  // let(term)   = A(factor, M(O(P("*"), P("/")), factor));
  // let(expr)   = A(term, M(O(P("+"), P("-")), term));

  let(array) = A(P("["), M(number), M(P(","), number), P("]"));

  // comb_dump(expr);

  ast = parse(&in[1], array);
  ast_dump(ast);

  ast = parse(&in[2], array);
  ast_dump(ast);

  ast = parse(&in[3], string);
  ast_dump(ast);

  ast = parse(&in[4], number);
  ast_dump(ast);

  ast = parse(&in[5], number);
  ast_dump(ast);

  ast = parse(&in[6], number);
  ast_dump(ast);

  ast = parse(&in[7], number);
  ast_dump(ast);

  return 0;
}
