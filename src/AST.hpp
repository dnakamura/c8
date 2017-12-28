#ifndef C8_AST_HPP
#define C8_AST_HPP

#include <lingo/symbol.hpp>
#include <lingo/token.hpp>

#include <vector>
#include <memory>

namespace c8{
namespace ast{

    class Node;

    template<class T = Node>
    using NodePtr = std::unique_ptr<T>;
    template<class T>
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


    class Visitor {
             /* void Dump(lingo::Printer& p, const ast::FunctionDeclaration &node);
        void Dump(lingo::Printer& p, const ast::Identifier &node);
        void Dump(lingo::Printer& p, const ast::BlockStatement &node );
        void Dump(lingo::Printer &p, const ast::Expression &node);
        void Dump(lingo::Printer &p, const ast::Statement &node);
        void Dump(lingo::Printer &p, const ast::BinaryExpression& node);
       // void Dump(lingo::Printer &p, const ast::Statement &node);
        void Dump(lingo::Printer &p, const ast::ReturnStatement &node);
        void Dump(lingo::Printer &p, const ast::Identifier &node);
        void Dump(lingo::Printer &p, const ast::Literal &node);*/

        //void Dump()
        public:
        virtual void Visit(Identifier *node) = 0;
        virtual void Visit(FunctionDeclaration *node) = 0;
        virtual void Visit(BlockStatement *node) = 0;
        //virtual void Visit(Expression *node)
        virtual void Visit(BinaryExpression *node) = 0;
        virtual void Visit(ReturnStatement *node) = 0;
        virtual void Visit(Literal *node) = 0;
        //virtual void Visit
        

    };
    
    class Node {

        public:
        virtual ~Node(){}
        //virtual void Accept(Visitor *v) = 0;
        enum NodeKind {
            Kind_STATEMENT_START,
            Kind_FunctionDeclaration,
            
            Kind_BlockStatement,
            Kind_ReturnStatement,
            Kind_STATEMENT_END,

            Kind_EXPRESSION_START,
            Kind_Identifier,
            Kind_Literal,
            Kind_UpdateExpression,
            Kind_UnaryExpression,
            Kind_BinaryExpression,
            Kind_MemberExpression,
            Kind_EXPRESSION_END
        };

        
        NodeKind Kind() const {return kind_;}
        protected:
        Node(NodeKind kind): kind_(kind){}
        
        private:
        const NodeKind kind_;


    };

    struct Expression: public Node {

        static bool classof(const Node *node){
            return (node->Kind() > Kind_EXPRESSION_START) && (node->Kind() < Kind_EXPRESSION_END);
        }
    protected:
        using Node::Node;
    };

    struct UpdateExpression: public Expression {
        NodePtr<Expression> argument;
        lingo::Token oper;
        bool prefix;

        static bool classof(const Node* node) { return node->Kind() == Kind_UpdateExpression; }

        UpdateExpression():Expression(Kind_UpdateExpression){};
    };

    struct UnaryExpression: public Expression {
        NodePtr<Expression> argument;
        lingo::Token oper;
        
        static bool classof(const Node* node) { return node->Kind() == Kind_UnaryExpression; }
        UnaryExpression(lingo::Token oper_, NodePtr<Expression> &&arg):
        Expression(Kind_UnaryExpression),
        argument(std::move(arg)),
        oper(oper_)
        {}
    };

    struct BinaryExpression: public Expression {
        NodePtr<Expression> left;
        NodePtr<Expression> right;
        lingo::Token oper;

        static bool classof(const Node* node) { return node->Kind() == Kind_BinaryExpression; }

        BinaryExpression(NodePtr<Expression> &&left_, NodePtr<Expression> &&right_, lingo::Token oper_):
            Expression(Kind_BinaryExpression),
            left(std::move(left_)),
            right(std::move(right_)),
            oper(oper_){}
    };
    
    struct MemberExpression: public Expression {
        NodePtr<Expression> object;
        NodePtr<Expression> property;
        bool computed;

        static bool classof(const Node* node) { return node->Kind() == Kind_MemberExpression; }

        MemberExpression(NodePtr<Expression> &&object_, NodePtr<Expression>  &&property_, bool computed_)
            : Expression(Kind_MemberExpression),
            object(std::move(object_)),
            property(std::move(property_)),
            computed(computed_) {}
    };

    struct Statement: public Node{

        static bool classof(const Node* node) {
            return (node->Kind() > Kind_STATEMENT_START) && (node->Kind() < Kind_STATEMENT_END);
        }
        protected:
        using Node::Node;
    };


    struct BlockStatement: public Statement {
        NodeVector<Statement> body;

        public:
        BlockStatement(): Statement(Kind_BlockStatement) { }

        static bool classof(const Node* node) { return node->Kind() == Kind_BlockStatement; }

        //void Accept(Visitor *visitor) override;
        protected:
        using Statement::Statement;

    };


    struct ReturnStatement: public Statement {
        NodePtr<Expression> argument;

        static bool classof(const Node* node) { return node->Kind() == Kind_ReturnStatement; }

        ReturnStatement(NodePtr<Expression> &&arg):
            Statement(Kind_ReturnStatement),
            argument(std::move(arg)){}

    };


    struct Identifier: public Expression {
        lingo::Symbol const *symbol;

        static bool classof(const Node* node) { return node->Kind() == Kind_Identifier; }
        Identifier(const lingo::Symbol *symbol_): Expression(Kind_Identifier), symbol(symbol_){}
        //protected:
        //using Expression::Expression;
    };

    struct Literal: public Expression {
        lingo::Symbol const *symbol;

        static bool classof(const Node* node) { return node->Kind() == Kind_Literal; }
        Literal(const lingo::Symbol *symbol_): Expression(Kind_Literal), symbol(symbol_){}
    };

    // Does not inherit from Node to avoid the diamond problem
    struct Function {
        NodePtr<Identifier> id;
        NodeVector<Identifier> params;
        NodePtr<BlockStatement> body;
    };

    struct FunctionDeclaration: public Statement, public Function {
        //NodePtr<Identifier> id;
        //NodeVector<Expression> params;
        //NodePtr<BlockStatement> body;
        static bool classof(const Node* node) { return node->Kind() == Kind_FunctionDeclaration; }
        FunctionDeclaration(): Statement(Kind_FunctionDeclaration) {}
    };
    


};
}



#endif
