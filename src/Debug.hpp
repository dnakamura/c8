#ifndef C8_DEBUG_HPP
#define C8_DEBUG_HPP

#include "AST.hpp"
#include "lingo/print.hpp"

#include <iostream>

namespace c8 {
namespace ast {
class DebugPrinter : public Visitor {
 public:
  void Visit(FunctionDeclaration *node) override;
  void Visit(BlockStatement *node) override;
  void Visit(ReturnStatement *node) override;
  void Visit(IfStatement *node) override;
  void Visit(ForStatement *node) override;

  void Visit(Literal *node) override;
  void Visit(Identifier *node) override;
  void Visit(BinaryExpression *node) override;
  void Visit(UnaryExpression *node) override;
  void Visit(UpdateExpression *node) override;

  DebugPrinter() : p(std::cout) {}

 protected:
  // Note these are helper dispatch methods
  // TODO should maybe be refactored into Visitor?
  void Visit(Node *node);
  void Visit(Expression *node);
  void Visit(Statement *node);
  void Visit(ExpressionStatement *node) { Visit(node->expression.get()); }

 private:
  lingo::Printer p;
  // virtual void Visit
};
}  // namespace ast

namespace dbg {

void Dump(lingo::Printer &p, const ast::FunctionDeclaration &node);
void Dump(lingo::Printer &p, const ast::Identifier &node);
void Dump(lingo::Printer &p, const ast::BlockStatement &node);
void Dump(lingo::Printer &p, const ast::Expression &node);
void Dump(lingo::Printer &p, const ast::Statement &node);
void Dump(lingo::Printer &p, const ast::BinaryExpression &node);
// void Dump(lingo::Printer &p, const ast::Statement &node);
void Dump(lingo::Printer &p, const ast::ReturnStatement &node);
void Dump(lingo::Printer &p, const ast::Identifier &node);
void Dump(lingo::Printer &p, const ast::Literal &node);
}  // namespace dbg
}  // namespace c8

#endif
