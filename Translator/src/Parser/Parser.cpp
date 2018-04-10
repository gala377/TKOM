#include "Parser/Parser.h"
#include "Lexer/Token.h"
#include "Parser/Nodes.h"

using namespace Parser;

Parser::Parser::Parser(Syntax::Lexer& lexer) : _lexer(lexer) {};

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
    auto function = assembleFunctionDeclaration();
    auto curr = _lexer.nextToken();
    if(curr.type() == Syntax::TokenType::Nil) {
        throw std::runtime_error("Function body expected got EOF!");
    }
    while(curr.identifier() != Syntax::TokenIdentifier::CloseCurlyBracket) {
        // TODO function->addChild( parseFunctionBodyInstruction() );
    }
    return function;
}

Function* Parser::Parser::assembleFunctionDeclaration() {
    _lexer.skip_new_lines = false;
    _lexer.skip_spaces = false;
    std::string identifier;
    std::vector<std::string> args;

    auto curr = _lexer.nextToken();
    if(curr.identifier() != Syntax::TokenIdentifier::Space) {
        throw std::runtime_error("Expected single space");
    }

    curr = _lexer.nextToken();
    if(curr.identifier() == Syntax::TokenIdentifier::Identifier) {
        if(_identifiers.count(curr.symbol())) {
            throw std::runtime_error("Identifier already used!");
        }
        _identifiers.insert(curr.symbol());
        identifier = curr.symbol();
    } else {
        throw std::runtime_error("Expected function name");
    }
    // TODO args = parseFunctionArguments();
    _lexer.skip_new_lines = true;
    _lexer.skip_spaces = true;
    return new Function(identifier, args);
}

std::vector<std::string> Parser::Parser::parseFunctionArguments() {
    auto curr = _lexer.nextToken();
    _lexer.skip_spaces = true;
}