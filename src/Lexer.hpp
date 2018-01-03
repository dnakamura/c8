#ifndef C8_LEXER_HPP
#define C8_LEXER_HPP

#include <fstream>
#include <string>
#include "Token.hpp"

namespace c8 {

class Lexer {
  std::string buffer_;
  TokenStream &ts_;

  const char *ts, *te;
  const char *linestart;
  int curline;

 public:
  Lexer(const char *filename, TokenStream &ts) : ts_(ts) {
    // TODO this is wildly inefficient
    // Read the file into the buffer
    std::ifstream f(filename);
    f.seekg(0, std::ios::end);
    buffer_.reserve(f.tellg());
    f.seekg(0, std::ios::beg);

    std::istreambuf_iterator<char> first = f;
    std::istreambuf_iterator<char> last;
    buffer_.assign(first, last);
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
