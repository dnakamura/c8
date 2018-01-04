#include <iostream>
#include <exception>

#include <gtest/gtest.h>

#include <c8/AST.hpp>
#include <c8/Debug.hpp>
#include <c8/Lexer.hpp>
#include <c8/Parser.hpp>


namespace c8 {

TEST(c8test, test) {
    // Get the test material directory
    auto test_material = getenv("C8_TEST_MATERIAL");
    ASSERT_NE(test_material, nullptr);

    // std::string test_file = test_material + "/test.js";
    std::string test_file = test_material;
    test_file += "/test.js";
    std::cout << test_file;

    // std::cout << (test_material += "/test.js");


    TokenStream ts;

    c8::Lexer lex(test_file.c_str(),ts);

    lex();
    Parser parser(ts);

    ast::NodePtr<ast::FunctionDeclaration> funcptr = parser.ParseFunctionDef();
    ast::DebugPrinter dbg;
    dbg.Visit(funcptr.get());
}
} // namespace c8

