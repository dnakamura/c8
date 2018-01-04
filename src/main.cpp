#include <c8/Debug.hpp>
#include "c8/Lexer.hpp"
#include "c8/Parser.hpp"
#include "c8/Token.hpp"

#include <exception>
#include <iostream>

using namespace c8;

int main(int argc, char **argv) {
  std::set_terminate(__gnu_cxx::__verbose_terminate_handler);
  if (argc != 2) {
    std::cerr << "usage: c8 <input-file>\n";
    return -1;
  }

  TokenStream ts;
  c8::Lexer lex(argv[1], ts);

  lex();
  Parser parser(ts);
  std::cout << "Finished lexing\n";

  ast::NodePtr<ast::FunctionDeclaration> funcptr = parser.ParseFunctionDef();
  ast::DebugPrinter dbg;
  dbg.Visit(funcptr.get());

  return 0;
}
