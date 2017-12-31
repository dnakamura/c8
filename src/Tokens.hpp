#ifndef TOKEN_H
#define TOKEN_H

namespace c8 {
namespace tok {
enum TokenKind {
#define TOK(x) x,
#include "TokenKinds.def"
  //#undef TOK
};
}  // namespace tok

typedef tok::TokenKind TokenKind;

const char* TokenKindToString(TokenKind kind);
}  // namespace c8
#endif
