#include "c8/Debug.hpp"
#include "c8/Casting.hpp"
#include "c8/IndentingStream.hpp"

using namespace c8::ast;
using nl = c8::util::IndentingStream::NewlineMarker;
using indent = c8::util::IndentingStream::IndentMarker;
using undent = c8::util::IndentingStream::UndentMarker;



void DebugPrinter::VisitIdentifier(Identifier *node) {
  out_ << "[Identifier] - \"" << node->name << "\"";
}

void DebugPrinter::VisitBlockStatement(BlockStatement *node) {
  out_ << "{";
  out_.indent();

  for (auto &child : node->body) {
    out_ << nl();
    VisitStatement(child.get());
  }
  out_.undent();
  out_ << nl() << "}";
}

void DebugPrinter::VisitFunctionDeclaration(FunctionDeclaration *node) {
  out_ << "[FunctionDeclaration]" << indent() << nl();

  out_ << "id:" << indent() << nl();
  VisitIdentifier(node->id.get());
  out_ << undent() << nl();

  out_ << "Params:" << indent() << nl();

  for (const ast::NodePtr<ast::Identifier> &param : node->params) {
    VisitIdentifier(param.get());
    out_ << nl();
  }
  out_ << undent() << nl();
  out_ << "Body:" << indent() << nl();
  VisitBlockStatement(node->body.get());

  out_ << undent() << undent() << nl();
}

void DebugPrinter::VisitLiteral(Literal *node) {
  out_ << "[literal]  \"";
  switch (node->value.kind()) {
    case tok::string_literal:
      out_ << node->value.string_value();
      break;
    case tok::numeric_literal:
      out_ << node->value.numeric_value();
      break;
    default:
      throw std::exception();
  }
  out_ << "\"";
}

void DebugPrinter::VisitReturnStatement(ReturnStatement *node) {
  out_ << "return" << indent() << nl();
  VisitExpression(node->argument.get());
  out_.undent();
}

void DebugPrinter::VisitBinaryExpression(BinaryExpression *node) {
  out_ << "[Binary] - \"" << node->oper.ToString() << "\"" << indent() << nl();

  VisitExpression(node->left.get());
  out_ << nl();
  VisitExpression(node->right.get());
  out_.undent();
}

void DebugPrinter::VisitUpdateExpression(UpdateExpression *node) {
  if (node->prefix) {
    out_ << "[Prefix Update] - \"";
  } else {
    out_ << "[Postfix Update] - \"";
  }
  out_ << node->oper.ToString() << "\"" << indent() << nl();
  VisitExpression(node->argument.get());
  out_.undent();
}

void DebugPrinter::VisitUnaryExpression(UnaryExpression *node) {
  out_ << "[Unary] - \"" << node->oper.ToString() << "\"" << indent() << nl();
  VisitExpression(node->argument.get());
  out_.undent();
}

void DebugPrinter::VisitIfStatement(IfStatement *node) {
  out_ << "if" << indent() << nl();
  VisitExpression(node->test.get());
  out_ << undent() << nl();
  out_ << "then" << indent() << nl();
  VisitStatement(node->consequent.get());
  out_.undent();

  if (node->alternate != nullptr) {
    out_ << nl() << "else" << indent() << nl();
    VisitStatement(node->alternate.get());
    out_.undent();
  }
}

// init test update body
void DebugPrinter::VisitForStatement(ForStatement *node) {
  out_ << "for" << nl();
  out_ << "- init" << indent() << nl();

  if (node->init == nullptr) {
    out_ << "[NULL]";
  } else {
    VisitNode(node->init.get());
  }
  out_ << undent() << nl();

  out_ << "- test" << indent() << nl();

  if (node->test == nullptr) {
    out_ << "[NULL]";
  } else {
    VisitExpression(node->test.get());
  }
  out_ << undent() << nl();

  out_ << "- update" << indent() << nl();

  if (node->update == nullptr) {
    out_ << "[NULL]";
  } else {
    VisitExpression(node->update.get());
  }
  out_ << undent() << nl();

  out_ << "- body" << indent() << nl();
  VisitStatement(node->body.get());
  out_.undent();
}

void DebugPrinter::VisitMemberExpression(MemberExpression *node) {
  out_ << "Member Expression" << nl();

  out_ << "- Object" << indent() << nl();
  VisitExpression(node->object.get());
  out_ << undent() << nl();

  out_ << "- Property" << indent() << nl();
  VisitExpression(node->property.get());
  out_ << undent() << nl();

  if (node->computed) {
    out_ << "- Computed = TRUE";
  } else {
    out_ << "- Computed = FALSE";
  }
}

void DebugPrinter::VisitCallExpression(CallExpression *node) {
  out_ << "Call" << nl();

  out_ << "-callee" << indent() << nl();
  VisitExpression(node->callee.get());
  out_ << undent() << nl();

  out_ << "- arguments" << indent() << nl();

  int i = 0;
  for (int i = 0; i < node->arguments.size(); ++i) {
    if (i != 0) {
      out_ << nl();
    }
    out_ << "[" << i << "] ";
    VisitExpression(node->arguments[i].get());
  }
  out_.undent();
}
