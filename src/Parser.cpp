#include "Parser.hpp"
#include "Tokens.h"
#include <iostream>
#include <memory>
using namespace c8;
using namespace c8::ast;

class UnimplementedException: public std::exception {
    const char *const str_;
    public:
    UnimplementedException(const char *str = ""): str_(str){}
    const char* what() const noexcept {return str_;}
};

namespace {
    //TODO this needs to be moved somewhere better
    static inline bool operator==(const lingo::Token& token, const TokenKind kind) {
        return token.kind() == kind;
    }
    static inline bool operator!=(const lingo::Token& token, const TokenKind kind) {
        return token.kind() != kind;
    }

    static inline bool IsAssignementOperator(TokenKind kind){
        switch(kind){
            case tok::equal:
            return true;
            default:
            return false;
        }
    }

    static inline bool IsAssignementOperator(const lingo::Token &token){
        return IsAssignementOperator((TokenKind) token.kind());
    }

    int BinaryPrecedence(int kind){
        switch((TokenKind)kind){
            case tok::equal:
            case tok::plusequal:
            case tok::minusequal:
            case tok::starequal:
            case tok::percentequal:
            case tok::starstarequal:
            case tok::lesslessequal:
            case tok::greatergreaterequal:
            case tok::greatergreatergreaterequal:
            case tok::ampequal:
            case tok::pipeequal:
            case tok::caretequal:
                return 1;
            case tok::pipepipe:
                return 5;
            case tok::ampamp:
                return 6;
            case tok::pipe:
                return 7;
            case tok::caret:
                return 8;
            case tok::amp:
                return 9;
            case tok::plus:
            case tok::minus:
                return 13;
            case tok::star:
            case tok::slash:
            case tok::percent:
                return 14;
            case tok::starstar:
                return 15;
            default:
                return 0;
        }
    }
}

NodePtr<Expression> Parser::ParseBinaryExpression(){
    //TODO this whole thing needs to be redone
    NodeVector<Expression> expression_stack;
    //TODO: should operator_stack hold pointers?
    std::vector<lingo::Token> operator_stack; 
    NodePtr<Expression> expr = ParseUnaryExpression();

    // Check precedence of next operator on stack
    int precedence = BinaryPrecedence(ts_.peek().kind());
    // If it isnt a binary operator we are done parsing
    if (precedence == 0){
        return expr;
    }
    
    expression_stack.push_back(std::move(expr));
    //operator_stack.push_back(ts_.get());
    //expression_stack.push_back

    // While we keep seeing valid binary operators
    while((precedence = BinaryPrecedence(ts_.peek().kind()) ) > 0) {

        //Reduce
        //While our expression stack has at least 2 items and we have a lower precendence operator
        while((expression_stack.size() > 1) &&  (precedence <= BinaryPrecedence(operator_stack.back().kind()))){
            NodePtr<Expression> right = std::move(expression_stack.back());
            expression_stack.pop_back();

            NodePtr<Expression> left = std::move(expression_stack.back());
            expression_stack.pop_back();

            expression_stack.push_back(
                std::make_unique<BinaryExpression>(
                    std::move(left),
                    std::move(right),
                    operator_stack.back()
                )
            );
            operator_stack.pop_back();
        }

        operator_stack.push_back(ts_.get());
        expression_stack.push_back(ParseUnaryExpression());
    }

    //keep reducing until we only have 1 expression
    // TODO would be nice to refactor this since this is just dumb duplicate code
    while(expression_stack.size() > 1){
        NodePtr<Expression> right = std::move(expression_stack.back());
            expression_stack.pop_back();

            NodePtr<Expression> left = std::move(expression_stack.back());
            expression_stack.pop_back();

            expression_stack.push_back(
                std::make_unique<BinaryExpression>(
                    std::move(left),
                    std::move(right),
                    operator_stack.back()
                )
            );
            operator_stack.pop_back();
    }
    expr = std::move(expression_stack.back());
    expression_stack.pop_back();
    return expr;
}

NodePtr<Expression> Parser::ParseUnaryExpression(){
    switch(ts_.peek().kind()){
        case tok::plusplus:
        case tok::minusminus:
        throw UnimplementedException("ParseUnaryExpression - ++/--");
        case tok::plus:
        case tok::minus:
        case tok::tilde:
        case tok::exclaim:{
            //eat the leading token
            lingo::Token operator_token = ts_.get();
            return std::make_unique<UnaryExpression>(operator_token, ParseUnaryExpression());
        }
        default:
            return ParsePostfixExpression();
    }
}

NodePtr<Expression> Parser::ParsePostfixExpression(){
    NodePtr<Expression> expr = ParseLHSExpression();
    switch(ts_.peek().kind()){
        case tok::plusplus:
        case tok::minusminus:
            //TODO make sure ther is no line terminator
            throw UnimplementedException("ParsePostfixExpression: ++/--");
        default:
        return expr;
    }
}

//ast::NodePtr<ast::Expression> ParseComputedMemberExpression();
NodePtr<Expression> Parser::ParseLHSExpression(){
    NodePtr<Expression> expr = (ts_.peek() == tok::kw_new) ? nullptr : ParsePrimaryExpression();
    if(ts_.peek() == tok::kw_new){
        //TODO we need to handle kw_new properly
        throw new UnimplementedException("ParseLHSExpression 'new'");
    } 
    while(true){

        switch(ts_.peek().kind()){
            case tok::l_paren:
                //TODO parse a call expression
                throw std::exception();
            case tok::l_square:
            {
                ts_.get();
                expr = std::make_unique<MemberExpression>(std::move(expr), ParseExpression(), true);
                ast::NodePtr<Expression> accessor_expr = ParseExpression();
                Expect(tok::l_square);
            
                
                //break;
            }
            break;
            case tok::period:
            {
                ts_.get();
                expr = std::make_unique<MemberExpression>(std::move(expr), ParseExpression(), false);
                ast::NodePtr<Expression> accessor_expr = ParseExpression();

                throw std::exception();
            }
            default:
             return expr;
        }
    }
    return expr;
}

