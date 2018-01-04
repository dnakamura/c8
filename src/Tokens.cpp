#include "c8/Token.hpp"

using namespace c8;

const char* c8::TokenKindToString(TokenKind kind) {
  switch (kind) {
#define TOK(x) \
  case tok::x: \
    return #x;
#include "c8/TokenKinds.def"
#undef TOK
    default:
      return "???";
  }
}

const char* c8::TokenSpelling(TokenKind kind) {
  switch (kind) {
#define TOK(x) \
  case tok::x: \
    return #x;
#define PUNCTUATOR(x, y) \
  case tok::x:           \
    return y;
#define KEYWORD(x)  \
  case tok::kw_##x: \
    return #x;
#include "c8/TokenKinds.def"
#undef TOK
    default:
      return "???";
  }
}
