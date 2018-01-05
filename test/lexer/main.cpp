#include <c8/Token.hpp>
#include <c8/Lexer.hpp>

#include <gtest/gtest.h>

#include <string>
#include <fstream>
#include <exception>
using namespace c8;
using ::testing::Values;


std::string LoadFileToString(const std::string &filename){
  std::string buffer;
  std::ifstream f(filename);
  if(!f.good())
    throw std::exception();
    f.seekg(0, std::ios::end);
    buffer.reserve(f.tellg());
    f.seekg(0, std::ios::beg);

    std::istreambuf_iterator<char> first = f;
    std::istreambuf_iterator<char> last;
    buffer.assign(first, last);
  return buffer;
}
class LexerTest: public ::testing::TestWithParam<const char *> {
  
};

TEST_P(LexerTest, ProducesValidTokenStream) {
  std::string base(GetParam());
  std::string js_file = base + ".js";
  std::string tokens_file = base + ".tokens";

  TokenStream expected;
  std::string expected_json;
  ASSERT_NO_THROW(expected_json = LoadFileToString(tokens_file));
  ASSERT_NO_THROW(DeserializeTokenStream(expected_json, expected));


  // Lex the input js
  TokenStream lexed_stream;
  Lexer lex(js_file.c_str(), lexed_stream);
  ASSERT_NO_THROW(lex());
  ASSERT_TRUE(lexed_stream == expected);\
  
}
INSTANTIATE_TEST_CASE_P(BaseTests, LexerTest,
  Values("data/test_01")
);
