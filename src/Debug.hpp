#ifndef C8_DEBUG_HPP
#define C8_DEBUG_HPP

#include "AST.hpp"
#include "lingo/print.hpp"

#include <iostream>


namespace c8 {
namespace ast{
    class DebugPrinter: public  Visitor{
                public:
        void Visit(Identifier *node) override;
        void Visit(FunctionDeclaration *node) override;
        void Visit(BlockStatement *node) override;
        void Visit(BinaryExpression *node) override;
        void Visit(ReturnStatement *node) override;
        void Visit(Literal *node) override;

        DebugPrinter(): p(std::cout){}
        protected:
        //Note these are helper dispatch methods
        //TODO should maybe be refactored into Visitor?
        void Visit(Node *node);
        void Visit(Expression *node);
        void Visit(Statement *node);
        private:
        lingo::Printer p;
        //virtual void Visit
    };

}

namespace dbg {

        void Dump(lingo::Printer& p, const ast::FunctionDeclaration &node);
        void Dump(lingo::Printer& p, const ast::Identifier &node);
        void Dump(lingo::Printer& p, const ast::BlockStatement &node );
        void Dump(lingo::Printer &p, const ast::Expression &node);
        void Dump(lingo::Printer &p, const ast::Statement &node);
        void Dump(lingo::Printer &p, const ast::BinaryExpression& node);
       // void Dump(lingo::Printer &p, const ast::Statement &node);
        void Dump(lingo::Printer &p, const ast::ReturnStatement &node);
        void Dump(lingo::Printer &p, const ast::Identifier &node);
        void Dump(lingo::Printer &p, const ast::Literal &node);

}
}

#endif

