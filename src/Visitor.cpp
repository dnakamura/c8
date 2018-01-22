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
      Visit(static_cast<BinaryExpression *>(node));
      break;
    case Node::Kind_Identifier:
      Visit(static_cast<Identifier *>(node));
      break;
    case Node::Kind_Literal:
      Visit(static_cast<Literal *>(node));
      break;
    case Node::Kind_UpdateExpression:
      Visit(static_cast<UpdateExpression *>(node));
      break;
    case Node::Kind_UnaryExpression:
      Visit(static_cast<UnaryExpression *>(node));
      break;
    case Node::Kind_MemberExpression:
      Visit(static_cast<MemberExpression *>(node));
      break;
    case Node::Kind_CallExpression:
      Visit(static_cast<CallExpression *>(node));
      break;
    default:
      throw std::exception();
  }
}

void Visitor::VisitStatement(Statement *node) {
  switch (node->Kind()) {
    case Node::Kind_BlockStatement:
      Visit(static_cast<BlockStatement *>(node));
      break;
    case Node::Kind_ReturnStatement:
      Visit(static_cast<ReturnStatement *>(node));
      break;
    case Node::Kind_ExpressionStatement:
      Visit(static_cast<ExpressionStatement *>(node));
      break;
    case Node::Kind_IfStatement:
      Visit(static_cast<IfStatement *>(node));
      break;
    case Node::Kind_ForStatement:
      Visit(static_cast<ForStatement *>(node));
      break;
    default:
      throw std::exception();
  }
  // TODO make this use out llvm-style rtti
}