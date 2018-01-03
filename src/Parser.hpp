#ifndef C8_PARSER_HPP
#define C8_PARSER_HPP

#include "AST.hpp"
#include "Token.hpp"

namespace c8 {

class Parser {
  TokenStream &ts_;

 public:
  void Expect(const Token &token, TokenKind kind);
  Token Expect(TokenKind kind);

  ast::NodeVector<ast::Identifier> ParseParameters();
  ast::NodeVector<ast::Expression> ParseArguments();
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

  Parser(TokenStream &ts) : ts_(ts) {}

  void *Parse();
};
}  // namespace c8

#endif
