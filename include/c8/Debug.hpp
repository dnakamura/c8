#ifndef C8_DEBUG_HPP
#define C8_DEBUG_HPP

#include <iostream>
#include "AST.hpp"
#include "IndentingStream.hpp"

namespace c8 {
namespace ast {
class DebugPrinter : public Visitor {
 public:
  void Visit(FunctionDeclaration *node) override;
  void Visit(BlockStatement *node) override;
  void Visit(ReturnStatement *node) override;
  void Visit(IfStatement *node) override;
  void Visit(ForStatement *node) override;
  void Visit(ExpressionStatement *node) override { VisitExpression(node->expression.get()); }

  void Visit(Literal *node) override;
  void Visit(Identifier *node) override;
  void Visit(BinaryExpression *node) override;
  void Visit(UnaryExpression *node) override;
  void Visit(UpdateExpression *node) override;
  void Visit(MemberExpression *node) override;
  void Visit(CallExpression *node) override;

  DebugPrinter() : out_(std::cout) {}

 private:
  util::IndentingStream out_;
  // virtual void Visit
};
}  // namespace ast

}  // namespace c8

#endif
