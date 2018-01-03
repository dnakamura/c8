#ifndef C8_LEXER_HPP
#define C8_LEXER_HPP

#include <lingo/character.hpp>
#include <lingo/symbol.hpp>
#include <lingo/token.hpp>
#include "Tokens.hpp"

namespace c8 {

class Lexer {
  lingo::Character_stream &cs_;
  lingo::Token_stream &ts_;
  lingo::Symbol_table symbols_;

  const char *ts, *te;
  const char *linestart;
  int curline;

 public:
  Lexer(lingo::Character_stream &cs, lingo::Token_stream &ts)
      : cs_(cs), ts_(ts) {
    init_operators();
  }

  void operator()() { lex(); }

  /// Run ragel generated lexer
  void lex();

  lingo::Token scan();

 private:
  void push_token(TokenKind kind);
  void init_operators();
};
}  // namespace c8

#endif
