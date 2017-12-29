#include "Debug.hpp"
#include "Casting.hpp"

using namespace c8::ast;

void DebugPrinter::Visit(Node *node){
    if(isa<Statement>(node)){
        Visit(static_cast<Statement*>(node));
    } else if (isa<Expression>(node)){
        Visit(static_cast<Expression*>(node));
    } else {
        throw std::exception();
    }
}

void DebugPrinter::Visit(Expression *node){
    //TODO make this use out llvm-style rtti
    switch(node->Kind()){
        case Node::Kind_BinaryExpression:
            Visit(dynamic_cast<BinaryExpression*>(node));
            break;
        case Node::Kind_Identifier:
            Visit(dynamic_cast<Identifier*>(node));
            break;
        case Node::Kind_Literal:
            Visit(dynamic_cast<Literal*>(node));
            break;
        default:
        throw std::exception();
    }

}

void DebugPrinter::Visit(Statement *node){
    switch(node->Kind()){
        case Node::Kind_BlockStatement:
            Visit(dynamic_cast<BlockStatement*>(node));
            break;
        case Node::Kind_ReturnStatement:
            Visit(dynamic_cast<ReturnStatement*>(node));
            break;
        default:
        throw std::exception();
    }
    //TODO make this use out llvm-style rtti
}



void DebugPrinter::Visit(Identifier *node) {
    lingo::print(p, "[Identifier] - ");
    lingo::print_quoted(p, node->symbol->spelling());
}

void DebugPrinter::Visit(BlockStatement *node){
    lingo::print(p, "{");
    lingo::indent(p);

    for(auto& child: node->body){
        lingo::print_newline(p);
        Visit(child.get());
    }
    lingo::undent(p);
    lingo::print_newline(p);
    lingo::print("}}");
}

void DebugPrinter::Visit(FunctionDeclaration *node){
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
    for(const ast::NodePtr<ast::Identifier> &param: node->params){
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

void DebugPrinter::Visit(Literal *node){
    lingo::print(p, "[literal]  ");
    lingo::print_quoted(p, node->symbol->spelling());
}

void DebugPrinter::Visit(ReturnStatement *node){
    lingo::print(p, "return");
    lingo::indent(p);
    lingo::print_newline(p);
    Visit(node->argument.get());
    lingo::undent(p);
}

void DebugPrinter::Visit(BinaryExpression *node){
    lingo::print(p,"[Binary] - ");
    lingo::print(p, node->oper.symbol()->spelling());
    lingo::indent(p);
    lingo::print_newline(p);

    Visit(node->left.get());
    lingo::print_newline(p);
    Visit(node->right.get());
    lingo::undent(p);
}

