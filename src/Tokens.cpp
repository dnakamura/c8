#include "Tokens.h"

using namespace c8;

const char* c8::TokenKindToString(TokenKind kind) {
  switch (kind) {
#define TOK(x) \
  case tok::x: \
    return #x;
#include "TokenKinds.def"
#undef TOK
    default:
      return "???";
  }
}
