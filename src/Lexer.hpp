#ifndef C8_LEXER_HPP
#define C8_LEXER_HPP

#include <lingo/character.hpp>

#include "Token.hpp"

namespace c8 {

class Lexer {
  lingo::Character_stream &cs_;
  TokenStream &ts_;

  const char *ts, *te;
  const char *linestart;
  int curline;

 public:
  Lexer(lingo::Character_stream &cs, TokenStream &ts) : cs_(cs), ts_(ts) {
    // init_operators();
  }

  void operator()() { lex(); }

  /// Run ragel generated lexer
  void lex();

  Token scan();

 private:
  void push_token(TokenKind kind);
};
}  // namespace c8

#endif
