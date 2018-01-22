#ifndef C8_DEBUG_HPP
#define C8_DEBUG_HPP

#include <iostream>
#include "AST.hpp"
#include "IndentingStream.hpp"

namespace c8 {
namespace ast {
class DebugPrinter : public Visitor {
 public:
  void VisitFunctionDeclaration(FunctionDeclaration *node) override;
  void VisitBlockStatement(BlockStatement *node) override;
  void VisitReturnStatement(ReturnStatement *node) override;
  void VisitIfStatement(IfStatement *node) override;
  void VisitForStatement(ForStatement *node) override;
  void VisitExpressionStatement(ExpressionStatement *node) override { VisitExpression(node->expression.get()); }

  void VisitLiteral(Literal *node) override;
  void VisitIdentifier(Identifier *node) override;
  void VisitBinaryExpression(BinaryExpression *node) override;
  void VisitUnaryExpression(UnaryExpression *node) override;
  void VisitUpdateExpression(UpdateExpression *node) override;
  void VisitMemberExpression(MemberExpression *node) override;
  void VisitCallExpression(CallExpression *node) override;

  DebugPrinter() : out_(std::cout) {}

 private:
  util::IndentingStream out_;
  // virtual void Visit
};
}  // namespace ast

}  // namespace c8

#endif
