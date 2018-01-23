#include "c8/Debug.hpp"
#include "c8/Casting.hpp"
#include "c8/IndentingStream.hpp"

using namespace c8::ast;
void Visitor::VisitNode(Node *node) {
  if (isa<Statement>(node)) {
    VisitStatement(static_cast<Statement *>(node));
  } else if (isa<Expression>(node)) {
    VisitExpression(static_cast<Expression *>(node));
  } else {
    throw std::exception();
  }
}

void Visitor::VisitExpression(Expression *node) {
  // TODO make this use out llvm-style rtti
  switch (node->Kind()) {
    case Node::Kind_BinaryExpression:
      VisitBinaryExpression(static_cast<BinaryExpression *>(node));
      break;
    case Node::Kind_Identifier:
      VisitIdentifier(static_cast<Identifier *>(node));
      break;
    case Node::Kind_Literal:
      VisitLiteral(static_cast<Literal *>(node));
      break;
    case Node::Kind_UpdateExpression:
      VisitUpdateExpression(static_cast<UpdateExpression *>(node));
      break;
    case Node::Kind_UnaryExpression:
      VisitUnaryExpression(static_cast<UnaryExpression *>(node));
      break;
    case Node::Kind_MemberExpression:
      VisitMemberExpression(static_cast<MemberExpression *>(node));
      break;
    case Node::Kind_CallExpression:
      VisitCallExpression(static_cast<CallExpression *>(node));
      break;
    default:
      throw std::exception();
  }
}

void Visitor::VisitStatement(Statement *node) {
  switch (node->Kind()) {
    case Node::Kind_BlockStatement:
      VisitBlockStatement(static_cast<BlockStatement *>(node));
      break;
    case Node::Kind_ReturnStatement:
      VisitReturnStatement(static_cast<ReturnStatement *>(node));
      break;
    case Node::Kind_ExpressionStatement:
      VisitExpressionStatement(static_cast<ExpressionStatement *>(node));
      break;
    case Node::Kind_IfStatement:
      VisitIfStatement(static_cast<IfStatement *>(node));
      break;
    case Node::Kind_ForStatement:
      VisitForStatement(static_cast<ForStatement *>(node));
      break;
    case Node::Kind_FunctionDeclaration:
      VisitFunctionDeclaration(static_cast<FunctionDeclaration*>(node));
      break;
    default:
      throw std::exception();
  }
  // TODO make this use out llvm-style rtti
}