#include "Debug.hpp"

using namespace c8;
using namespace c8::dbg;



void c8::dbg::Dump(lingo::Printer& p, const ast::FunctionDeclaration &node) {
    lingo::print(p, "[FunctionDeclaration]");
    indent(p);
    lingo::print_newline(p);

    lingo::print(p, "id:");
    lingo::indent(p);
    lingo::print_newline(p);
    Dump(p, *node.id);
    lingo::undent(p);
    lingo::print_newline(p);
    lingo::print_newline(p);

    lingo::print(p, "Params:");
    lingo::indent(p);
    lingo::print_newline(p);
    for(const ast::NodePtr<ast::Identifier> &param: node.params){
        Dump(p, *param);
        lingo::print_newline(p);
    }
    lingo::undent(p);

    lingo::print_newline(p);
    lingo::print(p, "Body:");
    lingo::indent(p);
    lingo::print_newline(p);
    Dump(p, *node.body);

    lingo::undent(p);
    lingo::undent(p);
    lingo::print_newline(p);
    lingo::print_newline(p);


}

void c8::dbg::Dump(lingo::Printer& p, const ast::Identifier &node) {
    lingo::print(p, "[Identifier] - ");
    lingo::print_quoted(p, node.symbol->spelling());
    //lingo::print_newline(p);
}


void c8::dbg::Dump(lingo::Printer &p, const ast::BinaryExpression& node){
    lingo::print(p,"[Binary] - ");
    lingo::print(p,node.oper.symbol()->spelling());
    lingo::indent(p);
    lingo::print_newline(p);

    Dump(p, *node.left);
    lingo::print_newline(p);
    Dump(p, *node.right);
    lingo::undent(p);
}



void c8::dbg::Dump(lingo::Printer& p, const ast::Literal &node){
    lingo::print(p, "[literal]  ");
    lingo::print_quoted(p, node.symbol->spelling());
}

void c8::dbg::Dump(lingo::Printer& p, const ast::Expression &node){
    using namespace c8::ast;
    switch(node.Kind()){
        case Node::Kind_BinaryExpression:
            Dump(p, dynamic_cast<const BinaryExpression&>(node));
            break;
        case Node::Kind_Identifier:
            Dump(p, dynamic_cast<const Identifier&>(node));
            break;
        case Node::Kind_Literal:
            Dump(p, dynamic_cast<const Literal&>(node));
            break;
        default:
        throw std::exception();
    }
}
void c8::dbg::Dump(lingo::Printer &p, const ast::Statement &node){
    using namespace c8::ast;
    //TODO this should be using llvm style rtti
    switch(node.Kind()){
        case Node::Kind_UpdateExpression:
        case Node::Kind_UnaryExpression:
        case Node::Kind_BinaryExpression:
        case Node::Kind_MemberExpression:
        case Node::Kind_Identifier:
        case Node::Kind_Literal:
            Dump(p, dynamic_cast<const Expression&>(node));
            break;

        case Node::Kind_BlockStatement:
            Dump(p, dynamic_cast<const BlockStatement&>(node));
            break;
        case Node::Kind_ReturnStatement:
            Dump(p, dynamic_cast<const ReturnStatement&>(node));
            break;
        default:
        throw std::exception();
    }
}
void c8::dbg::Dump(lingo::Printer &p, const ast::ReturnStatement& node){
    lingo::print(p, "return");
    lingo::indent(p);
    lingo::print_newline(p);
    Dump(p, *node.argument);
    lingo::undent(p);
}

void c8::dbg::Dump(lingo::Printer &p, const ast::BlockStatement &node){
    lingo::print(p, "Block");
    lingo::indent(p);
    //lingo::print_newline(p);
    for(auto& child: node.body){
        lingo::print_newline(p);
        Dump(p, *child);
    }
    lingo::undent(p);
    lingo::print_newline(p);
}