NodePtr<Expression> Parser::ParseAssignmentExpression() {
    NodePtr<Expression> left_side = ParseConditionalExpression();
    if(IsAssignementOperator(ts_.peek())){
        //We dont actually handle assignements
        throw UnimplementedException("ParseAssignmentExpression");
    }
    return left_side;
}

NodePtr<Expression> Parser::ParseConditionalExpression() {

    NodePtr<Expression> expr = ParseBinaryExpression();

    if(ts_.peek() == tok::question){
        throw UnimplementedException("ParseConditionalExpression");
        //TODO implement this
    }
    return expr;
}

NodePtr<Expression> Parser::ParseUpdateExpression() {
    int kind = ts_.peek().kind();
    if( kind == tok::plusplus || kind == tok::minusminus){
        NodePtr<UpdateExpression> expr = std::make_unique<UpdateExpression>();
        expr->prefix = true;
        expr->oper = ts_.get();
        expr->argument = ParseUnaryExpression();
        return expr;
    }

    NodePtr<Expression> child_expr = ParseLHSExpression();
    kind =  ts_.peek().kind();
    if( kind == tok::plusplus || kind == tok::minusminus){
        NodePtr<UpdateExpression> expr;
        expr = std::make_unique<UpdateExpression>();
        expr->prefix = false;
        expr->oper = ts_.get();
        expr->argument = std::move(child_expr);
        return expr;
    } else {
        return child_expr;
    }
}


lingo::Token Parser::Expect(TokenKind kind){
    lingo::Token t = ts_.get();
    if(t.kind() != kind){
        std::cerr << "Unexpected Token Error\n";
        throw std::exception();
    }
    return t;
}

void Parser::Expect(const lingo::Token &token, TokenKind kind){
    if(token != kind) {
        std::cerr << "Unexpected token\n";
        throw std::exception();
    }
}

NodePtr<FunctionDeclaration> Parser::ParseFunctionDef(){
    Expect(tok::kw_function);
    NodePtr<FunctionDeclaration> function(new FunctionDeclaration());

    lingo::Token id  = Expect(tok::identifier);
    function->id = std::make_unique<Identifier>(id.symbol());
    //params, body
    function->params = ParseParameters();
    function->body = std::move(ParseBlock());

    return function;
}

NodeVector<Identifier> Parser::ParseParameters(){
    Expect(tok::l_paren);
    NodeVector<Identifier> params;
    //lingo::Token token;

    /*while((token = ts_.get()) == tok::identifier){
        params.push_back(std::make_unique<Identifier>(token.symbol()));     
    }

    if(tok != tok::r_paren)*/
    //token = ts_.get();
    // we may have no arguments, so just return
    if(ts_.peek() == tok::r_paren){
        ts_.get();
        return params;
    }
    while(true) {
        lingo::Token token = Expect(tok::identifier);
        params.push_back(std::make_unique<Identifier>(token.symbol()));
        token = ts_.get();
        if(token == tok::r_paren){
            break;
        }
        Expect(token, tok::comma);
    }
    return params;
}

NodePtr<BlockStatement> Parser::ParseBlock(){
    Expect(tok::l_brace);
    NodePtr<BlockStatement> block = std::make_unique<BlockStatement>();

    while(ts_.peek() != tok::r_brace){
        block->body.push_back(ParseSourceElement());
    }
    Expect(tok::r_brace);
    return block;
}


NodePtr<Expression> Parser::ParseExpression(){
    NodePtr<Expression> expr = ParseAssignmentExpression();
    if(ts_.peek() == tok::comma){
        throw UnimplementedException("Commas not handled in ParseExpression");
    }
    return expr;
}

NodePtr<Expression> Parser::ParsePrimaryExpression(){
    switch(ts_.peek().kind()){
        case tok::kw_this:
        case tok::identifier:
            return std::make_unique<Identifier>(ts_.get().symbol());

        //various literals
        case tok::kw_null:
        case tok::kw_true:
        case tok::kw_false:
        case tok::string_literal:
        case tok::numeric_literal:
            return std::make_unique<Literal>(ts_.get().symbol());
        default:
        //unexpected token??
            std::cerr << "unexpected token '" << ts_.peek().symbol()->spelling() <<"'\n";
            
            throw std::exception();
    }
}

//TODO this is a big hack
NodePtr<Statement> Parser::ParseStatement(){
    switch(ts_.peek().kind()){

        case tok::l_brace: return ParseBlock();
        case tok::semi: throw UnimplementedException("ParseStatement - Semicolon"); //TODO need actually return an empty statement
        case tok::kw_export: throw std::exception();
        case tok::kw_for: throw std::exception();
        case tok::kw_if: throw std::exception();
        case tok::kw_return:{
            ts_.get(); // Eat the token
            NodePtr<Statement> stmt =  std::make_unique<ReturnStatement>(ParseExpression());
            Expect(tok::semi);
            return stmt;
        }
        case tok::kw_var: throw std::exception();
        case tok::kw_while: throw std::exception();
            break;
        default:
            break;
    }
    throw std::exception();
}

NodePtr<Statement> Parser::ParseSourceElement(){
    switch(ts_.peek().kind()){
    case tok::kw_function:
        return ParseFunctionDef();
        //TODO we need to handle const and let
    default:
        return ParseStatement();
    }
}

void *Parser::Parse(){

    return NULL;
}
