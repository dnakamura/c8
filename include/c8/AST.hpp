#ifndef C8_AST_HPP
#define C8_AST_HPP

#include "Token.hpp"

#include <memory>
#include <vector>

namespace c8 {
namespace ast {

class Node;

template <class T = Node>
using NodePtr = std::unique_ptr<T>;
template <class T>
using NodeVector = std::vector<NodePtr<T>>;

// Forward declare node types
class Node;
struct Expression;
struct Statement;
struct BlockStatement;
struct Identifier;
struct Function;
struct FunctionDeclaration;
struct Literal;
struct BinaryExpression;
struct ReturnStatement;
struct ExpressionStatement;
struct UnaryExpression;
struct UpdateExpression;
struct IfStatement;
struct ForStatement;
struct Declaration;
struct VariableDeclaration;
struct MemberExpression;
struct CallExpression;

class Visitor {
 public:
  virtual void VisitFunctionDeclaration(FunctionDeclaration *node) = 0;
  virtual void VisitBlockStatement(BlockStatement *node) = 0;

  virtual void VisitReturnStatement(ReturnStatement *node) = 0;
  virtual void VisitExpressionStatement(ExpressionStatement *node) = 0;
  virtual void VisitIfStatement(IfStatement *node) = 0;
  virtual void VisitForStatement(ForStatement *node) = 0;

  // Expression visitors
  virtual void VisitBinaryExpression(BinaryExpression *node) = 0;
  virtual void VisitUnaryExpression(UnaryExpression *node) = 0;
  virtual void VisitUpdateExpression(UpdateExpression *node) = 0;
  virtual void VisitLiteral(Literal *node) = 0;
  virtual void VisitIdentifier(Identifier *node) = 0;
  virtual void VisitMemberExpression(MemberExpression *node) = 0;
  virtual void VisitCallExpression(CallExpression *node) = 0;

  //Dispatch helper methods
  void VisitNode(Node*);
  void VisitExpression(Expression*);
  void VisitStatement(Statement*);
};

class Node {
 public:
  virtual ~Node() {}
  // virtual void Accept(Visitor *v) = 0;
  enum NodeKind {
    Kind_STATEMENT_START,
    Kind_BlockStatement,
    Kind_ReturnStatement,
    Kind_ExpressionStatement,
    Kind_IfStatement,
    Kind_ForStatement,

    Kind_DEVLARATION_START,
    Kind_FunctionDeclaration,
    Kind_VariableDeclaration,
    Kind_DECLARATION_END,
    Kind_STATEMENT_END = Kind_DECLARATION_END,

    Kind_EXPRESSION_START = Kind_STATEMENT_END,
    Kind_Identifier,
    Kind_Literal,
    Kind_UpdateExpression,
    Kind_UnaryExpression,
    Kind_BinaryExpression,
    Kind_MemberExpression,
    Kind_CallExpression,
    Kind_EXPRESSION_END,

    Kind_VariableDeclarator
  };

  NodeKind Kind() const { return kind_; }

 protected:
  Node(NodeKind kind) : kind_(kind) {}

 private:
  const NodeKind kind_;
};

struct Expression : public Node {
  static bool classof(const Node *node) {
    return (node->Kind() > Kind_EXPRESSION_START) &&
           (node->Kind() < Kind_EXPRESSION_END);
  }

 protected:
  using Node::Node;
};

struct UpdateExpression : public Expression {
  NodePtr<Expression> argument;
  Token oper;
  bool prefix;

  static bool classof(const Node *node) {
    return node->Kind() == Kind_UpdateExpression;
  }

  [[deprecated]]
  UpdateExpression() : Expression(Kind_UpdateExpression){};
  UpdateExpression(NodePtr<Expression> &&arg, const Token &op, bool pfx)
      : Expression(Kind_UpdateExpression),
        argument(std::move(arg)),
        oper(op),
        prefix(pfx) {}
};

struct UnaryExpression : public Expression {
  NodePtr<Expression> argument;
  Token oper;

  static bool classof(const Node *node) {
    return node->Kind() == Kind_UnaryExpression;
  }

  UnaryExpression(Token oper_, NodePtr<Expression> &&arg)
      : Expression(Kind_UnaryExpression),
        argument(std::move(arg)),
        oper(oper_) {}
};

struct BinaryExpression : public Expression {
  NodePtr<Expression> left;
  NodePtr<Expression> right;
  Token oper;

  static bool classof(const Node *node) {
    return node->Kind() == Kind_BinaryExpression;
  }

  BinaryExpression(NodePtr<Expression> &&left_, NodePtr<Expression> &&right_,
                   Token oper_)
      : Expression(Kind_BinaryExpression),
        left(std::move(left_)),
        right(std::move(right_)),
        oper(oper_) {}
};

struct MemberExpression : public Expression {
  NodePtr<Expression> object;
  NodePtr<Expression> property;
  bool computed;

  static bool classof(const Node *node) {
    return node->Kind() == Kind_MemberExpression;
  }

