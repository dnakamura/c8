#include "Lexer.hpp"

#include <lingo/string.hpp>
#include <string>
#include <iostream>

using namespace lingo;
using namespace c8;



static lingo::Location get_location(lingo::Buffer *buff, const char *loc){
    //TODO check that loc is within bounds of buffer
    return Location(buff,  loc - buff->begin());

}

void Lexer::push_token(TokenKind kind){
    //std::cout << "push_token\n";
    // We need to handle string literals special to get rid of the surrounding quotes
    const bool is_str_lit = kind == tok::string_literal;

    const char * const token_start = is_str_lit ? ts + 1 : ts;
    const char * const token_end  = is_str_lit ? te - 1 : te;
    const size_t token_length = token_end - token_start;
    //TODO check to make sure length >= 0;
    std::string str(token_start, token_length);

    //std::cout <<"Token str = '" << str << "'\n";

    const Symbol *sym = NULL;
    switch(kind){
        case tok::string_literal:
            sym = symbols_.put_string(tok::string_literal, str, str);
            break;
        case tok::numeric_literal:
            //TODO floating point is broken
            {
                int n = string_to_int<int>(str, 10);
                sym = symbols_.put_integer(tok::numeric_literal, str, n);
                break;
            }
        case tok::identifier:
            sym = symbols_.put_identifier(kind, str);
            break;
        #define KEYWORD(x) case tok::kw_##x:
        #define PUNCTUATOR(x,y) case tok::x:
        #include "TokenKinds.def"
            sym = symbols_.get(str);
            break;
        // Handle keywords and punctuators
    //    default:
    
    }


    Token tok = Token(get_location(&((Buffer&)cs_), token_start), sym);

    //std::cout << "DBG: inserting token " << tok << "\n";
    ts_.put(tok);
}

void Lexer::init_operators(){
    //std::cout << "Init operators\n";
    // Add symbols for our punctuators/keywords.
    #define KEYWORD(x) symbols_.put_symbol(tok::kw_##x, #x);
    #define PUNCTUATOR(x,y) symbols_.put_symbol(tok::x, y);
    #include "TokenKinds.def"
    //#undef KEYWORD
    //#undef PUNCTUATOR

}
