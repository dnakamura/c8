#include "c8/Lexer.hpp"

#include <iostream>
#include <string>

using namespace c8;

void Lexer::push_token(TokenKind kind) {
  SourceLocation dummy_loc;

  switch (kind) {
    case tok::string_literal: {
      std::string string_value(ts + 1, te - 1);
      ts_.Put(Token(dummy_loc, tok::string_literal, string_value));
      break;
    }
    case tok::identifier: {
      std::string string_value(ts, te);
      ts_.Put(Token(dummy_loc, tok::identifier, string_value));
      break;
    }
    case tok::numeric_literal:
      // TODO floating point is broken
      {
        std::string string_value(ts, te);
        int n = std::stoi(string_value, nullptr, 10);
        ts_.Put(Token(dummy_loc, static_cast<double>(n)));
        break;
      }
    default:
      ts_.Put(Token(dummy_loc, kind));
  }
}
