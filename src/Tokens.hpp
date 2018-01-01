#ifndef C8_TOKENS_HPP
#define C8_TOKENS_HPP

namespace c8 {
namespace tok {
enum TokenKind {
#define TOK(x) x,
#include "TokenKinds.def"
};
}

typedef tok::TokenKind TokenKind;

const char* TokenKindToString(TokenKind kind);
}
#endif
