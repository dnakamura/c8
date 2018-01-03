#include "Debug.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Token.hpp"

#include <lingo/character.hpp>
#include <lingo/error.hpp>
#include <lingo/file.hpp>
#include <lingo/io.hpp>

#include <exception>
#include <iostream>

using namespace c8;

int main(int argc, char **argv) {
  std::set_terminate(__gnu_cxx::__verbose_terminate_handler);
  if (argc != 2) {
    std::cerr << "usage: c8 <input-file>\n";
    return -1;
  }

  lingo::File input(argv[1]);
  lingo::Character_stream cs(input);
  TokenStream ts;

  c8::Lexer lex(cs, ts);

  lex();
  Parser parser(ts);
  std::cout << "Finished lexing\n";

  ast::NodePtr<ast::FunctionDeclaration> funcptr = parser.ParseFunctionDef();
  ast::DebugPrinter dbg;
  dbg.Visit(funcptr.get());

  return 0;
}
