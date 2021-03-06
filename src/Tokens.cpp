#include "c8/Token.hpp"
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>

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

c8::TokenKind c8::TokenKindFromString(const std::string& str){
  #define TOK(x) else if (str == #x) return c8::tok::x;
  if(false);
  #include "c8/TokenKinds.def"
  else { return c8::tok::invalid;}
}

namespace {

template<typename Writer>
void SerializePosition(const c8::Position &position, Writer &writer){
  writer.StartObject();
  writer.String("line");
  writer.Int(position.line);
  writer.String("column");
  writer.Int(position.column);
  writer.EndObject();
}

template<typename Writer>
void SerializeSourceLocation(const c8::SourceLocation &loc, Writer &writer){
  writer.StartObject();
  writer.String("start");
  SerializePosition(loc.start, writer);
  writer.String("end");
  SerializePosition(loc.end, writer);
  writer.EndObject();
}


template<typename Writer>
void SerializeToken(const c8::Token &token, Writer &writer){
  writer.StartObject();

  writer.String("type");
  writer.String(c8::TokenKindToString(token.kind()));
  writer.String("loc");
  SerializeSourceLocation(token.location(), writer);
  switch(token.kind()){
    case c8::tok::identifier:
    case c8::tok::string_literal:
      writer.String("value");
      writer.String(token.string_value());
      break;
    case c8::tok::numeric_literal:
      writer.String("value");
      writer.Double(token.numeric_value());
      break;
  }
  writer.EndObject();
}

template<typename Encoding, typename Allocator>
c8::Position DeserializePosition(rapidjson::GenericValue<Encoding, Allocator> &object){
  c8::Position pos;
  assert(object.IsObject());
  auto& line = object["line"];
  assert(line.IsInt());
  auto& column = object["column"];
  assert(column.IsInt());

  pos.line = line.GetInt();
  pos.column = column.GetInt();
  return pos;
}

template<typename Encoding, typename Allocator>
c8::SourceLocation DeserializeSourceLocation(rapidjson::GenericValue<Encoding, Allocator> &object){
  c8::SourceLocation loc;
  assert(object.IsObject());
  loc.start = DeserializePosition(object["start"]);
  loc.end = DeserializePosition(object["end"]);
  return loc;
}



template<typename Encoding, typename Allocator>
c8::Token DeserializeToken(rapidjson::GenericValue<Encoding, Allocator> &object){
  using namespace c8;
  assert(object.IsObject());
  auto& kindString = object["type"];
  assert(kindString.IsString());
  c8::TokenKind kind = TokenKindFromString(kindString.GetString());
  assert(kind != tok::invalid);
  c8::SourceLocation location = DeserializeSourceLocation(object["loc"]);

  switch(kind){
    case tok::identifier:
    case tok::string_literal:
      {
        auto& value = object["value"];
        assert(value.IsString());
        return Token(location, kind, value.GetString());
      }
    case tok::numeric_literal:
    {
      auto &value = object["value"];
      assert(value.IsNumber());
      return Token(location, value.GetDouble());
    }
    default:
      return Token(location, kind);
  }
}

}

std::string TokenStream::Serialize() const{
  rapidjson::StringBuffer sb;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
  writer.StartArray();
  TokenList::const_iterator it = list_.begin();
  for( it = list_.begin(); it != list_.end(); ++it){
    SerializeToken(*it, writer);
  }
  writer.EndArray();
  return sb.GetString();
}



void c8::DeserializeTokenStream(std::string json, TokenStream &stream){
  rapidjson::Document document;
  document.Parse(json);
  assert(document.IsArray());
  auto it = document.Begin();
  while(it != document.End()){
    stream.Put(DeserializeToken(*it));
    ++it;
  }
}
