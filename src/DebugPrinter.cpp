#include "Casting.hpp"
#include "Debug.hpp"

using namespace c8::ast;

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
  lingo::print(p, "[Identifier] - ");
  lingo::print_quoted(p, node->symbol->spelling());
}

void DebugPrinter::Visit(BlockStatement *node) {
  lingo::print(p, "{");
  lingo::indent(p);

  for (auto &child : node->body) {
    lingo::print_newline(p);
    Visit(child.get());
  }
  lingo::undent(p);
  lingo::print_newline(p);
  lingo::print("}}");
}

void DebugPrinter::Visit(FunctionDeclaration *node) {
  lingo::print(p, "[FunctionDeclaration]");
  indent(p);
  lingo::print_newline(p);

  lingo::print(p, "id:");
  lingo::indent(p);
  lingo::print_newline(p);
  Visit(node->id.get());
  lingo::undent(p);
  lingo::print_newline(p);
  lingo::print_newline(p);

  lingo::print(p, "Params:");
  lingo::indent(p);
  lingo::print_newline(p);
  for (const ast::NodePtr<ast::Identifier> &param : node->params) {
    Visit(param.get());
    lingo::print_newline(p);
  }
  lingo::undent(p);

  lingo::print_newline(p);
  lingo::print(p, "Body:");
  lingo::indent(p);
  lingo::print_newline(p);
  Visit(node->body.get());

  lingo::undent(p);
  lingo::undent(p);
  lingo::print_newline(p);
  lingo::print_newline(p);
}

void DebugPrinter::Visit(Literal *node) {
  lingo::print(p, "[literal]  ");
  lingo::print_quoted(p, node->symbol->spelling());
}

void DebugPrinter::Visit(ReturnStatement *node) {
  lingo::print(p, "return");
  lingo::indent(p);
  lingo::print_newline(p);
  Visit(node->argument.get());
  lingo::undent(p);
}

void DebugPrinter::Visit(BinaryExpression *node) {
  lingo::print(p, "[Binary] - ");
  lingo::print_quoted(p, node->oper.symbol()->spelling());
  lingo::indent(p);
  lingo::print_newline(p);

  Visit(node->left.get());
  lingo::print_newline(p);
  Visit(node->right.get());
  lingo::undent(p);
}

void DebugPrinter::Visit(UpdateExpression *node) {
  if (node->prefix) {
    lingo::print(p, "[Prefix Update] - ");
  } else {
    lingo::print(p, "[Postfix Update] - ");
  }
  lingo::print_quoted(p, node->oper.symbol()->spelling());
  lingo::indent(p);
  lingo::print_newline(p);
  Visit(node->argument.get());
  lingo::undent(p);
}

void DebugPrinter::Visit(UnaryExpression *node) {
  lingo::print(p, "[Unary] - ");
  lingo::print_quoted(p, node->oper.symbol()->spelling());
  lingo::indent(p);
  lingo::print_newline(p);
  Visit(node->argument.get());
  lingo::undent(p);
}

void DebugPrinter::Visit(IfStatement *node) {
  lingo::print(p, "if");
  lingo::indent(p);
  lingo::print_newline(p);
  Visit(node->test.get());
  lingo::undent(p);
  lingo::print_newline(p);
  lingo::print(p, "then");
  lingo::indent(p);
  lingo::print_newline(p);
  Visit(node->consequent.get());
  lingo::undent(p);

  if (node->alternate != nullptr) {
    lingo::print_newline(p);
    lingo::print("else");
    lingo::indent(p);
    lingo::print_newline(p);
    Visit(node->alternate.get());
    lingo::undent(p);
  }
}

// init test update body
void DebugPrinter::Visit(ForStatement *node) {
  lingo::print(p, "for");
  lingo::print_newline(p);
  lingo::print(p, "- init");
  lingo::indent(p);
  lingo::print_newline(p);
  if (node->init == nullptr) {
    lingo::print(p, "[NULL]");
  } else {
    Visit(node->init.get());
  }
  lingo::undent(p);

  lingo::print_newline(p);
  lingo::print(p, "- test");
  lingo::indent(p);
  lingo::print_newline(p);
  if (node->test == nullptr) {
    lingo::print(p, "[NULL]");
  } else {
    Visit(node->test.get());
  }
  lingo::undent(p);

  lingo::print_newline(p);
  lingo::print(p, "- update");
  lingo::indent(p);
  lingo::print_newline(p);
  if (node->update == nullptr) {
    lingo::print(p, "[NULL]");
  } else {
    Visit(node->update.get());
  }
  lingo::undent(p);

  lingo::print_newline(p);
  lingo::print(p, "- body");
  lingo::indent(p);
  lingo::print_newline(p);
  Visit(node->body.get());
  lingo::undent(p);
}
