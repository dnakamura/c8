#include "Debug.hpp"
#include "Casting.hpp"
#include "IndentingStream.hpp"

using namespace c8::ast;
using nl = c8::util::IndentingStream::NewlineMarker;
using indent = c8::util::IndentingStream::IndentMarker;
using undent = c8::util::IndentingStream::UndentMarker;

void DebugPrinter::Visit(Node *node) {
  if (isa<Statement>(node)) {
    Visit(static_cast<Statement *>(node));
  } else if (isa<Expression>(node)) {
    Visit(static_cast<Expression *>(node));
  } else {
    throw std::exception();
  }
}

void DebugPrinter::Visit(Expression *node) {
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

void DebugPrinter::Visit(Statement *node) {
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

void DebugPrinter::Visit(Identifier *node) {
  out_ << "[Identifier] - \"" << node->symbol << "\"";
}

void DebugPrinter::Visit(BlockStatement *node) {
  out_ << "{";
  out_.indent();

  for (auto &child : node->body) {
    out_ << nl();
    Visit(child.get());
  }
  out_.undent();
  out_ << nl() << "}";
}

void DebugPrinter::Visit(FunctionDeclaration *node) {
  out_ << "[FunctionDeclaration]" << indent() << nl();

  out_ << "id:" << indent() << nl();
  Visit(node->id.get());
  out_ << undent() << nl();

  out_ << "Params:" << indent() << nl();

  for (const ast::NodePtr<ast::Identifier> &param : node->params) {
    Visit(param.get());
    out_ << nl();
  }
  out_ << undent() << nl();
  out_ << "Body:" << indent() << nl();
  Visit(node->body.get());

  out_ << undent() << undent() << nl();
}

void DebugPrinter::Visit(Literal *node) {
  out_ << "[literal]  \"";
  switch (node->symbol.kind()) {
    case tok::string_literal:
      out_ << node->symbol.string_value();
      break;
    case tok::numeric_literal:
      out_ << node->symbol.numeric_value();
      break;
    default:
      throw std::exception();
  }
  out_ << "\"";
}

void DebugPrinter::Visit(ReturnStatement *node) {
  out_ << "return" << indent() << nl();
  Visit(node->argument.get());
  out_.undent();
}

void DebugPrinter::Visit(BinaryExpression *node) {
  out_ << "[Binary] - \"" << node->oper.ToString() << "\"" << indent() << nl();

  Visit(node->left.get());
  out_ << nl();
  Visit(node->right.get());
  out_.undent();
}

void DebugPrinter::Visit(UpdateExpression *node) {
  if (node->prefix) {
    out_ << "[Prefix Update] - \"";
  } else {
    out_ << "[Postfix Update] - \"";
  }
  out_ << node->oper.ToString() << "\"" << indent() << nl();
  Visit(node->argument.get());
  out_.undent();
}

void DebugPrinter::Visit(UnaryExpression *node) {
  out_ << "[Unary] - \"" << node->oper.ToString() << "\"" << indent() << nl();
  Visit(node->argument.get());
  out_.undent();
}

void DebugPrinter::Visit(IfStatement *node) {
  out_ << "if" << indent() << nl();
  Visit(node->test.get());
  out_ << undent() << nl();
  out_ << "then" << indent() << nl();
  Visit(node->consequent.get());
  out_.undent();

  if (node->alternate != nullptr) {
    out_ << nl() << "else" << indent() << nl();
    Visit(node->alternate.get());
    out_.undent();
  }
}

// init test update body
void DebugPrinter::Visit(ForStatement *node) {
  out_ << "for" << nl();
  out_ << "- init" << indent() << nl();

  if (node->init == nullptr) {
    out_ << "[NULL]";
  } else {
    Visit(node->init.get());
  }
  out_ << undent() << nl();

  out_ << "- test" << indent() << nl();

  if (node->test == nullptr) {
    out_ << "[NULL]";
  } else {
    Visit(node->test.get());
  }
  out_ << undent() << nl();

  out_ << "- update" << indent() << nl();

  if (node->update == nullptr) {
    out_ << "[NULL]";
  } else {
    Visit(node->update.get());
  }
  out_ << undent() << nl();

  out_ << "- body" << indent() << nl();
  Visit(node->body.get());
  out_.undent();
}

void DebugPrinter::Visit(MemberExpression *node) {
  out_ << "Member Expression" << nl();

  out_ << "- Object" << indent() << nl();
  Visit(node->object.get());
  out_ << undent() << nl();

  out_ << "- Property" << indent() << nl();
  Visit(node->property.get());
  out_ << undent() << nl();

  if (node->computed) {
    out_ << "- Computed = TRUE";
  } else {
    out_ << "- Computed = FALSE";
  }
}

void DebugPrinter::Visit(CallExpression *node) {
  out_ << "Call" << nl();

  out_ << "-callee" << indent() << nl();
  Visit(node->callee.get());
  out_ << undent() << nl();

  out_ << "- arguments" << indent() << nl();

  int i = 0;
  for (int i = 0; i < node->arguments.size(); ++i) {
    if (i != 0) {
      out_ << nl();
    }
    out_ << "[" << i << "] ";
    Visit(node->arguments[i].get());
  }
  out_.undent();
}
