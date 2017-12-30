#ifndef C8_PARSER_HPP
#define C8_PARSER_HPP

#include <lingo/token.hpp>
#include "Tokens.hpp"

#include "AST.hpp"

namespace c8 {

class Parser {
  lingo::Token_stream &ts_;

 public:
  void Expect(const lingo::Token &token, TokenKind kind);
  lingo::Token Expect(TokenKind kind);

  ast::NodeVector<ast::Identifier> ParseParameters();
  ast::NodePtr<ast::BlockStatement> ParseBlock();
  ast::NodePtr<ast::Statement> ParseStatement();
  ast::NodePtr<ast::Statement> ParseSourceElement();
  ast::NodePtr<ast::IfStatement> ParseIfStatement();
  ast::NodePtr<ast::ForStatement> ParseForStatement();

  ast::NodePtr<ast::Expression> ParseExpression();
  ast::NodePtr<ast::ExpressionStatement> ParseExpressionStatement();
  ast::NodePtr<ast::Expression> ParseAssignmentExpression();
  ast::NodePtr<ast::Expression> ParseConditionalExpression();
  ast::NodePtr<ast::Expression> ParseBinaryExpression();
  ast::NodePtr<ast::Expression> ParseUnaryExpression();
  ast::NodePtr<ast::Expression> ParsePostfixExpression();

  ast::NodePtr<ast::Expression> ParseUpdateExpression();
  ast::NodePtr<ast::Expression> ParseLHSExpression();

  ast::NodePtr<ast::Expression> ParsePrimaryExpression();
  ast::NodePtr<ast::Expression> ParseComputedMemberExpression();

 public:
  ast::NodePtr<ast::FunctionDeclaration> ParseFunctionDef();

  Parser(lingo::Token_stream &ts) : ts_(ts) {}

  void *Parse();
};
}

#endif