  MemberExpression(NodePtr<Expression> &&object_,
                   NodePtr<Expression> &&property_, bool computed_)
      : Expression(Kind_MemberExpression),
        object(std::move(object_)),
        property(std::move(property_)),
        computed(computed_) {}
};

struct CallExpression : public Expression {
  NodePtr<Expression> callee;
  NodeVector<Expression> arguments;

  CallExpression(NodePtr<Expression> &&callee_, NodeVector<Expression> &&args_)
      : Expression(Kind_CallExpression),
        callee(std::move(callee_)),
        arguments(std::move(args_)) {}
};

struct Statement : public Node {
  static bool classof(const Node *node) {
    return (node->Kind() > Kind_STATEMENT_START) &&
           (node->Kind() < Kind_STATEMENT_END);
  }

 protected:
  using Node::Node;
};

struct Declaration : public Statement {
 protected:
  using Statement::Statement;
};

struct VariableDeclarator : public Node {
  NodePtr<Identifier> id;
  NodePtr<Expression> init;

  VariableDeclarator(NodePtr<Identifier> &&id_, NodePtr<Expression> &&init_)
      : Node(Kind_VariableDeclarator),
        id(std::move(id_)),
        init(std::move(init_)) {}
};

struct VariableDeclaration : public Declaration {
  NodeVector<VariableDeclarator> declarations;
  VariableDeclaration() : Declaration(Kind_VariableDeclaration) {}
};

struct BlockStatement : public Statement {
  NodeVector<Statement> body;

 public:
  BlockStatement() : Statement(Kind_BlockStatement) {}
  //TODO write new style constructor

  static bool classof(const Node *node) {
    return node->Kind() == Kind_BlockStatement;
  }

 protected:
  using Statement::Statement;
};

struct ReturnStatement : public Statement {
  NodePtr<Expression> argument;

  static bool classof(const Node *node) {
    return node->Kind() == Kind_ReturnStatement;
  }

  ReturnStatement(NodePtr<Expression> &&arg)
      : Statement(Kind_ReturnStatement), argument(std::move(arg)) {}
};

struct ExpressionStatement : public Statement {
  NodePtr<Expression> expression;

  ExpressionStatement(NodePtr<Expression> &&expr)
      : Statement(Kind_ExpressionStatement), expression(std::move(expr)) {}
};

struct Identifier : public Expression {
  // TODO this feels stupid
  const std::string name;

  static bool classof(const Node *node) {
    return node->Kind() == Kind_Identifier;
  }
  Identifier(const std::string &symbol_)
      : Expression(Kind_Identifier), name(symbol_) {}
  // protected:
  // using Expression::Expression;
};

struct Literal : public Expression {
  // TODO this feels wrong
  Token value;

  static bool classof(const Node *node) { return node->Kind() == Kind_Literal; }
  Literal(const Token &symbol_) : Expression(Kind_Literal), value(symbol_) {}
};

// Does not inherit from Node to avoid the diamond problem
struct Function {
  NodePtr<Identifier> id;
  NodeVector<Identifier> params;
  NodePtr<BlockStatement> body;
};

struct FunctionDeclaration : public Statement, public Function {
  static bool classof(const Node *node) {
    return node->Kind() == Kind_FunctionDeclaration;
  }
  FunctionDeclaration() : Statement(Kind_FunctionDeclaration) {}
  //TODO neeeds new style constructor
};

struct IfStatement : public Statement {
  NodePtr<Expression> test;
  NodePtr<Statement> consequent;
  NodePtr<Statement> alternate;

  IfStatement(NodePtr<Expression> &&test_, NodePtr<Statement> consequent_)
      : Statement(Kind_IfStatement),
        test(std::move(test_)),
        consequent(std::move(consequent_)),
        alternate(nullptr) {}
  IfStatement(NodePtr<Expression> &&test_, NodePtr<Statement> consequent_,
              NodePtr<Statement> alternate_)
      : Statement(Kind_IfStatement),
        test(std::move(test_)),
        consequent(std::move(consequent_)),
        alternate(std::move(alternate_)) {}
};

struct ForStatement : public Statement {
  NodePtr<Node> init;
  NodePtr<Expression> test;
  NodePtr<Expression> update;
  NodePtr<Statement> body;

  ForStatement(NodePtr<Expression> &&init_, NodePtr<Expression> &&test_,
               NodePtr<Expression> &&update_, NodePtr<Statement> &&body_)
      : Statement(Kind_ForStatement),
        init(std::move(init_)),
        test(std::move(test_)),
        update(std::move(update_)),
        body(std::move(body_)) {}
  ForStatement(NodePtr<VariableDeclaration> &&init_,
               NodePtr<Expression> &&test_, NodePtr<Expression> &&update_,
               NodePtr<Statement> &&body_)
      : Statement(Kind_ForStatement),
        init(std::move(init_)),
        test(std::move(test_)),
        update(std::move(update_)),
        body(std::move(body_)) {}
};
}  // namespace ast
}  // namespace c8

#endif
