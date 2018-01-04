#include "c8/Parser.hpp"
#include "c8/Token.hpp"

#include <iostream>
#include <memory>

using namespace c8;
using namespace c8::ast;

class UnimplementedException : public std::exception {
  const char *const str_;

 public:
  UnimplementedException(const char *str = "") : str_(str) {}
  const char *what() const noexcept { return str_; }
};

namespace {
// TODO this needs to be moved somewhere better
static inline bool operator==(const Token &token, const TokenKind kind) {
  return token.kind() == kind;
}
static inline bool operator!=(const Token &token, const TokenKind kind) {
  return token.kind() != kind;
}

static inline bool IsAssignementOperator(TokenKind kind) {
  switch (kind) {
    case tok::equal:
      return true;
    default:
      return false;
  }
}

static inline bool IsAssignementOperator(const Token &token) {
  return IsAssignementOperator((TokenKind)token.kind());
}

int BinaryPrecedence(int kind) {
  switch ((TokenKind)kind) {
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
    case tok::equalequal:
    case tok::exclaimequal:
    case tok::equalequalequal:
    case tok::exclaimequalequal:
      return 10;
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
}  // namespace

NodePtr<Expression> Parser::ParseBinaryExpression() {
  // TODO this whole thing needs to be redone
  NodeVector<Expression> expression_stack;
  // TODO: should operator_stack hold pointers?
  std::vector<Token> operator_stack;
  NodePtr<Expression> expr = ParseUnaryExpression();

  // Check precedence of next operator on stack
  int precedence = BinaryPrecedence(ts_.Peek().kind());
  // If it isnt a binary operator we are done parsing
  if (precedence == 0) {
    return expr;
  }

  expression_stack.push_back(std::move(expr));
  // operator_stack.push_back(ts_.get());
  // expression_stack.push_back

  // While we keep seeing valid binary operators
  while ((precedence = BinaryPrecedence(ts_.Peek().kind())) > 0) {
    // Reduce
    // While our expression stack has at least 2 items and we have a lower
    // precendence operator
    while ((expression_stack.size() > 1) &&
           (precedence <= BinaryPrecedence(operator_stack.back().kind()))) {
      NodePtr<Expression> right = std::move(expression_stack.back());
      expression_stack.pop_back();

      NodePtr<Expression> left = std::move(expression_stack.back());
      expression_stack.pop_back();

      expression_stack.push_back(std::make_unique<BinaryExpression>(
          std::move(left), std::move(right), operator_stack.back()));
      operator_stack.pop_back();
    }

    operator_stack.push_back(ts_.Get());
    expression_stack.push_back(ParseUnaryExpression());
  }

  // keep reducing until we only have 1 expression
  // TODO would be nice to refactor this since this is just dumb duplicate code
  while (expression_stack.size() > 1) {
    NodePtr<Expression> right = std::move(expression_stack.back());
    expression_stack.pop_back();

    NodePtr<Expression> left = std::move(expression_stack.back());
    expression_stack.pop_back();

    expression_stack.push_back(std::make_unique<BinaryExpression>(
        std::move(left), std::move(right), operator_stack.back()));
    operator_stack.pop_back();
  }
  expr = std::move(expression_stack.back());
  expression_stack.pop_back();
  return expr;
}

NodePtr<Expression> Parser::ParseUnaryExpression() {
  switch (ts_.Peek().kind()) {
    case tok::plusplus:
    case tok::minusminus: {
      Token tok = ts_.Get();
      return std::make_unique<UpdateExpression>(ParseUnaryExpression(), tok,
                                                true);
    }
      throw UnimplementedException("ParseUnaryExpression - ++/--");
    case tok::plus:
    case tok::minus:
    case tok::tilde:
    case tok::exclaim: {
      // eat the leading token
      Token operator_token = ts_.Get();
      return std::make_unique<UnaryExpression>(operator_token,
                                               ParseUnaryExpression());
    }
    default:
      return ParsePostfixExpression();
  }
}

NodePtr<Expression> Parser::ParsePostfixExpression() {
  NodePtr<Expression> expr = ParseLHSExpression();
  switch (ts_.Peek().kind()) {
    case tok::plusplus:
    case tok::minusminus:
      // TODO make sure ther is no line terminator
      // TODO check valid LHS expression, and not bad keywords
      return std::make_unique<UpdateExpression>(std::move(expr), ts_.Get(),
                                                false);
    default:
      return expr;
  }
}

// ast::NodePtr<ast::Expression> ParseComputedMemberExpression();
NodePtr<Expression> Parser::ParseLHSExpression() {
  NodePtr<Expression> expr = (ts_.Peek() == tok::kw_new)
                                 ? throw std::exception()
                                 : ParsePrimaryExpression();
  if (ts_.Peek() == tok::kw_new) {
    // TODO we need to handle kw_new properly
    throw new UnimplementedException("ParseLHSExpression 'new'");
  }
  while (true) {
    switch (ts_.Peek().kind()) {
      case tok::l_paren:
        return std::make_unique<CallExpression>(std::move(expr),
                                                ParseArguments());
      case tok::l_square: {
        ts_.Get();
        expr = std::make_unique<MemberExpression>(std::move(expr),
                                                  ParseExpression(), true);
        NodePtr<Expression> accessor_expr = ParseExpression();
        Expect(tok::l_square);

        // break;
      } break;
      case tok::period: {
        ts_.Get();
        expr = std::make_unique<MemberExpression>(std::move(expr),
                                                  ParseExpression(), false);
      }
      default:
        return expr;
    }
  }
  return expr;
}

NodePtr<Expression> Parser::ParseAssignmentExpression() {
  NodePtr<Expression> left_side = ParseConditionalExpression();
  if (IsAssignementOperator(ts_.Peek())) {
    // We dont actually handle assignements
    throw UnimplementedException("ParseAssignmentExpression");
  }
  return left_side;
}

NodePtr<Expression> Parser::ParseConditionalExpression() {
  NodePtr<Expression> expr = ParseBinaryExpression();

  if (ts_.Peek() == tok::question) {
    throw UnimplementedException("ParseConditionalExpression");
    // TODO implement this
  }
  return expr;
}

NodePtr<Expression> Parser::ParseUpdateExpression() {
  int kind = ts_.Peek().kind();
  if (kind == tok::plusplus || kind == tok::minusminus) {
    NodePtr<UpdateExpression> expr = std::make_unique<UpdateExpression>();
    expr->prefix = true;
    expr->oper = ts_.Get();
    expr->argument = ParseUnaryExpression();
    return expr;
  }

  NodePtr<Expression> child_expr = ParseLHSExpression();
  kind = ts_.Peek().kind();
  if (kind == tok::plusplus || kind == tok::minusminus) {
    NodePtr<UpdateExpression> expr;
    expr = std::make_unique<UpdateExpression>();
    expr->prefix = false;
    expr->oper = ts_.Get();
    expr->argument = std::move(child_expr);
    return expr;
  } else {
    return child_expr;
  }
}

Token Parser::Expect(TokenKind kind) {
  Token t = ts_.Get();
  if (t.kind() != kind) {
    std::cerr << "Unexpected Token Error\n";
    throw std::exception();
  }
  return t;
}

void Parser::Expect(const Token &token, TokenKind kind) {
  if (token != kind) {
    std::cerr << "Unexpected token\n";
    throw std::exception();
  }
}

NodePtr<FunctionDeclaration> Parser::ParseFunctionDef() {
  Expect(tok::kw_function);
  NodePtr<FunctionDeclaration> function(new FunctionDeclaration());

  Token id = Expect(tok::identifier);
  function->id = std::make_unique<Identifier>(id.string_value());
  // params, body
  function->params = ParseParameters();
  function->body = std::move(ParseBlock());

  return function;
}

NodeVector<Identifier> Parser::ParseParameters() {
  Expect(tok::l_paren);
  NodeVector<Identifier> params;

  // we may have no arguments, so just return
  if (ts_.Peek() == tok::r_paren) {
    ts_.Get();
    return params;
  }
  while (true) {
    Token token = Expect(tok::identifier);
    params.push_back(std::make_unique<Identifier>(token.string_value()));
    token = ts_.Get();
    if (token == tok::r_paren) {
      break;
    }
    Expect(token, tok::comma);
  }
  return params;
}

NodePtr<BlockStatement> Parser::ParseBlock() {
  Expect(tok::l_brace);
  NodePtr<BlockStatement> block = std::make_unique<BlockStatement>();

  while (ts_.Peek() != tok::r_brace) {
    block->body.push_back(ParseSourceElement());
  }
  Expect(tok::r_brace);
  return block;
}

NodePtr<Expression> Parser::ParseExpression() {
  NodePtr<Expression> expr = ParseAssignmentExpression();
  if (ts_.Peek() == tok::comma) {
    throw UnimplementedException("Commas not handled in ParseExpression");
  }
  return expr;
}

NodePtr<Expression> Parser::ParsePrimaryExpression() {
  switch (ts_.Peek().kind()) {
    case tok::kw_this:
    case tok::identifier:
      // TODO this may be broken for tok::kw_this
      // also im not sure about using ToString() vs string_value()
      return std::make_unique<Identifier>(ts_.Get().ToString());

    // various literals
    case tok::kw_null:
    case tok::kw_true:
    case tok::kw_false:
    case tok::string_literal:
    case tok::numeric_literal:
      return std::make_unique<Literal>(ts_.Get());
    default:
      // unexpected token??
      std::cerr << "unexpected token '" << ts_.Peek().ToString() << "'\n";

      throw std::exception();
  }
}

// TODO this is a big hack
NodePtr<Statement> Parser::ParseStatement() {
  switch (ts_.Peek().kind()) {
    case tok::l_brace:
      return ParseBlock();
    case tok::semi:
      throw UnimplementedException("ParseStatement - Semicolon");  // TODO need
                                                                   // actually
                                                                   // return an
                                                                   // empty
                                                                   // statement
    case tok::kw_export:
      throw std::exception();
    case tok::kw_for:
      return ParseForStatement();
    case tok::kw_if:
      return ParseIfStatement();
    case tok::kw_return: {
      ts_.Get();  // Eat the token
      NodePtr<Statement> stmt =
          std::make_unique<ReturnStatement>(ParseExpression());
      Expect(tok::semi);
      return stmt;
    }
    case tok::kw_var:
      throw std::exception();
    case tok::kw_while:
      throw std::exception();
      break;
    default:
      break;
  }
  NodePtr<Expression> expr = ParseExpression();
  if (ts_.Peek() == tok::colon) {
    throw UnimplementedException("ParseStatement() - labled statement");
  }

  Expect(tok::semi);
  return std::make_unique<ExpressionStatement>(std::move(expr));
}

NodePtr<ExpressionStatement> Parser::ParseExpressionStatement() {
  throw UnimplementedException("ParseExpressionStatement()");
}

NodePtr<Statement> Parser::ParseSourceElement() {
  switch (ts_.Peek().kind()) {
    case tok::kw_function:
      return ParseFunctionDef();
    // TODO we need to handle const and let
    default:
      return ParseStatement();
  }
}

NodePtr<IfStatement> Parser::ParseIfStatement() {
  Expect(tok::kw_if);
  Expect(tok::l_paren);
  NodePtr<Expression> condition = ParseExpression();
  Expect(tok::r_paren);
  NodePtr<Statement> consequent = ParseStatement();
  if (ts_.Peek() == tok::kw_else) {
    // Eat the else token
    ts_.Get();
    return std::make_unique<IfStatement>(
        std::move(condition), std::move(consequent), ParseStatement());
  } else {
    return std::make_unique<IfStatement>(std::move(condition),
                                         std::move(consequent));
  }
}

NodePtr<ForStatement> Parser::ParseForStatement() {
  Expect(tok::kw_for);
  Expect(tok::l_paren);

  NodePtr<Expression> init;
  // short cut for empty initializer
  if (ts_.Peek() != tok::semi) {
    // TODO should this also check for the let keyword?
    if (ts_.Peek() == tok::kw_var) {
      throw UnimplementedException(
          "ParseForStatement() 'var' not supported in ParseForStatement");
    } else {
      init = ParseExpression();
      if (ts_.Peek() == tok::kw_in) {
        throw UnimplementedException(
            "in keyword not supported in for statement");
      }
    }
  }
  Expect(tok::semi);

  NodePtr<Expression> test;
  if (ts_.Peek() != tok::semi) {
    test = ParseExpression();
  }
  Expect(tok::semi);

  NodePtr<Expression> update;
  if (ts_.Peek() != tok::r_paren) {
    update = ParseExpression();
  }
  Expect(tok::r_paren);
  return std::make_unique<ForStatement>(std::move(init), std::move(test),
                                        std::move(update), ParseStatement());
}

NodeVector<Expression> Parser::ParseArguments() {
  // throw std::exception();
  Expect(tok::l_paren);
  NodeVector<Expression> args;
  if (ts_.Peek() != tok::r_paren) {
    while (true) {
      args.push_back(ParseAssignmentExpression());
      if (ts_.Peek() == tok::r_paren) {
        break;
      }
      Expect(tok::comma);
    }
  }
  Expect(tok::r_paren);
  return args;
}

void *Parser::Parse() { return NULL; }
