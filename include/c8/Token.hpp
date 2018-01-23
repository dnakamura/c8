#ifndef C8_TOKEN_HPP
#define C8_TOKEN_HPP
#include <cassert>
#include <list>
#include <string>

namespace c8 {

namespace tok {
enum TokenKind {
#define TOK(x) x,
#include "TokenKinds.def"
};
}  // namespace tok
typedef tok::TokenKind TokenKind;

const char *TokenKindToString(TokenKind kind);
const char *TokenSpelling(TokenKind kind);
TokenKind TokenKindFromString(const std::string & str);


struct Position {
  int line;
  int column;

  bool operator==(const Position&) const;
  bool operator!=(const Position&) const;
};

inline bool Position::operator==(const Position& other) const {
  return (line == other.line) && (column == other.column);
}

inline bool Position::operator!=(const Position& other) const {
  return !(*this == other);
}

// TODO we should actually implement SourceLocation
struct SourceLocation {
  //TODO: we need to track which file it came from
  Position start;
  Position end;

  bool operator==(const SourceLocation &) const;
  bool operator!=(const SourceLocation &) const;
};
inline bool SourceLocation::operator==(const SourceLocation& other) const {
  return (start == other.start) && (end == other.end);
}

inline bool SourceLocation::operator!=(const SourceLocation& other) const {
  return !(*this == other);
}

class Token {
 public:
  Token(const SourceLocation &loc, TokenKind kind);
  Token(const SourceLocation &loc, TokenKind kind,
        const std::string &string_value);
  Token(const SourceLocation &loc, double numeric_value);
  Token();
  const SourceLocation &location() const { return loc_; };
  TokenKind kind() const { return kind_; };

  double numeric_value() const;
  std::string string_value() const;

  std::string ToString() const;

  bool operator==(const Token& other) const;
  bool operator!=(const Token& other) const { return !(*this == other);}
 private:
  SourceLocation loc_;
  TokenKind kind_;
  std::string string_value_;
  double numeric_value_;
};

inline Token::Token(const SourceLocation &loc, TokenKind kind)
    : kind_(kind), loc_(loc) {}
inline Token::Token(const SourceLocation &loc, TokenKind kind,
                    const std::string &string_value)
    : kind_(kind), loc_(loc), string_value_(string_value) {
  assert(kind == tok::identifier || kind == tok::string_literal);
}

inline Token::Token(const SourceLocation &loc, double numeric_value)
    : kind_(tok::numeric_literal), loc_(loc), numeric_value_(numeric_value) {}

inline Token::Token() : kind_(tok::invalid) {}
inline double Token::numeric_value() const {
  assert(kind_ == tok::numeric_literal);
  return numeric_value_;
}

inline std::string Token::string_value() const {
  assert(kind_ == tok::identifier || kind_ == tok::string_literal);
  return string_value_;
}

inline std::string Token::ToString() const {
  if (kind_ == tok::identifier || kind_ == tok::string_literal) {
    return string_value_;
  } else {
    return TokenSpelling(kind_);
  }
}

inline bool Token::operator==(const Token& other) const {
  if(kind_ != other.kind_)
    return false;
  //TODO compare source locations
  switch(kind_){
    case tok::identifier:
    case tok::string_literal:
      return  string_value_ == other.string_value_;
    case tok::numeric_literal:
      return numeric_value_ == other.numeric_value_;
    default:
      //Other tokens only have kind, which matches
      return true;
  }
}

// TODO redisign this api. Its currently set up to be source compatable with
// lingo to ease the transition
class TokenStream {
 public:
  typedef std::list<Token> TokenList;
  typedef TokenList::iterator Iterator;

  TokenStream();
  void Put(Token);
  Token Get();
  Token Peek();
  Token Peek(int);
  bool is_eof();
  void Reset() { position_ = list_.begin(); }

  std::string Serialize() const;
  bool operator==(const TokenStream& other) const;
  bool operator!=(const TokenStream& other) const {return !(*this == other);}

 private:
  TokenList list_;
  Iterator position_;
};

inline TokenStream::TokenStream() : list_(), position_(list_.begin()) {}

inline void TokenStream::Put(Token t) {
  bool empty = list_.empty();
  list_.push_back(t);
  // if we just inserted into an empty list update the iterator
  if (empty) {
    position_ = list_.begin();
  }
}
inline bool TokenStream::is_eof() { return position_ == list_.end();}
inline Token TokenStream::Get() { return *position_++; }

inline Token TokenStream::Peek() { return *position_; }

inline Token TokenStream::Peek(int lookahead) {
  Iterator it = position_;
  assert(lookahead > 0);
  while (it != list_.end() && lookahead) {
    ++it;
    --lookahead;
  }
  assert(it != list_.end());
  return *it;
}

inline bool TokenStream::operator==(const TokenStream& other) const {
  auto it1 = list_.begin();
  auto it2 = other.list_.begin();
  while(it1 != list_.end()){
    if(it2 == other.list_.end())
      return false;
    if( *it1 != *it2)
      return false;
    ++it1;
    ++it2;
  }
  return it2 == other.list_.end();
}

void DeserializeTokenStream(std::string json, TokenStream &stream);
}  // namespace c8
#endif
