#include "Parser/Parser.h"
#include "Lexer/Token.h"
#include "Parser/Nodes.h"

using namespace Parser;

Parser::Parser::Parser(Syntax::Lexer& lexer) : _lexer(lexer) {};

// TODO no 'main' function is an error
// TODO check identfiers

Tree Parser::Parser::parse() {
    Syntax::Token curr;
    _lexer.skip_new_lines = true;
    _lexer.skip_spaces = true;
    do {
        curr = _lexer.nextToken();
        if(curr.type() == Syntax::TokenType::Comment) {
            continue;
        } else if(curr.identifier() == Syntax::TokenIdentifier::Function) {
            _tree.getCurrent()->addChild( parseFunction() );
        } else {
            // TODO - throw custom exception with wrong token info inside
            throw std::runtime_error("Expected comment or function got something else");
        }
    } while(curr.type() != Syntax::TokenType::Nil);
    return _tree;
}

Tree::Node* Parser::Parser::parseFunction() {
    auto func_node = parseFunctionDeclaration();
    auto body = parseCodeBlock();
    for(auto expr: body) {
        func_node->addChild(expr);
    }
    return func_node;
}

Function* Parser::Parser::parseFunctionDeclaration() {
    _lexer.skip_new_lines = false;
    // fn keyword is already parsed at this point
    auto identifier = parseFunctionIndetifier();
    auto args = parseFunctionArguments();
    _lexer.skip_new_lines = true;
    return new Function(identifier, args);
}

std::string Parser::Parser::parseFunctionIndetifier() {
    auto curr = _lexer.nextToken();
    if(curr.identifier() == Syntax::TokenIdentifier::Identifier) {
        if(_identifiers.count(curr.symbol())) {
            throw std::runtime_error("Identifier already used!");
        }
        _identifiers.insert(curr.symbol());
    } else {
        throw std::runtime_error("Expected function name");
    }
    return curr.symbol();
}

std::vector<std::string> Parser::Parser::parseFunctionArguments() {
    _lexer.skip_spaces = true;
    auto curr = _lexer.nextToken();
    auto previous = curr;
    if(curr.identifier() != Syntax::TokenIdentifier::OpenBracket) {
        throw std::runtime_error("( expected got something else");
    }
    do {
        curr = _lexer.nextToken();
        if(curr.identifier() == Syntax::TokenIdentifier::CloseBracket)
    } while(true);
}

std::vector<Tree::Node*> Parser::Parser::parseCodeBlock() {

}